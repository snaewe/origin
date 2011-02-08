// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_LIST_UNDIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_LIST_UNDIRECTED_HPP

#include <origin/graph/adjacency_list/directed.hpp>

namespace origin
{
  /** @internal */
  namespace adjacency_list_
  {
    /**
     * The adjacency lists' undirected edge type wraps a pointer to an
     * underlying edge type, and also includes a source vertex. This allows
     * the edge to retain source/target information.
     *
     * An undirected edge is valid if it's edge pointer is non-null. If the
     * edge is non-null, then the source vertex must also be non-null.
     *
     * Two undirected edges are equal if they refer to the same underlying
     * edge.
     *
     * @invariant e.edge() => e.source()
     */
    template<typename Edge, typename Vertex>
    class undirected_edge
      : public implicit_bool_facade<undirected_edge<Edge, Vertex>>
    {
    public:
      typedef Edge* edge_pointer;
      typedef Vertex* vertex_pointer;

      undirected_edge(edge_pointer e = nullptr,
                      vertex_pointer s = nullptr)
        : edge_(e), source_(s)
      { assert(( e != nullptr ? s != nullptr : true )); }

      bool valid() const
      { return edge_; }

      bool equal(undirected_edge const& x) const
      { return edge_ == x.edge_; }

      edge_pointer edge() const
      { return edge_; }

      vertex_pointer source() const
      { return source_; }

      vertex_pointer target() const
      { return static_cast<vertex_pointer>(edge_->opposite(source_)); }

    private:
      edge_pointer edge_;
      vertex_pointer source_;
    };

    /**
     * The edge list iterator for undirected graphs wraps an underlying node
     * iterator (specifially an edge iterator) and returns undirected edge
     * objects when dereferenced.
     */
    template<typename Iter, typename Edge, typename Vertex>
    class undirected_edge_iterator
      : public bidirectional_iterator_facade<
          undirected_edge_iterator<Iter, Edge, Vertex>,
          undirected_edge<Edge, Vertex>
        >
    {
      typedef Iter base_iterator;
      typedef Vertex* vertex_pointer;
    public:
      typedef undirected_edge<Edge, Vertex> value_type;
      typedef value_type reference;
      typedef value_type pointer;
      typedef typename std::iterator_traits<base_iterator>::difference_type difference_type;

      undirected_edge_iterator(base_iterator i)
        : iter_{i}
      { }

      reference dereference() const
      { return {*iter_, static_cast<vertex_pointer>((*iter_)->source)}; }

      bool equal(undirected_edge_iterator const& x) const
      { return iter_ == x.iter_; }

      void increment()
      { ++iter_; }

      void decrement()
      { --iter_; }

    private:
      base_iterator iter_;
    };

    /**
     * The incident edge iterator chains together the in and out edges of a
     * directed graph's out and in edges (in that order). Dereferencing an
     * incident edge iterator results in an undirected edge.
     *
     * @tparam Out An out edge iterator.
     * @tparam In An in edge iterator.
     * @tparam Edge The edge type.
     * @tparam Vertex The vertex type.
     *
     * @note The implementatio of this class relies heavily on the fact that
     * a default constructed node list iterator is PTE. By relying on this fact,
     * the implementation can omit a reference to the parent graph type.
     */
    template<typename Out, typename In, typename Edge, typename Vertex>
    class incident_edge_iterator
      : public bidirectional_iterator_facade<
          incident_edge_iterator<Out, In, Edge, Vertex>,
          undirected_edge<Edge, Vertex>
        >
    {
      typedef Out out_iterator;
      typedef In in_iterator;
      typedef Vertex* vertex_pointer;
    public:
      typedef undirected_edge<Edge, Vertex> value_type;
      typedef value_type reference;
      typedef value_type pointer;
      typedef typename std::iterator_traits<out_iterator>::difference_type difference_type;

      incident_edge_iterator(out_iterator out = out_iterator{},
                             in_iterator in = in_iterator{},
                             vertex_pointer src = nullptr)
        : out_{out}, in_{in}, source_{src}
      { }

      bool equal(incident_edge_iterator const& x) const
      { return out_ == x.out_ && in_ == x.in_; }

      reference dereference() const
      { return {(out_pte() ? *in_ : *out_), source_}; }

      void increment()
      {
        if(!out_pte()) {
          ++out_;
        } else if(!in_pte()) {
          ++in_;
        }
      }

      // FIXME: Implement this function.
      void decrement()
      { throw not_implemented{"incident_edge_iterator::decrement"}; }

    private:
      // Return true if we are no longer in the "out" section of the chain.
      // NOTE: This relies on the fact that PTE for node list iterators is a
      // default initialized iterator (i.e., a nullptr). We could use
      // end(graph_->out_edges(source_)), but what's the point?
      bool out_pte() const
      { return out_ == out_iterator{}; }

      bool in_pte() const
      { return in_ == in_iterator{}; }

    private:
      out_iterator out_;
      in_iterator in_;
      vertex_pointer source_;
    };
  } // namespace adjacency_list_

  /**
   * The undirected adjacency list class implements a undirected graph.
   *
   * In actuality, the undirected graph is really an adaptor on an underlying
   * directed graph that
   *
   * @tparam Vertex A value type associated with vertices.By default, this
   * type is empty. Vertex must be a Regular type.
   *
   * @tparam Edge A value type associated with edges. By default, this type
   * is empty. Edge must be a Regular type.
   *
   * @tparam Alloc An Allocator type.
   */
  template<typename Vertex = empty_t,
           typename Edge = empty_t,
           typename Alloc = std::allocator<Vertex>>
  class undirected_adjacency_list
    : private directed_adjacency_list<Vertex, Edge, Alloc>
  {
    typedef adjacency_list_::impl<Vertex, Edge, Alloc> impl_type;
    typedef directed_adjacency_list<Vertex, Edge, Alloc> base_type;
    typedef undirected_adjacency_list<Vertex, Edge, Alloc> this_type;

  public:
    struct graph_category : undirected_graph_tag { };

    typedef Vertex vertex_value_type;
    typedef Edge edge_value_type;
    typedef Alloc allocator_type;

    typedef typename base_type::size_type size_type;
    typedef typename base_type::difference_type difference_type;

  private:
    typedef typename base_type::vertex_type vertex_type;
    typedef typename base_type::edge_type edge_type;
  public:

    // Vertex and edge types.
    typedef typename base_type::vertex vertex;
    typedef typename base_type::const_vertex const_vertex;
    typedef adjacency_list_::undirected_edge<edge_type, vertex_type> edge;
    typedef adjacency_list_::undirected_edge<edge_type const, vertex_type const> const_edge;

    // Iterator types.
    typedef typename base_type::vertex_iterator vertex_iterator;
    typedef typename base_type::const_vertex_iterator const_vertex_iterator;

    typedef adjacency_list_::undirected_edge_iterator<
      typename base_type::edge_iterator, edge_type, vertex_type
    > edge_iterator;
    typedef adjacency_list_::undirected_edge_iterator<
      typename base_type::const_edge_iterator, edge_type const, vertex_type const
    > const_edge_iterator;

    typedef adjacency_list_::incident_edge_iterator<
      typename base_type::out_edge_iterator,
      typename base_type::in_edge_iterator,
      edge_type, vertex_type
    > incident_edge_iterator;
    typedef adjacency_list_::incident_edge_iterator<
      typename base_type::const_out_edge_iterator,
      typename base_type::const_in_edge_iterator,
      edge_type const, vertex_type const
    > const_incident_edge_iterator;

    // Range types.
    typedef typename base_type::vertex_range vertex_range;
    typedef typename base_type::const_vertex_range const_vertex_range;

    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<const_edge_iterator> const_edge_range;

    typedef iterator_range<incident_edge_iterator> incident_edge_range;
    typedef iterator_range<const_incident_edge_iterator> const_incident_edge_range;

    /** @name Cosntruction, Assignment and Destruction. */
    //@{
    // Default construction and destruction.
    undirected_adjacency_list(allocator_type const& alloc = allocator_type{})
      : base_type{alloc}
    { }

    // Copy semantics
    undirected_adjacency_list(undirected_adjacency_list const& x)
      : base_type{x}
    { }

    undirected_adjacency_list& operator=(undirected_adjacency_list const& x)
    { undirected_adjacency_list tmp{x}; swap(tmp); return *this; }

    // Move semantics
    undirected_adjacency_list(undirected_adjacency_list&& x)
      : base_type{std::move(x)}
    { }

    undirected_adjacency_list& operator=(undirected_adjacency_list&& x)
    { swap(x); return *this; }

    // Order constructor
    undirected_adjacency_list(size_type n, allocator_type const& alloc = allocator_type{});

    // FIXME: Range constructors? Others?

    ~undirected_adjacency_list()
    { clear(); }
    //@}

    /** @name Properties */
    //@{
    bool equal(undirected_adjacency_list const& x) const
    { return base_type::equal(x); }
    //@}

    /** @name Properties */
    //@{
    allocator_type get_allocator() const
    { return base_type::get_allocator(); }

    constexpr size_type max_order() const
    { return base_type::max_size(); }

    constexpr size_type max_size() const
    { return base_type::max_size(); }
    //@}

    /** @name Graph Properties */
    //@{
    bool null() const
    { return base_type::null(); }

    size_type order() const
    { return base_type::order(); }

    bool empty() const
    { return base_type::empty(); }

    size_type size() const
    { return base_type::size(); }
    //@}

    /** @name Value Type Access */
    //@{
    vertex_value_type& operator[](vertex v)
    { return impl_type::get_value(v); }

    vertex_value_type const& operator[](const_vertex v) const
    { return impl_type::get_value(v); }

    edge_value_type& operator[](edge e)
    { return impl_type::get_value(e.edge()); }

    edge_value_type const& operator[](const_edge e) const
    { return impl_type::get_value(e.edge()); }
    //@}

    /** @name Graph Operations */
    //@{
    void swap(undirected_adjacency_list& x)
    { base_type::swap(x); }

    void clear()
    { base_type::clear(); }
    //@}

    /** @name Vertex Operations and Properties */
    //@{
    vertex add_vertex(vertex_value_type const& x = vertex_value_type{})
    { return base_type::add_vertex(x); }

    void remove_vertex(vertex v)
    { base_type::remove_vertex(v); }

    size_type degree(const_vertex v) const
    { return base_type::degree(v); }
    //@}

    /** @name Edge Operations and Properties */
    //@{
    edge add_edge(vertex u, vertex v, edge_value_type const& x = edge_value_type{})
    { return {base_type::add_edge(u, v, x), u}; }

    void remove_edge(edge e)
    { base_type::remove_edge(e.edge()); }

    void remove_edges(vertex u, vertex v)
    {
      base_type::remove_edges(u, v);
      base_type::remove_edges(v, u);
    }

    void remove_edges(vertex v)
    { base_type::remove_edges(v); }

    void remove_edges()
    { base_type::remove_edges(); }

    // Edge access
    // Note that the resulting edge will have the source vertex rooted at the
    // vertex for which the edge was actually found.
    edge get_edge(vertex u, vertex v)
    {
      auto e = base_type::get_edge(u, v);
      return e ? edge{e, u} : edge{base_type::get_edge(v, u), v};
    }

    const_edge get_edge(const_vertex u, const_vertex v) const
    {
      auto e = base_type::get_edge(u, v);
      return e ? edge{e, u} : edge{base_type::get_edge(v, u), v};
    }

    vertex source(edge e)
    { return e.source(); }

    const_vertex source(const_edge e) const
    { return e.source(); }

    vertex target(edge e)
    { return e.target(); }

    const_vertex target(const_edge e) const
    { return e.target(); }
    //@}

    /** @name Ranges */
    //@{
    vertex_range vertices()
    { return base_type::vertices(); }

    const_vertex_range vertices() const
    { return base_type::vertices(); }

    edge_range edges()
    {
      auto r = base_type::edges();
      return {begin(r), end(r)};
    }

    const_edge_range edges() const
    {
      auto r = base_type::edges();
      return {begin(r), end(r)};
    }

    incident_edge_range incident_edges(vertex v);
    const_incident_edge_range incident_edges(const_vertex v) const;
    //@}
  };

  // Out edge range
  template<typename V, typename E, typename A>
  inline auto
  undirected_adjacency_list<V, E, A>::incident_edges(vertex v)
    -> incident_edge_range
  {
    assert(( v ));
    return {
      incident_edge_iterator{
        begin(base_type::out_edges(v)), begin(base_type::in_edges(v)), v
      },
      incident_edge_iterator{}
    };
  }

  template<typename V, typename E, typename A>
  inline auto
  undirected_adjacency_list<V, E, A>::incident_edges(const_vertex v) const
    -> const_incident_edge_range
  {
    assert(( v ));
    return {
      incident_edge_iterator{
        begin(base_type::out_edges(v)), begin(base_type::in_edges(v)), v
      },
      incident_edge_iterator{}
    };
  }

} // namesapce origin

#endif
