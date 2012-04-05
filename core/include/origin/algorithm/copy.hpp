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
  //    copy_n(first, last, result)
  //
  //    copy_if(first, last, pred)
  //    copy_if_not(first, last, pred)
  //    ...
  //
  // The range-based algorithms support the following interfaces.
  //
  //    copy(in, out)
  //    copy_n(in, out)
  //    copy_if(in, out, pred)
  //    copy_if_not(in, out, pred)
  //
  // Here, out can either be an output iterator or an output range.
  //
  // Move algorithms also transfer values, but invalidate the original object.
  //
  //    move(first, last, result)
  //    ...
  //    move_if(first, last, result, pred)
  //    move_if(in, out, pred)
  //    move_if_not(first, last, result, pred)
  //    move_if_not(in, out, pred)
  //
  // For good measure, we include swap algorithms:
  //
  //    iter_swap(i, j)
  //
  //    exchange(first1, last1, first2)
  //    exchange_if(first1, last1, first2, pred)
  //
  // FIXME: Finish documenting all of these algorithms.
  //
  // FIXME: Add overlap requirements for all of these algorithms.

  
   
  // Copy step
  // Copy the value of *i to the object pointed at by o and increment both
  // iterators.
  template <typename I, typename O>
    inline void copy_step(I& i, O& o)
    {
      static_assert(Weak_input_iterator<I>(), "");
      static_assert(Weak_output_iterator<O, Value_type<I>>(), "");

      *o = *i;
      ++i;
      ++o;
    }
    
    
    
  // Copy
  // Copy the elements in a range a into another range b.
  //
  // FIXME: Optimize this algorithm for trivially copyable types.
  template <typename I, typename O>
    inline O o_copy(I first, I last, O result)
    {
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, distance(first, last), *first));
      // FIXME: Overlapping requirements

      while (first != last)
        copy_step(first, result);
      return result;
    }
  


  // Copy (range to iterator)
  // Copy the elements from range into the output range [first, ...).
  template <typename I, typename O>
    inline auto copy(const I& range, O result)
      -> Requires<Range_copy_out<I, O>(), O>
    {
      // FIXME: Overlapping requirements
      return o_copy(o_begin(range), o_end(range), result);
    }


  
  // Copy (range to range)
  // Copy the elements from range into result where size(range) <= size(result).
  template <typename I, typename O>
    inline auto copy(const I& range, O&& result)
      -> Requires<Range_copy<I, Forwarded<O>>(), decltype(o_begin(result))>
    {
      assume(size(range) <= size(out));
      // FIXME: Overlapping requirements
      
     return o_copy(o_begin(range), o_end(range), o_begin(result));
    }



  // Copy n
  template<typename I, typename O>
    inline O o_copy_n(I first, Distance_type<I> n, O result)
    {
      static_assert(Copy<I, O>(), "");
      
      while(n != 0) {
        copy_step(first, result);
        --n;
      }
      return result;
    }



  // Copy if
  template<typename I, typename O, typename P>
    inline O o_copy_if(I first, I last, O result, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_count_if(first, last, pred), *first));
      
      while(first != last) {
        if(pred(*first))
          copy_step(first, result);
        else
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

      o_copy_if(std::begin(in), std::end(in), std::begin(out), pred);
    }


    
  // Copy if not
  template<typename I, typename O, typename P>
    inline O copy_if_not(I first, I last, O result, P pred)
    {
      while(first != last) {
        if(!pred(*first))
          copy_step(first, result);
        else
          ++first;
      }
      return result;
    }
    
    

  // Copy if not (range)
  template<typename R, typename O, typename P>
    inline void copy_if_not(R&& in, O&& out, P pred)
    {
      using Rx = Forwarded<R>;
      using Ox = Forwarded<O>;
      static_assert(Range_query<Rx, P>(), "");
      static_assert(Copy<Rx, Ox>(), "");
      
      o_copy_if_not(std::begin(in), std::end(in), std::begin(out), pred);
    }

    

  // Copy backward step
  // Decrement both iterators and copy the value of *i to the object pointed
  // at by o.
  template<typename I, typename O>
    void copy_backward_step(I& i, O& o)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Bidirectional_iterator<O>(), "");
      static_assert(Output_iterator<O, Value_type<I>>(), "");

      --i;
      --o;
      *o = *i;
    }
    

  // Copy backward
  template<typename I, typename O>
    inline O o_copy_backward(I first, I last, O result)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Bidirectional_iterator<O>(), "");
      static_assert(Copy<I, O>(), "");

      while(first != last)
        copy_backward_step(first, result);
      return result;
    }
    
    
  
  // Copy backward (range)
  template<typename R1, typename R2>
    inline void copy_backward(const R1& in, R2& out)
    {
      static_assert(Bidirectional_range<R1>(), "");
      static_assert(Bidirectional_range<R2>(), "");
      static_assert(Range_copy<R1, R2>(), "");
      
      return o_copy_backward(std::begin(in), std::end(in), std::begin(out));
    }


    
  // Move step
  // Move the value of *i into the object pointed at by *o and increment both
  // iterators.
  template<typename I, typename O>
    void move_step(I& i, O& o)
    {
      static_assert(Weak_input_iterator<I>(), "");
      static_assert(Weak_output_iterator<O, Value_type<I>&&>(), "");

      *o = std::move(*i);
      ++i;
      ++o;
    }

    
    
  // Move
  // Move the elements in the range [first, last) into the range 
  // [result, result + (last - first)).
  template<typename I, typename O>
    inline O o_move(I first, I last, O result)
    {
      static_assert(Move<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_movable_range(result, distance(first, last), *first));

      while(first != last)
        move_step(first, result);
      return result;
    }
  
  
  
  // Move (range)
  template<typename I, typename O>
    inline void move(const I& i, O& o)
    {
      static_assert(Range_move<I, O>(), "");
      assume(size(i) <= size(o));
      
      o_copy(std::begin(i), std::end(i), std::begin(o));
    }

    
  
  // Move if
  template<typename I, typename O, typename P>
    inline O move_if(I first, I last, O result)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Move<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_movable_range(result, distance(first, last), *first));

      while(first != last) {
        if(pred(*first))
          move_step(first, result);
        else
          ++first;
      }
      return result;
    }
    
    
    
  // Move if (range)
  template<typename R, typename O, typename P>
    inline void move_if(R&& in, O&& out, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      static_assert(Range_move<R, O>(), "");
      return move_if(std::begin(in), std::end(in), std::begin(out), pred);
    }

    
    
  // Move if not
  template<typename I, typename O, typename P>
    inline O move_if_not(I first, I last, O result)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Move<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_movable_range(result, distance(first, last), *first));

      while(first != last) {
        if(!pred(*first))
          move_step(first, result);
        else
          ++first;
      }
      return result;
    }

    
    
  // Move if not (range)
  template<typename R, typename O, typename P>
    inline void move_if_not(R&& in, O&& out, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      static_assert(Range_move<R, O>(), "");
      return move_if_not(std::begin(in), std::end(in), std::begin(out), pred);
    }

    
    
  // Move backward step
  // Move the value of *i into the object pointed at by *o and increment both
  // iterators.
  template<typename I, typename O>
    void move_backward_step(I& i, O& o)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Bidirectional_iterator<O>(), "");
      static_assert(Output_iterator<O, Value_type<I>>(), "");

      --i;
      --o;
      *o = std::move(*i);
    }

    
    
  // Move backward
  template<typename I, typename O>
    inline O o_move_backward(I first, I last, O result)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Bidirectional_iterator<O>(), "");
      static_assert(Move<I, O>(), "");

      while(first != last)
        move_backward_step(first, result);
      return result;
    }


    
  // Move backward (range)
  template<typename I, typename O>
    inline void move_backward(const I& in, O& out)
    {
      static_assert(Bidirectional_range<I>(), "");
      static_assert(Bidirectional_range<O>(), "");
      static_assert(Range_move<I, O>(), "");

      return o_move_backward(std::begin(in), std::end(in), std::begin(out));
    }
  


  // Iterator swap
  template<typename I1, typename I2>
    inline void o_iter_swap(I1 i, I2 j)
    {
      static_assert(Writable<I1, Value_type<I2>&&>(), "");
      static_assert(Writable<I2, Value_type<I1>&&>(), "");

      Value_type<I1> x = std::move(*i);
      *i = std::move(*j);
      *j = std::move(x);
    }


    
  // Exchange step
  template<typename I1, typename I2>
    inline void exchange_step(I1& i, I2& j)
    {
      iter_swap(i, j);
      ++i;
      ++j;
    }

    
    
  // Exchange
  //
  // This is the same as swap_ranges in the standard library and exchange_values
  // in Elements of Programming.
  template<typename I1, typename I2>
    inline I2 exchange(I1 first1, I1 last1, I2 first2)
    {
      static_assert(Move<I1, I2>(), "");
      static_assert(Move<I2, I1>(), "");

      while(first1 != last1)
        swap_step(first1, first2);
      return first2;
    }

    
  
  // Exchange if
  // Selectively exchange the values of [first1, last1) and 
  // [first2, first2 + n).
  template<typename I1, typename I2, typename P>
    inline I2 exchange_if(I1 first1, I1 last1, I2 first2, P pred)
    {
      while(first1 != last1) {
        if(pred(*first1))
          exchange_step(first1, first2);
        else
          ++first1;
      }
      return first2;
    }
    
  
} // namespace origin

#endif
