// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SYMMETRIC_CLOSURE_HPP
#define ORIGIN_GRAPH_ALGORITHM_SYMMETRIC_CLOSURE_HPP

#include <origin/graph/traits.hpp>

namespace origin
{
  /**
   * Return the symmetric closure of a directed graph is the symmetric closure
   * of its edge set. For each edge (u, v) in E(g), the symmetric edge (v, u)
   * is also in E.
   *
   * @tparam Graph A Directed Graph.
   *
   * @note This algorithm is syntactically conformant to any Graph type. For
   * Undirected Graphs, however, this is simply the identity operation. For
   * every edge (u, v), the edge (v, u) is the same.
   *
   * @todo Add an overload or additional parameter that supports edge
   * attribution.
   */
  template<typename Graph>
  Graph reflexive_closure(Graph g)
  {
    for(auto e : g.edges()) {
      auto u = g.source(e);
      auto v = g.target(e);
      if(!g.edge(v, u)) {
        g.add_edge(v, u);
      }
    }
    return g;
  }

} // namesapce origin

#endif
