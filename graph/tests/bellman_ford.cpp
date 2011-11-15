// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <functional>
#include <limits>
#include <cstddef>

#include <origin/graph/algorithm/shortest_path/bellman_ford.hpp>
#include <origin/graph/adjacency_matrix.hpp>
#include <origin/ordinal_map.hpp>
#include <origin/graph/label.hpp>

#include "dijkstra/detail.hpp"

using namespace origin;
using std::cerr;


int main() {

  typedef distance_matrix<char> graph_type;
  typedef typename graph_type::vertex vertex;
  typedef ordinal_map<vertex, float> distance_map;
  typedef vertex_label<distance_map> distance_label;

  const unsigned int num_vertices = 3u;
  distance_map dm(num_vertices);
  graph_type g(num_vertices);

  bellman_ford(g, vertex(0), distance_label(dm));

  cerr << "Test up\n";

  return 0;
} 
