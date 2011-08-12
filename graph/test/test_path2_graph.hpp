// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TEST_PATH2_GRAPH_HPP
#define TEST_PATH2_GRAPH_HPP

/**
 * Test the construction and destruction of a P2 (path-2) graph, a graph with
 * two vertices u and v, connected by a single edge (u, v).
 *
 * @tparam Graph A Directed or Undirected, dynamic graph.
 */
template<typename Graph>
struct path2_graph
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::edge Edge;
  
  // Add vertices to the graph if the graph is vertex-buildable.
  void add_vertices(origin::vertex_buildable_graph_tag)
  {
    u = g.add_vertex('a');
    v = g.add_vertex('b');
    assert(( g.order() == 2 ));
    assert(( g.size() == 0 ));
  }
  
  // Add vertices to the graph if it is vertex-initialized.
  void add_vertices(origin::vertex_initialized_graph_tag)
  {
    g = std::move(Graph(2));;
    auto vi = begin(g.vertices());
    
    // FIXME: Is there a more graceful way to do this? Assignment by iota?
    u = *vi++;
    v = *vi;
    g[u] = 'a';
    g[v] = 'b';
  }
  

  // Add an edge to the graph and evaluate its properties.
  void add_edge()
  {
    // Check the state of vertex/edge iterators.
    add_vertices(typename Graph::graph_category{});
    auto vr = g.vertices();
    auto vi = begin(vr);
    auto ve = end(vr);
    assert(( std::distance(vi, ve) == 2 ));
    assert(( g[*vi++] == 'a' ));
    assert(( g[*vi] == 'b' ));

    // Add an edge and check invariants.
    e = g.add_edge(u, v, 1);
    assert(( e ));
    assert(( g.size() == 1 ));
    assert(( !g.empty() ));

    // Check the edge iterators.
    auto er = g.edges();
    auto ei = begin(er);
    auto ee = end(er);
    assert(( std::distance(ei, ee) == 1 ));
    assert(( g[*ei] == 1 ));
//     for(auto x : g.edges()) ;
  }


  // Remove vertices and re-validate properties of the graph.
  void remove_edge(origin::vertex_dynamic_graph_tag)
  {
    g.remove_edge(e);
    assert(( g.empty() ));

    // FIXME: This should probably be partially specialized for directed
    // and undirected graphs.
    assert(( g.degree(u) == 0 ));
    assert(( g.degree(v) == 0 ));
  }

  // Do not remove vertices if the graph is not dynamic.
  template<typename Tag>
    void remove_edge(Tag)
    { }

  /** Check in and out edges of the directed graph. */
  void check(origin::directed_graph_tag)
  {
    assert(( g.source(e) == u ));
    assert(( g.target(e) == v ));

    assert(g.get_edge(u, v));
    assert(!g.get_edge(v, u));

    // FIXME: It's entirely possible that these operations will not be part
    // of the graph (see source and target).
    assert(g.out_degree(u) == 1);
    assert(g.in_degree(v) == 1);

    auto er = g.edges();

    // Check the out edges of u
    auto uor = g.out_edges(u);
    auto uoi = begin(uor);
    auto uoe = end(uor);
    assert(( g.source(*uoi) == u ));
    assert(( g.target(*uoi) == v ));
    assert(( distance(uoi, uoe) == 1 ));
    assert(( g[*uoi] == 1 ));
    assert(( *uoi == *begin(er) ));
    for(auto x : g.out_edges(u))
      assert(( x ));

    // Check the in edges of u
    auto uir = g.in_edges(u);
    assert(( std::distance(begin(uir), end(uir)) == 0 ));

    // Check the out edge iterators (for v).
    auto vor = g.out_edges(v);
    assert(( std::distance(begin(vor), end(vor)) == 0 ));

    // Check the in edge iterators (for v).
    auto vir = g.in_edges(v);
    auto vii = begin(vir);
    auto vie = end(vir);
    assert(( g.source(*vii) == u ));
    assert(( g.target(*vii) == v ));
    assert(( distance(vii, vie) == 1 ));
    assert(( g[*vii] == 1 ));
    assert(( *vii == *begin(er) ));
    for(auto x : g.in_edges(v))
      assert(( x ));

    // FIXME: These guys aren't comparable because they're slightly different
    // iterator types. It would be great if these were interoperable, but I'm
    // not sure that this would be a good thing.
    // begin(oer) == begin(er);
  }

  // Check the incident edges of undirected graphs.
  void check(origin::undirected_graph_tag)
  {
    assert(( g.get_edge(u, v) ));
    assert(( g.get_edge(v, u) ));

    auto er = g.edges();

    // Check the incident iterators (for u).
    auto uir = g.incident_edges(u);
    auto uii = begin(uir);
    auto uie = end(uir);
    assert(( distance(uii, uie) == 1 ));
    assert(( g.source(*uii) == u ));
    assert(( g.target(*uii) == v ));
    assert(( g[*uii] == 1 ));
    assert(( *uii == *begin(er) ));
//     for(auto x : g.incident_edges(u)) ;

    // Check the incident edges (for v).
    auto vir = g.incident_edges(u);
    auto vii = begin(vir);
    auto vie = end(vir);
    assert(( distance(vii, vie) == 1 ));
    assert(( g[*vii] == 1 ));
    assert(( *vii == *begin(er) ));
//     for(auto x : g.incident_edges(u)) ;
  }

  void test()
  {
    typename Graph::graph_category tag;
    add_edge(); // FIXME: Adapt for non-edge buildable graphs?
    check(tag);
    remove_edge(tag);
  }

  Graph g;
  Vertex u, v;
  Edge e;
};


#endif
