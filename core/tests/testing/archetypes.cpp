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


template <typename I>
  void test_input_iterator()
  {
    int n = 0;
    I i {dummy_t {}, &n};
    
    // Reading
    assert(*i == 0);
    
    // Pre-increment
    assert((++i).value == &n + 1);

    // Can't write to input iterators!
    // *i = 5;
  }

template <typename I>
  void test_forward_iterator()
  {
    test_input_iterator<I>();
    
    int n = 0;
    I i {dummy_t {}, &n};

    // Post-increment
    assert((i++).value == &n);
    assert(i.value == &n + 1);

    // Can't decrement
    // --i;
  }

template <typename I>
  void test_bidirectional_iterator()
  {
    test_forward_iterator<I>();

    int n = 0;
    I i {dummy_t {}, &n + 1};

    // Pre-decrement
    assert((--i).value == &n);
    
    // Post-decrement
    i.value = &n + 1;
    assert((i--).value == &n + 1);
    assert(i.value == &n);
  }

template <typename I>
  void test_random_access_iterator()
  {
    test_bidirectional_iterator<I>();

    int n[] {0, 1};

    I i {dummy_t {}, &n[0]};
    I j {dummy_t {}, &n[2]};
    
    assert((i + 1).value == &n[1]);
    assert((1 + i).value == &n[1]);
    assert((i += 1).value == &n[1]);

    assert((i - 1).value ==&n[0]);
    assert((i -= 1).value == &n[0]);

    assert(j - i == 2);
  }

int main()
{
  // Movable
  {
    using T = Movable_archetype<int>;
    test_movable<T>();
  }

  // Copyable
  {
    using T = Copyable_archetype<int>;
    test_copyable<T>();
  }

  // Equality comparable
  {
    using T = Equality_comparable_archetype<int>;
    test_equality_comparable<T>();
    
    using U = Equality_comparable_archetype<int, int>;
    test_equality_comparable<U>(5);

    using V = Equality_comparable_archetype<int, long>;
    test_equality_comparable<V>(5l);
  }

  // Weakly ordered
  {
    using T = Weakly_ordered_archetype<int>;
    test_weakly_ordered<T>();

    using U = Weakly_ordered_archetype<int, int>;
    test_weakly_ordered<U>(5);

    using V = Weakly_ordered_archetype<int, long>;
    test_weakly_ordered<V>(5l);
  }

  // Totally ordered
  {
    using T = Totally_ordered_archetype<int>;
    test_totally_ordered<T>();

    using U = Totally_ordered_archetype<int, int>;
    test_totally_ordered<U>(5);

    using V = Totally_ordered_archetype<int, long>;
    test_totally_ordered<V>(5l);
  }

  // Regular
  {
    using T = Regular_archetype<int>;
    test_regular<T>();
  }

  // Function
  {
    using F = Function_archetype<Equal_to, int, int>;
    F f {dummy_t {}, eq()};
    assert(f(0, 0));
    assert(!f(0, 1));
  }

  // Predicate
  {
    using P = Predicate_archetype<To_bool, int>;
    P p {dummy_t {}, To_bool {}};
    assert(p(1));
    assert(!p(0));
  }

  // Relation
  {
    using R1 = Relation_archetype<Equal_to, int>;
    R1 r1 {dummy_t {}, eq()};
    assert(r1(1, 1));
    assert(!r1(1, 0));


    using R2 = Relation_archetype<Equal_to, int, long>;
    R2 r2 {dummy_t {}, eq()};
    assert(r1(1, 1l));
    assert(r1(1l, 1));
    assert(!r1(0l, 1));
    assert(!r1(1, 0l));
  }

  // FIXME: Operation archetypes

  // Input iterator
  {
    using I = Input_iterator_archetype<int*>;
    test_input_iterator<I>();
  }

  // Output iterator
  {
    int n = 0;

    using I = Output_iterator_archetype<int*, int>;
    I i {dummy_t {}, &n};
    *i = 3;
    assert(*i.value == 3);
    assert((++i).value == &n + 1);

    // Look! No reads
    // const int& x = *i;
  }

  // Forward iterator
  {
    using I = Forward_iterator_archetype<int*>;
    test_forward_iterator<I>();
  }

  // Bidirectional iterator
  {
    using I = Bidirectional_iterator_archetype<int*>;
    test_bidirectional_iterator<I>();
  }

  // Random access iterator
  {
    using I = Random_access_iterator_archetype<int*>;
    test_random_access_iterator<I>();
  }
}

