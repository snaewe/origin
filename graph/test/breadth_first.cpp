// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/algorithm/search/breadth_first.hpp>

using namespace std;
using namespace origin;


// Actually test a graph to see if it works right.
template<typename Graph>
  void test()
  {
    typedef typename Graph::vertex Vertex;
    typedef typename Graph::const_vertex Const_Vertex;

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
    g.add_edge(a, b);
    g.add_edge(a, c);
    g.add_edge(c, d);
    
    // Check distances
    vertex_map<Graph, int> dist(g.order());
    breadth_first_distance(g, a, label(dist));
    assert(( dist[a] == 0 ));
    assert(( dist[b] == 1 ));
    assert(( dist[c] == 1 ));
    assert(( dist[d] == 2 ));
    
    // Check predecessors
    vertex_map<Graph, Vertex> pred(g.order());
    breadth_first_search_tree(g, a, label(pred));
    assert(( pred[a] == a ));
    assert(( pred[b] == a ));
    assert(( pred[c] == a ));
    assert(( pred[d] == c ));
    
    auto vis = on_discover_vertex(ostream_visit(cout, " "), bfs_visitor{});
    breadth_first_search(g, a, vis);
    cout << "\n";
  }

int main()
{
  typedef directed_adjacency_list<char, int> Graph;
  test<Graph>();
}
