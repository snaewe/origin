// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>
#include <vector>

#include <origin/testing.hpp>

using namespace std;
using namespace origin;

// Test for overload resolution issues in the check and quickcheck
// operations.

// A checkable property over integer values.
struct prop
{
  bool operator()(int a, int b) const { return true; }
};


struct gen_prop
{
  template <typename T>
    auto operator()(T a, T b) const -> Requires<Integral<T>(), bool>
    { 
      return true; 
    }

  template <typename T, typename U>
    auto operator()(T a, T b, U&& c) const -> Requires<Integral<T>(), bool>
    { 
      return true; 
    }
};


int main()
{
  assert_checker<> env;

  auto num = checkable_var<int>(env);

  check(env, prop {}, num, num);
  quick_check(env, prop {}, num, num);

  check(env, gen_prop {}, num, num);
  quick_check(env, gen_prop {}, num, num);

  check(env, gen_prop {}, num, num, num);
  quick_check(env, gen_prop {}, num, num, num);
}

