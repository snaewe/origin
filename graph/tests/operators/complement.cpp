// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/generator.hpp>
#include <origin/graph/operator.hpp>
#include <origin/graph/io.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef directed_adjacency_list<char, int> Digraph;
  
  // Make the graph a -> b -> c -> d.
  Digraph g = iota_path_graph<Digraph>({'a', 'b', 'c', 'd'}, 1);
  write_edge_list(cout, g);
  cout << "---\n";

  // The complement should be:
  Digraph h = graph_complement(g);
  write_edge_list(cout, h);
}
