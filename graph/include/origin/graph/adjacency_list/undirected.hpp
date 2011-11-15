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
      typedef typename base_type::vertex_allocator_type vertex_allocator_type;
      typedef typename base_type::edge_allocator_type   edge_allocator_type;

      typedef typename base_type::size_type       size_type;
      typedef typename base_type::difference_type difference_type;
      
      typedef Vertex vertex_value_type;
      typedef Edge   edge_value_type;
      
      typedef vertex_t vertex;
      typedef vertex_t const_vertex;
      
      typedef undirected_edge_t edge;
      typedef undirected_edge_t const_edge;
    private:
      typedef undirected_edge_iterator<this_type> edge_iterator;
      typedef undirected_edge_iterator<this_type const> const_edge_iterator;
      
      typedef undirected_incident_edge_iterator<this_type> incident_edge_iterator;
      typedef undirected_incident_edge_iterator<this_type const> const_incident_edge_iterator;
    public:
      typedef bounded_range<vertex_iterator> vertex_range;
      typedef bounded_range<vertex_iterator> const_vertex_range;
      
      typedef bounded_range<edge_iterator>       edge_range;
      typedef bounded_range<const_edge_iterator> const_edge_range;
      
      typedef bounded_range<incident_edge_iterator>       incident_edge_range;
      typedef bounded_range<const_incident_edge_iterator> const_incident_edge_range;
      
      struct graph_category : undirected_graph_tag, buildable_graph_tag { };

      // Initialization
      
      // Semiregular
      // Copy and move constructors and assignment operators are generated.
      undirected_adjacency_list()
        : base_{}
      { }

      undirected_adjacency_list(size_type n, vertex_value_type const& value = vertex_value_type{})
        : base_(n, value)
      { }

      template<typename Iter>
        undirected_adjacency_list(Iter first, Iter last)
          : base_(first, last)
        { }
        
      undirected_adjacency_list(std::initializer_list<vertex_value_type> list)
        : base_(list)
      { }

      // Container properties
      
      // Return the maximum number of vertices possible for the graph.
      constexpr size_type max_order() const { return base_.max_order(); }

      // Return the maximum number of edges possible for the graph.
      constexpr size_type max_size() const  { return base_.max_size(); }
      
      // Return the graph's vertex allocator.
      vertex_allocator_type get_vertex_allocator() const { return base_.get_vertex_allocator(); }

      // Return the graph's edge allocator.
      edge_allocator_type get_edge_allocator() const { return base_.get_edge_allocator(); }

      // Return the underlying directed graph.
      base_type&        base()       { return base_; }
      base_type const&  base() const { return base_; } 

      // Graph properties
      
      // Return true if the graph has no vertices.
      bool null() const { return base_.null(); }

      // Return the number of vertices in the graph.
      size_type order() const{ return base_.order(); }
      
      // Return true if the graph has no edges.
      bool empty() const { return base_.empty(); }

      // Return the number of edges in the graph.
      size_type size() const { return base_.size(); }

      // Return the value associated with the vertex v.
      vertex_value_type&       operator[](vertex v)       { return base_[v]; }
      vertex_value_type const& operator[](vertex v) const { return base_[v]; }

      // Return the value associated with the edge e.
      edge_value_type&       operator[](edge e)       { return base_[e.edge]; }
      edge_value_type const& operator[](edge e) const { return base_[e.edge]; }
      
      // Get the nth vertex in the graph.
      vertex       get_vertex(size_type n)       { return n; }
      const_vertex get_vertex(size_type n) const { return n; }
    
      // Return the degree of the the vertex v, the number of incident edges.
      size_type degree(vertex v) const { return base_.degree(v); }

      // Get the nth edge in the graph.
      edge       get_edge(size_type n)       { return make_edge(edge_t{n}); }
      const_edge get_edge(size_type n) const { return make_edge(edge_t{n}); }

      // Get the nth incident edge of vertex v.
      edge       get_incident_edge(vertex v, size_type n);
      const_edge get_incident_edge(vertex v, size_type n) const;
          
      // Get the edge connecting the vertices u and v.
      edge get_edge(vertex u, vertex v);
      const_edge get_edge(vertex u, vertex v) const;
      
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
      vertex_range       vertices()       { return base_.vertices(); }
      const_vertex_range vertices() const { return base_.vertices(); }

      // Return the range of all edges in the graph.
      edge_range       edges()       { return {begin_edges(), end_edges()}; }
      const_edge_range edges() const { return {begin_edges(), end_edges()}; }

      // REturn the range of edges incident to the vertex v.
      incident_edge_range       incident_edges(vertex v);
      const_incident_edge_range incident_edges(vertex v) const;

    private:
      // Create an undirected edge using the underlying edge and source vertex.
      edge       make_edge(edge_t e, vertex_t v)       { return {e, v, base_.target(e)}; }
      const_edge make_edge(edge_t e, vertex_t v) const { return {e, v, base_.target(e)}; }

      // Create an undirected edge over the underlying edge type. Use underlying
      // source of the edge as the source vertex.
      edge       make_edge(edge_t e)       { return make_edge(e, base_.source(e)); }
      const_edge make_edge(edge_t e) const { return make_edge(e, base_.source(e)); }

      edge_iterator       begin_edges()       { return {*this, get_edge(0)}; }
      const_edge_iterator begin_edges() const { return {*this, get_edge(0)}; }
      
      edge_iterator       end_edges()       { return {*this, get_edge(size())}; }
      const_edge_iterator end_edges() const { return {*this, get_edge(size())}; }
      
      incident_edge_iterator       begin_incident_edges(vertex v)       { return {*this, v, 0}; }
      const_incident_edge_iterator begin_incident_edges(vertex v) const { return {*this, v, 0}; }
      
      incident_edge_iterator       end_incident_edges(vertex v)       { return {*this, v, degree(v)}; }
      const_incident_edge_iterator end_incident_edges(vertex v) const { return {*this, v, degree(v)}; }
    private:
      base_type base_;
    };

  // FIXME: I think that this might be optimized by searching the vertex
  // with the smaller degree first.
  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      get_edge(vertex u, vertex v) -> edge
      {
        typename base_type::edge e = base_.get_edge(u, v);
        return e ? make_edge(e, u) : make_edge(base_.get_edge(v, u), v);
      }
    
  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      get_edge(vertex u, vertex v) const -> const_edge
      {
        typename base_type::edge e = base_.get_edge(u, v);
        return e ? make_edge(e, u) : make_edge(base_.get_edge(v, u), v);
      }


  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      get_incident_edge(vertex v, size_type n) -> edge
      {
        size_type d = base_.out_degree(v);
        edge_t e = n < d ? base_.get_out_edge(v, n) : base_.get_in_edge(v, n - d);
        return {e, v, target(make_edge(e, v))};
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      get_incident_edge(vertex v, size_type n) const -> const_edge
      {
        size_type d = base_.out_degree(v);
        edge_t e = n < d ? base_.get_out_edge(v, n) : base_.get_in_edge(v, n - d);
        return {e, v, target(make_edge(e, v))};
      }


  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      add_vertex(vertex_value_type const& x) -> vertex
      { 
        return base_.add_vertex(x);
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      add_edge(vertex u, vertex v, edge_value_type const& x) -> edge
      {
        return make_edge(base_.add_edge(u, v, x), u); 
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      incident_edges(vertex v) -> incident_edge_range
      {
        return {begin_incident_edges(v), end_incident_edges(v)};
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_list<V, E, A>::
      incident_edges(vertex v) const -> const_incident_edge_range
      {
        return {begin_incident_edges(v), end_incident_edges(v)};
      }

} // namespace origin

#endif
