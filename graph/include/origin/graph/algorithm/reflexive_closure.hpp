// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_REFLEXIVE_CLOSURE_HPP
#define ORIGIN_GRAPH_ALGORITHM_REFLEXIVE_CLOSURE_HPP

#include <origin/graph/traits.hpp>

namespace origin
{
  /**
   * Return the reflexive closure of the given graph. The reflexive closure
   * of a graph is the reflexive closure of its edge set. This is to say,
   * for every vertex v in V(G), the loop (v, v) is in the graph G.
   *
   * @tparam Graph Any Directed or Undirected Pseudograph.
   *
   * @todo Add an overload or additional parameter that supports edge
   * attribution.
   */
  template<typename Graph>
  Graph reflexive_closure(Graph g)
  {
    for(auto v : g.vertices()) {
      if(!g.edge(v, v)) {
        g.add_edge(v, v);
      }
    }
  }

} // namesapce origin

#endif
