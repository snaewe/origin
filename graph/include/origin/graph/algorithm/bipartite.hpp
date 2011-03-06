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
   * The is-bipartite algoirthm object determines if a graph is bipartite, 
   * i.e., that it can be colored with two colors.
   *
   * @tparam Graph A graph type
   * @tparam Color_Label A Read_Write_Label mapping vertices to a Color type.
   *         This defaults to an internal label mapping vertices to bool 
   *         values.
   */
  template<typename Graph, 
           typename Color_Label = internal_label<Graph, bool>>
  struct is_bipartite_algo
  {
    typedef Graph graph_type;

    typedef vertex_property<Graph, Color_Label> color_property;
    typedef typename color_property::label_type color_label;

    typedef two_color_visitor<Graph, color_label> visitor_type;
  
    // Construct the algorithm with a default label.
    is_bipartite_algo(Graph& g)
      : graph(g), color(g), visitor(color.label)
    { }
  
    // Construct the algorithm with a custom label.
    is_bipartite_algo(Graph& g, Color_Label label)
      : graph(g), color(label), visitor(color.label)
    { }

    bool operator()()
    {
      try {
        breadth_first_search(graph, visitor);
      } catch(...) {
        return false;
      }
      return true;
    }
    
    graph_type& graph;
    color_property color;
    visitor_type visitor;
  };

  // FIXME: How do you access the color map, if you just want to use the 
  // default? I suspect that the best answer would be to simply use the
  // is_bipartite algorithm object since it retains state after the completion
  // of the algorithm.
  //
  // FIXME: Write this as a kind of recipe in the documentation.

  /**
   * @function is_biparite(g)
   *
   * Return true if the graph is bipartite.
   */
  //@{
  template<typename Graph>
  bool is_bipartite(Graph& g)
  {
    is_bipartite_algo<Graph> algo(g);
    return algo();
  }
  
  template<typename Graph>
  bool is_bipartite(Graph const& g)
  {
    is_bipartite_algo<Graph> algo(g);
    return algo();
  }
  //@}

  /**
   * @function is_bipartite(g, color)
   *
   * Return true if the graph is bipartite. Store the results of the mapping
   * in the given color mapping.
   */
  //@{
  template<typename Graph, typename Color_Label>
  bool is_bipartite(Graph& g, Color_Label label)
  {
    is_bipartite_algo<Graph, Color_Label> algo(g, label);
    return algo();
  }

  template<typename Graph, typename Color_Label>
  bool is_bipartite(Graph const& g, Color_Label label)
  {
    is_bipartite_algo<Graph const, Color_Label> algo(g, label);
    return algo();
  }
  //@}
} // namespace origin

#endif

