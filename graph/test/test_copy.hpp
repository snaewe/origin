// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TEST_COPY_GRAPH_HPP
#define TEST_COPY_GRAPH_HPP

// FIXME: Generalize the testing of copy semantics. It's fairly axiomatic, so
// it should not be too hard. Note that testing copy semantics actually
// implies that the equality operator is trusted.

#include <origin/utility/typestr.hpp>
#include <origin/range/zip_range.hpp>

/**
 * Test the copy semantics of graph types.
 *
 * @tparam Graph A Directed or Undirected Graph. The graph must be dynamic.
 */
template<typename Graph>
struct copy_graph
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::edge Edge;
  void test()
  {
    using namespace std;

    Graph g;
    Vertex v[] = {
      g.add_vertex('a'), g.add_vertex('b'), g.add_vertex('c')
    };
    Edge e[] = {
      g.add_edge(v[0], v[1], 1),
      g.add_edge(v[1], v[2], 2),
      g.add_edge(v[2], v[0], 3)
    };

    // Perform the copy.
    Graph h = g;
    assert(( h == h ));

    // Basic assertions
    assert(( h.order() == g.order() ));
    assert(( h.size() == g.size() ));

    // Check that each vertex is a copy of the original.
    for(auto i : zip(g.vertices(), h.vertices())) {
      Vertex gv = get<0>(i);
      Vertex hv = get<1>(i);
      assert(( g[gv] == h[hv] ));
      assert(( g.degree(gv) == g.degree(hv) ));
    }

    // Check that each edge is a copy of the original.
    for(auto i : zip(g.edges(), h.edges())) {
      Edge ge = get<0>(i);
      Edge he = get<0>(i);
      assert(( g[ge] == h[he] ));
      assert(( g[g.source(ge)] == g[h.source(he)] ));
      assert(( g[g.target(ge)] == g[h.target(he)] ));
    }
  }
};

#endif
