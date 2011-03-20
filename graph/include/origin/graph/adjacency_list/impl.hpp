// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_LIST_IMPL_HPP
#define ORIGIN_GRAPH_ADJACENCY_LIST_IMPL_HPP

#include <cassert>
#include <cstdint>
#include <unordered_map>

#include <origin/exception.hpp>
#include <origin/graph/adjacency_list/node_list.hpp>

namespace origin {

  /** @internal */
  namespace adjacency_list_
  {
    struct vertex_node_base;
    struct edge_node_base;

    // The classes in this file define the foundation of the adjacency list
    // data structures, which is essentially a directed graph. Undirected
    // graphs are simply wrappers that hide much of the underlying
    // directionality.

    /**
     * The edge node base implements the non-dependent linking structure for edges
     * in an adjacency list. Each edge node resides simulataneously in three
     * different lists. The next/prev members establish linkage in the graph's edge
     * list. The out members establish linkage in the out edge list of a source
     * vertex, and the in members establish linkage in the in edge list of a target
     * vertex. The source and target links refer to the source and target vertices.
     *
     * The total size of the edge node base is 8 * sizeof(void*), which is 32 bytes
     * on a 32 bit architecture.
     */
    struct edge_node_base
    {
      typedef void value_type;
      typedef vertex_node_base* vertex_pointer;
      typedef vertex_node_base const* const_vertex_pointer;
      typedef edge_node_base* edge_pointer;
      typedef edge_node_base const* const_edge_pointer;

      edge_node_base(vertex_pointer u, vertex_pointer v)
        : next(), prev()
        , next_out(), prev_out()
        , next_in(), prev_in()
        , source(u), target(v)
      { }

      /**
       * Given either the source or target, return the opposite end of the
       * edge.
       *
       * @pre p == source || p == target
       */
      vertex_pointer opposite(vertex_pointer p) const
      {
        assert(( p == source || p == target ));
        return p == source ? target : source;
      }

      const_vertex_pointer opposite(const_vertex_pointer p) const
      {
        assert(( p == source || p == target ));
        return p == source ? target : source;
      }

      edge_pointer next;      ///< The next edge in the graph
      edge_pointer prev;      ///< The previous edge in the graph
      edge_pointer next_out;  ///< The next out edge of a vertex
      edge_pointer prev_out;  ///< The previous out edge of a vertex
      edge_pointer next_in;   ///< The next in edge of a vertex
      edge_pointer prev_in;   ///< The prev in edge of a vertex.
      vertex_pointer source;  ///< The source vertex of the edge
      vertex_pointer target;  ///< The target vertex of the edge
    };

    /** Adapt list linkage accessors to create an out edge list. */
    struct out_linkage
    {
      static edge_node_base*& next(edge_node_base* p)
      { return p->next_out; }

      static void set_next(edge_node_base* p, edge_node_base* q)
      { p->next_out = q; }

      static edge_node_base*& prev(edge_node_base* p)
      { return p->prev_out; }

      static void set_prev(edge_node_base* p, edge_node_base* q)
      { p->prev_out = q; }
    };

    /** Adapt list linkage accessros to create an in edge list. */
    struct in_linkage
    {
      static edge_node_base*& next(edge_node_base* p)
      { return p->next_in; }

      static void set_next(edge_node_base* p, edge_node_base* q)
      { p->next_in = q; }

      static edge_node_base*& prev(edge_node_base* p) {
        return p->prev_in;
      }

      static void set_prev(edge_node_base* p, edge_node_base* q)
      { p->prev_in = q; }
    };

    /**
    * The vertex node base implements the non-dependent linking structure for
    * vertices in an adjacency list. A vertex is linked to several different
    * components in a graph. The next/prev members define links in the graph's
    * edge list while the out- and in-edge node lists represent the incident
    * edges of the vertex.
    */
    struct vertex_node_base
    {
      typedef void value_type;
      typedef vertex_node_base* vertex_pointer;
      typedef edge_node_base* edge_pointer;
      typedef adjacency_list_::node_list<edge_node_base, out_linkage> out_list;
      typedef adjacency_list_::node_list<edge_node_base, in_linkage> in_list;
      typedef std::size_t size_type;

      vertex_node_base()
        : next(), prev(), out(), in()
      { }

      /** Return the out degree of this vertex. */
      size_type out_degree() const
      { return out.size(); }

      /** Return the in degree of this vertex. */
      size_type in_degree() const
      { return in.size(); }

      /** Return the cumulative degree of this vertex. */
      size_type degree() const
      { return out_degree() + in_degree(); }

      vertex_pointer next;  ///< The next vertex in the graph
      vertex_pointer prev;  ///< The previous vertex in the graph
      out_list out;         ///< The out edge list of the vertex
      in_list in;           ///< The in edge list of the vertex
    };

    // FIXME: Use the empty base optimization to opportunistically reduce the
    // size of vertex and edge nodes if these proporties are given as empty.

    /**
     * The vertex node represents a vertex in an adjacency list. The node is
     * parameterized over a user-defined value type.
     */
    template<typename V>
    struct vertex_node
      : vertex_node_base
    {
      typedef V value_type;

      // Forwarding constructor
      template<typename... Args>
      vertex_node(Args&&... args)
        : vertex_node_base(), data(std::forward<Args>(args)...)
      { }

      value_type data;
    };

    /**
     * The edge node represetns an edge in an adjacency list. The node is
     * parameterized over a user-defined value type.
     */
    template<typename E>
    struct edge_node
      : edge_node_base
    {
      typedef typename edge_node_base::vertex_pointer vertex_pointer;
      typedef E value_type;

      // Forwarding constructor
      template<typename... Args>
      edge_node(vertex_pointer u, vertex_pointer v, Args&&... args)
        : edge_node_base(u, v), data(std::forward<Args>(args)...)
      { }

      value_type data;
    };

    /**
     * The edge iterator template is a special kind of a node list iterator,
     * that provides explicit access to the source and target vertices of the
     * edge.
     *
     * @note Const versions of these iterators are simply constructed over
     * the const vertex and edge iterator components.
     *
     * @tparam Edge_Iter A node list iterator for an edge list.
     * @tparam Vertex_Iter A node list iterator for a vertex list.
     */
    template<typename Edge_Iter, typename Vertex_Iter>
    class edge_iterator
      : public Edge_Iter
    {
      typedef Edge_Iter base_type;
    public:
      typedef typename base_type::node_pointer edge_node_pointer;

      typedef Vertex_Iter vertex_iterator;
      typedef typename vertex_iterator::node_pointer vertex_node_pointer;

      edge_iterator(edge_node_pointer p = nullptr)
        : base_type{p}
      { }

      edge_iterator& operator=(edge_node_pointer p)
      { base_type::operator=(p); return * this; };

      // FIXME: I may need a special copy operation to support const/non-const
      // interoperability.

      vertex_iterator source() const
      { return static_cast<vertex_node_pointer>(this->node()->source); }

      vertex_iterator target() const
      { return static_cast<vertex_node_pointer>(this->node()->target); }
    };



    /**
     * @internal
     * The adjacency list data structure provides the underlying implementation
     * for directed and undirected adjacency lists. This exposes all of its
     * iterfaces in terms of node operations.
     */
    template<typename Vertex = empty_t,
             typename Edge = empty_t,
             typename Alloc = std::allocator<Vertex>>
    class impl
    {
      typedef impl<Vertex, Edge, Alloc> this_type;

      // Node base types.
      typedef vertex_node_base vertex_base_type;
      typedef edge_node_base edge_base_type;
    public:
      // Basic types.
      typedef Vertex vertex_value_type;
      typedef Edge edge_value_type;
      typedef Alloc allocator_type;

      // Vertex set types
      typedef vertex_node<vertex_value_type> vertex_type;
      typedef edge_node<edge_value_type> edge_type;

      typedef adjacency_list_::node_list<vertex_base_type> vertex_list;
      typedef adjacency_list_::node_list<edge_base_type> edge_list;
      typedef typename vertex_base_type::out_list out_list;
      typedef typename vertex_base_type::in_list in_list;

      // Specific allocator types
      // FIXME: Should these be private?
      typedef typename allocator_type::template rebind<vertex_type>::other
        vertex_allocator_type;
      typedef typename allocator_type::template rebind<edge_type>::other
        edge_allocator_type;

      // NOTE: This is effectively defined as the maximum degree of any vertex
      // in the graph, which is essentially required to be >= to the number of
      // vertices.
      typedef typename edge_allocator_type::size_type size_type;
      typedef typename edge_allocator_type::difference_type difference_type;

      /** @name Cosntruction, Assignment and Destruction. */
      //@{
      // Default construction and destruction.
      impl(allocator_type const& alloc = allocator_type{})
        : vertices_{}, edges_{}, alloc_{alloc}
      { }

      // Copy semantics
      impl(impl const& x);

      impl& operator=(impl const& x)
      { impl tmp(x); swap(tmp); return *this; }

      // Move semantics
      impl(impl&& x)
        : vertices_{}, edges_{}, alloc_{}
      { swap(x); }

      impl& operator=(impl&& x)
      { swap(x); return *this; }

      // Order constructor
      impl(size_type n, allocator_type const& alloc = allocator_type{});

      // FIXME: Range constructors? Others?

      ~impl()
      { clear(); }
      //@}

      /** @name Properties */
      //@{
      allocator_type get_allocator() const
      { return alloc_; }

      vertex_allocator_type get_vertex_allocator() const
      { return get_allocator(); }

      edge_allocator_type get_edge_allocator() const
      { return get_allocator(); }

      constexpr size_type max_order() const
      { return get_vertex_allocator().max_size(); }

      constexpr size_type max_size() const
      { return get_edge_allocator().max_size(); }

      // FIXME: I don't like the operation name get_value, but I'm also
      // not convinced about the function call operator or the square brackets
      // operator. Meh.
      vertex_value_type& get_value(vertex_type* v)
      { return v->data; }

      vertex_value_type const& get_value(vertex_type const* v) const
      { return v->data; }

      edge_value_type& get_value(edge_type* e)
      { return e->data; }

      edge_value_type const& get_value(edge_type const* e) const
      { return e->data; }
      //@}

      /** @name Graph Properties */
      //@{
      bool null() const
      { return vertices_.empty(); }

      size_type order() const
      { return vertices_.size(); }

      bool empty() const
      { return edges_.empty(); }

      size_type size() const
      { return edges_.size(); }
      //@}

      /** @name Vertex Operations and Properties */
      //@{
      vertex_type* add_vertex(vertex_value_type const& x = vertex_value_type{});

      void remove_vertex(vertex_type* v);

      size_type degree(vertex_type const* v) const
      { return v->degree(); }

      size_type out_degree(vertex_type const* v) const
      { return v->out_degree(); }

      size_type in_degree(vertex_type const* v) const
      { return v->in_degree(); }
      //@}

      /** @name Edge Operations and Properties */
      //@{
      edge_type* add_edge(vertex_type* u,
                          vertex_type* v,
                          edge_value_type const& x = edge_value_type{});

      void add_edge(vertex_type* u, vertex_type* v, edge_type* e);

      void remove_edge(edge_type* e);
      void remove_edges(vertex_type* u, vertex_type* v);
      void remove_edges(vertex_type* v);
      void remove_edges();

      // Edge access
      edge_type* get_edge(vertex_type* u, vertex_type* v);
      edge_type const* get_edge(vertex_type const* u, vertex_type const* v) const;

      // FIXME: Build multigraph support---get_edges(). I'll need a new
      // range type for this.

      // Source access (from edge)
      vertex_type* source(edge_type* e)
      { return cast(e->source); }

      vertex_type const* source(edge_type const* e) const
      { return cast(e->source); }

      // Target access (from edge)
      vertex_type* target(edge_type *e)
      { return cast(e->target); }

      vertex_type const* target(edge_type const* e) const
      { return cast(e->target); }
      //@}

      /** @name Graph Operations */
      //@{
      void swap(impl& x);
      void clear();
      //@}

    protected:
      // Allocate and construct a vertex or edge.
      template<typename ElemAlloc, typename... Args>
      typename ElemAlloc::pointer create(ElemAlloc alloc, Args&&...);

      // Destroy and delete a vertex or edge.
      template<typename ElemAlloc>
      void destroy(ElemAlloc alloc, typename ElemAlloc::pointer p);

      // Determine if two graphs are equivalent.
      bool equal(impl const& x) const;

      // Downcasting support
      static vertex_type* cast(vertex_base_type* v)
      { return static_cast<vertex_type*>(v); }

      static vertex_type const* cast(vertex_base_type const* v)
      { return static_cast<vertex_type const*>(v); }

      static edge_type* cast(edge_base_type* v)
      { return static_cast<edge_type*>(v); }

      static edge_type const* cast(edge_base_type const* v)
      { return static_cast<edge_type const*>(v); }

    protected:
      // FIXME: The layout of this object is sub-optimal. Use EBO to remove the
      // allocator overhead.
      vertex_list vertices_;
      edge_list edges_;
      allocator_type alloc_;
    };

    /** Cosntruct and allocate a vertex or edge element. */
    template<typename V, typename E, typename A>
    template<typename Alloc, typename... Args>
    inline typename Alloc::pointer
    impl<V, E, A>::create(Alloc alloc, Args&&... args)
    {
      typename Alloc::pointer p = alloc.allocate(1);
      try {
        alloc.construct(p, std::forward<Args>(args)...);
      } catch(...) {
        alloc.deallocate(p, 1);
        throw;
      }
      return p;
    }

    /**
     * Destroy and deallocate a vertex or edge object.
     *
     * Note that destroying a nullptr is not permitted in this implementation.
     * The restriction is intended to help ensure that we're only calling
     * destroy when we have valid objects, which reduces the number of test
     * cases.
     *
     * @pre p != nullptr
     */
    template<typename V, typename E, typename A>
    template<typename Alloc>
    inline void
    impl<V, E, A>::destroy(Alloc alloc, typename Alloc::pointer p)
    {
      assert(( p != nullptr ));
      alloc.destroy(p);
      alloc.deallocate(p, 1);
    }

    /**
     * Return true if this graph is equivalent to the other. By default, two
     * adjacency lists are equivalent if one is a copy of the other.
     */
    template<typename V, typename E, typename A>
    bool impl<V, E, A>::equal(impl const& x) const
    {
      // If the dimensions of the graph don't match, then they cannot be
      // equivalent.
      if(order() != x.order() || size() != x.size()) {
        return false;
      }

      // Construct a label over the vertices in each graph.
      typedef std::unordered_map<vertex_base_type const*, size_type> Label;
      Label gl{order()}, hl{x.order()};
      size_type n = 0;
      vertex_base_type const* gv = vertices_.head();
      vertex_base_type const* hv = x.vertices_.head();
      while(gv && hv) {
        gl[gv] = n;
        hl[hv] = n;

        // FIXME: If the vertex value types are not equivalent, then we
        // should return false, here.

        ++n;
        gv = vertices_.next(gv);
        hv = x.vertices_.next(hv);
      }

      // Check edge set equality. For each pair of edges, the source and target
      // must have the same vertex labels.
      edge_node_base const* ge = edges_.head();
      edge_node_base const* he = x.edges_.head();
      while(ge && he) {
        int gul = gl[ge->source], gvl = gl[ge->target];
        int hul = hl[he->source], hvl = hl[he->target];
        if(gul != hul || gvl != hvl) {
          return false;
        }

        // FIXME: If the edge value types are not equivalent, then we should
        // return false.

        ge = edges_.next(ge);
        he = x.edges_.next(he);
      }
      return true;
    }

    // FIXME: Do I have to pass the allocator to the different vertex/edge
    // sets here? I think I might.
    /** Construct a copy of the given graph. */
    template<typename V, typename E, typename A>
    impl<V, E, A>::impl(impl const& x)
      : vertices_{}, edges_{}, alloc_{x.alloc_}
    {
      typedef std::unordered_map<vertex_base_type const*, vertex_type*> Map;

      // While adding vertices, construct a mapping from the vertices in x
      // to those in this class.
      Map verts(vertices_.size());
      vertex_base_type const* v = x.vertices_.head();
      while(v) {
        verts[v] = add_vertex(cast(v)->data);
        v = x.vertices_.next(v);
      }

      // Add all of the edges, using the vertex mapping to ensure that the
      // endpoints are correct.
      edge_base_type const* e = x.edges_.head();
      while(e) {
        add_edge(verts[e->source], verts[e->target], cast(e)->data);
        e = x.edges_.next(e);
      }
    }

    /** Construct a graph with order n. */
    template<typename V, typename E, typename A>
    impl<V, E, A>::impl(size_type n, allocator_type const& alloc)
        : vertices_{}, edges_{}, alloc_{alloc}
    {
      for( ; n > 0; --n) {
        add_vertex();
      }
    }

    /** Add a vertex to the graph. */
    template<typename V, typename E, typename A>
    inline auto
    impl<V, E, A>::add_vertex(vertex_value_type const& x)
      -> vertex_type*
    {
      vertex_type* v = create(get_vertex_allocator(), x);
      vertices_.push_back(v);
      return v;
    }

    /**
     * Remove a vertex from the graph, including any edges incident on that
     * vertex.
     */
    template<typename V, typename E, typename A>
    void
    impl<V, E, A>::remove_vertex(vertex_type* v)
    {
      remove_edges(v);
      vertices_.erase(v);
      destroy(get_vertex_allocator(), v);
    }

    /** Add an edge connecting the given vertices. */
    template<typename V, typename E, typename A>
    auto
    impl<V, E, A>::add_edge(vertex_type* u,
                            vertex_type* v,
                            edge_value_type const& x)
      -> edge_type*
    {
      edge_type* e = create(get_edge_allocator(), u, v, x);
      edges_.push_back(e);
      u->out.push_back(e);
      v->in.push_back(e);
      return e;
    }

    // FIXME: Implement this function!
    /** Connect the vertices u and v by the given edge. */
    template<typename V, typename E, typename A>
    void
    impl<V, E, A>::add_edge(vertex_type* u, vertex_type* v, edge_type* e)
    { throw not_implemented("impl::add_edge(u, v, e)"); }

    /** Remove an edge from the graph. */
    template<typename V, typename E, typename A>
    void
    impl<V, E, A>::remove_edge(edge_type* e)
    {
      e->source->out.erase(e);
      e->target->in.erase(e);
      edges_.erase(e);
      destroy(get_edge_allocator(), e);
    }

    // FIXME: I'm having a really hard time implementing algorithms that both
    // iterate and erase. Basically, we have a problem where the head can shift
    // during iteration, which wreaks havoc with the loop conditions. The best
    // solution so far seems to be iterating over [front, back) and then
    // applying the loop code once more on back. Not terribly graceful.

    /** Remove all edges connecting vertex u to vertex v. */
    template<typename V, typename E, typename A>
    void
    impl<V, E, A>::remove_edges(vertex_type* u, vertex_type* v)
    {
      edge_base_type* e = u->out.head();
      while(e) {
        if(e->target == v) {
          edges_.erase(e);
          v->in.erase(e);
          edge_type* x = cast(e);
          e = u->out.erase(e);
          destroy(get_edge_allocator(), x);
        } else {
          e = u->out.next(e);
        }
      }
    }

    /** Remove all edges incident on the vertex v. */
    template<typename V, typename E, typename A>
    void
    impl<V, E, A>::remove_edges(vertex_type* v)
    {
      // Erase in edges (u, v) incident on v.
      edge_base_type* ie = v->in.head();
      while(ie) {
        vertex_type* u = cast(ie->source);
        edges_.erase(ie);
        u->out.erase(ie);
        edge_type* x = cast(ie);
        ie = v->in.erase(ie);
        destroy(get_edge_allocator(), x);
      }

      edge_base_type *oe = v->out.head();
      while(oe) {
        vertex_type* w = cast(oe->target);
        edges_.erase(oe);
        w->in.erase(oe);
        edge_type *x = cast(oe);
        oe = v->out.erase(oe);
        destroy(get_edge_allocator(), x);
      }
    }

    /** Remove all of the edges in the graph. */
    template<typename V, typename E, typename A>
    void impl<V, E, A>::remove_edges()
    {
      // Start by simply deleting all edges. Note that vertices will point to
      // deleted edges after this operation. These get cleaned up later.
      edge_base_type* e = edges_.head();
      while(e) {
        edge_type* x = cast(e);
        e = edges_.erase(e);
        destroy(get_edge_allocator(), x);
      }

      // Clean up the loose ends on the vertices.
      vertex_base_type* v = vertices_.head();
      while(v) {
        v->out.clear();
        v->in.clear();
        v = vertices_.next(v);
      }
    }

    /** 
     * Return the first edge (if any) connecting the vertex u to v. 
     */
    template<typename V, typename E, typename A>
    auto
    impl<V, E, A>::get_edge(vertex_type* u, vertex_type* v) -> edge_type*
    {
      edge_base_type* e = u->out.head();
      while(e) {
        if(e->target == v) {
          return cast(e);
        }
        e = u->out.next(e);
      }
      return nullptr;
    }

    template<typename V, typename E, typename A>
    auto
    impl<V, E, A>::get_edge(vertex_type const* u, vertex_type const* v) const
      -> edge_type const*
    {
      edge_base_type const* e = u->out.head();
      while(e) {
        if(e->target == v) {
          return cast(e);
        }
        e = u->out.next(e);
      }
      return nullptr;
    }

    template<typename V, typename E, typename A>
    void impl<V, E, A>::clear()
    {
      edge_base_type*e = edges_.head();
      while(e) {
        edge_type* x = cast(e);
        e = edges_.erase(e);
        destroy(get_edge_allocator(), x);
      }

      vertex_base_type* v = vertices_.head();
      while(v) {
        vertex_type* x = cast(v);
        v = vertices_.erase(v);
        destroy(get_vertex_allocator(), x);
      }
    }

    template<typename V, typename E, typename G>
    void impl<V, E, G>::swap(impl& x)
    {
      using std::swap;
      vertices_.swap(x.vertices_);
      edges_.swap(x.edges_);

      // FIXME: Use allocator traits to support more intelligence about
      // allocator swapping.
      alloc_.swap(x.alloc_);
    }
  } // namespace adjacency_list_

} // namespace origin

#endif
