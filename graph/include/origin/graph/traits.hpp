// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_TRAITS_HPP
#define ORIGIN_GRAPH_TRAITS_HPP

#include <type_traits>

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

    typedef typename Graph::vertex vertex;
    typedef typename Graph::edge edge;
    typedef typename Graph::vertex_range vertex_range;
    typedef typename Graph::edge_range edge_range;

    // For convenience
    typedef typename vertex_range::iterator vertex_iterator;
    typedef typename edge_range::iterator edge_iterator;
  };

  template<typename Graph>
  struct graph_traits<Graph const>
  {
    typedef typename Graph::size_type size_type;

    typedef typename Graph::const_vertex vertex;
    typedef typename Graph::const_edge edge;
    typedef typename Graph::const_vertex_range vertex_range;
    typedef typename Graph::const_edge_range edge_range;

    // For convenience
    typedef typename vertex_range::iterator vertex_iterator;
    typedef typename edge_range::iterator edge_iterator;
  };

  // FIXME: Build similar accessors for edge vertices. Consider building
  // similar abstractions for all common graph operations (vertices, edges,
  // etc.).

  // FIXME: This requires that the graph has interable vertex set. In other
  // words, this is only instantiable if g.vertices() is a valid expresson
  // of g.

  /**
   * @fn begin_vertices
   * Return an iterator to the first vertex in the graph.
   */
  //@{
  template<typename Graph>
  typename graph_traits<Graph>::vertex_iterator
  begin_vertex(Graph& g)
  { return std::begin(g.vertices()); }

  template<typename Graph>
  typename graph_traits<Graph const>::vertex_iterator
  begin_vertex(Graph const& g)
  { return std::begin(g.vertices()); }
  //@}

  /**
   * @fn end_vertices
   * Return an iterator past the end of the graph's vertex set.
   */
  //@{
  template<typename Graph>
  typename graph_traits<Graph>::vertex_iterator
  end_vertex(Graph& g)
  { return std::end(g.vertices()); }

  template<typename Graph>
  typename graph_traits<Graph const>::vertex_iterator
  end_vertex(Graph const& g)
  { return std::end(g.vertices()); }
  //@}

} // namesapce origin

#endif
