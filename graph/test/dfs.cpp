// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/algorithm/search/depth_first.hpp>

using namespace std;
using namespace origin;

// Check instantiations of the algorithm.
template<typename Graph>
void test()
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::const_vertex Const_Vertex;
  typedef typename Graph::edge Edge;
  typedef typename Graph::const_edge Const_Edge;
  typedef dfs_visitor Visitor;

  Graph g;
  auto u = g.add_vertex();
  auto v = g.add_vertex();
  g.add_edge(u, v);

  // Get some const objects for const instantiations.
  Graph const& cg = g;
  Const_Vertex cu = u;

  Visitor vis;

  // Build a custom color map for various instantiations.
  // I think that keys in these maps HAVE to be const in order to be generic.
  // Note that the underlying map will actually make the key const, but that
  // constness has to preserved in the label argument. Otherwise, we can end
  // up with situations where a visitor over a const graph tries to call into
  // a non-const label. That is a compiler error.
  unordered_map<Const_Vertex, basic_color_t> cm;
  auto color = [&](Const_Vertex v) -> basic_color_t& { return cm[v]; };

  // Check search instantiations
  depth_first_search(g, u, vis);
  depth_first_search(g, u, vis, color);
  depth_first_search(cg, cu, vis);
  depth_first_search(cg, cu, vis, color);

  // Check traversal instantiations
  depth_first_traverse(g, vis);
  depth_first_traverse(cg, vis);
  depth_first_traverse(g, vis, color);
  depth_first_traverse(cg, vis, color);

  // Check dfs range implementations.
//   for(auto x : dfs(g, u)) ;
//   for(auto x : dfs(g, u, color));
//   for(auto x : dfs(cg, cu)) ;
//   for(auto x : dfs(cg, cu, color));

//   for(auto x : dfs(g));
//   for(auto x : dfs(cg));
//   for(auto x : dfs(g, color));
//   for(auto x : dfs(cg, color));
}

int main()
{
  test<directed_adjacency_list<>>();
  test<undirected_adjacency_list<>>();
}
