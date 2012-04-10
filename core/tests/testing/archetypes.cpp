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
  void test_default()
  {
    T x;
    (void) x;
    (void) T {};
  }

template <typename T>
  void test_movable()
  {
    T x(dummy_t {}, 5);
    T y = move(x);
    assert(y.value == 5);

    y.value = 10;
    x = move(y);
    assert(x.value == 10);

    // These should be errors.
    // T z;
    // T z = x;
    // x = y;
  }

template <typename T>
  void test_copyable()
  {
    test_movable<T>();

    T x(dummy_t {}, 5);
    T y = x;
    assert(x.value == y.value);

    y.value = 10;    
    x = y;
    assert(x.value == y.value);

    // This is still an error.
    // T w;
  }


template <typename T>
  void test_equality_comparable()
  {
    T x(dummy_t {}, 5);
    T y(dummy_t {}, 5);
    assert(x == y);
    assert(!(x != y));

    // assert(x < y);
  }

template <typename T, typename U>
  void test_equality_comparable(const U& value)
  {
    T x(dummy_t {}, value);
    assert(x == value);
    assert(value == x);
    assert(!(x != value));
    assert(!(value != x));
  }


template <typename T>
  void test_weakly_ordered()
  {
    T x(dummy_t {}, 5);
    T y(dummy_t {}, 6);
    assert(x < y);
    assert(y > x);
    assert(x <= y);
    assert(y >= x);
  }

template <typename T, typename U>
  void test_weakly_ordered(const U& value)
  {
    T x(dummy_t {}, value);
    assert(!(x < value));
    assert(!(value < x));
    assert(!(x > value));
    assert(!(value > x));
    assert(x <= value);
    assert(value <= x);
    assert(x >= value);
    assert(value >= x);
  }

template <typename T>
  void test_totally_ordered()
  {
    test_equality_comparable<T>();
    test_weakly_ordered<T>();
  }

template <typename T, typename U>
  void test_totally_ordered(const U& value)
  {
    test_equality_comparable<T>(value);
    test_weakly_ordered<T>(value);
  }


template <typename T>
  void test_regular()
  {
    test_default<T>();
    test_copyable<T>();
    test_equality_comparable<T>();
  }


int main()
{
  assert_checker<> env;

  {
    using T = archetype<int, movable_type>;
    test_movable<T>();
  }

  {
    using T = archetype<int, copyable_type>;
    test_copyable<T>();
  }

  {
    using T = 
      archetype< int, equality_comparable_type<>, equality_comparable_type<int>>;
    test_equality_comparable<T>();
    test_equality_comparable<T>(5);
  }

  {
    using T = archetype<int, weakly_ordered_type<>, weakly_ordered_type<int>>;
    test_weakly_ordered<T>();
    test_weakly_ordered<T>(5);
  }

  {
    using T = archetype<int, totally_ordered_type<>, totally_ordered_type<int>>;
    test_totally_ordered<T>();
    test_totally_ordered<T>(5);
  }

  {
    using T = archetype<int, regular_type>;
    test_regular<T>();
  }
}

