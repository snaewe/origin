// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BELLMAN_FORD_COMMON
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BELLMAN_FORD_COMMON

//#include <origin/graph/algorithm/shortest_path/shortest_path_common.hpp>
#include <origin/graph/traits.hpp>
#include <origin/graph/label.hpp>

namespace origin {

  /**
   * Default Bellman-Ford visitor
   */
  class default_bellman_ford_visitor
  {
  public:
    template<typename Graph, typename Edge>
    void examine_edge(Graph const&, Edge)
    { }

    template<typename Graph, typename Edge>
    void edge_relaxed(Graph const&, Edge)
    { }

    template<typename Graph, typename Edge>
    void edge_not_relaxed(Graph const&, Edge)
    { }

    template<typename Graph, typename Edge>
    void edge_minimized(Graph const&, Edge)
    { }

    template<typename Graph, typename Edge>
    void edge_not_minimized(Graph const&, Edge)
    { }
  };

}

#endif //ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BELLMAN_FORD_COMMON
