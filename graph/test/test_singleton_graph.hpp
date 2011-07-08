// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TEST_SINGLETON_GRAPH_HPP
#define TEST_SINGLETON_GRAPH_HPP

/**
 * Test the construction and destruction of the singleton graph. a graph with
 * exactly 1 vertex and no edges.
 *
 * @tparam Graph A Directed or Undirected Graph. The graph must be dynamic.
 */
template<typename Graph>
struct singleton_graph
{
  void test()
  {
    Graph g;
    assert(( g.null() ));
    assert(( g.empty() ));

    // Check the addition of a single vertex.
    auto v = g.add_vertex('a');
    assert(( v ));
    assert(( !g.null() ));
    assert(( g.order() == 1 ));

    // Validate iterator state.
    auto vr = g.vertices();
    assert(( std::distance(std::begin(vr), std::end(vr)) == 1 ));
    assert(( g[*begin(vr)] == 'a' ));
    for(auto x : g.vertices()) ;

    g.remove_vertex(v);
    assert(( g.order() == 0 ));
    assert(( g.null() ));
  }
};

#endif
