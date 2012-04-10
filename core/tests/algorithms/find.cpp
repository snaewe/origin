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
#include <origin/testing/prototypes.hpp>

using namespace std;
using namespace std::placeholders;
using namespace origin;



template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      cout << x << ' ';
    cout << '\n';
  }



// Find if (property)
struct find_if_check
{
  template <typename R, typename P>
    auto operator()(const R& range, P pred) const
      -> Requires<Range_query<R, P>(), bool>
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
    auto operator()(const R& range, P pred) const
      -> Requires<Range_query<R, P>(), bool>
    {
      return find_if_not(range, pred) == find_if(range, negation(pred));
    }
};



// Find next if (propertY)
struct find_next_if_check
{
  template <typename R, typename P>
    auto operator()(const R& range, P pred) const
      -> Requires<Range_query<R, P>(), bool>
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_next_if(first, last, pred);
      if (first != last)
        return i == find_if(next(first), last, pred);
      else
        return i == last;
    }
};



// Find nth if (property)
struct find_nth_if_check
{
  template <typename R, typename P>
    auto operator()(const R& range, Distance_type<R> n, P pred) const
      -> Requires<Range_query<R, P>(), bool>
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_nth_if(range, n, pred);
      if (i != last)
        return pred(*i) && count_if(first, i, pred) == (n - 1);
      else
        return true;
    }
};



// Find (property)
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
      return find(range, value, comp) == find_if(range, bind(comp, _1, value));
    }
};



// Find not equal (property)
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



// Find next (property)
struct find_next_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_next(first, last, value);
      if (first != last)
        return i == find(next(first), last, value);
      else
        return i == last;
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_next(first, last, value, comp);
      if (first != last)
        return i == find(next(first), last, value, comp);
      else
        return i == last;
    }
};



// Find nth (property)
struct find_nth_check
{
  template <typename R, typename T>
    auto operator()(const R& range, Distance_type<R> n, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_nth(range, n, value);
      if (i != last)
        return (*i == value) && (count(first, i, value) == (n - 1));
      else
        return true;
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, Distance_type<R> n, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      auto first = begin(range);
      auto last = end(range);
      auto i = find_nth(range, n, value, comp);
      if (i != last)
        return comp(*i, value) && (count(first, i, value, comp) == (n - 1));
      else
        return true;
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
    auto operator()(const R& range, C comp) const
      -> Requires<Range_relational_query<R, C>(), bool>
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
    using Small_dist = uniform_int_distribution<int>;
    using Bool_dist = single_value_distribution<P>;
    
    auto range = checkable_var<V>(env);
    auto small = checkable_var(env, Small_dist {0, 5});
    auto pred = checkable_var(env, Bool_dist {});

    quick_check(env, find_if_check {}, range, pred);
    quick_check(env, find_if_not_check {}, range, pred);
    quick_check(env, find_next_if_check {}, range, pred);
    quick_check(env, find_nth_if_check {}, range, small, pred);
  }


  // Test value sequences
  {
    using V = vector<int>;
    using Small_dist = uniform_int_distribution<int>;
    using Less_dist = single_value_distribution<Less_than>;
    using Greater_dist = single_value_distribution<Greater_than>;

    auto range = checkable_var<V>(env);
    auto value = checkable_var<int>(env);
    auto small = checkable_var(env, Small_dist {0, 5});
    auto less = checkable_var(env, Less_dist {});
    auto greater = checkable_var(env, Greater_dist {});

    quick_check(env, find_check {}, range, value);
    quick_check(env, find_check {}, range, value, less);
    quick_check(env, find_check {}, range, value, greater);
    
    quick_check(env, find_not_equal_check {}, range, value);
    quick_check(env, find_not_equal_check {}, range, value, less);
    quick_check(env, find_not_equal_check {}, range, value, greater);

    quick_check(env, find_next_check {}, range, value);
    quick_check(env, find_next_check {}, range, value, less);
    quick_check(env, find_next_check {}, range, value, greater);

    quick_check(env, find_nth_check {}, range, small, value);
    quick_check(env, find_nth_check {}, range, small, value, less);
    quick_check(env, find_nth_check {}, range, small, value, greater);
    
    quick_check(env, find_first_in_check {}, range, range);
    quick_check(env, find_first_in_check {}, range, range, less);
    quick_check(env, find_first_in_check {}, range, range, greater);
    
    quick_check(env, find_adjacent_check {}, range);
    quick_check(env, find_adjacent_check {}, range, less);
    quick_check(env, find_adjacent_check {}, range, greater);
    
    quick_check(env, find_not_adjacent_check {}, range);
    quick_check(env, find_not_adjacent_check {}, range, less);
    quick_check(env, find_not_adjacent_check {}, range, greater);
    
    quick_check(env, is_relation_preserving_check {}, range, less);
    quick_check(env, is_relation_preserving_check {}, range, greater);
  }

  {
    using Seq = predicate_sequence;

    // NOTE: These could be equivalently written using function objects.    
    auto test_find_if = [](const Seq& seq) -> bool
    {
      auto i = find_if(seq, To_bool {});
      if (i != end(seq))
        return i == seq.first_true();
      else
        return true;
    };

    auto test_find_if_not = [](const Seq& seq) -> bool
    {
      auto i = find_if_not(seq, To_bool {});
      if (i != end(seq))
        return i == seq.first_false();
      else
        return true;
    };

    auto test_find_next_if = [](const Seq& seq) -> bool
    {
      if (empty(seq))
        return true;
      
      auto p = seq.predicate();
      auto i = find_next_if(find_if(seq, p), end(seq), p);
      if (i != end(seq))
        return i == seq.nth_true(1);
      else
        return true;
    };

    auto test_find_nth_if = [](const Seq& seq, Size_type<Seq> n) -> bool
    {
      if (n >= seq.num_true())
        return true;
      else if (n == 0)
        return find_nth_if(seq, n, seq.predicate()) == end(seq);
      else
        return find_nth_if(seq, n, seq.predicate()) == seq.nth_true(n - 1);
    };


    using Small_dist = uniform_int_distribution<Size_type<Seq>>;
    Small_dist small_dist {0, 5};
    auto small = checkable_var(env, small_dist);

    using Seq_dist = predicate_sequence_distribution<>;
    Seq_dist seq_dist;
    auto seq = checkable_var(env, seq_dist);

    quick_check(env, test_find_if, seq);
    quick_check(env, test_find_if_not, seq);
    quick_check(env, test_find_next_if, seq);
    quick_check(env, test_find_nth_if, seq, small);
  }
}
