// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TEST_ADJACENCY_MATRIX_CREATE_GRAPH_HPP
#define TEST_ADJACENCY_MATRIX_CREATE_GRAPH_HPP

#include <origin/utility/typestr.hpp>
#include <origin/range/zip_range.hpp>

/**
 * Test the construction and copy semantics of graph types.
 *
 * Tests:
 *    boolean_adjacency_matrix(alloc = default)
 *    boolean_adjacency_matrix(n, m, alloc = default)
 *    equal(g) - operator ==
 *    defualt copy constructor
 *    null()
 *    empty()
 *    add_edge(v, u)
 *    order()
 *    size()
 *    source(e)
 *    target(e)
 *    g[e]
 *    g[v]
 *
 * @tparam Graph A Directed or Undirected Graph. The graph must be dynamic.
 */
template<typename Graph>
struct create_copy_graph_bool
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::edge Edge;
  typedef typename Graph::matrix_type Matrix;

  void test()
  {
    using namespace std;

    // Null graph construction.
    {
      Graph g;
      assert(( g.null() ));
      assert(( g.empty() ));
    }

    {
      Graph g(3);
      
      Vertex v[] = {
        Vertex(0), Vertex(1), Vertex(2)
      };

      g[v[0]] = 'a';
      g[v[1]] = 'b';
      g[v[2]] = 'c';

      Edge e[] = {
        g.add_edge(v[0], v[1]),
        g.add_edge(v[1], v[2]),
        g.add_edge(v[2], v[0])
      };

      // Perform the copy.
      Graph h = g;
      assert(( h == h ));

      // Basic assertions
      assert(( h.order() == g.order() ));
      assert(( h.size() == g.size() ));

      // Check that each vertex is a copy of the original.
      /*for(auto i : zip(g.vertices(), h.vertices())) {
        Vertex gv = get<0>(i);
        Vertex hv = get<1>(i);
        assert(( g[gv] == h[hv] ));
        assert(( g.degree(gv) == g.degree(hv) ));
      }*/

      // Check that each edge is a copy of the original.
      for(auto i : zip(g.edges(), h.edges())) {
        Edge ge = get<0>(i);
        Edge he = get<0>(i);
        assert(( g[ge] == h[he] ));
        assert(( g[g.source(ge)] == g[h.source(he)] ));
        assert(( g[g.target(ge)] == g[h.target(he)] ));
      }
    }
  }
};

template<typename Graph>
struct create_copy_graph_dist
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::edge Edge;
  typedef typename Graph::matrix_type Matrix;

  void test()
  {
    using namespace std;

    // Null graph construction.
    {
      Graph g;
      assert(( g.null() ));
      assert(( g.empty() ));
    }

    {
      Graph g(3);
      
      Vertex v[] = {
        Vertex(0), Vertex(1), Vertex(2)
      };

      g[v[0]] = 'a';
      g[v[1]] = 'b';
      g[v[2]] = 'c';

      Edge e[] = {
        g.add_edge(v[0], v[1], 1.0f),
        g.add_edge(v[1], v[2], 1.0f),
        g.add_edge(v[2], v[0], 1.0f)
      };

      // Perform the copy.
      Graph h = g;
      assert(( h == h ));

      // Basic assertions
      assert(( h.order() == g.order() ));
      assert(( h.size() == g.size() ));

      // Check that each vertex is a copy of the original.
      /*for(auto i : zip(g.vertices(), h.vertices())) {
        Vertex gv = get<0>(i);
        Vertex hv = get<1>(i);
        assert(( g[gv] == h[hv] ));
        assert(( g.degree(gv) == g.degree(hv) ));
      }*/

      // Check that each edge is a copy of the original.
      for(auto i : zip(g.edges(), h.edges())) {
        Edge ge = get<0>(i);
        Edge he = get<0>(i);
        assert(( g[ge] == h[he] ));
        assert(( g[g.source(ge)] == g[h.source(he)] ));
        assert(( g[g.target(ge)] == g[h.target(he)] ));
      }
    }
  }
};

#endif
