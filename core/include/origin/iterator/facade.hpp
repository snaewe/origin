// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_FACADE_HPP
#define ORIGIN_ITERATOR_FACADE_HPP

#include <origin/tuple.hpp>
#include <origin/iterator/core.hpp>

namespace origin
{
  // FIXME: Facade may not be the right name...


  // Weak facade
  // The weak facade class provides an overload of the post-increment operator
  // that effectively acts as though it did not exist. The result type of
  // the operator is subst_failure, making queries for its existence appear
  // as though the lookup failed.
  struct weak_iterator_facade
  {
    subst_failure operator++() { return {}; }
    subst_failure operator--() { return {}; }
    subst_failure operator[](int) { assert(false); return {}; }
  };


  // Forward facade
  // The forward facade provides a default implementation of the post-
  // increment operator in terms of the pre-increment operator.
  template <typename Derived>
    struct forward_iterator_facade
    {
      Derived operator++(int)
      {
        Derived* self = static_cast<Derived*>(this);
        Derived tmp {*self};
        self->operator++();
        return tmp;
      }

      subst_failure operator--() { assert(false); return {}; }
      subst_failure operator[](int) { assert(false); return {}; }
    };


  // Bidirectional facade
  // The bidirectional facade provides a default implementation of the
  // post-decrement operator in terms of the pre-decrement operator.
  template <typename Derived>
    struct bidirectional_iterator_facade : forward_iterator_facade<Derived>
    {
      Derived operator--(int)
      {
        Derived* self = static_cast<Derived*>(this);
        Derived tmp {*self};
        self->operator--();
        return tmp;
      }

      subst_failure operator[](int) { assert(false); return {}; }
    };



  // Random access facade
  // The random access facade provides overloads of the + and - operators
  // for advancing (but not iterator subtraction), written in terms of the
  // += and -= operator.
  template <typename Derived, typename Ref, typename Diff>
    struct random_access_iterator_facade : bidirectional_iterator_facade<Derived>
    {
      Ref operator[](Diff n) const
      { 
        const Derived& self = *static_cast<const Derived*>(this);
        return *(self + n);
      }

      friend Derived operator+(Derived i, Diff n) { return i += n; }
      friend Derived operator+(Diff n, Derived i) { return i += n; }
      friend Derived operator-(Derived i, Diff n) { return i -= n; }
    };



  // A helper trait for choosing the appropriate base class.
  template <typename Derived, typename Iter, typename Ref, typename Diff>
    using Choose_iterator_facade =
      If<
        Strict_weakly_incrementable<Iter>(),
        weak_iterator_facade,
        If<
          Forward_iterator<Iter>() && !Bidirectional_iterator<Iter>(),
          forward_iterator_facade<Derived>,
          If<
            Bidirectional_iterator<Iter>() && !Random_access_iterator<Iter>(),
            bidirectional_iterator_facade<Derived>,
            random_access_iterator_facade<Derived, Ref, Diff>
          >
        >
      >;



  // Iterator facade
  // The iterator facade provides support for common implementations of
  // some (not all!) operators required by various iterator concepts.
  //
  // The iterator reference type (Ref) and difference type (diff) can be
  // given explicitly if they differ from those of the underlying iterator.
  // Note that these are only used in the case where a random access iterator
  // is being constructed.
  template <typename Derived, 
            typename Iter, 
            typename Ref = Iterator_reference<Iter>,
            typename Diff = Difference_type<Iter>>
    struct iterator_facade : Choose_iterator_facade<Derived, Iter, Ref, Diff>
    {
      using Base = Choose_iterator_facade<Derived, Iter, Ref, Diff>;

      using Base::operator++;
      using Base::operator--;
      using Base::operator[];
    };

} // namespace origin

#endif
