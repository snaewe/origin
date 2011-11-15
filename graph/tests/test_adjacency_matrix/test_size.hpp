// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TEST_ADJACENCY_MATRIX_SIZE_GRAPH_HPP
#define TEST_ADJACENCY_MATRIX_SIZE_GRAPH_HPP

#include <origin/utility/typestr.hpp>
#include <origin/range/zip_range.hpp>

/**
 * Test the size and capacity behavior of adjacency matrix.
 *
 * Tests:
 *    boolean_adjacency_matrix(alloc = default)
 *    boolean_adjacency_matrix(n, m, alloc = default)
 *    operator =
 *    null()
 *    empty()
 *    order()
 *    size()
 *    max_order()
 *    max_size()
 *    add_edge(v, u)
 *    remove_edge(v, u)
 *
 * @tparam Graph An adjacency matrix
 */
template<typename Graph>
struct graph_sizes_bool
{
  typedef typename Graph::vertex Vertex;
  //typedef typename Graph::edge Edge;
  typedef typename Graph::matrix_type Matrix;

  void test()
  {
    using namespace std;

    // Null graph sizes.
    {
      Graph g;
      assert(( g.null() ));
      assert(( g.empty() ));
      assert(( g.size() == 0u ));
      assert(( g.order() == 0u ));
    }

    // Test the first 5 positive integers.
    for(auto i = 1u; i < 6u; ++i) {
      Graph g(i);
      assert(( !g.null() ));
      assert(( g.empty() ));
      assert(( g.order() == i ));
      for(auto j = 0u; j < i; ++j) {
        g.add_edge(Vertex(j), Vertex(j));
        assert(( g.size() == j + 1 ));
      }
      Graph h = g;
      for(auto j = 0u; j < i; ++j) {
        h.remove_edges(Vertex(j), Vertex(j));
        assert(( h.size() == i - j - 1 ));
      }
      assert(( h.empty() ));
      assert(( !g.empty() ));
    }
  }
};

template<typename Graph>
struct graph_sizes_dist
{
  typedef typename Graph::vertex Vertex;
  //typedef typename Graph::edge Edge;
  typedef typename Graph::matrix_type Matrix;

  void test()
  {
    using namespace std;

    // Null graph sizes.
    {
      Graph g;
      assert(( g.null() ));
      assert(( g.empty() ));
      assert(( g.size() == 0u ));
      assert(( g.order() == 0u ));
    }

    // Test the first 5 positive integers.
    for(auto i = 1u; i < 6u; ++i) {
      Graph g(i);
      assert(( !g.null() ));
      assert(( g.empty() ));
      assert(( g.order() == i ));
      for(auto j = 0u; j < i; ++j) {
        g.add_edge(Vertex(j), Vertex(j), 1.0f);
        assert(( g.size() == j + 1 ));
      }
      Graph h = g;
      Graph k = g;
      for(auto j = 0u; j < i; ++j) {
        h.remove_edges(Vertex(j), Vertex(j));
        assert(( h.size() == i - j - 1 ));
      }
      k.remove_edges();
      assert(( h.empty() ));
      assert(( k.empty() ));
      assert(( !g.empty() ));
    }
  }
};



#endif
