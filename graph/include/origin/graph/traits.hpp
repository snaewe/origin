// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_TRAITS_HPP
#define ORIGIN_GRAPH_TRAITS_HPP

#include <origin/utility/meta.hpp>

namespace origin
{
  // FIXME: What are the different kinds of graph mutability:
  // Static Vertex Graph - The graph is initialized over a set of vertices
  // Static Graph - The graph is initialized over a set of vertices and edges
  // Buildable Vertex Graph - Vertices can be added to the graph
  // Buildable Edge Graph - Edges can be added to the graph.
  // Dynamic Vertex Graph - Vertices can be added and removed.
  // Dynamic Edge Graph - Edges can be added and removed.
  // Buildable Graph - Vertices and edges can be added to the graph.
  // Dynamic Graph - Vertices and edges can be added and removed.

  // the adjacency vector is buildable
  // the adjacency list is dynamic
  // the adjacency matrix is static vertex/dynamic edge
  // the static graph is static.

  // Directionality
  struct directed_graph_tag { };
  struct undirected_graph_tag { };
  
  // Mutability
  struct vertex_initialized_graph_tag { };
  struct initialized_graph_tag : vertex_initialized_graph_tag { };

  // FIXME: Is a buildable graph also initialized?
  struct vertex_buildable_graph_tag : vertex_initialized_graph_tag { };
  struct edge_buildable_graph_tag { };
  struct buildable_graph_tag : vertex_buildable_graph_tag, edge_buildable_graph_tag { };

  struct vertex_dynamic_graph_tag : vertex_buildable_graph_tag { };
  struct edge_dynamic_graph_tag : edge_buildable_graph_tag { };
  struct dynamic_graph_tag : vertex_dynamic_graph_tag, edge_dynamic_graph_tag { };
  
  
  template<typename G>
    class __is_graph
    {
      template<typename T>
        static typename T::graph_category check(T const&);
      static substitution_failure check(...);
    public:
      typedef decltype(check(std::declval<G>())) type;
    };
    
  // True if G is a graph. Graph's declare a nested type, graph_category.
  template<typename G>
    struct is_graph
      : substitution_succeeded<typename __is_graph<G>::type>::type
    { };
  
  // FIXME: Many of these can be syntactically evaluated without tag classes.
  // For example, vertex buildable graphs have add_vertex, dynamic vertex
  // graphs have remove_vertex. Implement this.
  
  // Return true if G can be initialized over a vertex set. Vertices may not
  // be added or removed.
  template<typename G>
    struct is_vertex_initialized_graph
      : std::is_base_of<vertex_initialized_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G can be initialized over a vertex and edge set. Vertices
  // and edges may not be removed.
  template<typename G>
    struct is_initialized_graph
      : std::is_base_of<initialized_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental construction of the vertex set.
  // Vertices can be added but not removed.
  template<typename G>
    struct is_vertex_buildable_graph
      : std::is_base_of<vertex_buildable_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental construction of the edge set. Edges
  // can be added but not removed.
  template<typename G>
    struct is_edge_buildable_graph
      : std::is_base_of<edge_buildable_graph_tag, typename G::graph_category>::type
    { };
    
  // Return true if G is incremtnally constructible. Vertices and edges can 
  // added but not removed.
  template<typename G>
    struct is_buildable_graph
      : std::is_base_of<buildable_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental construction and destruction of the
  // vertex set. Vertices can be added and removed.
  template<typename G>
    struct is_vertex_dynamic_graph
      : std::is_base_of<vertex_dynamic_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental consruction and destruction of the
  // edge set. Edges can be added and removed.
  template<typename G>
    struct is_edge_dynamic_graph
      : std::is_base_of<edge_dynamic_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental construction and destruction. 
  // Vertices and edges can be added and removed.
  template<typename G>
    struct is_dynamic_graph 
      : std::is_base_of<dynamic_graph_tag, typename G::graph_category>::type
    { };
    
  /**
   * Return true if the graph G is directed.
   */
  template<typename G>
  struct is_directed_graph
    : std::is_base_of<directed_graph_tag, typename G::graph_category>::type
  { };

  /**
   * Return true if the graph G is undirected.
   */
  template<typename G>
  struct is_undirected_graph
    : std::is_base_of<undirected_graph_tag, typename G::graph_category>::type
  { };
  //@}

  /**
   * The graph traits class abstracts the access to associated types of a graph
   * class, providing a uniform and simplified abstraction. The traits class
   * effetively hides the difference between graphs and const graphs.
   */
  template<typename Graph>
  struct graph_traits
  {
    typedef typename Graph::size_type size_type;
    typedef typename Graph::vertex_value_type vertex_value_type;
    typedef typename Graph::edge_value_type edge_value_type;

    typedef typename Graph::vertex vertex;
    typedef typename Graph::edge edge;
    typedef typename Graph::vertex_range vertex_range;
    typedef typename Graph::edge_range edge_range;
  };

  template<typename Graph>
  struct graph_traits<Graph const>
  {
    typedef typename Graph::size_type size_type;

    typedef typename Graph::const_vertex vertex;
    typedef typename Graph::const_edge edge;
    typedef typename Graph::const_vertex_range vertex_range;
    typedef typename Graph::const_edge_range edge_range;
  };

  // The graph interface is exposed as free functions.

  // Return true if the graph has no vertices.
  template<typename G>
    inline typename std::enable_if<is_graph<G>::value, bool>::type 
    null(G const& g)
    {
      return g.null();
    }
    
  // Return the number of vertices in a graph.
  template<typename G>
    inline auto order(G const& g) -> decltype(g.order())
    {
      return g.order();
    }

  // Return true if the graph is empty.
  template<typename G>
    inline typename std::enable_if<is_graph<G>::value, bool>::type
    empty(G const& g)
    {
      return g.empty();
    }

  // Return the size of a graph
  template<typename G>
    inline auto size(G const& g) ->
      typename std::enable_if<is_graph<G>::value, decltype(g.size())>::type
    {
      return g.size();
    }

  // Return the out degree of the vertex v in the directed graph g.
  template<typename G, typename V>
    inline auto out_degree(G const& g, V v) -> decltype(g.out_degree(v))
    {
      return g.out_degree(v);
    }
    
  // Return the in degree of the vertex v in the directed graph g.
  template<typename G, typename V>
    inline auto in_degree(G const& g, V v) -> decltype(g.in_degree(v))
    {
      return g.in_degree(v);
    }

  // Return the degree of the vertex v. This is the total number of edges
  // incident to the vertex.
  template<typename G, typename V>
    inline auto degree(G const& g, V v) -> decltype(g.degree(v))
    {
      return g.degree(v);
    }

  // Return the vertex set of a graph
  template<typename G>
    inline auto vertices(G& g) -> decltype(g.vertices())
    {
      return g.vertices();
    }
    
  // Add a vertex to the graph
  template<typename G>
    inline auto add_vertex(G& g) -> decltype(g.add_vertex())
    {
      return g.add_vertex();
    }

  // FIXME: Replace T with VertexValueType<G>.

  // Add a vertex to the graph with the specified property.
  template<typename G, typename T>
    inline auto add_vertex(G& g, T const& value) -> decltype(g.add_vertex(value))
    {
      return g.add_vertex(value);
    }

    
  // Return the edge set of a graph
  template<typename G>
    inline auto edges(G& g) -> decltype(g.edges())
    {
      return g.edges();
    }
    
  // Return an edge descriptor connecting the vertices u and v, if one exists.
  // If not, return a null edge descriptor.
  template<typename G, typename V>
    inline auto get_edge(G& g, V u, V v) -> decltype(g.get_edge(u, v))
    {
      return g.get_edge(u, v);
    }

  // Const version of the previous function.
  template<typename G, typename V>
    inline auto get_edge(G const& g, V u, V v) -> decltype(g.get_edge())
    {
      return g.edge();
    }

    
  // FIXME: Replace E with EdgeType<G> for source and target.

  template<typename G, typename E>
    inline auto source(G const& g, E e) -> decltype(g.source(e))
    {
      return g.source(e);
    }

  template<typename G, typename E>
    inline auto target(G const& g, E e) -> decltype(g.target(e))
    {
      return g.target(e);
    }
    
  // TODO: I should really be using more specific graph properties, but for
  // some reason the compiler isn't connecting with them.

  // Add an edge to the graph.
  template<typename G, typename V>
    inline auto add_edge(G& g, V u, V v) -> decltype(g.add_edge(u, v))
    {
      return g.add_edge(u, v);
    }

  // Add an edge to the graph with the specified property.
  template<typename G, typename V, typename T>
    inline auto add_edge(G& g, V u, V v, T const& value)
      -> decltype(g.add_edge(u, v, value))
    {
      return g.add_edge(u, v, value);
    }


  // Graph operations



  // Return a range of edges incident to the vertex. For directed graphs this
  // is always the out edges. For undirected graphs, it is the entire set of
  // incident edges.
  //
  // FIXME: Should be called IncidentEdges?
  template<typename Graph, typename V>
    inline auto out_edges(Graph& g, V v)
      -> typename std::enable_if<
          is_directed_graph<Graph>::value, decltype(g.out_edges(v))
        >::type
    {
      return g.out_edges(v); 
    }

  // Specialization for const directed graphs
  template<typename Graph, typename V>
    inline auto out_edges(Graph const& g, V v)
      -> typename std::enable_if<
          is_directed_graph<Graph>::value, decltype(g.out_edges(v))
        >::type
    {
      return g.out_edges(v); 
    }

  // Specialization for undirected graphs.
  template<typename Graph, typename V>
    inline auto out_edges(Graph& g, V v)
      -> typename std::enable_if<
          is_undirected_graph<Graph>::value, decltype(g.incident_edges(v))
        >::type
    { 
      return g.incident_edges(v); 
    }

  // Specialization for const undirected graphs
  template<typename Graph, typename V>
    inline auto out_edges(Graph const& g, V v)
      -> typename std::enable_if<
          is_undirected_graph<Graph>::value, decltype(g.incident_edges(v))
        >::type
    { 
      return g.incident_edges(v); 
    }

  // Return the in edges of the vertex v in the directed graph g.
  template<typename G, typename V>
    inline auto in_edges(G& g, V v) -> decltype(g.in_edges(v))
    {
      return g.in_edges(v);
    }
  
  template<typename G, typename V>
    inline auto in_edges(G const& g, V v) -> decltype(g.in_edges(v))
    {
      return g.in_edges(v);
    }

  
  // Return the incident edges of the vertex v in the undirected graph g.
  template<typename G, typename V>
    inline auto incident_edges(G& g, V v) -> decltype(g.incident_edges(v))
    {
      return g.incident_edges(v);
    }
    
  template<typename G, typename V>
    inline auto incident_edges(G const& g, V v) -> decltype(g.incident_edges(v))
    {
      return g.incident_edges(v);
    }


  // Return the opposite end of the given edge.
  template<typename Graph, typename Edge, typename Vertex>
    Vertex opposite(Graph& g, Edge e, Vertex v)
    { 
      return source(g, e) == v ? target(g, e) : v; 
    }

} // namesapce origin

#endif
