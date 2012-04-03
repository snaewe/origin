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

int main()
{
  assert_checker<> env;

  using V = vector<bool>;
  using P = as_bool<bool>;

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

