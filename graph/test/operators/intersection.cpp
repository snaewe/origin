// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/range.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/generator.hpp>
#include <origin/graph/operator.hpp>
#include <origin/graph/io.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef undirected_adjacency_list<char, int> Graph;

  // Make the graph K4
  Graph g = iota_complete_graph<Graph>({'a', 'b', 'c', 'd'}, 1);
  write_edge_list(cout, g);
  cout << "---\n";

  // Make the graph C4
  Graph h = iota_cycle_graph<Graph>({'a', 'b', 'c', 'd'}, 1);
  write_edge_list(cout, h);
  cout << "---\n";
  
  // Result == C4.
  Graph result = graph_intersection(g, h);
  write_edge_list(cout, result);
}
