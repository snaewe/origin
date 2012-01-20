// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <functional>
#include <limits>
#include <cstddef>

#include "dijkstra/detail.hpp"
#include <origin/graph/algorithm/shortest_path/dijkstra.hpp>

using namespace origin;

int main() {

  // Type definitions
  /*typedef distance_matrix<char> Dist_Graph;
  typedef typename Dist_Graph::vertex Vertex;
  typedef typename Dist_Graph::edge Edge;
  typedef ordinal_map<Vertex, float> Distance_Map;
  typedef vertex_label<Distance_Map> Distance_Label;
  typedef debug_dijkstra_visitor<Dist_Graph> Visitor;
  typedef typename Visitor::parent_label_type shortest_path_parent_map;

  Dist_Graph g(4);

  Vertex v[] = { Vertex(0), Vertex(1), Vertex(2), Vertex(3) };

  g[v[0]] = 'a';
  g[v[1]] = 'b';
  g[v[2]] = 'c';
  g[v[3]] = 'd';*/

  /**   0
   *   / \
   *  1---2
   *   \ /
   *    3
   */

  /*g.add_edge(v[0], v[1], 1.0f);
  g.add_edge(v[0], v[2], 2.0f);
  g.add_edge(v[1], v[2], 0.2f);
  g.add_edge(v[2], v[1], 0.1f);
  g.add_edge(v[1], v[3], 3.0f);
  g.add_edge(v[2], v[3], 1.0f);

  shortest_path_parent_map parents;
  Distance_Map dm(4u);
  dijkstra_shortest_paths(g, v[0], Distance_Label(dm), Visitor(parents));*/


  /*auto path = dsp.visitor_.get_path_to(v[3]);
  
  dsp.visitor_.print(g);*/

  return 0;
}
