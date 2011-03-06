// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_COLORING_TWO_COLORING_HPP
#define ORIGIN_GRAPH_ALGORITHM_COLORING_TWO_COLORING_HPP

#include <origin/graph/algorithm/coloring/common.hpp>

namespace origin
{
  /**
   * The two-color visitor attempts to assign a two-color assignment to each
   * vertex in a graph, as visited by a breadth- or depth-first search. If the
   * graph is not connected, then each root vertex is initially colored white.
   */
  template<typename Graph, typename Color_Label>
  struct two_color_visitor : bfs_visitor
  {
    typedef Graph graph_type;
    typedef typename graph_traits<Graph>::vertex vertex;
    typedef typename graph_traits<Graph>::edge edge;
    
    // FIXME: This is gross. Isn't there simpler way of getting the value
    // type from the result of a function? Maybe I should just break down and
    // write a value_type trait, if that's really what's needed. Maybe remove
    // reference is sufficient?
    typedef typename std::remove_cv<
      typename std::remove_reference<
        typename std::result_of<Color_Label(vertex)>::type
      >::type
    >::type color_value;
    
    two_color_visitor(Color_Label label)
      : color(label)
    { }
   
    // Color each search tree root white.
    void root_vertex(Graph& g, vertex v)
    { color(v) = color_traits<color_value>::white(); }

    // Color the end of each edge as the opposite color of the source.
    void tree_edge(Graph& g, edge e)
    { 
      vertex u = g.source(e);
      vertex v = g.target(e);
      color(v) = two_color_traits<color_value>::opposite(color(u));
    }
    
    // If the target of a nontree edge is colored the same as the source vertex
    // then the a two-coloring cannot be computed.
    void nontree_edge(Graph& g, edge e)
    {
      vertex u = g.source(e);
      vertex v = g.target(e);
      if(color(u) == color(v)) {
        throw graph_coloring_error("cannot compute two-coloring");
      }
    }
    
    Color_Label color;
  };
  
  // FIXME: It might be nice to have a couple of functions that return the
  // coloring. I'm not sure what this would look like, just yet, and it doesn't
  // really match the style set forth by any other algorithms.

  /**
   * @function two_coloring(g, color)
   *
   * Compute a two-coloring of the given graph, assigning the results through
   * the given color label.
   *
   * An exception is thrown if the graph is not two-colorable (a.k.a., 
   * bipartite).
   */
  //@{
  template<typename Graph, typename Color_Label>
  void two_coloring(Graph& g, Color_Label color)
  {
    two_color_visitor<Graph, Color_Label>(g, color);
    breadth_first_search(g, color);
  }

  template<typename Graph, typename Color_Label>
  void two_coloring(Graph const& g, Color_Label color)
  {
    two_color_visitor<Graph, Color_Label>(g, color);
    breadth_first_search(g, color);
  }
  //@}

} // namespace origin

#endif

