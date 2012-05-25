// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/graph/adjacency_vector/undirected.hpp>

using namespace origin;
using namespace std;

using G = undirected_adjacency_vector;
using V = G::vertex;

template <typename G>
void print_vertices(G const& g)
{
  cout << "Vertices:";
  for (auto v : g.vertices())
    cout << ' ' << v.value();
  cout << "\n";
}

template <typename G>
void print_edges(G const& g)
{
  cout << "Edges:";
  for (auto e : g.edges())
    cout << " (" << e.source.value() << ',' << e.target.value() << ')';
  cout << "\n";
}

template <typename G, typename Vertex>
void print_incident_edges(G const& g, Vertex v)
{
  cout << "Incident to " << v.value() << ":";
  for (auto e : g.incident_edges(v))
    cout << " (" << e.source.value() << ',' << e.target.value() << ')';
  cout << "\n";
}

template <typename G>
void print_graph(G const& g)
{
  print_vertices(g);
  print_edges(g);
  for (auto v : g.vertices())
    print_incident_edges(g,v);
}

int main ()
{
  G g(3);

  auto e = g.add_edge(V(1), V(0));

  assert(( g.size() == 1 ));
  assert(( g.order() == 3 ));

  g.remove_edge(e);

  assert(( g.size() == 0 ));

  g.add_edge(V(1), V(0));

  unsigned int i = 0;
  for (auto v : g.vertices())
    ++i;

  assert(( g.order() == i ));

  unsigned int j = 0;
  for (auto e_ : g.edges())
    ++j;

  assert(( g.size() == j ));

  i = 0;
  j = 0;
  for (auto k : g.incident_edges(V(1)))
    ++j;
  for (auto k : g.incident_edges(V(0)))
    ++i;
  
  assert(( i == j ));

  print_graph(g);

  return 0;
}