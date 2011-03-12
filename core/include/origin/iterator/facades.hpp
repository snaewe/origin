// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_FACADES_HPP
#define ORIGIN_ITERATOR_FACADES_HPP

#include <iterator>

#include <origin/utility/facades.hpp>

namespace origin
{
  // FIXME: Make sure that the template parameter names don't collide with
  // concepts. I'm looking at you Pointer and Difference.

  /**
   * @defgroup iter_facades
   * @ingroup iter
   *
   * The iterator facade classes are used to simplify the process of creating
   * iterators. They are implemented using the curiously recurring template
   * pattern (CRTP), and are parameterized over the the derived iterator type
   * and their associated types.
   *
   * The different facade classes are used to generate the "maximal" kind of 
   * iterator being defined. For example, a filter iterator is maximally a
   * bidirectional iterator but only if the underlying iterator is also
   * bidirectional. If the underlying iterator is a forward iterator, then the
   * resulting filter iterator must also be a forward iterator. In the other
   * hand, if the underlying iterator is a random access iterator, then the
   * resulting iterator is "bounded" by the use of the bidirectional iterator
   * facade. A filter iterator cannot be a random access iterator.
   *
   * Iterator facades all take the following parameters.
   *
   * @tparam Derived    The derive iterator implementation
   * @tparam Value      The value type of the iterator.
   * @tparam Ref        The type returned when the iterator is dereference.
   *                    This must be a Reference type and defaults to Value&.
   * @tparam Ptr        The type returned when the arrow operator is used with
   *                    the iterator. This must be a Pointer type and defaults
   *                    to Value*.
   *                    reference to the value type.
   * @tparam Diff       The difference type of the iterator. This must be a
   *                    Signed_Integral type and defaults to std::ptrdiff_t.
   * @tparam Kind       The iterator category. This must be one of the iterator
   *                    tag classes.
   */

  /** @internal */
  namespace iterator_
  {
    /** 
     * Clamp the iterator tag to the given maximum value. If the given Tag is
     * convertible to Max, then return Max. Otherwise, return Tag. For
     * example, if Tag is a forward iterator tag and Max is a bidirectional
     * iterator tag, then the result is the forward iterator tag. If 
     */
    template<typename Tag, typename Max>
    struct max_tag
    {
      typedef typename std::conditional<
        std::is_convertible<Tag, Max>::value, Max, Tag
      >::type type;
    };
  } // namespace iterator_

  /**
   * @ingroup iter_facades
   * The input iterator facade aggregates a number of facades that comprise
   * the implementation of the Intput_Iterator concept. 
   */
  template<typename Derived,
           typename Value,
           typename Ref = Value&,
           typename Ptr = Value*,
           typename Diff = std::ptrdiff_t>
  struct input_iterator_facade
    : dereference_facade<Derived, Ref>
    , increment_facade<Derived>
  {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef Diff difference_type;
    typedef std::input_iterator_tag iterator_category;

    // FIXME: requires Class<Value> && Pointer<Ptr>
    // FIXME: Does this exist for input iterators? Not sure why it wouldn't.
    Ptr operator->() const
    { return &(*this); }
  };

  // FIXME: I'm pretty sure that this isn't entirely correct. I need some
  // legitimate use cases for real output iterators (move_back_iterator?). The
  // same can be said for input iterators. I think it's unlikely that output
  // iterators would be adaptors. Just iterator implementations.
  /**
   * @ingroup iter_facades
   * The output iterator facade aggregates a number of facades that comprise
   * the implementation of the Output_Iterator concept. 
   */
  template<typename Derived,
           typename Value,
           typename Ref = Value&,
           typename Ptr = Value*,
           typename Diff = std::ptrdiff_t>
  struct output_iterator_facade
    : dereference_facade<Derived, Ref>
    , increment_facade<Derived>
  {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef Diff difference_type;
    typedef std::output_iterator_tag iterator_category;
  };


  /**
   * @ingroup iter_facades
   * The forward iterator facade aggregates a number of facades that comprise
   * the implementation of the Forward_Iterator concept. 
   */
  template<typename Derived,
           typename Value,
           typename Ref = Value&,
           typename Ptr = Value*,
           typename Diff = std::ptrdiff_t,
           typename Kind = std::forward_iterator_tag>
  struct forward_iterator_facade
    : dereference_facade<Derived, Ref>
    , increment_facade<Derived>
  {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef Diff difference_type;
    typedef typename iterator_::max_tag<
      Kind, std::forward_iterator_tag
    >::type iterator_category;

    // FIXME: requires Class<Value> && Pointer<Ptr>
    Ptr operator->() const
    { return &(*this); }
  };

  /**
   * @ingroup iter_facades
   * The bidirectional facade aggregates facades that comprise the
   * implementation of the Bidirectional_Iterator concept.
   */
  template<typename Derived,
           typename Value,
           typename Ref = Value&,
           typename Ptr = Value*,
           typename Diff = std::ptrdiff_t,
           typename Kind = std::bidirectional_iterator_tag>
  struct bidirectional_iterator_facade
    : forward_iterator_facade<Derived, Value, Ref, Ptr, Diff>
    , decrement_facade<Derived>
  {
    typedef typename iterator_::max_tag<
      Kind, std::bidirectional_iterator_tag
    >::type iterator_category;
  };

  /**
   * @ingroup iter_facades
   * The random access iterator facade provides implementations of the
   * operations required for a Derived implementation to be treated as a
   * Random_Access_Iterator.
   */
  template<typename Derived,
           typename Value,
           typename Ref = Value&,
           typename Ptr = Value*,
           typename Diff = std::ptrdiff_t,
           typename Kind = std::random_access_iterator_tag>
  struct random_access_iterator_facade
    : bidirectional_iterator_facade<Derived, Value, Ref, Ptr, Diff>
  {
    typedef typename iterator_::max_tag<
      Kind, std::random_access_iterator_tag
    >::type iterator_category;

    // NOTE: These aren't facades because the operations are between different
    // types. There could be facades for these things, but they don't exist
    // just yet.
    Derived& operator+=(Diff n)
    { static_cast<Derived*>(this)->advance(n); }

    Derived& operator-=(Diff n)
    { static_cast<Derived*>(this)->advance(-n); }

    friend Derived operator+(Derived x, Diff n)
    { x += n; return x; }

    friend Derived operator+(Diff n, Derived x)
    { x += n; return x; }

    friend Derived operator-(Derived x, Diff n)
    { x =- n; return x; }

    friend Diff operator-(Derived x, Derived y)
    { return x.distance(y); }

    Ref operator[](Diff n) const
    { return *((*static_cast<Derived*>(this)) + n); }
  };
} // namespace origin

#endif
