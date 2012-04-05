// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <vector>

#include <origin/algorithm.hpp>
#include <origin/testing.hpp>

using namespace std;
using namespace origin;



// Mismatch (property)
//
// NOTE: We have to enforce the preconditions of the algorithm here, because
// they cannot be effectively enforced by the algorithm.
struct mismatch_check
{
  template <typename R1, typename R2>
    auto operator()(const R1& range1, const R2& range2) const
      -> Requires<Range_comparison<R1, R2, Equal_to>(), bool>
    {
      if (size(range2) < size(range1))
        return true;

      auto first1 = begin(range1);
      auto last1 = end(range1);
      auto p = mismatch(range1, range2);
      if (p.first != last1)
        return (*p.first != *p.second)
            && (mismatch(first1, p.first, begin(range2)) == p);
      else
        return true;
    }

  // NOTE: Mismatch collides with the std namespace.
  template <typename R1, typename R2, typename C>
    auto operator()(const R1& range1, const R2& range2, C comp) const
      -> Requires<Range_comparison<R1, R2, C>(), bool>
    {
      if (size(range2) < size(range1))
        return true;

      auto first1 = begin(range1);
      auto last1 = end(range1);
      auto p = origin::mismatch(range1, range2, comp);
      if (p.first != last1)
        return (*p.first != *p.second)
            && (mismatch(first1, p.first, begin(range2), comp) == p);
      else
        return true;
    }
};



// Equal (property)
struct equal_check
{
  template <typename R1, typename R2>
    auto operator()(const R1& range1, const R2& range2) const
      -> Requires<Range_comparison<R1, R2, Equal_to>(), bool>
    {
      if (size(range2) < size(range1))
        return true;

      return equal(range1, range2) 
          == (mismatch(range1, range2).first == end(range1));
    }

  template <typename R1, typename R2, typename C>
    auto operator()(const R1& range1, const R2& range2, C comp) const
      -> Requires<Range_comparison<R1, R2, C>(), bool>
    {
      if (size(range2) < size(range1))
        return true;

      return origin::equal(range1, range2, comp) 
          == (origin::mismatch(range1, range2, comp).first == end(range1));
    }
};


int main()
{
  assert_checker<> env;

  using V = vector<int>;
  using Less_dist = single_value_distribution<Less_than>;

  auto range = checkable_var<V>(env);
  auto less = checkable_var(env, Less_dist {});

  quick_check(env, mismatch_check {}, range, range);
  quick_check(env, mismatch_check {}, range, range, less);

  quick_check(env, equal_check {}, range, range);
  quick_check(env, equal_check {}, range, range, less);
}
