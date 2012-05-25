// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_BOUNDED_RANGE_HPP
#define ORIGIN_RANGE_BOUNDED_RANGE_HPP

#include <origin/iterator.hpp>

namespace origin
{
  // Bounded range
  //
  // A bounded range class encapsulates a range bounded by a pair of iterators.
  // Iterating over a bounded range is equivalent to iterating over the same
  // pair of iterators. That is:
  //
  //    while (first != last) { 
  //      const auto& x = *first;
  //      // do something
  //      ++first; 
  //    }
  //
  // is euqivalent to:
  //
  //    for (const auto& x : bounded_range<I>{first, last})
  //      // do something.
  //
  // This is essentially the same as the boost::iterator_range, or 
  // std::pair<I, I> with appropriate overloads for begin and end.
  //
  // Template parameters:
  //    I -- The type of the underlying Iterator
  //
  // Invariants: 
  //    bounded_range(this->begin(), this->end());
  template <typename I>
    class bounded_range
    {
      static_assert(Weakly_incrementable<I>(), "");
      static_assert(Equality_comparable<I>(), "");
    public:
      using iterator = I;

      // Initialize the bounded range so that both values are the same. The
      // range is initially empty.
      bounded_range() 
        : first{}, last{first} 
      { }
    
      // Initialize the bounded range over [first, last).
      bounded_range(I first, I last)
        : first{first}, last{last}
      { 
        assert(is_bounded_range(first, last));
      }
      
      iterator begin() const { return first; }
      iterator end() const   { return last; }
      
    private:
      I first;
      I last;
    };

} // namespace origin

#endif
