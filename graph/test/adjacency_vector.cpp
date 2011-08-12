// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/graph/adjacency_vector.hpp>

#include "test_copy.hpp"
#include "test_singleton_graph.hpp"
#include "test_path2_graph.hpp"
// #include "test_path2_multigraph.hpp"

using namespace std;
using namespace origin;

// FIXME: This is copied out of graph.cpp. It should be moved into a more
// general framework.
template<typename Test>
  void test()
  {
    Test t;
    t.test();
  };

int main()
{
  typedef directed_adjacency_vector<char, int> Graph;

  // Basic semantics
//   test<copy_graph<Graph>>();

  // Exercise various topology tests.
  test<singleton_graph<Graph>>();
  test<path2_graph<Graph>>();
//   test<path2_multigraph<Graph>>();
  
}
