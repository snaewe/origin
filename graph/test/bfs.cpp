// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/algorithm/search/breadth_first.hpp>

using namespace std;
using namespace origin;

template<typename Graph>
void test() 
{
  typedef bfs_visitor Visitor;
  
  Graph g;
  auto u = g.add_vertex();
  auto v = g.add_vertex();
  g.add_edge(u, v);

  Visitor vis;
  breadth_first_search_from(g, u, vis);  
}

// FIXME: This has nothing to do with BFS. It's more of a conceptual assertion
// that -1 + 1 == 0, even for unsigned integers (i.e., -1 being the same
// as numeric_limits<Int>::max.
template<typename Int>
void test_int()
{
  Int x{-1};
  ++x;
  assert(( x == 0 ));
}

int main()
{
  test<directed_adjacency_list<>>();
  test<undirected_adjacency_list<>>();

  {
    typedef directed_adjacency_list<basic_color_t> Graph;
    typedef bfs_visitor Visitor;

    Graph g;
    auto u = g.add_vertex();
    auto v = g.add_vertex();
    g.add_edge(u, v);

    Visitor vis;
    auto color = [&g](Graph::vertex v) -> basic_color_t& { return g[v]; };
    breadth_first_search_from(g, v, vis, color);
  }


  {
    typedef directed_adjacency_list<basic_color_t> Graph;
    typedef bfs_visitor Visitor;

    Graph g;
    auto u = g.add_vertex();
    auto v = g.add_vertex();
    g.add_edge(u, v);

    // NOTE: If the graph binds to the const version of the algorithm, then
    // we probably have to be careful to use parameterize the map and label
    // over the const vertex type.
    // Basically, we're getting const-correctness through and through. That's
    // a good thing.
    Graph const& cg = g;
    Visitor vis;
    unordered_map<Graph::const_vertex, basic_color_t> colors;
    auto color = [&colors](Graph::const_vertex v) -> basic_color_t& { return colors[v]; };
    breadth_first_search_from(cg, v, vis, color);
  }
  
  {
    typedef undirected_adjacency_list<char> Graph;
    typedef bfs_visitor Visitor;

    Graph g;
    auto u = g.add_vertex('a');
    auto v = g.add_vertex('b');
    g.add_edge(u, v);
    
    for(auto x : bfs_from(g, v)) {
      cout << g[x] << "\n";
    }
    
    for(auto x : bfs(g)) {
      cout << g[x] << "\n";
    }
  }
  
  // FIXME: This is a concept check.  Move it somewhere more appropriate.
  {
    test_int<char>();
    test_int<signed char>();
    test_int<unsigned char>();
    test_int<short>();
    test_int<unsigned short>();
    test_int<int>();
    test_int<unsigned int>();
    test_int<long>();
    test_int<unsigned long>();
  }
}
