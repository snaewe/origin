// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/generator.hpp>

using namespace std;
using namespace origin;

template<typename G>
  void test_null_graph()
  {
    G g = make_null_graph<G>();
    assert(( null(g) ));
    assert(( empty(g) ));
  }

template<typename G>
  void test_trivial_graph()
  {
    G g = make_trivial_graph<G>('a');
    assert(( order(g) == 1 ));
    assert(( size(g) == 0 ));
    assert(( g[*begin(vertices(g))] == 'a' ));
  }


// Test for directed graphs
template<typename G>
  void test_cycle1_graph(G const& g, std::true_type)
  {
    auto v = *begin(vertices(g));
    assert(( out_degree(g, v) == 1 ));
    assert(( in_degree(g, v) == 1 ));
    assert(( degree(g, v) == 2 ));
  }

// Test for undirected graphs
template<typename G>
  void test_cycle1_graph(G const& g, std::false_type)
  {
    auto v = *begin(vertices(g));
    assert(( degree(g, v) == 2 ));
  }

template<typename G>
  void test_cycle1_graph()
  {
    G g = iota_cycle_graph<G>(1, 0);
    assert(( order(g) == 1 )) ;
    assert(( size(g) == 1 ));

    // Make sure the size agrees with the distance.
    assert(( distance(begin(vertices(g)), end(vertices(g))) == 1 ));
    assert(( distance(begin(edges(g)), end(edges(g))) == 1 ));
    
    // Test directed/undirected aspects
    test_cycle1_graph(g, is_directed_graph<G>{});
  }


// Test for directed graphs
template<typename G>
  void test_path2_graph(G const& g, std::true_type)
  {
  }

// Test for undirected graphs
template<typename G>
  void test_path2_graph(G const& g, std::false_type)
  {
  }

template<typename G>
  void test_path2_graph()
  {
    G g = iota_path_graph<G>({'a', 'b'}, 0);
    assert(( order(g) == 2 )) ;
    assert(( size(g) == 1 ));
    test_path2_graph(g, is_directed_graph<G>{});
  }

template<typename G>
  void test()
  {
    test_null_graph<G>();
    test_trivial_graph<G>();
    test_cycle1_graph<G>();
    test_path2_graph<G>();
  }
  
int main()
{
//   typedef undirected_adjacency_list<char, int> Graph;
//   test<Graph>();

typedef directed_adjacency_list<char, int> Digraph;
  test<Digraph>();
}
