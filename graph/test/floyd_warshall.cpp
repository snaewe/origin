// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <functional>
#include <limits>
#include <cstddef>

#include <origin/graph/algorithm/shortest_path/floyd_warshall.hpp>
#include <origin/graph/adjacency_matrix.hpp>
#include <origin/ordinal_map.hpp>
#include <origin/graph/label.hpp>

#include "floyd_warshall/detail.hpp"

using namespace origin;

const unsigned int num_vertices = 8;

typedef distance_matrix<char> graph;
typedef typename graph::vertex vertex;
typedef typename graph::edge edge;
typedef dist_mtx<float, vertex, num_vertices> distance;

int main() {

  distance d;

  // Create the graph
  graph g(num_vertices);
  for(char i = 0; i < (signed int)(num_vertices); ++i)
    g[vertex(i)] = 'a' + i;
  generate_wheel_graph(g, [](unsigned int n){ return 1.0f; });


  // Call the shortest paths alg on it.
  all_pairs_shortest_paths(g, d);

  for(auto i = 0u; i < num_vertices; ++i)
    for(auto j = 0u; j < num_vertices; ++j)
      std::cout << '(' << g[vertex(i)]<< ',' << g[vertex(j)] << ") : "
                << d(i,j) << '\n';

  return 0;
} 
