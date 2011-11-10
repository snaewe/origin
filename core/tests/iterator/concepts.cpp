// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <list>
#include <forward_list>
#include <iostream>

#include <origin/iterator.hpp>

using namespace std;
using namespace origin;

int main()
{
  static_assert(Readable<int*>(), "");
  static_assert(Move_writable<int*, int>(), "");
  
  static_assert(!Readable<int>(), "");
  
  using Sll = forward_list<int>;
  static_assert(Input_iterator<Sll::iterator>(), "");
  static_assert(Forward_iterator<Sll::iterator>(), "");
  
  using Dll = list<int>;
  static_assert(Input_iterator<Dll::iterator>(), "");
  static_assert(Forward_iterator<Dll::iterator>(), "");
  static_assert(Bidirectional_iterator<Dll::iterator>(), "");
  
  using Vec = vector<int>;
  static_assert(Input_iterator<Vec::iterator>(), "");
  static_assert(Forward_iterator<Vec::iterator>(), "");
  static_assert(Bidirectional_iterator<Vec::iterator>(), "");
  static_assert(Random_access_iterator<Vec::iterator>(), "");
}
