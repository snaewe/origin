// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>
#include <set>

#include <origin/algorithm.hpp>
#include <origin/container.hpp>
#include <origin/testing.hpp>

using namespace std;
using namespace origin;


// Count if is equivalent to a reduction.
template <typename R, typename P>
  struct count_if_spec
  {
    bool operator()(const R& range, P pred) const
    {
      // Returns n + 1 if pred(x) is true. Otherwise, n.
      auto f = [pred](const Value_type<R>& x, Distance_type<R> n)
        { 
          return pred(x) ? n + 1 : n; 
        };

      return count_if(range, pred) == reduce(range, 0, f);
    }
  };


template <typename R, typename P>
  struct count_if_not_spec
  {
    bool operator()(const R& range, P pred) const
    {
      return count_if_not(range, pred) == count_if(range, negation<P> {pred});
    }
  };



// Count if (specification)
template <typename R, typename P>
  struct count_if_specs
  {
    static_assert(Range_query<R, P>(), "");

    count_if_spec<R, P> count_if;
    count_if_not_spec<R, P> count_if_not;
    
    // Check with specific values.
    // Note that the same value will be used for each check.
    template <typename Env>
      void operator()(Env& env, const R& range, P pred) const
      {
        check(env, count_if, range, pred);
        check(env, count_if_not, range, pred);
      }

    // Check randomly.
    template <typename Env, typename Rgen, typename Pgen>
      auto operator()(Env& env, Rgen& range, Pgen& pred)
        -> Requires<Random_variable<Rgen>() && Random_variable<Pgen>()>
      {
        check(env, count_if, range, pred);
        check(env, count_if_not, range, pred);
      }
  };


int main()
{
  std::minstd_rand eng(time(0));
  assert_checker<std::minstd_rand> env{eng};

  using V = vector<int>;
  using P = non_zero_pred<int>;

  V v1 = {0, 0, 0, 0};
  V v2 = {0, 0, 0, 1};
  P pred;
  
  count_if_specs<V, P> spec;

  spec(env, v1, pred);
  spec(env, v2, pred);

  auto rvar = make_random<V>(eng);
  auto pvar = make_random(eng, single_value_distribution<P> {});
  quick_check(env, spec, rvar, pvar, 1000);

}

