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
#include <origin/graph/handle.hpp>

#include <origin/graph/adjacency_list.impl/pool.hpp>

namespace origin
{

  namespace adjacency_list_impl
  {
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

        void add_out(std::size_t e) { out().push_back(e); }
        void add_in(std::size_t e) { in().push_back(e); }

        std::tuple<edge_list, edge_list, V> data;
      };

    // A vertex set is a pool of vertices.
    template<typename V>
      using vertex_pool = pool<vertex<V>>;



    // ---------------------------------------------------------------------- //
    //                            Vertex Iterator
    //
    // The vertex iterator wraps a pool iterator and returns vertex handles
    // when dereferenced. Vertex iteratrs are forward iterators.
    template<typename I>
      struct vertex_iterator
      {
        vertex_iterator(I i)
          : iter(i)
        { }

        vertex_handle operator*() const;

        vertex_iterator& operator++();
        vertex_iterator  operator++(int);

        I iter;
      };

    template<typename I>
      inline vertex_handle
      vertex_iterator<I>::operator*() const
      {
        return iter.index();
      }

    template<typename I>
      inline vertex_iterator<I>&
      vertex_iterator<I>::operator++()
      {
        ++iter;
        return *this;
      }

    template<typename I>
      inline vertex_iterator<I>
      vertex_iterator<I>::operator++(int)
      {
        vertex_iterator tmp = *this;
        ++iter;
        return tmp;
      }

    // Equality
    template<typename I>
      inline bool
      operator==(const vertex_iterator<I>& a, const vertex_iterator<I>& b)
      {
        return a.iter == b.iter;
      }

    template<typename I>
      inline bool
      operator!=(const vertex_iterator<I>& a, const vertex_iterator<I>& b)
      {
        return a.iter != b.iter;
      }
    

    // FIXME: This should go away and I should be using bounded_range!
    template<typename I>
      struct bounded_vertex_range
      {
        using iterator = vertex_iterator<I>;

        bounded_vertex_range(I f, I l)
          : first(f), last(l)
        { }

        iterator begin() const { return first; }
        iterator end() const   { return last; }

        iterator first;
        iterator last;
      };

    // An alias for the vertex range.
    template<typename V>
      using vertex_range = 
        bounded_vertex_range<typename vertex_pool<V>::const_iterator>;


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


    template<typename E>
      using edge_pool = pool<edge<E>>;


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

      using edge_node = adjacency_list_impl::edge<E>;
      using edge_set = adjacency_list_impl::edge_pool<E>;
    public:
      using vertex = vertex_handle;
      using edge = edge_handle;
      using vertex_range = adjacency_list_impl::vertex_range<V>;


      // Observers
      bool        null() const  { return verts_.empty(); }
      std::size_t order() const { return verts_.size(); }

      bool        empty() const { return edges_.empty(); }
      std::size_t size() const  { return edges_.size(); }

      // Edge properties
      vertex source(edge e) const { return get_edge(e).source(); }
      vertex target(edge e) const { return get_edge(e).target(); }

      // Data access
      V&       operator()(vertex v)       { return get_vertex(v).value(); }
      const V& operator()(vertex v) const { return get_vertex(v).value(); }

      E&       operator()(edge e)       { return get_edge(e).value(); }
      const E& operator()(edge e) const { return get_edge(e).value(); }


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
      void remove_edges(vertex u, vertex v);
      void remove_edges(vertex v);
      void remove_edges();


      // Iterators
      vertex_range vertices() { return {verts_.begin(), verts_.end()}; }

    private:
      vertex_node&       get_vertex(vertex v)       { return verts_[v]; }
      const vertex_node& get_vertex(vertex v) const { return verts_[v]; }

      edge_node&       get_edge(edge e)       { return edges_[e]; }
      const edge_node& get_edge(edge e) const { return edges_[e]; }

      edge link_edge(vertex u, vertex v, std::size_t e);

    private:
      vertex_set verts_;
      edge_set edges_;
    };


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
      verts_.erase(v);
    }

  template<typename V, typename E>
    inline void
    directed_adjacency_list<V, E>::remove_vertices()
    {
      edges_.clear();
      verts_.clear();
    }

  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_edge(vertex u, vertex v) -> edge
    {
      std::size_t e = edges_.emplace(u, v);
      return link_edge(u, v, e);
    }

  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_edge(vertex u, vertex v, E&& x) -> edge
    {
      std::size_t e = edges_.emplace(u, v, std::move(x));
      return link_edge(e);
    }

  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_edge(vertex u, vertex v, const E& x) -> edge
    {
      std::size_t e = edges_.emplace(u, v, x);
      return link_edge(e);
    }


  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::link_edge(vertex u, vertex v, std::size_t e) -> edge
    {
      vertex_node& un = get_vertex(u);
      vertex_node& vn = get_vertex(v);
      un.add_out(e);
      vn.add_in(e);
      return e;
    }


} // namespace origin

#endif
