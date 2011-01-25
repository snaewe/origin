// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <origin/tuple.hpp>

// FIXME: This could probably be improved quite a bit.

using namespace std;
using namespace origin;

// A function being called
template<typename... Args>
int func(Args&&... args)
{ return sizeof...(Args); }

// The function doing the calling.
struct func_call
{
  typedef int result_type;
  template<typename... Args>
  int operator()(Args&&... args) const
  { return func(forward<Args>(args)...); }
};

// Test void functions.
int void_test = 0;

template<typename... Args>
void void_func(Args&&... args)
{ void_test = 1; }

struct void_call
{
  typedef void result_type;
  template<typename... Args>
  void operator()(Args&&... args) const
  { return void_func(forward<Args>(args)...); }
};


int main()
{
  auto x = make_tuple(0, 1, 2, 'a', 3.14);
  assert(( tuple_invoke(func_call{}, x) == 5 ));

  tuple_invoke(void_call{}, x);
  assert(( void_test == 1 ));
}
