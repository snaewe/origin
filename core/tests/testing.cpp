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


struct print_tuple
{
  template <typename T, typename... Args>
    void operator()(const T& x, Args&&... args) const
    {
      cout << x << ", ";
      operator()(args...);
    }

  template <typename T>
    void operator()(const T& x) const { cout << x; }

  void operator()() const { }
};

template <typename... Args>
  void print(const tuple<Args...>& args)
  {
    cout << '(';
    tuple_invoke(print_tuple{}, args);
    cout << ")\n";
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

bool f(int, char) { return true; }

  
int main()
{
  // Initialize the random data source and burn a cycle. The first value
  // seems to always be the same if I don't.
  std::minstd_rand eng(time(0));
  eng();
  
  // Initialize the checking invironment and some random variables.
  assert_checker<std::minstd_rand> env{eng};
  auto geni = checkable_var<int>(env);


  // Properties under test
  reflexive_property<eq> refl;
  symmetric_property<eq> sym;
  transitive_property<eq> trans;
  

  // Property checking with specific arguments.
  check(env, refl, 0);
  check(env, sym, 1, 1);
  check(env, trans, 1, 1, 1);

  // Property testing with quantified (random) arguments.
  check(env, refl, geni);
  check(env, sym, geni, geni);
  check(env, trans, geni, geni, geni);



  // Check relational specifications.
  check_equivalence_relation<int>(env, eq{});
  check_strict_partial_order<int>(env, lt{});
  check_strict_weak_order<int>(env, lt{});
  check_strict_total_order<int>(env, lt{});

  // Check concepts.
  check_equality_comparable<int>(env);
  check_equality_comparable<int, char>(env);
  check_weakly_ordered<int>(env);
  check_weakly_ordered<int, char>(env);
  check_totally_ordered<int>(env);
  check_totally_ordered<int, char>(env);

  check_copyable<int>(env);
  check_regular<int>(env);

  check_regular_function<eq(int, int)>(env, eq{});
  check_regular_function<decltype(f)>(env, f);
  check_predicate<eq(int, int)>(env, eq{});
  check_predicate<eq(int, int)>(env, eq{});
  check_relation<eq(int, int)>(env, eq{});
}

