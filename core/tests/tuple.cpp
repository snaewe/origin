// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/functional.hpp>
#include <origin/tuple.hpp>

using namespace std;
using namespace origin;

int f1(int a, char b) { return 3; }
void f2(double a, int& b) { b = 5; }

int main()
{
  // Check return values
  assert(( tuple_invoke(f1, make_tuple(1, 'a')) == 3 ));
  
  // Check void returns and modifying args.
  int x = 0;
  auto t = make_tuple(0.0, ref(x));
  tuple_invoke(f2, t);
  assert(( x == 5 ));
  
  
  // This is a system configuration test that determines if std::tuple will 
  // compress empty classes at any position in their structure. Good 
  // implementations will.
  static_assert(sizeof(tuple<int, int, equal_to<int>>) == sizeof(tuple<int, int>), "");
  static_assert(sizeof(tuple<int, equal_to<int>, int>) == sizeof(tuple<int, int>), "");
  static_assert(sizeof(tuple<equal_to<int>, int, int>) == sizeof(tuple<int, int>), "");
}
