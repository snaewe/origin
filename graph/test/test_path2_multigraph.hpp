// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TEST_PATH2_MULTIGRAPH_HPP
#define TEST_PATH2_MULTIGRAPH_HPP

using std::distance;
using std::end;
using std::next;

/**
 * Test the construction and destruction of a P2 (path-2) multigraph, a graph
 * with two vertices u and v, connected by the edges (u, v, 1) and (u, v, 2).
 *
 * @tparam Graph A Directed or Undirected, dynamic graph.
 */
template<typename Graph>
struct path2_multigraph
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::edge Edge;

  /** Test the common behavior of adding a single edge. */
  void setup()
  {
    u = g.add_vertex('a');
    v = g.add_vertex('b');
    assert(( g.order() == 2 ));
    assert(( g.size() == 0 ));

    // Add an edge and check invariants.
    e1 = g.add_edge(u, v, 1);
    e2 = g.add_edge(u, v, 2);
    assert(( e1 && e2 ));
    assert(( g.size() == 2 ));
    assert(( !g.empty() ));

    // Check the edge iterators.
    auto er = g.edges();
    auto ei = begin(er);
    assert(( distance(ei, end(er)) == 2 ));
    assert(( g[*ei++] == 1 ));
    assert(( g[*ei] == 2 ));
//     for(auto x : g.edges()) ;
  }

  /** Test common behavior of removing a single edge. */
  void teardown()
  {
    // FIXME: Implement this test.
  }

  /** Check in and out edges of the directed graph. */
  void check(origin::directed_graph_tag)
  {
    assert(( g.get_edge(u, v) == e1 || g.get_edge(u, v) == e2 ));

    assert(( g.out_degree(u) == 2 ));
    assert(( g.in_degree(u) == 0 ));
    assert(( g.out_degree(v) == 0 ));
    assert(( g.in_degree(v) == 2 ));

    auto uor = g.out_edges(u);
    auto uoi = begin(uor);
    auto uoe = end(uor);
    assert(( distance(uoi, uoe) == 2 ));
    assert(( g.source(*uoi) == u ));
    assert(( g.target(*uoi) == v ));
    assert(( g.source(*next(uoi)) == u ));
    assert(( g.target(*next(uoi)) == v ));

    auto uir = g.in_edges(u);
    assert(( distance(begin(uir), end(uir)) == 0 ));

    auto vir = g.in_edges(v);
    auto vii = begin(vir);
    auto vie = end(vir);
    assert(( distance(vii, vie) == 2 ));
    assert(( g.source(*vii) == u ));
    assert(( g.target(*vii) == v ));
    assert(( g.source(*next(vii)) == u ));
    assert(( g.target(*next(vii)) == v ));
  }

  /** Check incident edges of the undirected graph. */
  void check(origin::undirected_graph_tag)
  {
    // FIXME: Implement this test.
  }

  void test()
  {
    setup();
    check(typename Graph::graph_category{});
    teardown();
  }

  Graph g;
  Vertex u, v;
  Edge e1, e2;
};


#endif
