// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_COUNTER_HPP
#define ORIGIN_ITERATOR_COUNTER_HPP

// NOTE: This file is included by <origin/iterator.hpp>
#include <origin/iterator.hpp>

namespace origin
{

  // An alias for the difference type of a counter argument. For Integral types,
  // this is the signed value of that type. For Iterator types, it is the 
  // distance type.
  //
  // FIXME: This is not really correct for integral types. It should be the
  // next larger signed representation.
  template<typename I>
    using Incrementable_difference = 
      If<Integral<I>(), Make_signed<I>, Distance_type<I>>;
  

  // An alias for the iterator category of a counter. If I is an integral type,
  // the category is "random access". Otherwise, it is the iterator's category.
  template<typename I>
    using Incrementable_category = 
      If<Integral<I>(), std::random_access_iterator_tag, Iterator_category<I>>;



  // A counter is an iterator that traverses a counted sequence of 
  // incrementable objects, usually integers. It can also be used to count over 
  // other iterator types.
  //
  // The counter is parameterized over its underlying "counter" type and an
  // action that increments counter. The default action is to simply increment
  // the counter.
  template<typename I, typename Act = increment_action<I>>
    class counter 
    {
      static_assert(Weakly_incrementable<I>(), "");
      static_assert(Function<Act, I&>(), "");
    public:
      using value_type        = I;
      using reference         = const I&;
      using pointer           = const I*;
      using difference_type   = Incrementable_difference<I>;
      using iterator_category = Incrementable_category<I>;
      
      using advance_action = Act;

      // Default constructor
      counter(I i = {}, Act adv = {})
        : i{i}
      { }
      
      // Returns the underlying incrementable object. 
      reference base() const { return i; }
      
      // Returns the advance action.
      advance_action advance_act() const { return adv; }

      // Readable
      reference operator*() const { return i; }
      pointer operator->() const { return &i; }
  
      // Equality_comparable
      bool operator==(const counter& x) const { return i == x.i; }
      bool operator!=(const counter& x) const { return i != x.i; }

      bool operator<(const counter& x) const  { return i < x.i; }
      bool operator>(const counter& x) const  { return i > x.i; }
      bool operator<=(const counter& x) const { return i <= x.i; }
      bool operator>=(const counter& x) const { return i >= x.i; }

      // Increment
      counter& operator++() { ++i; return *this; }
      counter operator++(int) { counter tmp{*this}; operator++(); return tmp; }
      
      // Decrement
      counter& operator--() { --i; return *this; }
      counter operator--(int) { counter tmp{*this}; operator--(); return tmp; }

      // Advance
      counter& operator+=(difference_type n) { i += n; return *this; }
      counter& operator-=(difference_type n) { i -= n; return *this; }
      
      // requires Random_access_iterator<Iter> || Integral<Iter>
      friend counter operator+(counter c, difference_type n) { c += n; return c; }
      friend counter operator+(difference_type n, counter c) { c += n; return c; }
      friend counter operator-(counter c, difference_type n) { c -= n; return c; }

      // Difference
      friend difference_type distance(const counter& a, const counter& b)
      {
        assume(( a.adv == b.adv ));
        return distance(a.i, b.i, a.adv);
      }

    private:
      I i;    // The incremented object
      Act adv;  // The advance action
    };

  // Return a counter over the incrementable type. An increment action may be
  // optionally specified.
  template<typename I, typename Act = increment_action<I>>
    counter<I, Act> make_counter(const I& inc, Act act = {})
    {
      return {inc, act};
    }


} // namespace origin

#endif
