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

        // Make sure that the names actually align with the deduced types.
        // Otherwise, we can't be internally consistent.
        *i;
        tSame<decltype(*i), reference>{};
        
        ++i;
        tConvertible<decltype(++i), Iter&>{};
        
        i++;
        *i++; // Needed by irregular iterators (vector<bool>::iterator)
      }

      typedef std::tuple<
        cRegular<Iter>,
        cMoveable<reference>,
        tSigned_Int<difference_type>,
        tDereference<Iter>,
        tSame<typename tDereference<Iter>::result_type, reference>,
        tPre_Increment<Iter>,
        tConvertible<typename tPre_Increment<Iter>::result_type, Iter&>,
        tPost_Increment<Iter>,
        tDereference<typename tPost_Increment<Iter>::result_type>
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
    struct cForward_Iterator : cIterator<Iter>
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
        tConvertible<typename tPost_Increment<Iter>::result_type, Iter const&>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts
   * A bidirectional iterator...
   */
  template<typename Iter>
    struct cBidirectional_Iterator : cForward_Iterator<Iter>
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
        tPre_Decrement<Iter>,
        tConvertible<typename tPre_Decrement<Iter>::result_type, Iter&>,
        tPost_Decrement<Iter>,
        tConvertible<typename tPost_Decrement<Iter>::result_type, Iter const&>
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
        // Random access
        tConvertible<decltype(i += n), Iter&>{};
        tConvertible<decltype(i + n), Iter>{};
        tConvertible<decltype(n + i), Iter>{};
        tConvertible<decltype(i -= n), Iter&>{};
        tConvertible<decltype(i - n), Iter>{};

        // Difference
        tConvertible<decltype(i - i), difference_type>{};

        // Subscript
        tConvertible<decltype(i[n]), reference>{};
      }

      // FIXME: Write in terms of actual traits (tPlus_assign?)
      typedef std::tuple<
        cBidirectional_Iterator<Iter>,
        tPlus_Assign<Iter, difference_type>,
        tConvertible<typename tPlus_Assign<Iter, difference_type>::result_type, Iter&>,
        tPlus<Iter, difference_type>,
        tConvertible<typename tPlus<Iter, difference_type>::result_type, Iter>,
        tPlus<difference_type, Iter>,
        tConvertible<typename tPlus<difference_type, Iter>::result_type, Iter>,
        tMinus_Assign<Iter, difference_type>,
        tConvertible<typename tMinus_Assign<Iter, difference_type>::result_type, Iter&>,
        tMinus<Iter, difference_type>,
        tConvertible<typename tMinus<Iter, difference_type>::result_type, Iter>,
        tMinus<Iter, Iter>,
        tConvertible<typename tMinus<Iter, Iter>::result_type, difference_type>,
        tSubscript<Iter, difference_type>,
        tConvertible<typename tSubscript<Iter, difference_type>::result_type, reference>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };
  
} // namespace origin

#endif
