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
   * Return an iterator to the first vertex in the graph.
   */
  template<typename Graph>
  typename graph_traits<Graph>::vertex_iterator
  begin_vertex(Graph& g)
  { return begin(g.vertices()); }
  
  template<typename Graph>
  typename graph_traits<Graph const>::vertex_iterator
  begin_vertex(Graph const& g)
  { return end(g.vertices()); }

  /**
   * Return an iterator past the end of the graph's vertex set.
   */
  template<typename Graph>
  typename graph_traits<Graph>::vertex_iterator
  end_vertex(Graph& g)
  { return end(g.vertices()); }

  template<typename Graph>
  typename graph_traits<Graph const>::vertex_iterator
  end_vertex(Graph const& g)
  { return end(g.vertices()); }

  /**
   * @defgroup graph_traits
   * @ingroup graph
   *
   * The types and traits in this category support conceptual abstractions for
   * graph data structures.
   */
  //@{
  struct directed_graph_tag { };
  struct undirected_graph_tag { };

  /**
   * Return true if the graph G is directed.
   */
  template<typename G>
  struct is_directed_graph
    : std::is_convertible<typename G::graph_category, directed_graph_tag>::type
  { };

  /**
   * Return true if the graph G is undirected.
   */
  template<typename G>
  struct is_undirected_graph
    : std::is_convertible<typename G::graph_category, undirected_graph_tag>::type
  { };
  //@}
} // namesapce origin

#endif
