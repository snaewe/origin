// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include <origin/container.hpp>

using namespace std;
using namespace origin;

int main()
{
  using Vec = vector<double>;
  static_assert(Range<Vec>(), "");
  static_assert(Container<Vec>(), "");
  static_assert(!Associative_container<Vec>(), "");
  
  using Arr = double[5];
  static_assert(Range<Arr>(), "");
  static_assert(Container<Vec>(), "");
  
  static_assert(!Range<int>(), "");
  
  
  cout << typestr<Container_reference<Vec>>() << "\n";
  cout << typestr<Container_reference<Vec const>>() << "\n";
}

