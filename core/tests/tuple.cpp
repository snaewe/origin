// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/tuple.hpp>

// FIXME: This could probably be improved quite a bit.

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
}
