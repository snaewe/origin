// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_TEST_FLOYD_WARSHALL_DETAIL
#define ORIGIN_GRAPH_TEST_FLOYD_WARSHALL_DETAIL

#include <iostream>

namespace origin {

  // Distance matrix type. A total cheat, but it gets the job done.
  template<typename T, typename V, unsigned int N>
  struct dist_mtx
  {
    typedef T value_type;

    T& operator()(V u, V v)
    { return data[u.value * N + v.value]; }

    T const& operator()(V u, V v) const
    { return data[u.value * N + v.value]; }

    T& operator()(unsigned int u, unsigned int v)
    { return data[u * N + v]; }

    T const& operator()(unsigned int u, unsigned int v) const
    { return data[u * N + v]; }

    T data[N*N];
  };

  // Generates a wheel graph
  template<typename Graph, typename Label>
  void generate_wheel_graph(Graph& g, Label l = Label()) {
    auto vi = g.vertices().begin();
    auto vb = g.vertices().begin();
    for(auto i = 0u; i < g.order() - 1; ++i) {
      g.add_edge(vi, ++vi, l(i));
    }
    g.add_edge(vi, vb, l(g.size() - 1));
  }

} //namespace origin

#endif //ORIGIN_GRAPH_TEST_FLOYD_WARSHALL_DETAIL
