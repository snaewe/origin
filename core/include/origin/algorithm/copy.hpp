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
    
  
  // Iterative copy (algorithm concept)
  // The iterative copy concept defines the requirements of copying values 
  // from an I iterator range into an O iterator range.
  template<typename I, typename O>
    constexpr bool Copy()
    {
      return Input_iterator<I>() && Weak_output_iterator<O, Value_type<I>>();
    }
  
  
  
  // Range copy (algorithm concept)
  // The range copy concept defines the requirements of copying values from
  // an input range (R) into output range (O).
  template<typename R, typename O>
    constexpr bool Range_copy()
    {
      return Input_range<R>() && Output_range<O, Value_type<R>>();
    }
    
    
    
  // Iterative move (algorithm concept)
  template<typename I, typename O>
    constexpr bool Move()
    {
      return Input_iterator<I>() && Weak_output_iterator<O, Value_type<I>&&>();
    }
    
    
  
  // Range move (algorithm concept)
  template<typename R, typename O>
    constexpr bool Range_move()
    {
      return Input_range<R>() && Output_range<O, Value_type<R>&&>();
    }
    

  // Copy
  // Copy the elements in a range a into another range b.
  //
  // FIXME: Implement optimizations.
  template<typename I, typename O>
    inline O std_copy(I first, I last, O result)
    {
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, distance(first, last), *first));

      while(first != last) {
        *result = *first;
        ++result;
        ++first;
      }
      return result;
    }
  
  
  
  // Copy (range)
  // Copy the elements from range into result where size(range) <= size(result).
  template<typename I, typename O>
    inline void copy(const I& range, O& result)
    {
      static_assert(Range_copy<I, O>(), "");
      assume(size(range) <= size(out));
      // FIXME: Overlapping requirements
      
      std_copy(std::begin(range), std::end(range), std::begin(result));
    }



  // Copy n
  template<typename I, typename O>
    inline O std_copy_n(I first, Distance_type<I> n, O result)
    {
      static_assert(Copy<I, O>(), "");
      
      while(n != 0) {
        *result = *first;
        ++result;
        ++first;
        --n;
      }
      return result;
    }



  // Copy if
  template<typename I, typename O, typename P>
    inline O std_copy_if(I first, I last, O result, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, O>(), "");
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
  template<typename I, typename O, typename P>
    inline void copy_if(const I& in, O& out, P pred)
    {
      static_assert(Range_query<I, P>(), "");
      static_assert(Range_copy<I, O>(), "");
      assume(size(out) >= count_if(in, pred));

      std_copy_if(std::begin(in), std::end(in), std::begin(out), pred);
    }



  // Copy backward
  template<typename I, typename O>
    inline O std_copy_backward(I first, I last, O result)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Bidirectional_iterator<O>(), "");
      static_assert(Copy<I, O>(), "");
      
      return std::copy_backward(first, last, result);
    }
    
    
  
  // Copy backward (range)
  template<typename R1, typename R2>
    inline void copy_backward(const R1& in, R2& out)
    {
      static_assert(Bidirectional_range<R1>(), "");
      static_assert(Bidirectional_range<R2>(), "");
      static_assert(Range_copy<R1, R2>(), "");
      return std_copy_backward(std::begin(in), std::end(in), std::begin(out));
    }



  // Move
  // Move the elements in a range a into another range b.
  template<typename I, typename O>
    inline O std_move(I first, I last, O result)
    {
      static_assert(Move<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_movable_range(result, distance(first, last), *first));
      
      return std::copy(first, last, result);
    }
  
  
  
  // Move (range)
  template<typename I, typename O>
    inline void move(const I& i, O& o)
    {
      static_assert(Range_move<I, O>(), "");
      assume(size(i) <= size(o));
      
      std_copy(std::begin(i), std::end(i), std::begin(o));
    }


    
  // Move backward
  template<typename I, typename O>
    inline O std_move_backward(I first, I last, O result)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Bidirectional_iterator<O>(), "");
      static_assert(Move<I, O>(), "");

      return std::move_backward(first, last, result);
    }


    
  // Move backward (range)
  template<typename I, typename O>
    inline void move_backward(const I& in, O& out)
    {
      static_assert(Bidirectional_range<I>(), "");
      static_assert(Bidirectional_range<O>(), "");
      static_assert(Range_move<I, O>(), "");

      return std_move_backward(std::begin(in), std::end(in), std::begin(out));
    }
  
  
  // Iterator swap
  template<typename I1, typename I2>
    inline void std_iter_swap(I1 i, I2 j)
    {
      static_assert(Writable<I1, Value_type<I2>&&>(), "");
      static_assert(Writable<I2, Value_type<I1>&&>(), "");

      Value_type<I1> x = std::move(*i);
      *i = std::move(*j);
      *j = std::move(x);
    }
    


  // Swap ranges
  template<typename I1, typename I2>
    inline I2 std_swap_ranges(I1 first1, I1 last1, I2 first2)
    {
      static_assert(Move<I1, I2>(), "");
      static_assert(Move<I2, I1>(), "");

      return std::swap_ranges(first1, last1, first2);
    }

  
} // namespace origin

#endif
