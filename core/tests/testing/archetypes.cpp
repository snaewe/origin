// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>
#include <vector>

#include <origin/concepts.hpp>
#include <origin/testing.hpp>

using namespace std;
using namespace origin;


template <typename T>
  struct value_traits {
  };


int main()
{
  // Initialize the random data source and burn a cycle. The first value
  // seems to always be the same if I don't.
  assert_checker<> env;

  using reg_bool = regular_type<bool>;
  check_regular<reg_bool>(env);


  vector<reg_bool> v = { true, false, true };
  auto first = as_input_iterator(v.begin());
  auto last = as_input_iterator(v.end());

  assert(find(first, last, true) == first);
}

