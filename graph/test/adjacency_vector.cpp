// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/graph/adjacency_vector.hpp>

using namespace std;
using namespace origin;

int main()
{
  directed_adjacency_vector<char, int> g;
  auto u = g.add_vertex('a');
  auto v = g.add_vertex('b');
  g.add_edge(u, v, 0);
  g.add_edge(u, v, 1);
  assert(( g.order() == 2 ));
  assert(( g.size() == 2 ));
  
  for(auto x : g.vertices()) {
    cout << g[x] << "\n";
  }
  for(auto x : g.edges()) {
    cout << g[x] << "\n";
  }
  for(auto x : g.out_edges(u)) {
    cout << g[x] << "\n";
  }
  
}
