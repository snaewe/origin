// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>

#include <origin/concepts.hpp>
#include <origin/testing.hpp>
#include <origin/testing/prototypes.hpp>

using namespace std;
using namespace origin;

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      cout << x << ' ';
    cout << '\n';
  }

int main()
{
  assert_checker<> env;

  predicate_sequence_distribution<> dist;
  auto var = checkable_var(env, dist);

  for (auto i : range(0, 10))
    print(var());
}

