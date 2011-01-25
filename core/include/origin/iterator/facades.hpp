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
  /**
   * @ingroup iter
   * The forward iterator facade aggregates a number of facades that comprise
   * the implementation of the ForwardIterator concept.
   *
   * @tparam Derived The derive iterator implementation
   * @tparam Reference The iterator reference type
   */
  template<typename Derived, typename Reference>
  struct forward_iterator_facade
    : dereference_facade<Derived, Reference>
    , equality_facade<Derived>
    , increment_facade<Derived>
  {
    typedef std::forward_iterator_tag iterator_category;
    typedef Reference reference;
  };

  /**
   * @ingroup iter
   * The bidirectional facade aggregates facades that comprise the
   * implementation of the BidirectionalIterator concept.
   *
   * @tparam Derived The derive iterator implementation
   * @tparam Reference The iterator reference type
   */
  template<typename Derived, typename Reference>
  struct bidirectional_iterator_facade
    : forward_iterator_facade<Derived, Reference>
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
   * @tparam Derived The derive iterator implementation
   * @tparam Reference The iterator reference type
   * @tparam Distance The iterator difference type
   */
  template<typename Derived, typename Reference, typename Distance>
  struct random_access_iterator_facade
    : forward_iterator_facade<Derived, Reference>
  {
    typedef Distance difference_type;
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
