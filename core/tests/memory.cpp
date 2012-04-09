// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/memory.hpp>
#include <origin/iterator.hpp>
#include <origin/iterator/counter.hpp>
#include <origin/range.hpp>

using namespace std;
using namespace origin;

struct foo 
{
  foo& operator++() { return *this; }
};

struct faux
{
  // Value type is deduced from int*.
  int *begin() const { return nullptr; }
  int *end() const { return nullptr; }
};

struct bar
{
  int size() const { return 0; }
};

// using origin::deduce_size_type;

int main()
{
  using A = allocator<int>;
  static_assert(Same<Size_type<A>, std::size_t>(), "");
  static_assert(Same<Distance_type<A>, std::ptrdiff_t>(), "");


  // TODO: Push these tests into the concepts directory. They're definitely
  // not allocator tests. Same with the classes above.
  using V = vector<int>;
  using I = V::iterator;

  using C = counter<int>;
  using R = bounded_range<I>;

  static_assert(Same<Size_type<A>, std::size_t>(), "");
  static_assert(Same<Size_type<R>, Make_unsigned<Distance_type<R>>>(), "");
  static_assert(Same<Size_type<bar>, int>(), "");

  static_assert(Same<Distance_type<int>, int>(), "");
  static_assert(Same<Distance_type<unsigned>, unsigned>(), "");
  static_assert(Same<Distance_type<float>, float>(), "");
  static_assert(Same<Distance_type<foo>, std::ptrdiff_t>(), "");

  static_assert(Same<Value_type<int*>, int>(), "");
  static_assert(Same<Value_type<const int*>, int>(), "");
  static_assert(Same<Value_type<V>, int>(), "");
  static_assert(Same<Value_type<I>, int>(), "");
  static_assert(Same<Value_type<C>, int>(), "");
  static_assert(Same<Value_type<R>, int>(), "");
  static_assert(Same<Value_type<faux>, int>(), "");

  static_assert(Subst_failed<Value_type<foo>>(), "");
}
