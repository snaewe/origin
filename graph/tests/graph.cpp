// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <list>

#include <origin/utility/typestr.hpp>

// FIXME: I shouldn't have to explictly include edge.hpp!
#include <origin/graph/edge.hpp>

#include <origin/graph/adjacency_list/directed.hpp>
#include <origin/graph/adjacency_list/undirected.hpp>

#include "test_copy.hpp"
#include "test_singleton_graph.hpp"
#include "test_path2_graph.hpp"
#include "test_path2_multigraph.hpp"

using namespace std;
using namespace origin;

/**
 * A simple harness for running a test model. Note that setup and teardown are
 * performed in the (default) constructors of the test model.
 */
template<typename Test>
void test()
{
  Test t;
  t.test();
};

template<typename Graph>
void test_graph()
{
  // Basic semantics
  test<copy_graph<Graph>>();

  // Exercise various topology tests.
  test<singleton_graph<Graph>>();
  test<path2_graph<Graph>>();
  test<path2_multigraph<Graph>>();

  /*

  // FIXME: Clean this up.
  { // Test the creation and removal of (self) loops
    Graph g;
    auto v = g.add_vertex();
    auto e = g.add_edge(v, v);
    assert(( g.order() == 1 ));
    assert(( g.size() == 1 ));
    assert(( g.out_degree(v) == 1 ));
    assert(( g.in_degree(v) == 1 ));
    g.remove_edge(e);
    assert(( g.empty() ));

    g.add_edge(v, v);
    g.remove_edges(v, v);
    assert(( g.empty() ));

    g.add_edge(v, v);
    g.remove_edges(v);
    assert(( g.empty() ));
  }


  { // Test the detaching of a vertex.
    // Generate an SN (Star N) with all edges leading out.
    Graph g;
    typename Graph::vertex_iterator V[] = {
      g.add_vertex('a'),
      g.add_vertex('b'),
      g.add_vertex('c'),
      g.add_vertex('d'),
      g.add_vertex('e')
    };
    size_t const NV = sizeof(V) / sizeof(typename Graph::vertex_iterator);
    typename Graph::edge_iterator E[] = {
      g.add_edge(V[0], V[1], 0),
      g.add_edge(V[0], V[2], 1),
      g.add_edge(V[0], V[3], 2),
      g.add_edge(V[0], V[4], 3)
    };
    size_t const NE = sizeof(E) / sizeof(typename Graph::edge_iterator);

    assert(( NE == NV - 1 ));
    assert(( g.order() == NV ));
    assert(( g.size() == NE ));

    // Check the degree of each vertex.
    assert(( g.out_degree(V[0]) == NV - 1 ));
    for(size_t i = 1; i < NV; ++i)
      assert(( g.in_degree(V[1]) == 1 ));

    // FIXME: Validate the source/taraget edge pairs.

    // Check the removal of edges from only the central vertex.
    g.remove_edges(V[0]);
    assert(g.empty());
    for(size_t i = 0; i < NV; ++i)
      assert(( g.degree(V[i]) == 0 ));
  }

  { // Test the removal of all edges from the graph: the "partial" clear.
    Graph g;
    typename Graph::vertex_iterator V[] = {
      g.add_vertex('a'),
      g.add_vertex('b'),
      g.add_vertex('c'),
      g.add_vertex('d'),
      g.add_vertex('e')
    };
    size_t const NV = sizeof(V) / sizeof(typename Graph::vertex_iterator);
    typename Graph::edge_iterator E[] = {
      g.add_edge(V[0], V[1], 0),
      g.add_edge(V[0], V[2], 1),
      g.add_edge(V[0], V[3], 2),
      g.add_edge(V[0], V[4], 3)
    };
    assert(( g.order() == 5 ));
    assert(( g.size() == 4 ));
    g.remove_edges();
    assert(( g.order() == 5 ));
    assert(( g.size() == 0 ));

    // Make sure we don't have any dangling references?
    for(size_t i = 0; i < NV; ++i)
      assert(( g.degree(V[i]) == 0 ));
  }


  { // Test iterators
    // Generate an SN (Star N) with all edges leading out.
    Graph g;
    typename Graph::vertex_iterator V[] = {
      g.add_vertex('a'),
      g.add_vertex('b'),
      g.add_vertex('c'),
      g.add_vertex('d'),
      g.add_vertex('e')
    };
    size_t const NV = sizeof(V) / sizeof(typename Graph::vertex_iterator);
    typename Graph::edge_iterator E[] = {
      g.add_edge(V[0], V[1], 0),
      g.add_edge(V[0], V[2], 1),
      g.add_edge(V[0], V[3], 2),
      g.add_edge(V[0], V[4], 3)
    };
    size_t const NE = sizeof(E) / sizeof(typename Graph::edge_iterator);

    assert(( distance(begin(g.vertices()), end(g.vertices())) == int(NV) ));
    assert(( distance(begin(g.edges()), end(g.edges())) == int(NE) ));
  }
  */
}

int main()
{
  test_graph<directed_adjacency_list<char, int>>();
  test_graph<undirected_adjacency_list<char, int>>();

  return 0;
}
