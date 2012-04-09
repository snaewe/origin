// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_COUNTER_HPP
#define ORIGIN_ITERATOR_COUNTER_HPP

#include <origin/iterator.hpp>

namespace origin
{
  // A counter is an iterator that traverses a counted sequence of 
  // incrementable objects, usually integers. It can also be used to count over 
  // other iterator types.
  //
  // The counter is parameterized over its underlying "counter" type and an
  // action that increments counter. The default action is to simply increment
  // the counter. Note that counter increments are unchecked. If the action
  // is to advance by some number > 1, then it is possible to overrun the
  // limit of the range.
  template<typename I, typename Act = increment_action<I>>
    class counter 
    {
      static_assert(Weakly_incrementable<I>(), "");
      static_assert(Function<Act, I&>(), "");
    public:
      using value_type        = I;
      using reference         = const I&;
      using pointer           = const I*;
      using difference_type   = Incrementable_distance<I>;
      using iterator_category = Incrementable_category<I>;

      // Default constructor
      counter(I i = {}, Act adv = {})
        : data{i, adv}
      { }
      
      // Returns the underlying incrementable object. 
      reference base() const { return iter(); }
      
      // Returns the advance action.
      Act advance_act() const { return adv(); }

      // Readable
      reference operator*() const { return iter(); }
      pointer operator->() const { return &iter(); }
  
      // Equality_comparable
      bool operator==(const counter& x) const { return iter() == x.iter(); }
      bool operator!=(const counter& x) const { return iter() != x.iter(); }

      bool operator<(const counter& x) const  { return iter() < x.iter(); }
      bool operator>(const counter& x) const  { return iter() > x.iter(); }
      bool operator<=(const counter& x) const { return iter() <= x.iter(); }
      bool operator>=(const counter& x) const { return iter() >= x.iter(); }

      // Increment
      counter& operator++() { ++iter(); return *this; }
      counter operator++(int) { counter tmp{*this}; operator++(); return tmp; }
      
      // Decrement
      counter& operator--() { --iter(); return *this; }
      counter operator--(int) { counter tmp{*this}; operator--(); return tmp; }

      // Random Access operations
      // requires Random_access_iterator<Iter> || Integral<Iter>
      
      // Advance
      counter& operator+=(difference_type n) { iter() += n; return *this; }
      counter& operator-=(difference_type n) { iter() -= n; return *this; }
      
      friend counter operator+(counter c, difference_type n) { return c += n; }
      friend counter operator+(difference_type n, counter c) { return c += n; }
      friend counter operator-(counter c, difference_type n) { return c -= n; }

      // Difference
      friend difference_type operator-(const counter& i, const counter& j)
      {
        assume(( i.adv() == j.adv() ));
        return (i.iter() - j.iter()) / get_increment(i.adv());
      }

    private:
      I&       iter()       { return std::get<0>(data); }
      const I& iter() const { return std::get<0>(data); }
      
      const Act& adv() const { return std::get<1>(data); }
    
    private:
      std::tuple<I, Act> data;
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
