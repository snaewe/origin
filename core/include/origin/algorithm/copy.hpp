// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_COPY_HPP
#define ORIGIN_ALGORITHM_COPY_HPP

#include <algorithm>

namespace origin
{
  // Copy algorithms
  // The copy module concerns the transfer of values form one range of eleemnts
  // into (or over) another range. It has the following algorithms:
  //
  //    copy(first, last, result)
  //    copy(in, out)
  //    copy_n(first, last, result)
  //    copy_n(in, out)
  //    copy_if(first, last, pred)
  //    copy_if(in, out)
  //    ...
  //
  // Move algorithms also transfer values, but invalidate the original object.
  //
  //    move(first, last, result)
  //
  // For good measure, we include swap algorithms:
  //
  //    iter_swap
  //    swap_ranges

  // FIXME: Add overlap requirements for all of these algorithms.
    
    

  // Copy
  // Copy the elements in a range a into another range b.
  template<typename I, typename O>
    inline O std_copy(I first, I last, O result)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Writable<O, Value_type<I>>(), "");
      assert(( is_readable_range(first, last) ));
      assume(( is_writable_range(result, distance(first, last), *first) ));

      return std::copy(first, last, result);
    }
  
  
  
  // Copy (range)
  // Copy the elements from range into result where size(range) <= size(result).
  template<typename I, typename O>
    inline void copy(const I& range, O& result)
    {
      static_assert(Input_range<I>(), "");
      static_assert(Output_range<O, Value_type<I>>(), "");
      assume(( size(range) <= size(out) ));
      // FIXME: Overlapping requirements
      
      std_copy(std::begin(range), std::end(range), std::begin(result));
    }



  // Copy n
  template<typename Iter, typename Out>
    inline Out std_copy_n(Iter first, Distance_type<Iter> n, Out result)
    {
      return std::copy_n(first, n, result);
    }



  // Copy if
  template<typename I, typename O, typename P>
    inline O std_copy_if(I first, I last, O result, P pred)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Weak_output_iterator<O, Value_type<I>>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, std_count_if(first, last, pred), *first));
      
      while(first != last) {
        if(pred(*first)) {
          *result = *first;
          ++result;
        }
        ++first;
      }
      return result;
    }
    
    
  
  // Copy if (range)
  template<typename Ri, typename Ro, typename P>
    inline void copy_if(const Ri& in, Ro& out, P pred)
    {
      static_assert(Input_range<Ri>(), "");
      static_assert(Output_range<Ro, Value_type<Ri>>(), "");
      assume(size(out) >= count_if(in, pred));

      std_copy_if(std::begin(in), std::end(in), std::begin(out), pred);
    }
    

    
  // Copy backward
  template<typename Iter, typename Out>
    inline Out std_copy_backward(Iter first, Iter last, Out result)
    {
      return std::copy_backward(first, last, result);
    }



  // Move
  // Move the elements in a range a into another range b.

  template<typename Iter, typename Out>
    inline Out std_move(Iter first, Iter last, Out result)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Move_writable<Out, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      assume(( is_movable_range(result, distance(first, last), *first) ));
      
      return std::copy(first, last, result);
    }
  
  template<typename In, typename Out>
    inline Iterator_type<Out> move(const In& i, Out& o)
    {
      static_assert(Input_range<In>(), "");
      static_assert(Move_range<Out, Value_type<In>>(), "");
      assume(( size(i) <= size(o) ));
      
      return std_copy(std::begin(i), std::end(i), std::begin(o));
    }



  // Move backward
  template<typename Iter, typename Out>
    inline Out std_move_backward(Iter first, Iter last, Out result)
    {
      return std::move_backward(first, last, result);
    }


  
  // Iterator swap
  template<typename Iter1, typename Iter2>
    inline void std_iter_swap(Iter1 i, Iter2 j)
    {
      std::iter_swap(i, j);
    }
    


  // Swap ranges
  template<typename Iter1, typename Iter2>
    inline Iter2 std_swap_ranges(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      return std::swap_ranges(first1, last1, first2);
    }

  
} // namespace origin

#endif
