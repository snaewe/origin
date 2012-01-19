// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/graph/vertex.hpp>
#include <origin/graph/edge.hpp>

using namespace std;
using namespace origin;

int main()
{
  using V = vertex_handle<std::size_t>;
  using E = edge_handle<std::size_t>;
  
  V v0;
  V v1{1};
  assert(v0 != v1);
  assert(v0 < v1);
  assert(!v0);
  assert(v1);
  
  E e0;
  E e1{1};
  assert(e0 != e1);
  assert(e0 < e1);
  assert(!e0);
  assert(e1);
}
