// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <functional>
#include <limits>
#include <cstddef>

#include <origin/graph/algorithm/shortest_path/dijkstra.hpp>
#include <origin/graph/adjacency_matrix.hpp>
#include <origin/ordinal_map.hpp>

#include "dijkstra/detail.hpp"

using namespace origin;

int main() {

  /*typedef distance_matrix<char> DistGraph;
  typedef typename DistGraph::vertex Vertex;
  typedef typename DistGraph::edge Edge;
  typedef ordinal_label<Vertex, float> DistanceLabel;
  typedef default_weight_label<DistGraph> WeightLabel;
  typedef debug_dijkstra_visitor<DistGraph> Visitor;*/

  // Type definitions
  typedef distance_matrix<char> Dist_Graph;
  typedef typename Dist_Graph::vertex Vertex;
  typedef typename Dist_Graph::edge Edge;
  typedef ordinal_label<Vertex, float> Distance_Label;

  Dist_Graph g(4);

  Vertex v[] = { Vertex(0), Vertex(1), Vertex(2), Vertex(3) };

  g[v[0]] = 'a';
  g[v[1]] = 'b';
  g[v[2]] = 'c';
  g[v[3]] = 'd';

  /**   0
   *   / \
   *  1---2
   *   \ /
   *    3
   */

  g.add_edge(v[0], v[1], 1.0f);
  g.add_edge(v[0], v[2], 2.0f);
  g.add_edge(v[1], v[2], 0.2f);
  g.add_edge(v[2], v[1], 0.1f);
  g.add_edge(v[1], v[3], 3.0f);
  g.add_edge(v[2], v[3], 1.0f);

  dijkstra_shortest_paths(g, v[0], Distance_Label(), default_dijkstra_visitor());

  /*dijkstra_shortest_paths_draft_old<
    DistGraph,
    DistanceLabel,
    WeightLabel,
    Visitor
  > dsp(g, v[0]);
  dsp();

  auto path = dsp.visitor_.get_path_to(v[3]);
  
  dsp.visitor_.print(g);*/

  return 0;
}
