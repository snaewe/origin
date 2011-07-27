// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <functional>
#include <limits>
#include <cstddef>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/algorithm/search/breadth_first.hpp>
#include <origin/ordinal_map.hpp>

using namespace std;
using namespace origin;

struct ostream_visitor : bfs_visitor
{
  template<typename G, typename V>
    void discovered_vertex(G& g, V v)
    {
      cout << g[v] << "\n";
    }
};

int main() 
{
  typedef directed_adjacency_list<char> Graph;
  typedef Graph::vertex Vertex;
  typedef Graph::const_vertex Const_Vertex;

  Graph g;
  Vertex a =  g.add_vertex('a');
  Vertex b =  g.add_vertex('b');
  Vertex c =  g.add_vertex('c');
  Vertex d =  g.add_vertex('d');

  g.add_edge(a, b);
  g.add_edge(a, c);
  g.add_edge(c, d);
  
  // FIXME: Add some utilities to make working wtih these things a little
  // easier.
  
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
  assert(( pred[d] == b || pred[d] == c )); // Indeterminate
  

  return 0;
}
