// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_HPP
#define ORIGIN_ALGORITHM_HPP

#include <type_traits>

namespace origin
{
 
  // Returns true if and only if all elements in [first, last) are equal to
  // value or [first, last) is an empty range.
  template<typename Iter, typename T>
    bool all_equal(Iter first, Iter last, T const& value)
    {
      // precondition: readable_range(first, last)
      while(first != last) {
        if(*first != value)
          return false;
        ++first;
      }
      return true;
    }
    
  // Returns true if and only if [first, last) is not an empty range and at
  // least one element in [first, last) is equal to value.
  template<typename Iter, typename T>
    bool any_equal(Iter first, Iter last, T const& value)
    {
      // precondition: readable_range(first, last)
      while(first != last)
      {
        if(*first == value)
          return true;
      }
      return false;
    }
    
  // Returns true if and only if [first, last) is an empty range or there
  // are no elements in [first, last) that are equal to value (or equivalently,
  // all elements are not equal to value).
  template<typename Iter, typename T>
    bool none_equal(Iter first, Iter last, T const& value)
    {
      // precondition: readable_range(first, last)
      while(first != last)
      {
        if(*first == value)
          return false;
      }
      return true;
    }

  // Return the first element in [first, last) that satisfies the predicate
  // p.
  //
  // This function, along with next_if, can be used to iterate over the
  // subsequence of values satisfying some predicate p.
  template<typename Iter, typename Pred>
    inline Iter first_if(Iter first, Iter last, Pred p)
    {
      // precondition: readable_range(first, last)
      return find_if(first, last, p);
    }

  // Return the next element in [first, last) that satisfies pred.
  template<typename Iter, typename Pred>
    inline Iter next_if(Iter first, Iter last, Pred p)
    {
      // precondition: readable_range(first, last)
      if(first != last)
        return find_if(first + 1, last, p);
      else
        return last;
    }
    
  // Return an iterator to the nth element in [first, last) that satisfies
  // pred, if such an element exists. If no such element exists, return
  // last.
  //
  // TODO: Replace Size with DistanceType.
  template<typename Iter, typename Size, typename Pred>
    Iter find_nth_if(Iter first, Iter last, Size n, Pred p)
    {
      // precondition: readable_range(first, last)
      // precondition: n >= 0
      if(n == 0)
        return last;
      
      first = first_if(first, last, p);
      --n;
      while(n != 0 && first != last) {
        first = next_if(first, last, p);
        --n;
      }
      return first;
    }
  
} // namespace origin

#endif
