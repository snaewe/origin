// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.



#include <cassert>
#include <iostream>

#include <origin/graph/algorithm/spanning_tree/prim.hpp>
#include <origin/graph/adjacency_vector/undirected.hpp>



using namespace::std;
using namespace::origin;

template <typename G>
void print_vertices(G const& g)
{
  cout << "Vertices:";
  for (auto v : g.vertices())
    cout << ' ' << (*v).value();
  cout << "\n";
}

template <typename G>
void print_edges(G const& g)
{
  cout << "Edges:";
  for (auto e : g.edges())
    cout << " (" << (*e).source.value() << ',' << (*e).target.value() << ')';
  cout << "\n";
}

template <typename G, typename Vertex>
void print_incident_edges(G const& g, Vertex v)
{
  cout << "Incident to " << v.value() << ":";
  for (auto e : g.incident_edges(v))
    cout << " (" << (*e).source.value() << ',' << (*e).target.value() << ')';
  cout << "\n";
}

template <typename G>
void print_graph(G const& g)
{
  print_vertices(g);
  print_edges(g);
  for (auto v : g.vertices())
    print_incident_edges(g,*v);
}

int main()
{
  using G = undirected_adjacency_vector;
  using V = G::vertex;
  using E = G::edge;

  // Make graph
  G g(9);
  E e01 = g.add_edge(V(0),V(1));
  E e07 = g.add_edge(V(0),V(7));
  E e17 = g.add_edge(V(1),V(7));
  E e12 = g.add_edge(V(1),V(2));
  E e78 = g.add_edge(V(7),V(8));
  E e82 = g.add_edge(V(8),V(2));
  E e86 = g.add_edge(V(8),V(6));
  E e76 = g.add_edge(V(7),V(6));
  E e23 = g.add_edge(V(2),V(3));
  E e25 = g.add_edge(V(2),V(5));
  E e65 = g.add_edge(V(6),V(5));
  E e35 = g.add_edge(V(3),V(5));
  E e34 = g.add_edge(V(3),V(4));
  E e45 = g.add_edge(V(4),V(5));

  // Make edge weight
  auto edge_weight = make_edge_labeling(g, 99999);
  edge_weight(e01) = 4;
  edge_weight(e07) = 8;
  edge_weight(e17) = 11;
  edge_weight(e12) = 8;
  edge_weight(e78) = 7;
  edge_weight(e82) = 2;
  edge_weight(e86) = 6;
  edge_weight(e76) = 1;
  edge_weight(e23) = 7;
  edge_weight(e25) = 4;
  edge_weight(e65) = 2;
  edge_weight(e35) = 14;
  edge_weight(e34) = 9;
  edge_weight(e45) = 10;

  vertex_labeling<V> pred = prim(g, V(0), edge_weight);

  print_graph(g);

  for (auto v : g.vertices()) {
    cout << (*v).value() << " - " << pred(*v).value() << '\n';
  }

  return 0;
}