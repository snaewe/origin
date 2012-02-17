// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/algorithm.hpp>
#include <origin/container/list.hpp>

using namespace std;
using namespace origin;

template<typename R>
  void print(const R& range)
  {
    for(const auto& x : range)
      cout << x << ' ';
    cout << '\n';
  }
  

int main()
{
  // Default initialization
  o_list<int> v1;
  assert(v1.empty());
  
  // Fill initialization
  o_list<int> v2(5);
  assert(v2.size() == 5);
  assert(all_equal(v2, int{}));
  
  // Range initializaiton
  int a[] = {1, 2, 3};
  o_list<int> v3(a);
  assert(equal(a, v3));
  
  o_list<int> v4(a, a + 3);
  assert(equal(a, v4));
}
