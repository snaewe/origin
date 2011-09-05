// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_LIST_DIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_LIST_DIRECTED_HPP

#include <algorithm>
#include <vector>

#include <origin/utility/empty.hpp>
#include <origin/range/iterator_range.hpp>
#include <origin/graph/traits.hpp>
#include <origin/graph/vertex.hpp>
#include <origin/graph/edge.hpp>

namespace origin
{
  // FIXME: Consider splitting on dependent, non-dependent features of the
  // adj_vec components. Also, the value types might be optimized using EBO.

  // The edge node for a directed adjacency list stores the source and vertex
  // indices of the graph.
  template<typename E>
    struct adjacency_list_edge
    {
      typedef E value_type;
      
      adjacency_list_edge(vertex_t src, vertex_t tgt, value_type const& x = value_type{})
        : source{src}, target{tgt}, value{x}
      { }

      vertex_t source;
      vertex_t target;
      value_type value;
    };
  
  // The vertex node of an adjacency vector stores the out edge and in edge
  // lists.
  template<typename V>
    struct adjacency_list_vertex
    {
      typedef V value_type;
      typedef std::vector<edge_t> edge_list;
      
      adjacency_list_vertex(value_type const& x = value_type{})
        : value{x}
      { }
      
      std::size_t degree() const      { return out.size() + in.size(); }
      std::size_t out_degree() const  { return out.size(); }
      std::size_t in_degree() const   { return in.size(); }
      
      void add_out(edge_t e) { out.push_back(e); }
      void add_in(edge_t e) { in.push_back(e); }

      edge_list out;
      edge_list in;
      value_type value;
    };

  // FIXME: Could it be more efficient to have the edges in a single edge pool?
  // and then carve the edge lists out of that? Would that be any different
  // than Boost's CSR graph or LEDA's static graphs? I may be able to relax the 
  // sorted edge requirement for a more general solution.

  /** 
   * The directed adjacency vector implements an Adjacency_List data structure
   * whose vertex and edge sets are implemented using vectors.
   * 
   * @tparam Vertex   An Object type, empty_t by default
   * @tparam Edge     An Object type, empty_t by default
   * @tparam Alloc    An Allocator type, allocator<void> by default.
   */
  template<typename Vertex = empty_t, 
           typename Edge = empty_t, 
           typename Alloc = std::allocator<void>>
    class directed_adjacency_list
    {
    public:
      typedef typename Alloc::template rebind<Vertex>::other vertex_allocator_type;
      typedef typename Alloc::template rebind<Edge>::other   edge_allocator_type;

      typedef std::size_t    size_type;
      typedef std::ptrdiff_t difference_type;
      
      typedef Vertex vertex_value_type;
      typedef Edge   edge_value_type;
    private:
      typedef adjacency_list_vertex<vertex_value_type> vertex_type;
      typedef adjacency_list_edge<edge_value_type>     edge_type;

      typedef std::vector<vertex_type, vertex_allocator_type> vertex_list;
      typedef std::vector<edge_type, edge_allocator_type>     edge_list;
    public:
      typedef vertex_t vertex;
      typedef vertex_t const_vertex;
      
      typedef edge_t edge;
      typedef edge_t const_edge;
    private:
      // Const so that we can't accidentally modify the values in the vertex
      // handles in the incident edge lists.
      typedef typename vertex_type::edge_list::const_iterator incident_edge_iterator;
    public:
      typedef iterator_range<vertex_iterator>        vertex_range;
      typedef iterator_range<vertex_iterator>        const_vertex_range;
      typedef iterator_range<edge_iterator>          edge_range;
      typedef iterator_range<edge_iterator>          const_edge_range;
      typedef iterator_range<incident_edge_iterator> out_edge_range;
      typedef iterator_range<incident_edge_iterator> const_out_edge_range;
      typedef iterator_range<incident_edge_iterator> in_edge_range;
      typedef iterator_range<incident_edge_iterator> const_in_edge_range;
      
      struct graph_category : directed_graph_tag, buildable_graph_tag { };
      
      // Semiregular
      // Copy and move constructors and assignment operators are generated.
      directed_adjacency_list()
        : vertices_{}, edges_{}
      { }
      
      // Vertex fill constructor
      directed_adjacency_list(size_type n, vertex_value_type const& x = vertex_value_type{})
        : vertices_(n, x), edges_{}
      { }

      // Vertex range constructor
      template<typename Iter>
        directed_adjacency_list(Iter first, Iter last)
          : vertices_(first, last), edges_{}
        { }
        
      // Vertex initialization constructor
      directed_adjacency_list(std::initializer_list<vertex_value_type> list)
        : vertices_(list), edges_{}
      { }

      // Container properties.
      
      // Return the maximum number of vertices possible for the graph.
      constexpr size_type max_order() const { return vertices_.max_size(); }
      
      // Return the maximum number of edges possible for the graph.
      constexpr size_type max_size() const  { return edges_.max_size(); }
      
      // Return the graph's vertex allocator.
      vertex_allocator_type get_vertex_allocator() const { return vertex_allocator_type{}; }
      
      // Return the graph's edge allocator.
      edge_allocator_type get_edge_allocator() const   { return edge_allocator_type{}; }

      // Graph properties

      // Return true if the graph has no vertices.
      bool null() const { return vertices_.empty(); }
      
      // Return the number of vertices in the graph.
      size_type order() const { return vertices_.size(); }
      
      // Return true if the graph has no edges.
      bool empty() const { return edges_.empty(); }
      
      // Return the number of edges in the graph.
      size_type size() const { return edges_.size(); }
      
      // Return the value associated with the vertex v.
      vertex_value_type&       operator[](vertex v)       { return get(v).value; }
      vertex_value_type const& operator[](vertex v) const { return get(v).value; }

      // Return the value associated with the edge e.
      edge_value_type&       operator[](edge e)       { return get(e).value; }
      edge_value_type const& operator[](edge e) const { return get(e).value; }

      // Return the vertex indicated by the given index
      vertex       get_vertex(size_type n)       { return n; }
      const_vertex get_vertex(size_type n) const { return n; }
    
      // Return the degree, out degree or in degree of the given vertex
      size_type degree(const_vertex v) const     { return get(v).degree(); }
      size_type out_degree(const_vertex v) const { return get(v).out_degree(); }
      size_type in_degree(const_vertex v) const  { return get(v).in_degree(); }

      // Return the edge indicated by the given index.
      edge       get_edge(size_type n)       { return n; }
      const_edge get_edge(size_type n) const { return n; }

      // Return the edge connecting the vertex u to v, or an invalid edge if
      // u and v are not adjacent.
      edge        get_edge(vertex u, vertex v);
      const_edge  get_edge(vertex u, vertex v) const;
      
      // Return the nth out edge of vertex v.
      edge       get_out_edge(vertex v, size_type n)       { return get(v).out[n]; }
      const_edge get_out_edge(vertex v, size_type n) const { return get(v).out[n]; } 

      // Return the nth in edge of vertex v.
      edge       get_in_edge(vertex v, size_type n)       { return get(v).in[n]; }
      const_edge get_in_edge(vertex v, size_type n) const { return get(v).in[n]; } 

      // Return the source vertex of the given edge.
      vertex       source(edge e)             { return get(e).source; }
      const_vertex source(const_edge e) const { return get(e).source; }

      // Return the target vertex of the given edge.
      vertex       target(edge e)       { return get(e).target; }
      const_vertex target(edge e) const { return get(e).target; }

      // Graph operations

      // Add a value to the graph, returning a handle to the added vertex. The 
      // associated value may be specified.
      vertex add_vertex(vertex_value_type const& x = vertex_value_type{})
      {
        vertices_.push_back(vertex_type{x});
        return vertex{vertices_.size() - 1};
      }

      // Add an edge to the graph so that u is connected to v, returning a 
      // handle to the added edge. The associated value may be specified.
      edge add_edge(vertex u, vertex v, edge_value_type const& x = edge_value_type{})
      {
        edges_.push_back(edge_type{u, v, x});
        edge e = edges_.size() - 1;
        get(u).add_out(e);
        get(v).add_in(e);
        return e;
      }

      // Ranges

      // Return the range of vertices in the graph.
      vertex_range       vertices()       { return {begin_vertices(), end_vertices()}; }
      const_vertex_range vertices() const { return {begin_vertices(), end_vertices()}; }
      
      // Return the range of all edges in the graph.
      edge_range       edges()       { return {begin_edges(), end_edges()}; }
      const_edge_range edges() const { return {begin_edges(), end_edges()}; }

      // Return the set of outgoing edges of the vertex v.
      out_edge_range       out_edges(vertex v)       { return {begin_out_edges(v), end_out_edges(v)}; }
      const_out_edge_range out_edges(vertex v) const { return {begin_out_edges(v), end_out_edges(v)}; }
      
      // Return the set of incomming edges of the vertex v.
      in_edge_range       in_edges(vertex v)       { return {begin_in_edges(v), end_in_edges(v)}; }
      const_in_edge_range in_edges(vertex v) const { return {begin_in_edges(v), end_in_edges(v)}; }

    private:
      // Return the vertex object associated with the given handle.
      vertex_type&       get(vertex v)             { return vertices_[v.value]; }
      vertex_type const& get(const_vertex v) const { return vertices_[v.value]; }
      
      // Return the edge object associated with the given handle.
      edge_type&       get(edge e)             { return edges_[e.value]; }
      edge_type const& get(const_edge e) const { return edges_[e.value]; }
      
      // Return an iterator to the first vertex
      vertex_iterator begin_vertices()       { return vertex_t{0}; }
      vertex_iterator begin_vertices() const { return vertex_t{0}; }
      
      // Return an iterator past the last vertex
      vertex_iterator end_vertices()       { return vertex_t{order()}; }
      vertex_iterator end_vertices() const { return vertex_t{order()}; }

      // Return an iterator to the first edge
      edge_iterator begin_edges()       { return edge_t{0}; }
      edge_iterator begin_edges() const { return edge_t{0}; }
      
      // Return an iterator past the last edge
      edge_iterator end_edges()       { return edge_t{size()}; }
      edge_iterator end_edges() const { return edge_t{size()}; }
      
      // Return an iterator to the first out edge of v
      incident_edge_iterator begin_out_edges(vertex v)             { return get(v).out.begin(); }
      incident_edge_iterator begin_out_edges(const_vertex v) const { return get(v).out.begin(); }
      
      // Return an iterator past the last out edge of v
      incident_edge_iterator end_out_edges(vertex v)             { return get(v).out.end(); }
      incident_edge_iterator end_out_edges(const_vertex v) const { return get(v).out.end(); }

      // Return an iterator to the first incident edge of v
      incident_edge_iterator begin_in_edges(vertex v)       { return get(v).in.begin(); }
      incident_edge_iterator begin_in_edges(vertex v) const { return get(v).in.begin(); }
      
      // Return an iterator past the last incident edge of v.
      incident_edge_iterator end_in_edges(vertex v)       { return get(v).in.end(); }
      incident_edge_iterator end_in_edges(vertex v) const { return get(v).in.end(); }

    private:
      vertex_list vertices_;
      edge_list edges_;
    };

    
  template<typename V, typename E, typename A>
    inline auto directed_adjacency_list<V, E, A>::
      get_edge(vertex u, vertex v) -> edge
    {
      auto& out = get(u).out;
      auto iter = std::find_if(out.begin(), out.end(), if_has_target(*this, v));
      return iter == out.end() ? edge{} : *iter;
    }

  template<typename V, typename E, typename A>
    inline auto directed_adjacency_list<V, E, A>::
      get_edge(const_vertex u, const_vertex v) const -> const_edge
    {
      auto& out = get(u).out;
      auto iter = std::find_if(out.begin(), out.end(), if_has_target(*this, v));
      return iter == out.end() ? edge{} : *iter;
    }

} // namespace origin

#endif
