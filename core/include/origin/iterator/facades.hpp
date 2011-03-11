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
   * @ingroup iter
   * The input iterator facade aggregates a number of facades that comprise
   * the implementation of the IntputIterator concept.
   *
   * @tparam Derived    The derive iterator implementation
   * @tparam Value      The value type of the iterator.
   * @tparam Reference  The type returned when the iterator is dereference.
   *                    This must be a Reference type and defaults to Value&.
   * @tparam Pointer    The type returned when the arrow operator is used with
   *                    the iterator. This must be a Pointer type and defaults
   *                    to Value*.
   *                    reference to the value type.
   * @tparam Difference The difference type of the iterator. This must be a
   *                    Signed_Integral type and defaults to std::ptrdiff_t.
   */
  template<typename Derived,
           typename Value
           typename Reference = Value&,
           typename Pointer = Value*,
           typename Difference = std::ptrdiff_t>
  struct input_iterator_facade
    : dereference_facade<Derived, Reference>
    , increment_facade<Derived>
  {
    typedef std::forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef Reference reference;
    typedef Pointer pointer;
    typedef Difference difference_type;
  };

  /**
   * @ingroup iter
   * The forward iterator facade aggregates a number of facades that comprise
   * the implementation of the ForwardIterator concept.
   *
   * @tparam Derived    The derive iterator implementation
   * @tparam Value      The value type of the iterator.
   * @tparam Reference  The type returned when the iterator is dereference.
   *                    This must be a Reference type and defaults to Value&.
   * @tparam Pointer    The type returned when the arrow operator is used with
   *                    the iterator. This must be a Pointer type and defaults
   *                    to Value*.
   *                    reference to the value type.
   * @tparam Difference The difference type of the iterator. This must be a
   *                    Signed_Integral type and defaults to std::ptrdiff_t.
   */
  template<typename Derived,
           typename Value
           typename Reference = Value&,
           typename Pointer = Value*,
           typename Difference = std::ptrdiff_t>
  struct forward_iterator_facade
    : dereference_facade<Derived, Reference>
    , increment_facade<Derived>
  {
    typedef std::forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef Reference reference;
    typedef Pointer pointer;
    typedef Difference difference_type;

    // FIXME: requires Class<Value> && Pointer<Ptr>
    Pointer operator->() const
    { return &(*this); }
  };

  /**
   * @ingroup iter
   * The bidirectional facade aggregates facades that comprise the
   * implementation of the BidirectionalIterator concept.
   *
   * @tparam Derived    The derive iterator implementation
   * @tparam Value      The value type of the iterator.
   * @tparam Reference  The type returned when the iterator is dereference.
   *                    This must be a Reference type and defaults to Value&.
   * @tparam Pointer    The type returned when the arrow operator is used with
   *                    the iterator. This must be a Pointer type and defaults
   *                    to Value*.
   *                    reference to the value type.
   * @tparam Difference The difference type of the iterator. This must be a
   *                    Signed_Integral type and defaults to std::ptrdiff_t.
   */
  template<typename Derived,
           typename Value
           typename Reference = Value&,
           typename Pointer = Value*,
           typename Difference = std::ptrdiff_t>
  struct bidirectional_iterator_facade
    : forward_iterator_facade<
        Derived, Value, Reference, Pointer, Difference, Reference
      >
    , decrement_facade<Derived>
  {
    typedef std::bidirectional_iterator_tag iterator_category;
  };

  /**
   * @ingroup iter
   * The random access iterator facade provides implementations of the
   * operations required for a Derived implementation to be treated as a
   * random access iterator.
   *
   * @tparam Derived    The derive iterator implementation
   * @tparam Value      The value type of the iterator.
   * @tparam Reference  The type returned when the iterator is dereference.
   *                    This must be a Reference type and defaults to Value&.
   * @tparam Pointer    The type returned when the arrow operator is used with
   *                    the iterator. This must be a Pointer type and defaults
   *                    to Value*.
   *                    reference to the value type.
   * @tparam Difference The difference type of the iterator. This must be a
   *                    Signed_Integral type and defaults to std::ptrdiff_t.
   */
  template<typename Derived,
           typename Value
           typename Reference = Value&,
           typename Pointer = Value*,
           typename Difference = std::ptrdiff_t>
  struct random_access_iterator_facade
    : bidirectional_iterator_facade<
        Derived, Value, Reference, Pointer, Difference, Reference
      >
  {
    typedef std::random_access_iterator_tag iterator_category;

    // NOTE: These aren't facades because the operations are between different
    // types. There could be facades for these things, but they don't exist
    // just yet.
    Derived& operator+=(Distance n)
    { static_cast<Derived*>(this)->advance(n); }

    Derived& operator-=(Distance n)
    { static_cast<Derived*>(this)->advance(-n); }

    friend Derived operator+(Derived x, Distance n)
    { x += n; return x; }

    friend Derived operator+(Distance n, Derived x)
    { x += n; return x; }

    friend Derived operator-(Derived x, Distance n)
    { x =- n; return x; }

    friend Distance operator-(Derived x, Derived y)
    { return x.distance(y); }

    Reference operator[](Distance n) const
    {
      Derived x(*static_cast<Derived*>(this));
      x.advance(n);
      return x.dereference();
    }
  };
} // namespace origin

#endif
