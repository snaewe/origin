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
   * @ingroup iterator
   * @ingroup concepts
   *
   * The iterator concepts (and traits) define constraints on iterator types.
   */

  template<typename Iter> struct cIterator;
  template<typename Iter> struct cForward_Iterator;
  template<typename Iter> struct cBidirectional_Iterator;
  template<typename Iter> struct cRandom_Access_Iterator;

  /**
   * @ingroup iterator_concepts
   *
   * The Iterator concept is valid for all iterator types. An iterator is
   * an object that can be incremented and dereferenced.
   */
  template<typename Iter>
    struct cIterator : cRegular<Iter>
    {
      // NOTE: This will break concept checking if Iter is not actually an
      // iterator type, however, this greatly simplifies the writing of traits
      // involviong associated types.
      typedef typename std::iterator_traits<Iter>::value_type value_type;
      typedef typename std::iterator_traits<Iter>::reference reference;
      typedef typename std::iterator_traits<Iter>::pointer pointer;
      typedef typename std::iterator_traits<Iter>::difference_type difference_type;
      typedef typename std::iterator_traits<Iter>::iterator_category iterator_category;

      cIterator()
      {
        auto p = constraints;
      }

      static void constraints(Iter i)
      {
        cMoveable<reference>{};
        tSigned_Int<difference_type>{};
        tConvertible<reference, Iter&>{};
        i++;
        *i;
      }

      typedef std::tuple<
        cRegular<Iter>,
        cMoveable<reference>,
        tSigned_Int<difference_type>,
        has_pre_increment<Iter>,
        tConvertible<typename get_pre_increment_result<Iter>::type, Iter&>,
        has_post_increment<Iter>,
        has_dereference<Iter>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup iterator_concepts
   *
   * The Input_Iterator trait is valid for Iterators whose reference type can
   * be converted to the specified type. By default, an Input_Iterator's
   * reference type must be able to bind to a const reference to the value
   * type.
   */
  template<typename Iter,
           typename Value = typename std::iterator_traits<Iter>::value_type const&>
    struct tInput_Iterator
    {
      typedef typename std::iterator_traits<Iter>::reference reference;

      tInput_Iterator()
      {
        cIterator<Iter>{};
        tConvertible<reference, Value>{};
      }

      typedef std::tuple<
        tConvertible<reference, Value>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };


  /**
   * @ingroup iterator_concepts
   *
   * The Output_Iterator trait is valid for Iterators whose reference type
   * supports assignment to the specified type. By default an Ouput_Iterator's
   * reference must move assignable to the iterator's value type.
   */
  template<typename Iter,
           typename Value = typename std::iterator_traits<Iter>::value_type&&>
    struct tOutput_Iterator
    {
      typedef typename std::iterator_traits<Iter>::reference reference;

      tOutput_Iterator()
      {
        cIterator<Iter>{};
        tAssignable<reference, Value>{};
      }

      typedef std::tuple<
        tAssignable<reference, Value>
      > requirements;
      typedef typename requires_all<requirements>::type type;
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
  struct cForward_Iterator
    : cIterator<Iter>
  {
    typedef typename std::iterator_traits<Iter>::iterator_category iterator_category;

    cForward_Iterator()
    {
      auto p = constraints;
    }

    static void constraints(Iter i)
    {
      tInput_Iterator<Iter>{};
      tConvertible<iterator_category, std::forward_iterator_tag>{};

      // Strengthen requirements on the postincrement result.
      tConvertible<decltype(i++), Iter const&>{};
    }

    typedef std::tuple<
      tInput_Iterator<Iter>,
      tConvertible<iterator_category, std::forward_iterator_tag>,
      tConvertible<typename get_post_increment_result<Iter>::type, Iter const&>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * A bidirectional iterator...
   */
  template<typename Iter>
  struct cBidirectional_Iterator
    : cForward_Iterator<Iter>
  {
    cBidirectional_Iterator()
    {
      auto p = constraints;
    }

    static void constraints(Iter i)
    {
      tConvertible<decltype(--i), Iter&>{};
      tConvertible<decltype(i--), Iter const&>{};
    }

    // FIXME: Write in terms of tPre_Decrement, tPost_Decrement?
    typedef std::tuple<
      cForward_Iterator<Iter>,
      has_pre_decrement<Iter>,
      tConvertible<typename get_pre_decrement_result<Iter>::type, Iter&>,
      has_post_decrement<Iter>,
      tConvertible<typename get_post_decrement_result<Iter>::type, Iter const&>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup conceps
   *
   * The Random_Access_Iterator is valid for Bidirectional iterators that
   * support constant-time, random access traversal.
   */
  template<typename Iter>
  struct cRandom_Access_Iterator
  {
    typedef typename std::iterator_traits<Iter>::reference reference;
    typedef typename std::iterator_traits<Iter>::difference_type difference_type;

    cRandom_Access_Iterator()
    {
      auto p = constraints;
    }

    static void constraints(Iter i, difference_type n)
    {
      tConvertible<decltype(i += n), Iter&>{};
      tConvertible<decltype(i + n), Iter>{};
      tConvertible<decltype(n + i), Iter>{};

      tConvertible<decltype(i -= n), Iter&>{};
      tConvertible<decltype(i - n), Iter>{};

      tConvertible<decltype(i - i), difference_type>{};

      tConvertible<decltype(i[n]), reference>{};
    }

    // FIXME: Write in terms of actual traits (tPlus_assign?)
    typedef std::tuple<
      cBidirectional_Iterator<Iter>,

      has_plus_assign<Iter, difference_type>,
      tConvertible<typename get_plus_assign_result<Iter, difference_type>::type, Iter&>,

      has_plus<Iter, difference_type>,
      tConvertible<typename get_plus_result<Iter, difference_type>::type, Iter>,

      has_plus<difference_type, Iter>,
      tConvertible<typename get_plus_result<difference_type, Iter>::type, Iter>,

      has_minus_assign<Iter, difference_type>,
      tConvertible<typename get_minus_assign_result<Iter, difference_type>::type, Iter&>,

      has_minus<Iter, difference_type>,
      tConvertible<typename get_minus_result<Iter, difference_type>::type, Iter>,

      has_minus<Iter, Iter>,
      tConvertible<typename get_minus_result<Iter, Iter>::type, difference_type>,

      has_subscript<Iter, difference_type>,
      tConvertible<typename get_subscript_result<Iter, difference_type>::type, reference>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };
  
} // namespace origin

#endif
