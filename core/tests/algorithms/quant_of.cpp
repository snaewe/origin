// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/testing.hpp>

#include "query.hpp"

using namespace std;
using namespace origin;
using namespace testing;

int main()
{
  assert_checker<> env;

  using V = vector<bool>;
  using P = to_bool_function;

  find_if_check find_if;
  count_if_check count_if;

  quant_of_specs<V, P> quant_of;
  find_if(env, quant_of);
  count_if(env, quant_of);
}
