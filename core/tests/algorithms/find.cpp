// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/algorithm.hpp>
#include <origin/testing.hpp>

using namespace std;
using namespace std::placeholders;
using namespace origin;


// Find if results (property)
struct find_if_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_if(first, last, pred);
      if (i != last)
        return pred(*i) && find_if(first, i, pred) == i;
      else
        return true;
    }
};



// Find if not (property)
struct find_if_not_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return find_if_not(range, pred) == find_if(range, negation(pred));
    }
};



// Find (check)
struct find_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return find(range, value) == find_if(range, eq(value));
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      return find(range, value, comp) == find_if(range, bind(comp, value, _1));
    }
};



// Find not equal (check)
struct find_not_equal_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return find_not_equal(range, value) == find_if_not(range, eq(value));
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      return find_not_equal(range, value) == find_if_not(range, eq(value));
    }
};



// Find first in (property)
struct find_first_in_check
{
  template <typename R1, typename R2>
    auto operator()(const R1& range1, const R2& range2) const
      -> Requires<Range_comparison<R1, R2, Equal_to>(), bool>
    {
      auto first1 = begin(range1);
      auto last1 = end(range1);
      auto i = find_first_in(range1, range2);

      if (i != last1)
        return find_first_in(range(first1, i), range2) == i
            && some_equal(range2, *i);
      else
        return true;
    }

  template <typename R1, typename R2, typename C>
    auto operator()(const R1& range1, const R2& range2, C comp) const
      -> Requires<Range_comparison<R1, R2, C>(), bool>
    {
      auto first1 = begin(range1);
      auto last1 = end(range1);
      auto i = find_first_in(range1, range2, comp);

      if (i != last1)
        return find_first_in(range(first1, i), range2, comp) == i
            && some_equal(range2, *i, comp);
      else
        return true;
    }
};



// Find adjacent (property)
struct find_adjacent_check
{
  template <typename R>
    auto operator()(const R& range) const
      -> Requires<Range_relational_query<R, Equal_to>(), bool>
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_adjacent(range);

      if (i != last) {
        auto j = next(i);
        if (j != last)
          return find_adjacent(first, i) == i && (*i == *j);
        else
          return false;
      }
      return true;
    }

  template <typename R, typename C>
    auto operator()(const R& range, C comp) const
      -> Requires<Range_relational_query<R, Equal_to>(), bool>
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_adjacent(range, comp);

      if (i != last) {
        auto j = next(i);
        if (j != last)
          return find_adjacent(first, i, comp) == i && comp(*i, *j);
        else
          return false;
      }
      return true;
    }
};



// Find not adjacent (property)
struct find_not_adjacent_check
{
  template <typename R>
    auto operator()(const R& range) const
      -> Requires<Range_relational_query<R, Equal_to>(), bool>
    {
      return find_not_adjacent(range) 
          == find_adjacent(range, neq());
    }

  template <typename R, typename C>
    auto operator()(const R& range, C comp) const
      -> Requires<Range_relational_query<R, C>(), bool>
    {
      return find_not_adjacent(range, comp) 
          == find_adjacent(range, complement(comp));
    }
};



// Is relation preserving (property)
struct is_relation_preserving_check
{
  template <typename R, typename C>
    bool operator()(const R& range, C comp) const
    {
      return is_relation_preserving(range, comp) 
          == (find_not_adjacent(range, comp) == end(range));
    }
};



int main()
{
  assert_checker<> env;

  // Test predicate sequences
  {
    using V = vector<bool>;
    using P = to_bool_function;
    using Bool_dist = single_value_distribution<P>;
    
    auto range = checkable_var<V>(env);
    auto pred = checkable_var(env, Bool_dist {});

    quick_check(env, find_if_check {}, range, pred);
    quick_check(env, find_if_not_check {}, range, pred);
  }


  // Test value sequences
  {
    using V = vector<int>;
    using Less_dist = single_value_distribution<less_relation>;

    auto range = checkable_var<V>(env);
    auto value = checkable_var<int>(env);
    auto less = checkable_var(env, Less_dist {});

    quick_check(env, find_check {}, range, value);
    quick_check(env, find_check {}, range, value, less);
    quick_check(env, find_not_equal_check {}, range, value);
    quick_check(env, find_not_equal_check {}, range, value, less);
    quick_check(env, find_first_in_check {}, range, range);
    quick_check(env, find_first_in_check {}, range, range, less);
    quick_check(env, find_adjacent_check {}, range);
    quick_check(env, find_adjacent_check {}, range, less);
    quick_check(env, find_not_adjacent_check {}, range);
    quick_check(env, find_not_adjacent_check {}, range, less);
    quick_check(env, is_relation_preserving_check {}, range, less);
  }
}
