// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/utility/meta.hpp>
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

// NOTE: Can be T [cv].
template<typename T>
struct member_begin_result
{
  template<typename U>
  static auto check(U&& x) -> decltype(x.begin());

  static substitution_failure check(...);

  typedef decltype(check(declval<T>())) type;
};

template<typename T>
struct has_member_begin
  : is_different<typename member_begin_result<T>::type, substitution_failure>
{ };

template<typename T>
typename enable_if<
  has_member_begin<T>::value,
  typename member_begin_result<T>::type
>::type
foo(T& x)
{ return x.begin(); }

template<typename T>
typename enable_if<
  has_member_begin<T const>::value,
  typename member_begin_result<T const>::type
>::type
foo(T const& x)
{ return x.begin(); }

template<typename T, size_t N>
T* foo(T (&arr)[N])
{ return arr; }

int main()
{
  string const s;
  vector<int> v;
  int a[10];

  cout << typestr(foo(s)) << "\n";
  cout << typestr(foo(v)) << "\n";
  cout << typestr(foo(a)) << "\n";
//   foo(0);



  auto x = make_tuple(0, 1, 2, 'a', 3.14);
  assert(( tuple_invoke(func_call{}, x) == 5 ));

  tuple_invoke(void_call{}, x);
  assert(( void_test == 1 ));
}
