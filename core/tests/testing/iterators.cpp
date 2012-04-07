// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>
#include <vector>

#include <origin/testing.hpp>

using namespace std;
using namespace origin;

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      cout << x << ' ';
    cout << '\n';
  }

int main()
{
  assert_checker<> env;

  using V = vector<int>;

  check_input_range<V>(env);
  check_output_range<V, int>(env);
  check_forward_range<V>(env);
  check_bidirectional_range<V>(env);
  check_random_access_range<V>(env);

  // Create a vector of random values
  V v(50);
  uniform_int_distribution<int> dist {0, 50};
  auto num = checkable_var(env, dist);
  generate(v, num);

  // Create a variable for making random ranges of that vector.
  random_range_distribution<V> rdist {v};
  auto range = checkable_var(env, rdist);
  using R = decltype(range());

  // Quick check some properties.
  quick_check(env, input_range_spec<R> {}, range);
  quick_check(env, output_range_spec<R, int> {}, range, num);
  quick_check(env, forward_range_spec<R> {}, range);
  quick_check(env, bidirectional_range_spec<R> {}, range);
  quick_check(env, random_access_range_spec<R> {}, range);
}

