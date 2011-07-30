// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_BIPARTITE_HPP
#define ORIGIN_GRAPH_ALGORITHM_BIPARTITE_HPP

#include <origin/graph/algorithm/search/breadth_first.hpp>
#include <origin/graph/algorithm/coloring/two_coloring.hpp>

namespace origin
{
  /**
   * @fn is_biparite(g)
   * @fn is_biparite(g, color)
   *
   * Return true if the graph is bipartite. The algorithm tries to find a
   * two-coloring for the graph. The graph is bipartite if a two-coloring can
   * be found.
   *
   * @tparam Graph        An Outward_Graph type
   * @tparam Color_Label  If present, a Vertex_Label associating vertices with
   *                      a Two_Color (minimally).
   *
   * @param g       A Graph object
   * @param color   If specified, a Color_Label
   */
  //@{
  template<typename Graph, typename Color_Label>
    bool is_bipartite(Graph& g, Color_Label label)
    {
      try {
        two_coloring(g, label);
      } catch(...) {
        return false;
      }
      return true;
    }

  template<typename Graph>
    bool is_bipartite(Graph& g)
    {
      vertex_map<Graph, bool> cm(g.order());
      return is_bipartite(g, label(cm));
    }
  
  // HOWTO: How do I find which component each vertex is in?
  //
  // See two_coloring.

} // namespace origin

#endif

