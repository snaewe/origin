// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>

#include <origin/concepts.hpp>
#include <origin/range.hpp>
#include <origin/testing.hpp>

using namespace std;
using namespace origin;


template <typename Env, typename P, typename Eng, typename Gen>
  void stuff(Env& env, P pred, Eng&& eng, Gen&& gen)
  {
    cout << Randomized_specification_check<Env, P, Eng, Gen>() << '\n';
    cout << Randomized_property_check<Env, P, Eng, Gen>() << '\n';
  }

  
int main()
{
  // Initialize the random data source and burn a cycle. The first value
  // seems to always be the same if I don't.
  std::minstd_rand eng(time(0));
  eng();
  
  // Initialize the checking invironment.
  assert_checker env;

  auto gen_int = default_distribution<int>();
  auto gen_double = default_distribution<double>();

  // Properties under test
  reflexive_property<eq> refl;
  symmetric_property<eq> sym;
  transitive_property<eq> trans;
  
  check(env, refl, 0);
  check(env, sym, 1, 1);
  check(env, trans, 1, 1, 1);

  
  check(env, refl, eng, gen_int);
  check(env, sym, eng, gen_int, gen_int);
  check(env, trans, eng, gen_int, gen_int, gen_int);
  
  
  // Specifications under test
  equivalence_relation_spec<eq> equiv;
  check(env, equiv, eng, gen_int);
  
  equality_comparable_semantics<int> eq_int;
  totally_ordered_semantics<int> ord_int;
  check(env, eq_int,       eng, gen_int);
  check(env, ord_int,      eng, gen_int);

  equality_comparable_semantics<int, double> eq_int_double;
  totally_ordered_semantics<int, double> ord_int_double;
  check(env, eq_int_double, eng, gen_int, gen_double);
  check(env, ord_int_double, eng, gen_int, gen_double);
}
