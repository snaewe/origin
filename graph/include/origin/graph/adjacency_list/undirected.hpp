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
  // The undirected adjacency list implements an Undirected Adjacency_list.
  // It is implemented as a directed graph where the in-edges of a vertex are
  // considered to also be incident. In this implementation, an undirected edge 
  //is represented by a single directed edge.
  template<typename Vertex = empty_t,
           typename Edge = empty_t,
           typename Alloc = std::allocator<void>>
    class undirected_adjacency_list
    {
      using base_type = directed_adjacency_list<Vertex, Edge, Alloc>;
      using this_type = undirected_adjacency_list<Vertex, Edge, Alloc>;
    public:
      using vertex_allocator_type = typename base_type::vertex_allocator_type;
      using edge_allocator_type   = typename base_type::edge_allocator_type;

      using size_type       = typename base_type::size_type;
      using difference_type = typename base_type::difference_type;
      
      using vertex_value_type = Vertex;
      using edge_value_type   = Edge;
      
      using vertex       = vertex_t;
      using const_vertex = vertex_t;
      
      using edge       = undirected_edge_t;
      using const_edge = undirected_edge_t;
    // private:
      // These are used to shorten subsequent declarations.
      using edge_iterator       = undirected_edge_iterator<this_type>;
      using const_edge_iterator = undirected_edge_iterator<const this_type>;
      using incident_edge_iterator       = undirected_incident_edge_iterator<this_type>;
      using const_incident_edge_iterator = undirected_incident_edge_iterator<const this_type>;
    public:
      using vertex_range       = bounded_range<vertex_iterator>;
      using const_vertex_range = bounded_range<vertex_iterator>;
      using edge_range       = bounded_range<edge_iterator>;
      using const_edge_range = bounded_range<const_edge_iterator>;
      using incident_edge_range       = bounded_range<incident_edge_iterator>;
      using const_incident_edge_range = bounded_range<const_incident_edge_iterator>;
      
      struct graph_category : undirected_graph_tag, buildable_graph_tag { };

      // Initialization
      
      // Semiregular
      // NOTE: Copy and move constructors and assignment operators are generated.
      undirected_adjacency_list()
        : base()
      { }

      undirected_adjacency_list(size_type n, vertex_value_type const& value = {})
        : base(n, value)
      { }

      // Vertex range initialization
      // FIXME: Specialize for edge-list initialization.
      template<typename Iter>
        undirected_adjacency_list(Iter first, Iter last)
          : base(first, last)
        { }
        
      // Vertex list initialization
      undirected_adjacency_list(std::initializer_list<vertex_value_type> list)
        : base(list)
      { }
      
      // FIXME: Write more constructors.

      // Container properties
      
      // Return the maximum number of vertices possible for the graph.
      constexpr size_type max_order() const { return base.max_order(); }

      // Return the maximum number of edges possible for the graph.
      constexpr size_type max_size() const  { return base.max_size(); }
      
      // Return the graph's vertex allocator.
      vertex_allocator_type get_vertex_allocator() const { return base.get_vertex_allocator(); }

      // Return the graph's edge allocator.
      edge_allocator_type get_edge_allocator() const { return base.get_edge_allocator(); }

      // Return the underlying directed graph.
      base_type&        impl()       { return base; }
      base_type const&  impl() const { return base; } 

      // Graph properties
      
      // Return true if the graph has no vertices.
      bool null() const { return base.null(); }

      // Return the number of vertices in the graph.
      size_type order() const{ return base.order(); }
      
      // Return true if the graph has no edges.
      bool empty() const { return base.empty(); }

      // Return the number of edges in the graph.
      size_type size() const { return base.size(); }

      // Return the value associated with the vertex v.
      vertex_value_type&       operator[](vertex v)       { return base[v]; }
      vertex_value_type const& operator[](vertex v) const { return base[v]; }

      // Return the value associated with the edge e.
      edge_value_type&       operator[](edge e)       { return base[e.edge]; }
      edge_value_type const& operator[](edge e) const { return base[e.edge]; }
      
      // Get the nth vertex in the graph.
      vertex       get_vertex(size_type n)       { return n; }
      const_vertex get_vertex(size_type n) const { return n; }
    
      // Return the degree of the the vertex v, the number of incident edges.
      size_type degree(vertex v) const { return base.degree(v); }

      // Get the nth edge in the graph.
      edge       get_edge(size_type n)       { return make_edge(edge_t{n}); }
      const_edge get_edge(size_type n) const { return make_edge(edge_t{n}); }

      // Get the edge connecting the vertices u and v.
      edge       get_edge(vertex u, vertex v);
      const_edge get_edge(vertex u, vertex v) const;
      
      // Get the nth incident edge of vertex v.
      edge       get_incident_edge(vertex v, size_type n);
      const_edge get_incident_edge(vertex v, size_type n) const;

      // Get the source vertex of the edge e.
      vertex       source(edge e)       { return e.source; } 
      const_vertex source(edge e) const { return e.source; }
      
      // Get the target vertex of the edge e.
      vertex       target(edge e)       { return e.target; }
      const_vertex target(edge e) const { return e.target; }

      // Add a vertex to the graph.
      vertex add_vertex(vertex_value_type const& x = vertex_value_type{});
      
      // Add an edge connecting the vertices u and v.
      edge add_edge(vertex u, vertex v, edge_value_type const& x = edge_value_type{});

      // Ranges 
      
      // Return the range of vertices in the graph.
      vertex_range       vertices()       { return base.vertices(); }
      const_vertex_range vertices() const { return base.vertices(); }

      // Return the range of all edges in the graph.
      edge_range       edges()       { return {begin_edges(), end_edges()}; }
      const_edge_range edges() const { return {begin_edges(), end_edges()}; }

      // REturn the range of edges incident to the vertex v.
      incident_edge_range       edges(vertex v);
      const_incident_edge_range edges(vertex v) const;

    private:
      // Create an undirected edge using the underlying edge and source vertex.
      edge       make_edge(edge_t e, vertex_t v)       { return {e, v, base.target(e)}; }
      const_edge make_edge(edge_t e, vertex_t v) const { return {e, v, base.target(e)}; }

      // Create an undirected edge over the underlying edge type. Use underlying
      // source of the edge as the source vertex.
      edge       make_edge(edge_t e)       { return make_edge(e, base.source(e)); }
      const_edge make_edge(edge_t e) const { return make_edge(e, base.source(e)); }

      edge_iterator       begin_edges()       { return {*this, get_edge(0)}; }
      const_edge_iterator begin_edges() const { return {*this, get_edge(0)}; }
      
      edge_iterator       end_edges()       { return {*this, get_edge(size())}; }
      const_edge_iterator end_edges() const { return {*this, get_edge(size())}; }
      
      incident_edge_iterator       begin_incident_edges(vertex v)       { return {*this, v, 0}; }
      const_incident_edge_iterator begin_incident_edges(vertex v) const { return {*this, v, 0}; }
      
      incident_edge_iterator       end_incident_edges(vertex v)       { return {*this, v, degree(v)}; }
      const_incident_edge_iterator end_incident_edges(vertex v) const { return {*this, v, degree(v)}; }
    private:
      base_type base;
    };

  // FIXME: I think that this might be optimized by searching the vertex
  // with the smaller degree first.
  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      get_edge(vertex u, vertex v) -> edge
      {
        typename base_type::edge e = base.get_edge(u, v);
        return e ? make_edge(e, u) : make_edge(base.get_edge(v, u), v);
      }
    
  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      get_edge(vertex u, vertex v) const -> const_edge
      {
        typename base_type::edge e = base.get_edge(u, v);
        return e ? make_edge(e, u) : make_edge(base.get_edge(v, u), v);
      }


  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      get_incident_edge(vertex v, size_type n) -> edge
      {
        size_type d = base.out_degree(v);
        edge_t e = n < d ? base.get_out_edge(v, n) : base.get_in_edge(v, n - d);
        return {e, v, target(make_edge(e, v))};
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      get_incident_edge(vertex v, size_type n) const -> const_edge
      {
        size_type d = base.out_degree(v);
        edge_t e = n < d ? base.get_out_edge(v, n) : base.get_in_edge(v, n - d);
        return {e, v, target(make_edge(e, v))};
      }

  template<typename V, typename E, typename A>
    inline auto undirected_adjacency_list<V, E, A>::
      add_vertex(vertex_value_type const& x) -> vertex
      { 
        return base.add_vertex(x);
      }

  template<typename V, typename E, typename A>
    inline auto undirected_adjacency_list<V, E, A>::
      add_edge(vertex u, vertex v, edge_value_type const& x) -> edge
      {
        return make_edge(base.add_edge(u, v, x), u); 
      }

  template<typename V, typename E, typename A>
    inline auto undirected_adjacency_list<V, E, A>::
      edges(vertex v) -> incident_edge_range
      {
        return {begin_incident_edges(v), end_incident_edges(v)};
      }

  template<typename V, typename E, typename A>
    inline auto undirected_adjacency_list<V, E, A>::
      edges(vertex v) const -> const_incident_edge_range
      {
        return {begin_incident_edges(v), end_incident_edges(v)};
      }

} // namespace origin

#endif
