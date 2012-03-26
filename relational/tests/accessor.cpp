// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <string>
#include <vector>
#include <iostream>

#include <origin/relational.hpp>

using namespace std;
using namespace origin;


// Represents a person. There are some data fields.
struct person 
{
  string first;
  string last;
  int num;
  const string empty;

  const string& get_first() const { return first; }

  string& mutable_first() { return first; }
};

// Output_streamable.
// Print people by their first name and last name.
template <typename C, typename T>
  basic_ostream<C, T>& operator<<(basic_ostream<C, T>& os, const person& p)
  {
    return os << p.first << ' ' << p.last << '\n';
  }


int main()
{
  person p = {"Christian", "Ratti", 58};
  const person& cp = p;

  // Calling an memvar accessor with a non-const object returns a non-const
  // reference to the object.
  auto p_first = &person::first;
  Accessor_function<decltype(p_first)> a_first(p_first);
  static_assert(Same<decltype(a_first(p)), string&>(), "");
  static_assert(Same<decltype(a_first(cp)), const string&>(), "");

  // If the member variable is constant (or a const ref), then the resulting
  // referenc is always const-qualified.
  auto p_empty = &person::empty;
  Accessor_function<decltype(p_empty)> a_empty(p_empty);
  static_assert(Same<decltype(a_empty(p)), const string&>(), "");
  static_assert(Same<decltype(a_empty(cp)), const string&>(), "");


  // A const-memfun accessor can be called on a const or non-const object and
  // has the same result type as the wrapped member function.
  auto p_get_first = &person::get_first;
  Accessor_function<decltype(p_get_first)> a_get_first {p_get_first};
  static_assert(Same<decltype(a_get_first(p)), const string&>(), "");
  static_assert(Same<decltype(a_get_first(cp)), const string&>(), "");

  // A non-const memfun accessor cannot be called on a const-qualified object.
  // The result of the operation is the same as the wrapped member function.
  auto p_mutable_first = &person::mutable_first;
  Accessor_function<decltype(p_mutable_first)> a_mutable_first(p_mutable_first);
  static_assert(Same<decltype(a_mutable_first(p)), string&>(), "");
}

