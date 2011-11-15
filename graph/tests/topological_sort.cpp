// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/io.hpp>
#include <origin/graph/algorithm/topological_sort.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef directed_adjacency_list<int> Graph;
  typedef Graph::vertex Vertex;
  
  // This is the graph given on the Wikipedia page for topological sorting.
  Graph g = {
    {7, 11}, {7, 8},
    {5, 11},
    {3, 8}, {3, 10},
    {11, 2}, {11, 9}, {11, 10},
    {8, 9}
  };
  assert(( g.order() == 8 ));
  assert(( g.size() == 9 ));

  vector<Vertex> order(g.order());
  topological_sort(g, order.begin());
  reverse(order.begin(), order.end());
  for(auto v : order) {
    cout << g[v] << " ";
  }
  cout << "\n";
}
