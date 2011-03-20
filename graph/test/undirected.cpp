// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/graph/adjacency_list/undirected.hpp>
#include <origin/graph/adjacency_vector/undirected.hpp>

using namespace std;
using namespace origin;

template<typename G> void test_p2(G&);
template<typename G> void test_const_p2(G const&);

// Check the properties of an undirected P2 graph.
template<typename Graph>
void test_p2(Graph& g)
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::edge Edge;

  assert(( g.order() == 2 ));
  assert(( g.size() == 1 ));
  
  auto i = begin(g.vertices());
  Vertex u = *i++, v = *i++;
  
  Edge e1 = g.get_edge(u, v);
  Edge e2 = g.get_edge(v, u);
  
  assert(( e1 ));
  assert(( e2 ));
  assert(( e1 == e2 ));
  
  // Make sure that the const instantiation works also.
  test_const_p2(g);
}

// Check the properties of a const undirected P2 graph.
template<typename Graph>
void test_const_p2(Graph const& g)
{
  typedef typename Graph::const_vertex Vertex;
  typedef typename Graph::const_edge Edge;

  assert(( g.order() == 2 ));
  assert(( g.size() == 1 ));
  
  auto i = begin(g.vertices());
  Vertex u = *i++, v = *i++;
  
  Edge e1 = g.get_edge(u, v);
  Edge e2 = g.get_edge(v, u);
  
  assert(( e1 ));
  assert(( e2 ));
  assert(( e1 == e2 ));
}


// Create an undirected P2 graph.
template<typename Graph>
void make_p2(Graph& g)
{
  auto u = g.add_vertex(0);
  auto v = g.add_vertex(1);
  assert(( g[u] == 0 ));
  assert(( g[v] == 1 ));
  
  auto e = g.add_edge(u, v, 0);
  assert(( g[e] == 0 ));
  assert(( g.source(e) == u ));
  assert(( g.target(e) == v ));
}

int main()
{
  {
    undirected_adjacency_list<int, int> g;
    make_p2(g);
    test_p2(g);
  }
  
  {
    undirected_adjacency_vector<int, int> g;
    make_p2(g);
    test_p2(g);
  }
  
}
