// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>

#include <origin/algorithm.hpp>
#include <origin/testing.hpp>

#include "query.hpp"

using namespace std;
using namespace origin;
using namespace testing;

int main()
{
  assert_checker<> env;

  using V = vector<int>;

  count_specs<V> specs;
  quick_check(env, specs);
}
