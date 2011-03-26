// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_SUPPORT_HPP
#define ORIGIN_RANGE_SUPPORT_HPP

#include <origin/utility/meta.hpp>
#include <origin/iterator/facades.hpp>

// FIXME: Rename this file to "traits.hpp"

namespace origin
{
  /**
   * @internal
   * Deduce the type of T::const_iterator.
   */
  template<typename T>
  struct range_deduce_const_iterator_
  {
  private:
    template<typename U> static typename U::const_iterator check(U&&);
    static substitution_failure check(...);
  public:
    typedef decltype( check(std::declval<T>()) ) type;
  };

  /**
   * @internal
   * Return true if T does not have a const_iterator.
   */
  template<typename T>
  struct range_no_const_iterator_
    : substitution_failed<typename range_deduce_const_iterator_<T>::type>
  { };

  /**
   * @internal
   * Return true if T looks like a container by virtue of defining a const
   * iterator type.
   *
   * @todo This is a concept-like trait and should/could probably be
   * generalized.
   */
  template<typename T>
  struct range_is_container_
    : std::integral_constant<bool, !range_no_const_iterator_<T>::value>
  { };

  /**
   * @internal
   * A metafunction supporting lazy access to the Range's iterator type.
   */
  template<typename Range>
  struct range_get_iterator_
  { typedef typename Range::iterator type; };

  /**
   * @internal
   * A metafunction support lazy access to the Container's constant iterator
   * type.
   */
  template<typename Const>
  struct range_get_const_iterator_
  { typedef typename Const::const_iterator type; };

  /**
   * @internal
   * The choose iterator metafunction selects the iterator type for the
   * given range. Typically, this is simply the Range's defined iterator
   * type. If the Range is a constant Container type, then this returns the
   * Container's constant iterator type.
   */
  template<typename Range>
  struct range_choose_iterator_
  {
    typedef typename Range::iterator type;
  };

  template<typename Range>
  struct range_choose_iterator_<Range const>
  {
    typedef typename std::conditional<
      range_is_container_<Range>::value,
      range_get_const_iterator_<Range>,
      range_get_iterator_<Range>
    >::type Condition;
    typedef typename Condition::type type;
  };


  /**
   * The range traits class provides a single point of access for the associated
   * types of range types.
   */
  template<typename Range>
  struct range_traits
  {
    typedef typename range_choose_iterator_<Range>::type iterator;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;
  };

  // FIXME: This is probably required, but I'm not entirely sure.
  template<typename T, std::size_t N>
  struct range_traits<T[N]>
  {
    typedef T* iterator;
    typedef std::ptrdiff_t difference_type;
  };

} // namespace origin

#endif
