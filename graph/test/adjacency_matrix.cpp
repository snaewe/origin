// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.


#include <iostream>

#include <origin/graph/adjacency_matrix.hpp>
#include <origin/dynarray/square_dynarray.hpp>
#include <cassert>

//#include "test_adjacency_matrix/test_create.hpp"

using namespace std;
using namespace origin;

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
  boolean_adjacency_matrix<char> g(3, square_dynarray<bool>(3));
  {
    auto i = begin(g.vertices());
    g[i] = 'a';
    g[++i] = 'b';
    g[++i] = 'c';
  }
  for(auto i : g.vertices())
    cout << g[i];
  cout << '\n';

  boolean_adjacency_matrix<char> h(3, square_dynarray<bool>(3));
  cout << h.size() << '\n';

  return 0;
}

