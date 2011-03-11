// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_CONCEPTS_HPP
#define ORIGIN_ITERATOR_CONCEPTS_HPP

#include <iterator>

#include <origin/concepts.hpp>

namespace origin
{
  /**
   * @defgroup iterator_concepts
   * @ingroup concepts
   */

  /**
   * @ingroup iterator_concepts
   * The iterator concept defines the most basic requirements for a type to
   * be considered an iterator. It must be Regular, incrementable and
   * dereferencable. Additionally, std::iterator_traits<Iter> must be a valid
   * specialization.
   */
  template<typename Iter>
  struct Iterator
    : Regular<Iter>
  {
    Iterator()
    { auto p = constraints; }

    static void constraints(Iter i)
    {
      // These are all required to be valid expressions.
      typedef typename std::iterator_traits<Iter>::value_type Value_Type;
      typedef typename std::iterator_traits<Iter>::reference Reference;
      typedef typename std::iterator_traits<Iter>::pointer Pointer;
      typedef typename std::iterator_traits<Iter>::difference_type Difference_Type;
      typedef typename std::iterator_traits<Iter>::iterator_category Iterator_Category;

      ++i;
      i++;
      *i;
    }

    typedef std::tuple<
      Regular<Iter>,
      has_pre_increment<Iter>,
      has_post_increment<Iter>,
      has_dereference<Iter>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup iterator_concepts
   * The input iterator concept requires that the dereferenced value of an
   * iterator is convertible to a given value type. If not explictily specified
   * the result of dereferencing a value type must be convertible to the
   * iterator's value type.
   */
  template<typename Iter, typename Value = unspecified>
  struct Input_Iterator
    : Iterator<Iter>
  {
    Input_Iterator()
    { auto p = constraints; }

    static void constraints(Iter i)
    {
      // This is explicitly an implicit conversion.
      Value x = *i;
    }

    typedef std::tuple<
      Regular<Iter>,
      Convertible<typename deduce_dereference<Iter>::type, Value>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // This specialization reduces the input iterator concept to a unary
  // concept. Here, the value type is the iterator's value type.
  template<typename Iter>
  struct Input_Iterator<Iter, unspecified>
    : Iterator<Iter>
  {
    Input_Iterator()
    { auto p = constraints; }

    static void constraints(Iter i)
    {
      typedef typename std::iterator_traits<Iter>::value_type Value_Type;
      Value_Type x = *i;
    }

    typedef std::tuple<
      Regular<Iter>,
      Convertible<
        typename deduce_dereference<Iter>::type,
        typename std::iterator_traits<Iter>::value_type>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup iterator_concepts
   * An output iterator...
   */
  template<typename Iter, typename Value>
  struct Output_Iterator
    : Iterator<Iter>
  {
    Output_Iterator()
    { auto p = constraints; }

    static void constraints(Iter i, Value x)
    {
      *i = x;
    }

    typedef std::tuple<
      Regular<Iter>,
      Assignable<typename deduce_dereference<Iter>::type, Value>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup iterator_concepts
   * This class encapsulates the compile-time requirements of the forward
   * iterator class.
   */
  template<typename Iter>
  struct Forward_Iterator_Requirements
  {
    typedef std::tuple<
      Regular<Iter>,
      has_pre_increment<Iter>,
      has_post_increment<Iter>,
      has_dereference<Iter>
    > requirements;
    typedef std::false_type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup iterator_concepts
   * A forward iterator is an iterator that
   *
   * An iterator satisfies multipass axiom if neither dereferencing the
   * iterator, nor incrementing the iterator consumes the state of the
   * underlying object. Subsequent traversals of a sequence with a multipass
   * iterator (with no modifications to the data between traversals) will yield
   * the same sequence of objects.
   */
  template<typename Iter>
  struct Forward_Iterator
    : Iterator<Iter>
  {
    Forward_Iterator()
    { auto p = constraints; }

    static void constraints(Iter i)
    {
      // NOTE: We can't just static_assert the value since its not dependent.
      // The common_type trait makes expression dependent.
      typedef typename std::common_type<Iter>::type T;
      static_assert(Forward_Iterator<T>::value,
                    "no explicit model for Forward_Iterator");
      *i++;
    }

    struct axioms {
      // Given two equivalent iterators, they will still be equivalent when
      // both have been incremented.
      static auto Increment_Equivalence(Iter x, Iter y)
        -> decltype(implies(x == y, ++x == ++y))
      { return implies(x == y, ++x == ++y); }

      // Incrementing an iterator does not consume its state.
      static auto Increment_Stability(Iter x)
        -> decltype(*x == *x)
      { (void)++Iter(x), *x == *x; }
    };

    // FIXME: If these constraints were specified outside the body of the
    // concept checking class, then I could actually have checked concept
    // maps. As it is, this is virtually inaccessible from specializations.
  };

  template<typename T>
  struct Forward_Iterator<T*>
    : Concept_Map<Forward_Iterator_Requirements<T*>>
  { };

  template<typename T>
  struct Forward_Iterator<T const*>
    : Concept_Map<Forward_Iterator_Requirements<T const*>>
  { };

  /**
   * @ingroup concepts
   * A bidirectional iterator...
   */
  template<typename Iter>
  struct Bidirectional_Iterator
    : Forward_Iterator<Iter>
  {
    Bidirectional_Iterator()
    { auto p = constraints; }

    static void constraints(Iter i)
    {
      --i;
      i--;
    }

    typedef std::tuple<
      Forward_Iterator<Iter>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

} // namespace origin

#endif
