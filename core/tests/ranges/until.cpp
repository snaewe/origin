// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/range/until.hpp>

using namespace origin;

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      std::cout << x << ' ';
    std::cout << '\n';
  }

/*
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
*/


int main()
{
  vector<int> v {1, 2, 3, 4, 5};

  int x = 0;
  print(until(v, [&](int) { return x++ == 3; }));

  x = 0;
  print(until(v, [&](int) { return x++ == 5; }));

  x = 0;
  print(until(v, [&](int) { return x++ == 10; }));
}
