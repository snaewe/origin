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

template <typename T>
    using Fwd = Forwarded<T>;

template <typename Env, typename Check, typename... Vars>
  void stuff(Env& env, Check check, Vars&&... vars)
  {
    cout << Property_check<Check, Fwd<Vars>...>() << '\n';
    cout << Randomized_property_check<Env, Check, Fwd<Vars>...>() << '\n';
    cout << Randomized_specification_check<Env, Check, Fwd<Vars>...>() << '\n';
    cout << Specification<Check, Env, Fwd<Vars>...>() << '\n';
  }


  
int main()
{
  // Initialize the random data source and burn a cycle. The first value
  // seems to always be the same if I don't.
  std::minstd_rand eng(time(0));
  eng();
  
  // Initialize the checking invironment.
  assert_checker env;

  auto geni = make_random<int>(eng);
  auto gend = make_random<double>(eng);
  auto genstr = make_random<string>(eng);

 // Properties under test
  reflexive_property<eq> refl;
  symmetric_property<eq> sym;
  transitive_property<eq> trans;
  
  check(env, refl, 0);
  check(env, sym, 1, 1);
  check(env, trans, 1, 1, 1);

  check(env, refl, geni);
  check(env, sym, geni, geni);
  check(env, trans, geni, geni, geni);

  // Specifications under test
  equivalence_relation_spec<eq> equiv;
  check(env, equiv, geni);

  equality_comparable_semantics<int> eq_int;
  totally_ordered_semantics<int> ord_int;
  check(env, eq_int, geni);
  check(env, ord_int, geni);

  equality_comparable_semantics<int, double> eq_int_double;
  totally_ordered_semantics<int, double> ord_int_double;
  check(env, eq_int_double, geni, gend);
  check(env, ord_int_double, geni, gend);

  regular_function_semantics<eq> reg_eq;
  check(env, reg_eq, geni, geni);

  {
    // Testing iterators is not fun. Testing ranges should be easy.
    using V = vector<int>;
    using I = V::iterator;
    vector<int> v(100);
    generate(v, geni);
    auto geniter = make_random(eng, random_iterator_distribution<V>(v));
    auto genend = make_random(eng, single_value_distribution<I>(v.end()));

    test_pre_increment_result<vector<int>::iterator> t1;
    check(env, t1, geniter, genend);
  }
}
