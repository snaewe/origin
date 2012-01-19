// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_LIST_DIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_LIST_DIRECTED_HPP

#include <vector>

#include <origin/tuple.hpp>
#include <origin/range.hpp>
#include <origin/algorithm.hpp>

#include <origin/graph/traits.hpp>
#include <origin/graph/vertex.hpp>
#include <origin/graph/edge.hpp>

// #include <origin/graph/generator.hpp>

namespace origin
{
  // Adjacency list edge
  // The edge node for a directed adjacency list stores the source and vertex
  // indexes of the graph.
  template<typename E>
    struct adjacency_list_edge
    {
      using vertex = vertex_handle<std::size_t>;
      using value_type = E;
      
      adjacency_list_edge(vertex s, vertex t, const value_type& x = {})
        : data{s, t, x}
      { }
      
      // Get the source vertex
      vertex&       source()       { return std::get<0>(data); }
      const vertex& source() const { return std::get<0>(data); }
      
      // Get the target vertex
      vertex&       target()       { return std::get<1>(data); }
      const vertex& target() const { return std::get<1>(data); }
      
      // Get the edge value.
      value_type&      value()       { return std::get<2>(data); }
      const value_type value() const { return std::get<2>(data); }
      
      std::tuple<vertex, vertex, value_type> data;
    };
  
    

  // Adjacency list vertex
  // The vertex node of an adjacency vector stores the out edge and in edge
  // lists.
  template<typename V>
    struct adjacency_list_vertex
    {
      using edge = edge_handle<std::size_t>;
      using edge_list = std::vector<edge>;
      using value_type = V;
      
      adjacency_list_vertex(value_type const& x = {})
        : data{edge_list{}, edge_list{}, x}
      { }
      
      // Get the out edge list.
      edge_list&       out()       { return std::get<0>(data); }
      const edge_list& out() const { return std::get<0>(data); }
      
      // Get the in edge list.
      edge_list&       in()       { return std::get<1>(data); }
      const edge_list& in() const { return std::get<1>(data); }
      
      // Get the vertex value.
      value_type&       value()       { return std::get<2>(data); }
      const value_type& value() const { return std::get<2>(data); }
      
      // Get the degree, out degree or in degree of the vertex.
      std::size_t degree() const      { return out().size() + in().size(); }
      std::size_t out_degree() const  { return out().size(); }
      std::size_t in_degree() const   { return in().size(); }

      // Add an out edge.
      void add_out(edge e) { out().push_back(e); }
      
      // Add an in edge.
      void add_in(edge e) { in().push_back(e); }

      std::tuple<edge_list, edge_list, value_type> data;
    };

    
  // The directed adjacency vector implements an Adjacency_List data structure
  // whose vertex and edge sets are implemented using vectors.
  template<typename Vertex = empty_t, 
           typename Edge = empty_t, 
           typename Alloc = std::allocator<void>>
    class directed_adjacency_list
    {
    public:
      using vertex_allocator_type = typename Alloc::template rebind<Vertex>::other;
      using edge_allocator_type = typename Alloc::template rebind<Edge>::other;

      using size_type = std::size_t;
      using difference_type = std::ptrdiff_t;
      
      using vertex_value_type = Vertex;
      using edge_value_type = Edge;
      
      using vertex = vertex_handle<std::size_t>;
      using edge = edge_handle<std::size_t>;

    private:
      using vertex_type = adjacency_list_vertex<vertex_value_type>;
      using edge_type = adjacency_list_edge<edge_value_type>;

      using vertex_list = std::vector<vertex_type, vertex_allocator_type>;
      using edge_list = std::vector<edge_type, edge_allocator_type>;

      using vertex_iterator = origin::vertex_iterator<std::size_t>;
      using edge_iterator = origin::edge_iterator<std::size_t>;
      using incident_edge_iterator = typename vertex_type::edge_list::iterator;

    public:
      using vertex_range   = bounded_range<vertex_iterator>;
      using edge_range     = bounded_range<edge_iterator>;
      using out_edge_range = bounded_range<incident_edge_iterator>;
      using in_edge_range  = bounded_range<incident_edge_iterator>;
      
      
      // Semiregular
      // NOTE: Copy and move constructors and assignment operators are generated.
      directed_adjacency_list()
        : vlist{}, elist{}
      { }
      
      // Vertex fill initialization
      directed_adjacency_list(size_type n, vertex_value_type const& x = {})
        : vlist(n, x), elist{}
      { }

      // Vertex range initialization
      // FIXME: Specialize edge-list initialization
      template<typename Iter>
        directed_adjacency_list(Iter first, Iter last)
          : vlist(), elist()
        {
          while(first != last) {
            add_vertex(*first);
            ++first;
          }
        }

      // Vertex list initialization
      directed_adjacency_list(std::initializer_list<vertex_value_type> list)
        : vlist{}, elist{}
      {
        for(auto const& x : list)
          add_vertex(x);
      }
      
      // FIXME: Generalize for edge tuples
      directed_adjacency_list(std::initializer_list<std::pair<vertex_value_type, vertex_value_type>> list)
        : vlist{}, elist{}
      {
        // build_edge_graph(*this, list.begin(), list.end());
      }

      
      
      // Object properties
      
      // Return the maximum number of vertices possible for the graph.
      constexpr size_type max_order() const { return vlist.max_size(); }
      
      // Return the maximum number of edges possible for the graph.
      constexpr size_type max_size() const  { return elist.max_size(); }
      
      // Return the graph's vertex allocator.
      vertex_allocator_type get_vertex_allocator() const { return vertex_allocator_type{}; }
      
      // Return the graph's edge allocator.
      edge_allocator_type get_edge_allocator() const { return edge_allocator_type{}; }


      
      // Graph properties

      // Return true if the graph has no vertices.
      bool null() const { return vlist.empty(); }
      
      // Return the number of vertices in the graph.
      size_type order() const { return vlist.size(); }
      
      // Return true if the graph has no edges.
      bool empty() const { return elist.empty(); }
      
      // Return the number of edges in the graph.
      size_type size() const { return elist.size(); }
      
      // Return the degree, out degree or in degree of the given vertex
      size_type degree(vertex v) const     { return get(v).degree(); }
      size_type out_degree(vertex v) const { return get(v).out_degree(); }
      size_type in_degree(vertex v) const  { return get(v).in_degree(); }
    

    
      // Data accessors
      
      // Return the value associated with the vertex v.
      vertex_value_type&       operator[](vertex v)       { return get(v).value(); }
      vertex_value_type const& operator[](vertex v) const { return get(v).value(); }

      // Return the value associated with the edge e.
      edge_value_type&       operator[](edge e)       { return get(e).value(); }
      edge_value_type const& operator[](edge e) const { return get(e).value(); }

      // Return the vertex indicated by the given index
      vertex       get_vertex(size_type n)       { return n; }
    
    
    
      // Vertex and edge accessors
    
      // Return the edge indicated by the given index.
      edge get_edge(size_type n) const { return n; }

      // Return the edge connecting the vertex u to v, or an invalid edge if
      // u and v are not adjacent.
      edge get_edge(vertex u, vertex v) const;
      
      // Return the nth out edge of vertex v.
      edge get_out_edge(vertex v, size_type n) const { return get(v).out()[n]; }

      // Return the nth in edge of vertex v.
      edge get_in_edge(vertex v, size_type n) const { return get(v).in[n]; }

      // Return the source vertex of the given edge.
      vertex source(edge e) const { return get(e).source; }

      // Return the target vertex of the given edge.
      vertex target(edge e) const { return get(e).target; }



      // Graph operations

      // Add a value to the graph, returning a handle to the added vertex. The 
      // associated value may be specified.
      vertex add_vertex(const vertex_value_type& x = {})
      {
        vlist.push_back(vertex_type{x});
        return vertex{vlist.size() - 1};
      }

      // Add an edge to the graph so that u is connected to v, returning a 
      // handle to the added edge. The associated value may be specified.
      edge add_edge(vertex u, vertex v, const edge_value_type& x = {})
      {
        elist.push_back(edge_type{u, v, x});
        edge e = elist.size() - 1;
        get(u).add_out(e);
        get(v).add_in(e);
        return e;
      }

      
      
      // Ranges

      // Return the range of vertices in the graph.
      vertex_range vertices() const { return {begin_vertices(), end_vertices()}; }
      
      // Return the range of all edges in the graph.
      edge_range edges() const { return {begin_edges(), end_edges()}; }

      // Return the set of outgoing edges of the vertex v.
      out_edge_range out_edges(vertex v) const { return {begin_out_edges(v), end_out_edges(v)}; }
      
      // Return the set of incoming edges of the vertex v.
      in_edge_range in_edges(vertex v) const { return {begin_in_edges(v), end_in_edges(v)}; }

      
      
    private:
      // Return the vertex object associated with the given handle.
      vertex_type&       get(vertex v)       { return vlist[v.value()]; }
      const vertex_type& get(vertex v) const { return vlist[v.value()]; }
      
      // Return the edge object associated with the given handle.
      edge_type&       get(edge e)       { return elist[e.value()]; }
      const edge_type& get(edge e) const { return elist[e.value()]; }
      
      // Return an iterator to the first vertex.
      vertex_iterator begin_vertices() const { return 0; }
      
      // Return an iterator past the last vertex.
      vertex_iterator end_vertices() const { return order(); }

      // Return an iterator to the first edge.
      edge_iterator begin_edges() const { return 0; }
      
      // Return an iterator past the last edge.
      edge_iterator end_edges() const { return size(); }
      
      // Return an iterator to the first out edge of v.
      incident_edge_iterator begin_out_edges(vertex v) const { return get(v).out().begin(); }
      
      // Return an iterator past the last out edge of v.
      incident_edge_iterator end_out_edges(vertex v) const { return get(v).out().end(); }

      // Return an iterator to the first incident edge of v.
      incident_edge_iterator begin_in_edges(vertex v) const { return get(v).in().begin(); }
      
      // Return an iterator past the last incident edge of v.
      incident_edge_iterator end_in_edges(vertex v) const { return get(v).in().end(); }

    private:
      vertex_list vlist;
      edge_list elist;
    };

    
    
  template<typename V, typename E, typename A>
    inline auto directed_adjacency_list<V, E, A>::
      get_edge(vertex u, vertex v) const -> edge
      {
        auto& out = get(u).out();
        auto iter = std_find_if(out.begin(), out.end(), if_has_target(*this, v));
        return iter == out.end() ? edge{} : *iter;
      }

} // namespace origin

#endif
