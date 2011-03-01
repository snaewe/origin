// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/algorithm/breadth_first_search.hpp>

using namespace std;
using namespace origin;

template<typename Graph>
void test() 
{
  typedef breadth_first_search_visitor Visitor;
  
  Graph g;
  auto u = g.add_vertex();
  auto v = g.add_vertex();
  g.add_edge(u, v);

  Visitor vis;
  breadth_first_search_from(g, u, vis);  
}

int main()
{
  test<directed_adjacency_list<>>();
  test<undirected_adjacency_list<>>();
  
  
  {
    typedef directed_adjacency_list<color_t> Graph;
    typedef breadth_first_search_visitor Visitor;

    Graph g;
    auto u = g.add_vertex();
    auto v = g.add_vertex();
    g.add_edge(u, v);

    Visitor vis;
    auto color = [&g](Graph::vertex v) -> color_t& { return g[v]; };
    breadth_first_search_from(g, v, vis, color);
  }


  {
    typedef directed_adjacency_list<color_t> Graph;
    typedef breadth_first_search_visitor Visitor;

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
    unordered_map<Graph::const_vertex, color_t> colors;
    auto color = [&colors](Graph::const_vertex v) -> color_t& { return colors[v]; };
    breadth_first_search_from(cg, v, vis, color);
  }
}
