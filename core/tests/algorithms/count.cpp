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



// Count if (property)
// The semantics of this algorithm are defined in terms of a reduction that
// increments the reduced value when a matching element is found.
struct count_if_check
{
  template <typename R, typename P>
    auto operator()(const R& range, P pred) const
      -> Requires<Range_query<R, P>(), bool>
    {
      auto f = [pred](const Value_type<R>& x, Distance_type<R> n)
      {
        return (pred(x)) ? n + 1 : n;
      };

      return count_if(range, pred) == reduce(range, 0, f);
    }
};



// Count if not (property)
struct count_if_not_check
{
  template <typename R, typename P>
    auto operator()(const R& range, P pred) const
      -> Requires<Range_query<R, P>(), bool>
    {
      return count_if_not(range, pred) == count_if(range, negation(pred));
    }
};



// Count (property)
struct count_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return count(range, value) == count_if(range, eq(value));
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      return count(range, value, comp) == count_if(range, bind(comp, _1, value));
    }
};



// Count not equal (property)
struct count_not_equal_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return count_not_equal(range, value) == count_if_not(range, eq(value));
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      return count_not_equal(range, value, comp)
          == count_if_not(range, bind(comp, _1, value));
    }
};



// Specification testing

using Seq = predicate_sequence;

struct test_count_if
{
  bool operator()(const Seq& seq) const
  {
    Size_type<Seq> n = count_if(seq.begin(), seq.end(), seq.predicate());
    return n == seq.num_true();
  }
};

struct test_count_if_not
{
  bool operator()(const Seq& seq) const
  {
    Size_type<Seq> n = count_if_not(seq.begin(), seq.end(), seq.predicate());
    return n == seq.num_false();
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

    quick_check(env, count_if_check {}, range, pred);
    quick_check(env, count_if_not_check {}, range, pred);
  }


  // Test value sequences
  {
    using V = vector<int>;
    using Less_dist = single_value_distribution<less_relation>;

    auto range = checkable_var<V>(env);
    auto value = checkable_var<int>(env);
    auto less = checkable_var(env, Less_dist {});

    quick_check(env, count_check {}, range, value);
    quick_check(env, count_check {}, range, value, less);
    quick_check(env, count_not_equal_check {}, range, value);
    quick_check(env, count_not_equal_check {}, range, value, less);
  }

  {
    using Seq_dist = predicate_sequence_distribution<>;
    Seq_dist seq_dist;
    auto seq = checkable_var(env, seq_dist);

    // using Value_dist = bernoulli_distribution;
    // Value_dist value_dist;
    // auto value = checkable_var(env, value_dist);

    quick_check(env, test_count_if {}, seq);
    quick_check(env, test_count_if_not {}, seq);
  }
}
