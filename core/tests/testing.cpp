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
#include <origin/tuple.hpp>
#include <origin/testing.hpp>

using namespace std;
using namespace origin;


template <typename R>
  void print(const R& range)
  {
    for (const auto& x : range)
      cout << x << ' ';
    cout << '\n';
  }

template <typename Env, typename Check, typename Eng, typename... Gens>
  void stuff(Env& env, Check check, Eng& eng, Gens&... gens)
  {
    cout << Randomized_property_check<Env, Check, Eng, Gens...>() << '\n';
  }


  
int main()
{
  // Initialize the random data source and burn a cycle. The first value
  // seems to always be the same if I don't.
  std::minstd_rand eng(time(0));
  eng();
  
  // Initialize the checking invironment.
  assert_checker env;

  auto gen_int = uniform_int_distribution<int>(0, 100);
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

  regular_function_semantics<eq> reg_eq;
  check(env, reg_eq, eng, gen_int, gen_int);


  {
    // Testing iterators is not fun. Testing ranges should be easy.
    using V = vector<int>;
    using I = V::iterator;
    vector<int> v(100);
    generate_random(v, eng, gen_int);
    random_iterator_generator<V> gen_iter(v);
    constant_value_generator<I> gen_end(v.end());


    test_pre_increment_result<vector<int>::iterator> t1;
    check(env, t1, eng, gen_iter, gen_end);
  }
}
