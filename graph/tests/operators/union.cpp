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

  // Make the graphs: g == a -> b -> c -> d.
  Digraph g = iota_path_graph<Digraph>({'a', 'b', 'c', 'd'}, 1);
  write_edge_list(cout, g);
  cout << "---\n";

  // Make the graph: h == a -> c, a -> d, b -> d.
  Digraph h; 
  {
    auto a = add_vertex(h, 'a');
    auto b = add_vertex(h, 'b');
    auto c = add_vertex(h, 'c');
    auto d = add_vertex(h, 'd');
    add_edge(h, a, c, 4);
    add_edge(h, a, d, 5);
    add_edge(h, b, d, 6);
  }
  write_edge_list(cout, h);
  cout << "---\n";
  
  // The transpose should be complete (directed) graph K4
  // FIXME: Actually assert the structure of the graph.
  Digraph result = graph_union(g, h);
  write_edge_list(cout, result);
}
