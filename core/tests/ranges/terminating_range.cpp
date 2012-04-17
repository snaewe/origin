// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/functional.hpp>
#include <origin/range/terminating_range.hpp>
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

// A range terminated by a predicate is equal to the same range constructed by
// querying for the first element satisfying that predicate.
struct terminate_if
{
  template <typename R, typename P>
    auto operator()(const R& range, P pred) const
      -> Requires<Range_query<R, P>(), bool>
    {
      auto r1 = terminated(range, pred);
      auto r2 = origin::range(begin(range), find_if(range, pred));
      return equal(r1, r2);
    }
};

// A range terminated by a value is equal to the same range constructed by
// searching for that value.
struct terminate_equal
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T>(), bool>
    {
      auto r1 = terminated(range, value);
      auto r2 = origin::range(begin(range), find(range, value));
      return equal(r1, r2);
    }
};



int main()
{
  assert_checker<> env;

  auto val = checkable_var<int>(env);
  auto vec = checkable_var<vector<int>>(env);
  auto pred = make_constant_function(is_zero_pred<int> {});

  check(env, terminate_if {}, vec, pred);
  check(env, terminate_equal {}, vec, val);
}
