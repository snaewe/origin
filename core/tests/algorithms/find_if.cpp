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



// Find if not is equivalent to find_if when the predicat is negated.
template <typename R, typename P>
  struct find_if_not_equiv
  {
    bool operator()(const R& range, P pred) const
    {
      return find_if_not(range, pred) == find_if(range, negation<P> {pred});
    }
  };



// Find if (specification)
template <typename R, typename P>
  struct find_if_specs
  {
    static_assert(Range_query<R, P>(), "");

    find_if_not_equiv<R, P> find_if_not;
    
    // Check with specific values.
    template <typename Env>
      void operator()(Env& env, const R& range, P pred) const
      {
        check(env, find_if_not, range, pred);
      }

    // Check randomly.
    template <typename Env, typename Rgen, typename Pgen>
      auto operator()(Env& env, Rgen& range, Pgen& pred)
        -> Requires<Random_variable<Rgen>() && Random_variable<Pgen>()>
      {
        operator()(env, range(), pred());
      }
  };


// Check find_if against prototypical inputs: boolean sequences. This in
// turn checks against the derived specifiations of find_if_not.
struct find_if_check
{
  using V = vector<bool>;
  using P = as_bool<bool>;

  P pred;
  V v0;
  V v1;
  V v2;

  find_if_check() 
    : pred()
    , v0 {}         // Empty sequence
    , v1 {0, 0, 0}  // No such element
    , v2 {1, 0, 1}  // Returns the first such element
  { }

  // Check the default property
  template <typename Env>
    void operator()(Env& env) const
    {
      // An empty sequence has no such element.
      check(env, eq {}, find_if(v0, pred), end(v0));

      // No such element exists in the sequence.
      check(env, eq {}, find_if(v1, pred), end(v1));

      // Returns the fuirst such element.
      check(env, eq {}, find_if(v2, pred), begin(v2));
    }

  // Test the given specification using these inputs.
  template <typename Env, typename Spec>
    void operator()(Env& env, const Spec& spec) const
    {
      spec(env, v0, pred);
      spec(env, v1, pred);
      spec(env, v2, pred);
    }
};


int main()
{
  assert_checker<> env;

  using V = vector<bool>;
  using P = as_bool<bool>;

  // Test find_if
  find_if_check find_if;
  find_if(env);

  // Test affiliated relationships.
  find_if_specs<V, P> specs;
  find_if(env, specs);

  // Test randomly for good measure.
  auto& eng = env.random_engine();
  auto pdist = single_value_distribution<P> {};
  auto pvar = make_random(eng, pdist);
  auto rvar = make_random<V>(eng);
  quick_check(env, specs, rvar, pvar, 1000);
}


