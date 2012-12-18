// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef GRAPH_TEST_TESTING_HPP
#define GRAPH_TEST_TESTING_HPP

#include <cassert>
#include <vector>

#include <origin/graph/graph.hpp>

template<typename G>
  void check_default_init()
  {
    G g;
    assert(g.empty());
    assert(g.null());
  }

template<typename G>
  void check_add_vertices()
  {
    using namespace std;
    using namespace origin;

    G g;
    for (int i = 0; i < 3; ++i)
      g.add_vertex('a' + i);
    assert(g.order() == 3);
    assert(g.empty());

    assert(g(Vertex<G>(0)) == 'a');
    assert(g(Vertex<G>(1)) == 'b');
    assert(g(Vertex<G>(2)) == 'c');
  }

template<typename G>
  void
  check_add_edges()
  {
    using namespace std;
    using namespace origin;

    G g;
    for (int i = 0; i < 3; ++i)
      g.add_vertex('a' + i);

    vector<Edge<G>> es;
    for (int i = 0; i < 3; ++i) {
      for (int j = i; j < 3; ++j) {
        Edge<G> e = g.add_edge(i, j, es.size());
        es.push_back(e);
      }
    }

    assert(g.size() == 6);
    assert(g(0, 0) == es[0]);
    assert(g(0, 1) == es[1]);
    assert(g(0, 2) == es[2]);
    assert(g(1, 1) == es[3]);
    assert(g(1, 2) == es[4]);
    assert(g(2, 2) == es[5]);

    // Check for symmetry in undirected graphs.
    if (Undirected_graph<G>()) {
      assert(g(1, 0) == es[1]);
      assert(g(2, 0) == es[2]);
      assert(g(2, 1) == es[4]);
    }
  }

#endif
