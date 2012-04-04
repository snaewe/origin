// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include "query.hpp"

using namespace std;
using namespace origin;
using namespace testing;


// FIXME: I don't care for this. Why isn't this just a function that has
// some fixed values that we test against?

// Check count_if against prototypical inputs: boolean sequences. This in
// turn checks against the derived specifiations of find_if_not.
struct count_if_check
{
  using V = vector<bool>;
  using P = to_bool_function;

  P pred;
  V v0;
  V v1;
  V v2;

  count_if_check() 
    : pred()
    , v0 {}     // Empty sequence
    , v1 {0}    // No such element
    , v2 {0, 1} // At least one element.
  { }

  // Check the default property
  template <typename Env>
    void operator()(Env& env) const
    {
      // An empty list has no matching elements.
      check(env, eq(), count_if(v0, pred), 0);

      // There are no matching elements.
      check(env, eq(), count_if(v1, pred), 0);

      // Returns the number of matching elements.
      check(env, eq(), count_if(v2, pred), 1);
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
  using P = to_bool_function;

  // Test find_if
  count_if_check count_if;
  count_if(env);

  // Test affiliated relationships.
  count_if_specs<V, P> specs;
  count_if(env, specs);

  // Test randomly for good measure.
  auto& eng = env.random_engine();
  auto pdist = single_value_distribution<P> {};
  auto pvar = make_random(eng, pdist);
  auto rvar = make_random<V>(eng);
  quick_check(env, specs, rvar, pvar, 1000);
}

