// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/graph/adjacency_vector.hpp>
#include <origin/graph/algorithm/bipartite.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef directed_adjacency_vector<char, int> Graph;

  { // K2 is bipartite
    Graph g;
    auto u = g.add_vertex();
    auto v = g.add_vertex();
    g.add_edge(u, v);    
    assert(( is_bipartite(g) ));
  }
  
  { // K3 is not.
//     Graph g;
//     auto u = g.add_vertex();
//     auto v = g.add_vertex();
//     auto w = g.add_vertex();
//     g.add_edge(u, v);
//     g.add_edge(v, w);
//     g.add_edge(w, u);
//     assert(( !is_bipartite(g) ));
  }
}
