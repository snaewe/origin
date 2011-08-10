// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TEST_SINGLETON_GRAPH_HPP
#define TEST_SINGLETON_GRAPH_HPP

#include <iterator>

/**
 * Test the construction and destruction of the singleton graph. a graph with
 * exactly 1 vertex and no edges.
 *
 * @tparam Graph A Dynamic Directed or Undirected Graph. The graph must be 
 * dynamic.
 */
template<typename Graph>
struct singleton_graph
{
  typedef typename origin::graph_traits<Graph>::vertex Vertex;
  
  // Test the initialization of buildable vertex graphs
  void initialize(Graph& g, std::true_type)
  {
    Graph h;
    assert(( h.null() ));
    assert(( h.empty() ));

    auto v = h.add_vertex('a');
    assert(( v ));
    g = std::move(h);
  }
  
  // Test the initialzation of vertex initialized graphs
  void initialize(Graph& g, std::false_type)
  {
    Graph h(1);
    h[*begin(g.vertices())] = 'a';  // Label this correctly.
    g = std::move(h);
  }
  
  void finalize(Graph& g, Vertex v, std::true_type)
  {
    g.remove_vertex(v);
    assert(( g.order() == 0 ));
    assert(( g.null() ));
  }
  
  void finalize(Graph& g, Vertex v, std::false_type)
  { }
  
  void test()
  {
    Graph g;
    initialize(g, typename origin::is_vertex_buildable_graph<Graph>::type{});

    // Assert basic properties of a singleton graph.
    assert(( !g.null() ));
    assert(( g.order() == 1 ));

    // Check the vertex range.
    // FIXME: This is technically only valid for vertex list graphs, but
    // since we don't have any data strutures that don't model that concept,
    // I'm not sure that it matters.
    auto vr = g.vertices();
    assert(( std::distance(begin(vr), end(vr)) == 1 ));
    for(auto x : g.vertices()) {
      assert(( x ));
    };
    
    // Make sure the vertex has the right data.
    Vertex v = *begin(vr);
    assert(( g[v] == 'a' ));

    finalize(g, v, typename origin::is_vertex_dynamic_graph<Graph>::type{});
  }
};

#endif
