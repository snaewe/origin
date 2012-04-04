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


// FIXME: I don't care for this.

// Find if (check)
// Check find_if against prototypical inputs: boolean sequences.
struct find_if_check
{
  using V = vector<bool>;
  using P = to_bool_function;

  P pred;
  V v0;
  V v1;
  V v2;

  find_if_check() 
    : pred()
    , v0 {}     // Empty sequence
    , v1 {0}    // No such element
    , v2 {1, 1} // Returns the first such element
  { }

  // Check the default property
  template <typename Env>
    void operator()(Env& env) const
    {
      // An empty sequence has no such element.
      check(env, eq(), find_if(v0, pred), end(v0));

      // No such element exists in the sequence.
      check(env, eq(), find_if(v1, pred), end(v1));

      // Returns the first such element.
      check(env, eq(), find_if(v2, pred), begin(v2));
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
  find_if_check find_if;
  find_if(env);

  // Test affiliated relationships.
  find_if_specs<V, P> specs;
  find_if(env, specs);

  // 
  quick_check(env, specs, 1000);
}


