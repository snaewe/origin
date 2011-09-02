// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/graph/adjacency_vector.hpp>
#include <origin/graph/algorithm/topological_sort.hpp>

using namespace std;
using namespace origin;

// Actually test a graph to see if it works right.
template<typename Graph>
  void test()
  {
    typedef typename Graph::vertex Vertex;

    /*
        a
       / \
      b   c
          |
          d
    */
    Graph g;
    Vertex a = g.add_vertex('a');
    Vertex b = g.add_vertex('b');
    Vertex c = g.add_vertex('c');
    Vertex d = g.add_vertex('d');
    g.add_edge(a, c);
    g.add_edge(a, b);
    g.add_edge(c, d);

    auto vis = on_discover_vertex(ostream_visit(cout, " "), dfs_visitor{});
    depth_first_search(g, a, vis);
    cout << "\n";
    
    vector<Vertex> order(4);
    topological_sort(g, order.begin());
    
    for(auto v : order) {
      cout << g[v] << " ";
    }
    cout << "\n";
  }

int main()
{
  typedef directed_adjacency_vector<char, int> Graph;
  test<Graph>();
}
