// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/algorithm.hpp>

using namespace origin;


template <typename R>
  void print(const R& range)
  {
    for(auto x : range)
      std::cout << x << ' ';
    std::cout << '\n';
  }

int main()
{
  int a[] { 1, 2, 3, 4, 5 };
  int b[5];
  copy (begin(a), end(a), b);
  assert(lexicographical_equal(a, b));
}

/*

// Copy (property)
struct copy_check
{
  // FIXME: I have to capture by value because of a bug in the checking
  // framework.
  template <typename R, typename O>
    auto operator()(const R& range, O result) const
      -> Requires<Range_copy<R, O>(), bool>
    {
      if (size(range) > size(result))
        return true;

      copy(range, result);
      return equal(range, result);
    }
};



// Copy n (property)
struct copy_n_check
{
  template <typename R, typename O>
    auto operator()(const R& range, Difference_type<R> n, O result) const
      -> Requires<Range_copy<R, O>(), bool>
    {
      if (size(range) > size(result))
        return true;
      if (n > distance(range))
        return true;

      auto first = begin(range);
      auto last = next(first, n);
      o_copy_n(first, n, begin(result));
      return o_equal(first, last, begin(result));
    }
};


int main()
{
  assert_checker<> env;

  using V = vector<int>;
  using Small_dist = uniform_int_distribution<int>;
  using Less_dist = single_value_distribution<Less_than>;
  using Greater_dist = single_value_distribution<Greater_than>;

  auto range = checkable_var<V>(env);
  // auto value = checkable_var<int>(env);
  auto small = checkable_var(env, Small_dist {0, 5});

  quick_check(env, copy_check {}, range, range);
  quick_check(env, copy_n_check {}, range, small, range);
}

*/