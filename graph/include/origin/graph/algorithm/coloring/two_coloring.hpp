// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_COLORING_TWO_COLORING_HPP
#define ORIGIN_GRAPH_ALGORITHM_COLORING_TWO_COLORING_HPP

#include <origin/graph/color.hpp>
#include <origin/graph/algorithm/coloring/common.hpp>

namespace origin
{
  /**
   * @ingroup graph_color
   *
   * The two-color visitor attempts to assign a two-color assignment to each
   * vertex in a graph, as visited by a breadth- or depth-first search. If the
   * graph is not connected, then each root vertex is initially colored white.
   */
  template<typename Graph, typename Color_Label, typename Base>
    struct two_color_visitor : Base
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename graph_traits<Graph>::edge Edge;
      typedef typename label_traits<Color_Label, Vertex>::value_type Color;

      two_color_visitor(Color_Label label)
        : color(label)
      { }

      // Color each search tree root white.
      void root_vertex(Graph& g, Vertex v)
      { 
        color(v) = color_traits<Color>::white(); 
      }

      // Color the end of each edge as the opposite color of the source.
      void tree_edge(Graph& g, Edge e)
      {
        Vertex u = g.source(e);
        Vertex v = g.target(e);
        color(v) = two_color_traits<Color>::opposite(color(u));
      }

      // If the target of a nontree edge is colored the same as the source 
      // vertex then the a two-coloring cannot be computed.
      void nontree_edge(Graph& g, Edge e)
      {
        Vertex u = g.source(e);
        Vertex v = g.target(e);
        if(color(u) == color(v))
          throw graph_coloring_error("cannot assign two-coloring");
      }

      Color_Label color;
    };

  /**
   * @ingroup graph_color
   * @fn two_coloring(g, color)
   *
   * Compute a two-coloring of the given graph, assigning the results through
   * the given color label. After completion, the color label can be queried
   * to deterimine the component of each vertex.
   *
   * An exception is thrown if a two-coloring cannot be assigned to the graph.
   */
  //@{
  template<typename Graph, typename Color_Label>
    void two_coloring(Graph& g, Color_Label color)
    {
      two_color_visitor<Graph, Color_Label, bfs_visitor> vis(color);
      breadth_first_search_all(g, vis);
    }
  
  // HOWTO: How do I find which component each vertex belongs to?
  //
  // vertex_map<Graph, bool> color;
  // two_coloring(g, label(color));
  // if(color(v))
  //   v is in component 0
  // else
  //   v is in component 1

} // namespace origin

#endif

