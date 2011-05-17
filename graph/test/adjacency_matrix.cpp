// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.


#include <iostream>

//#include <origin/graph/adjacency_matrix.hpp>

//#include "test_adjacency_matrix/test_create.hpp"

using namespace std;
//using namespace origin;

/**
 * A simple harness for running a test model. Note that setup and teardown are
 * performed in the (default) constructors of the test model.
 */
/*template<typename Test>
void test()
{
  Test t;
  t.test();
};

void test_graph()
{
  test<copy_graph<directed_adjacency_matrix<char, int>>>();
}

typedef origin::directed_adjacency_matrix<int, float> g_type;
const g_type::size_type g_size = 5;

g_type r_copy(g_type g) { return g; }
*/
int main()
{
  /*test_graph();

  g_type null_g;

  assert(null_g.null());
  assert(null_g.empty());
  assert(null_g.order() == 0u);
  assert(null_g.size() == 0u);

  g_type g(g_size);

  assert(!g.null());
  assert(g.empty());
  assert(g.order() == g_size);
  assert(g.size() == 0u);

  // Testing: Add edges
  g.add_edge(g_type::vertex(1), g_type::vertex(2));
  assert(g.size() == 1);

  // Testing additional constructors.

  // Testing ranges.

  {
    bool found_edge = false;
    auto edges = g.edges();
    for(; edges.begin() != edges.end(); ++edges.begin())
    {
      found_edge = true;
      break;
    }
    assert(found_edge);
  }*/

  return 0;
}

