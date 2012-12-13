// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_LIST_HPP
#define ORIGIN_GRAPH_ADJACENCY_LIST_HPP

#include <cassert>

#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include <origin/type/concepts.hpp>
#include <origin/type/empty.hpp>
#include <origin/type/typestr.hpp>
#include <origin/type/functional.hpp>
#include <origin/sequence/algorithm.hpp>
#include <origin/sequence/range.hpp>

#include <origin/graph/handle.hpp>
#include <origin/graph/graph.hpp>
#include <origin/graph/io.hpp>

#include <origin/graph/adjacency_list.impl/pool.hpp>

namespace origin
{

  namespace adjacency_list_impl
  {
    // ---------------------------------------------------------------------- //
    //                            Handle Iterator
    //
    // The handle iterator wraps a pool iterator and returns vertex handles
    // when dereferenced. Vertex iteratrs are forward iterators. Here, H is
    // specific kind of handle being returned -- either vertex_handle or
    // edge_handle for adjacency list class.
    template<typename I, typename H>
      struct handle_iterator
      {
        using handle_type = H;

        handle_iterator(I i)
          : iter(i)
        { }

        handle_type operator*() const { return iter.index(); }

        handle_iterator& operator++();
        handle_iterator  operator++(int);

        I iter;
      };

    template<typename I, typename H>
      inline handle_iterator<I, H>&
      handle_iterator<I, H>::operator++()
      {
        ++iter;
        return *this;
      }

    template<typename I, typename H>
      inline handle_iterator<I, H>
      handle_iterator<I, H>::operator++(int)
      {
        handle_iterator tmp = *this;
        ++iter;
        return tmp;
      }

    // Equality
    template<typename I, typename H>
      inline bool
      operator==(const handle_iterator<I, H>& a, const handle_iterator<I, H>& b)
      {
        return a.iter == b.iter;
      }

    template<typename I, typename H>
      inline bool
      operator!=(const handle_iterator<I, H>& a, const handle_iterator<I, H>& b)
      {
        return a.iter != b.iter;
      }
  

    struct vertex_base;
    struct edge_base;


    // ---------------------------------------------------------------------- //
    //                           Vertex Representation
    
    // A vertex in the adjacency list is implemented as a pair of edge lisst. An
    // edge list is simoply a vector of indexes that refer to edges in a
    // separate edge container.
    //
    // Note that the class will compress the value type if it is empty.
    template<typename V>
      struct vertex
      {
        using value_type = V;
        using edge_list = std::vector<std::size_t>;
        using iterator = typename edge_list::iterator;
    
        vertex()
          : data()
        { }

        template<typename... Args>
          vertex(Args&&... args) 
            : data(edge_list{}, edge_list{}, std::forward<Args>(args)...)
          { }

        // Returns the out ege list
        edge_list&       out()       { return std::get<0>(data); }
        const edge_list& out() const { return std::get<0>(data); }
        
        // Returns the in edge list
        edge_list&       in()       { return std::get<1>(data); }
        const edge_list& in() const { return std::get<1>(data); }

        // Returns the user-supplied data object.
        V&       value()       { return std::get<2>(data); }
        const V& value() const { return std::get<2>(data); }

        // Out edges
        std::size_t out_degree() const { return out().size(); }

        void insert_out(std::size_t e) { insert_edge(out(), e); }
        void erase_out(std::size_t e)  { erase_edge(out(), e); }

        iterator begin_out() { return out().begin(); }
        iterator end_out()   { return out().end(); }

        // In edges
        std::size_t in_degree() const { return in().size(); }
        
        void insert_in(std::size_t e) { insert_edge(in(), e); }
        void erase_in(std::size_t e)  { erase_edge(in(), e); }

        iterator begin_in() { return in().begin(); }
        iterator end_in()   { return in().end(); }

        // Helper functions
        void insert_edge(edge_list& l, std::size_t e);
        void erase_edge(edge_list& l, std::size_t e);

      public:
        std::tuple<edge_list, edge_list, V> data;
      };

    template<typename V>
      inline void
      vertex<V>::insert_edge(edge_list& l, std::size_t e)
      {
        l.push_back(e);
      }

    template<typename V>
      inline void
      vertex<V>::erase_edge(edge_list& l, std::size_t e)
      {
        auto i = std::find(l.begin(), l.end(), e);
        if (i != l.end())
          l.erase(i);
      }

    // A vertex set is a pool of vertices.
    template<typename V>
      using vertex_pool = pool<vertex<V>>;

    // An alias for the vertex iterator.
    template<typename V>
      using vertex_iterator 
        = handle_iterator<Iterator_of<const vertex_pool<V>>, vertex_handle>;

    // An alias for the vertex range.
    template<typename V>
      using vertex_range = bounded_range<vertex_iterator<V>>;


    // ---------------------------------------------------------------------- //
    //                            Edge Representation

    template<typename E>
      struct edge
      {
        using value_type = E;

        edge()
          : data(-1, -1, E{})
        { }

        edge(std::size_t s, std::size_t t)
          : data(s, t, E{})
        { }

        template<typename... Args>
          edge(std::size_t s, std::size_t t, Args&&... args)
            : data(s, t, std::forward<Args>(args)...)
          { }

        std::size_t& source()       { return std::get<0>(data); }
        std::size_t  source() const { return std::get<0>(data); }

        std::size_t& target()       { return std::get<1>(data); }
        std::size_t  target() const { return std::get<1>(data); }

        E&       value()       { return std::get<2>(data); }
        const E& value() const { return std::get<2>(data); }

        std::tuple<std::size_t, std::size_t,  E> data;
      };

    // An alias for the edge pool.
    template<typename E>
      using edge_pool = pool<edge<E>>;

    // An alias for the vertex iterator.
    template<typename E>
      using edge_iterator 
        = handle_iterator<Iterator_of<const edge_pool<E>>, edge_handle>;

    // An alias for the edge range.
    template<typename E>
      using edge_range = bounded_range<edge_iterator<E>>;

  } // namespace adjacency_list_impl


  // ------------------------------------------------------------------------ //
  //                        Directed Adjacency List
  //
  // Blah blah blah
  template<typename V = empty_t, typename E = empty_t>
    class directed_adjacency_list
    {
      using vertex_node = adjacency_list_impl::vertex<V>;
      using vertex_set = adjacency_list_impl::vertex_pool<V>;
      using vertex_iter = adjacency_list_impl::vertex_iterator<V>;

      using edge_node = adjacency_list_impl::edge<E>;
      using edge_set = adjacency_list_impl::edge_pool<E>;
      using edge_iter = adjacency_list_impl::edge_iterator<E>;
    public:
      using vertex = vertex_handle;
      using vertex_range = adjacency_list_impl::vertex_range<V>;

      using edge = edge_handle;
      using edge_range = adjacency_list_impl::edge_range<E>;


      // Observers
      bool        null() const  { return verts_.empty(); }
      std::size_t order() const { return verts_.size(); }

      bool        empty() const { return edges_.empty(); }
      std::size_t size() const  { return edges_.size(); }

      // Vertex observers
      std::size_t out_degree(vertex v) const { return get_vertex(v).out_degree(); }
      std::size_t in_degree(vertex v) const  { return get_vertex(v).in_degree(); }
      std::size_t degree(vertex v) const { return out_degree(v) + in_degree(v); }

      // Edge observers
      vertex source(edge e) const { return get_edge(e).source(); }
      vertex target(edge e) const { return get_edge(e).target(); }

      // Data access
      V&       operator()(vertex v)       { return get_vertex(v).value(); }
      const V& operator()(vertex v) const { return get_vertex(v).value(); }

      E&       operator()(edge e)       { return get_edge(e).value(); }
      const E& operator()(edge e) const { return get_edge(e).value(); }

      // Edge relation
      bool operator()(vertex u, vertex v) const;

      // Vertex set
      vertex add_vertex();
      vertex add_vertex(V&& x);
      vertex add_vertex(const V& x);
      void remove_vertex(vertex v);
      void remove_vertices();

      // Edge set
      edge add_edge(vertex u, vertex v);
      edge add_edge(vertex u, vertex v, E&& x);
      edge add_edge(vertex u, vertex v, const E& x);
      void remove_edge(edge e);
      void remove_edge(vertex u, vertex v);
      void remove_edges(vertex u, vertex v);
      void remove_edges(vertex v);
      void remove_edges();


      // Iterators
      vertex_range vertices() const;
      edge_range   edges() const;

    private:
      vertex_node&       get_vertex(vertex v)       { return verts_[v]; }
      const vertex_node& get_vertex(vertex v) const { return verts_[v]; }

      edge_node&       get_edge(edge e)       { return edges_[e]; }
      const edge_node& get_edge(edge e) const { return edges_[e]; }

      // Helper functions for finding, connecting, disconnecting edges.
      bool find_out_edge(vertex u, vertex v) const;
      bool find_in_edge(vertex u, vertex v) const;
      void link_edge(vertex u, vertex v, edge e);
      void unlink_edge(vertex u, vertex v, edge e);
      void unlink_out_edge(vertex u, vertex v);
      void unlink_in_edge(vertex u, vertex v);
      void unlink_out_edges(vertex u, vertex v);
      void unlink_in_edges(vertex u, vertex v);
      void unlink_target(edge e);
      void unlink_source(edge e);

      template<typename S, typename P>
        void unlink_first_edge(S& seq, P pred);

      template<typename S1, typename S2, typename P>
        void unlink_multi_edge(S1& seq1, S2& seq2, P pred);

      // Helper predicates.
      struct has_target;
      struct has_source;

    private:
      vertex_set verts_;
      edge_set edges_;
    };


  template<typename V, typename E>
    inline bool
    directed_adjacency_list<V, E>::operator()(vertex u, vertex v) const
    {
      if (out_degree(u) <= in_degree(v))
        return find_out_edge(u, v);
      else
        return find_in_edge(u, v);
    }

  template<typename V, typename E>
    inline bool
    directed_adjacency_list<V, E>::find_out_edge(vertex u, vertex v) const
    {
      vertex_node& un = get_vertex(u);
      return any_of(un.out(), has_target(this, v));
    }

  template<typename V, typename E>
    inline bool
    directed_adjacency_list<V, E>::find_in_edge(vertex u, vertex v) const
    {
      vertex_node& vn = get_vertex(v);
      return any_of(vn.in(), has_source(this, v));
    }

  // Add a vertex to the graph, returning a handle to the new object. If
  // V is a user-supplied type, its value is default constructed.
  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_vertex() -> vertex
    {
      return verts_.emplace();
    }

  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_vertex(V&& x) -> vertex
    {
      return verts_.emplace(std::move(x));
    }

  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_vertex(const V& x) -> vertex
    {
      return verts_.emplace(x);
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::remove_vertex(vertex v)
    {
      remove_edges(v);
      verts_.erase(v);
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::remove_vertices()
    {
      edges_.clear();
      verts_.clear();
    }

  // Add a defaul edge from u to v.
  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_edge(vertex u, vertex v) -> edge
    {
      edge e = edges_.emplace(u, v);
      link_edge(u, v, e);
      return e;
    }

  // Move x into an edge connecting u to v.
  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_edge(vertex u, vertex v, E&& x) -> edge
    {
      edge e = edges_.emplace(u, v, std::move(x));
      link_edge(u, v, e);
      return e;
    }

  // Copy x into an edge connecting u to v.
  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_edge(vertex u, vertex v, const E& x) -> edge
    {
      edge e = edges_.emplace(u, v, x);
      link_edge(u, v, e);
      return e;
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::link_edge(vertex u, vertex v, edge e)
    {
      vertex_node& un = get_vertex(u);
      vertex_node& vn = get_vertex(v);
      un.insert_out(e);
      vn.insert_in(e);
    }

  // Remove the specified edge from the graph.
  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::remove_edge(edge e)
    {
      unlink_edge(source(e), target(e), e);
    }

  // Unlink the given edge from the source and target vertices, and erase
  // it from the edge set.
  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::unlink_edge(vertex u, vertex v, edge e)
    {
      vertex_node& un = get_vertex(u);
      vertex_node& vn = get_vertex(v);
      un.erase_out(e);
      vn.erase_in(e);
      edges_.erase(e);
    }


  // Remove the first edge connecting u to v.
  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::remove_edge(vertex u, vertex v)
    {
      if (out_degree(u) <= in_degree(v))
        unlink_out_edge(u, v);
      else
        unlink_in_edge(u, v);
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::unlink_out_edge(vertex u, vertex v)
    {
      vertex_node& un = get_vertex(u);
      unlink_first_edge(un.out(), has_target(*this, v));
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::unlink_in_edge(vertex u, vertex v)
    {
      vertex_node& vn = get_vertex(v);
      unlink_first_edge(vn.in(), has_source(*this, u));
    }

  template<typename V, typename E>
    template<typename S, typename P>
      inline void
      directed_adjacency_list<V, E>::unlink_first_edge(S& seq, P pred)
      {
        auto i = find_if(seq, pred);
        if (i != seq.end());
          remove_edge(*i);
      }

  // Remove all edges connecting u to v. 
  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::remove_edges(vertex u, vertex v)
    {
      if (out_degree(u) <= in_degree(v))
        unlink_out_edges(u, v);
      else
        unlink_in_edges(u, v);
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::unlink_out_edges(vertex u, vertex v)
    {
      vertex_node& un = get_vertex(u);
      vertex_node& vn = get_vertex(v);
      unlink_multi_edge(un.out(), vn.in(), has_target(*this, v));
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::unlink_in_edges(vertex u, vertex v)
    {
      vertex_node& un = get_vertex(u);
      vertex_node& vn = get_vertex(v);
      unlink_multi_edge(vn.in(), un.out(), has_source(*this, u));
    }

  // Remove all edges from seq1 that are connected to seq2. 
  template<typename V, typename E>
    template<typename S1, typename S2, typename P>
      inline void
      directed_adjacency_list<V, E>::unlink_multi_edge(S1& seq1, S2& seq2, P pred)
      {
        // Partition the 1st sequence by the given predicate into "save" and
        // "erase" components. 
        //
        // NOTE: We may want this to be a stable partition... not sure. It
        // seems like the saved edges are not reordered by the partitioning.
        auto i = partition(seq1, negate(pred));
        for (auto j = i; j != seq1.end(); ++j) {
          // Remove those edges from the in 2nd sequence.
          auto k = remove(seq2, *j);
          seq2.erase(k, seq2.end());

          // Erase the edge from the graph's edge set.
          edges_.erase(*j);
        }

        // Finally, erase those edges from the first sequence.
        seq1.erase(i, seq1.end());
      }


  // Remove all edges incident to the vertex v.
  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::remove_edges(vertex v)
    {
      vertex_node& vn = get_vertex(v);
      
      // Clear the out edges
      for (auto e : vn.out())
        unlink_target(e);
      vn.out().clear();
      
      // Clear the in edges
      for(auto e : vn.in())
        unlink_source(e);
      vn.in().clear();
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::unlink_target(edge e)
    {
      vertex_node& t = get_vertex(target(e));
      auto i = find(t.in(), e.value);
      t.in().erase(i);
      edges_.erase(e);
    }

  // Note that loops will not result in the double erasure of an edge. The
  // edge is initially erased in unlink_source, and the erase operation
  // here will have no effect.
  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::unlink_source(edge e)
    {
      vertex_node& t = get_vertex(source(e));
      auto i = find(t.out(), e.value);
      t.out().erase(i);
      edges_.erase(e);
    }


  // Remove all edges from a graph, making it empty.
  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::remove_edges()
    {
      for (vertex_node& n : verts_) {
        n.out().clear();
        n.in().clear();
      }
      edges_.clear();
    }

  // Retrun a range over the vertex set.
  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::vertices() const -> vertex_range
    {
      return {vertex_iter(verts_.begin()), vertex_iter(verts_.end())};
    }

  // Return a range over the edge set.
  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::edges() const -> edge_range
    {
      return {edge_iter(edges_.begin()), edge_iter(edges_.end())};
    }


  // Returns true when an edge is is the same as some target vertex.
  template<typename V, typename E>
    struct directed_adjacency_list<V, E>::has_target
    {
      has_target(const directed_adjacency_list& g, vertex t)
        : graph(g), target(t)
      { }

      inline bool
      operator()(edge e) const { return graph.target(e) == target; }

      const directed_adjacency_list& graph;
      vertex target;
    };

  // Returns true when an edge is is the same as some source vertex.
  template<typename V, typename E>
    struct directed_adjacency_list<V, E>::has_source
    {
      has_source(const directed_adjacency_list& g, vertex t)
        : graph(g), source(t)
      { }

      inline bool
      operator()(edge e) const { return graph.source(e) == source; }

      const directed_adjacency_list& graph;
      vertex source;
    };

} // namespace origin

#endif
