// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <vector>

#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/algorithm/search/depth_first.hpp>
#include <origin/graph/io/edge_list.hpp>

using namespace std;
using namespace origin;

typedef directed_adjacency_list<int, int> Graph;

// FIXME: This is a nice start, but I have a feeling that it's not terribly
// general. I wonder how we might apply this to other graphs. Note that there
// is more than one DFS tree in this graph also. If you change the order in
// which the vertices are visited, you get different results.

enum edge_kind {
  unknown,
  tree,
  back,
  nontree
};

struct edge_visitor : dfs_visitor
{
  edge_visitor(vector<edge_kind>& e)
    : edges(e)
  { }

  void tree_edge(Graph& g, Graph::edge e)
  { edges[g[e]] = tree; }

  void back_edge(Graph& g, Graph::edge e)
  { edges[g[e]] = back; }

  void nontree_edge(Graph& g, Graph::edge e)
  { edges[g[e]] = nontree; }

  vector<edge_kind>& edges;
};

int main()
{
  Graph g;

  // Create the graph on the Wikipedia page labeled with the tree, back,
  // forward and cross edges.
  // FIXME: Use make graph to create these graphs. That will allow testing
  // with adjacency matrices too.

  // Create 8 vertices.
  Graph::vertex v[8];
  for(int i = 1; i <= 8; ++i) {
    v[i - 1] = g.add_vertex(i);
  }

  // Add the 10 edges
  pair<int, int> e[] = {
    {1, 2},   // 0 tree
    {1, 5},   // 1 tree
    {1, 8},   // 2 nontree (fwd)
    {2, 3},   // 3 tree
    {3, 4},   // 4 tree
    {4, 1},   // 5 back
    {5, 6},   // 6 tree
    {6, 3},   // 7 nontree (cross)
    {6, 7},   // 8 tree
    {6, 8}    // 9 tree
  };
  for(int i = 0; i < 10; ++i) {
    g.add_edge(v[e[i].first - 1], v[e[i].second - 1], i);
  }

  vector<edge_kind> edges(10, unknown);
  vector<edge_kind> result = {
    tree, tree, nontree, tree, tree, back, tree, nontree, tree, tree
  };

  edge_visitor vis(edges);
  depth_first_traverse(g, vis);
  assert(( edges == result ));

}
