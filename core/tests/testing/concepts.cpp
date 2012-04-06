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

using namespace std;
using namespace origin;


bool f(int, char) { return true; }

  
int main()
{
  // Initialize the checking invironment and some random variables.
  assert_checker<> env;
  
  // Some properties under test
  reflexive_property ref;
  symmetric_property sym;
  transitive_property trans;
  

  // Property checking with specific arguments.
  check(env, ref, eq(), 0);
  check(env, sym, eq(), 1, 1);
  check(env, trans, eq(), 1, 1, 1);

  // Setup for random variables, etc.
  using Equal_dist = single_value_distribution<Equal_to>;
  auto equal = checkable_var(env, Equal_dist {});
  auto num = checkable_var<int>(env);
  auto num2 = checkable_var<long>(env);

  // Property testing with quantified (random) arguments.
  check(env, ref, equal, num);
  check(env, sym, equal, num, num);
  check(env, trans, equal, num, num, num);

  // Quick check those relations.
  quick_check(env, ref, equal, num);
  quick_check(env, sym, equal, num, num);
  quick_check(env, trans, equal, num, num, num);


  // Check relational specifications.
  check_equivalence_relation<int>(env, eq());
  check_strict_partial_order<int>(env, lt());
  check_strict_weak_order<int>(env, lt());
  check_strict_total_order<int>(env, lt());


  // Concept checking
  check(env, equality_comparable_spec<int> {}, num);
  check(env, equality_comparable_spec<int, long> {}, num, num2);

  check(env, weakly_ordered_spec<int> {}, num);
  check(env, weakly_ordered_spec<int, long> {}, num, num2);

  check(env, totally_ordered_spec<int> {}, num);
  check(env, totally_ordered_spec<int, long> {}, num, num2);

  // Quick check the same concepts.
  quick_check(env, equality_comparable_spec<int> {}, num);
  quick_check(env, equality_comparable_spec<int, long> {}, num, num2);

  quick_check(env, weakly_ordered_spec<int> {}, num);
  quick_check(env, weakly_ordered_spec<int, long> {}, num, num2);

  quick_check(env, totally_ordered_spec<int> {}, num);
  quick_check(env, totally_ordered_spec<int, long> {}, num, num2);

  // Check functions
  check_equality_comparable<int>(env);
  check_equality_comparable<int, long>(env);
  check_weakly_ordered<int>(env);
  check_weakly_ordered<int, long>(env);
  check_totally_ordered<int>(env);
  check_totally_ordered<int, long>(env);


  // Semantics of regular types.
  check_copyable<int>(env);
  check_regular<int>(env);


  // NOTE: f is declared above.
  // TODO: Test regularity with lambda expressions.
  // check_regular_function<equal_relation(int, int)>(env, eq());
  // check_regular_function<decltype(f)>(env, f);
  // check_predicate<equal_relation(int, int)>(env, eq());
  // check_predicate<equal_relation(int, int)>(env, eq());
  // check_relation<equal_relation(int, int)>(env, eq());
}

