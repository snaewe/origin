// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/generator.hpp>
#include <origin/graph/io.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef directed_adjacency_list<char, int> Graph;

  Graph g = iota_path_graph<Graph>({'a', 'b', 'c', 'd', 'e'}, 0);
  assert(( g.order() == 5 ));
  assert(( g.size() == 4 ));
  write_edge_list(cout, g);
}
