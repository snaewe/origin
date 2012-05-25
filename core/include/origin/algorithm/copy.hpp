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
      *o = *i;
      ++i;
      ++o;
    }



  // Copy implementation
  //
  // FIXME: The optimizations should require trivially copy assignable, but
  // GCC does not support those traits yet.
  template <typename I, typename O>
    inline O copy_impl(I first, I last, O result)
    {
      while (first != last)
        copy_step(first, result);
      return result;
    }
  
  template <typename T>
    inline auto copy_impl(const T* first, const T* last, T* result) 
      -> Requires<Trivial<T>(), T*>
    {
      std::size_t n = last - first;
      std::memcpy(result, first, n * sizeof(T));
      return result + n;
    }  

  template <typename T>
    inline auto copy_impl(T* first, T* last, T* result) 
      -> Requires<Trivial<T>(), T*> 
    {
      std::size_t n = last - first;
      std::memcpy(result, first, n * sizeof(T));
      return result + n;
    }



  // Copy
  // Copy the elements in the range [first, last) to the output range pointed
  // at by result.

  template <typename I, typename O>
    inline O copy(I first, I last, O result)
    {
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, distance(first, last), *first));
      assume(not_overlapped(first, last, result));
      return copy_impl(unwrap(first), unwrap(last), unwrap(result));
    }
  


  // Copy n implementation
  //
  // FIXME: The optimizations should require trivially copy assignable, but
  // GCC does not support those traits yet.
  template <typename I, typename O>
    inline O copy_n_impl(I first, Difference_type<I> n, O result)
    {
      while(n != 0) {
        copy_step(first, result);
        --n;
      }
      return result;
    }

  template <typename T>
    inline auto copy_n_impl(const T* first, std::ptrdiff_t n, T* result) 
      -> Requires<Trivial<T>(), T*>
    {
      std::memcpy(result, first, n * sizeof(T));
      return result + n;
    }  

  template <typename T>
    inline auto copy_n_impl(T* first, std::ptrdiff_t n, T* result) 
      -> Requires<Trivial<T>(), T*> 
    {
      std::memcpy(result, first, n * sizeof(T));
      return result + n;
    }  


  // Copy n
  // Copy the values in the range [first, first + n) into the output range
  // pointed to be result.
  template <typename I, typename O>
    inline O copy_n(I first, Difference_type<I> n, O result)
    {
      static_assert(Copy<I, O>(), "");
      return copy_n_impl(unwrap(first), n, unwrap(result));
    }




  // Copy if
  template <typename I, typename O, typename P>
    inline O copy_if(I first, I last, O result, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, count_if(first, last, pred), *first));
      
      while(first != last) {
        if(pred(*first))
          copy_step(first, result);
        else
          ++first;
      }
      return result;
    }
    
  

    
  // Copy if not
  template <typename I, typename O, typename P>
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




  // Copy backward step
  // Decrement both iterators and copy the value of *i to the object pointed
  // at by o.
  template <typename I, typename O>
    inline void copy_backward_step(I& i, O& o)
    {
      --i;
      --o;
      *o = *i;
    }
    

  // Copy backward
  //
  // TODO: Optimize for the the case where the value can be trivially moved
  // from the input range to the output range.
  template <typename I, typename O>
    inline O copy_backward(I first, I last, O result)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Bidirectional_iterator<O>(), "");
      static_assert(Copy<I, O>(), "");

      while(first != last)
        copy_backward_step(first, result);
      return result;
    }
    
    
  
  // Copy backward (range)
  template <typename R, typename O>
    inline void copy_backward(const R& in, O&& out)
    {
      static_assert(Bidirectional_range<O>(), "");
      static_assert(Bidirectional_range<Auto<O>>(), "");
      static_assert(Range_copy<R, Auto<O>>(), "");
      return copy_backward(begin(in), end(in), begin(out));
    }


    
  // Move step
  // Move the value of *i into the object pointed at by *o and increment both
  // iterators.
  template <typename I, typename O>
    void move_step(I& i, O& o)
    {
      *o = std::move(*i);
      ++i;
      ++o;
    }



  // Move implementation
  template <typename I, typename O>
    inline O move_impl(I first, I last, O result)
    {
      while(first != last)
        move_step(first, result);
      return result;
    }

  template <typename T>
    inline auto move(const T* first, const T* last, T* result)
      -> Requires<Trivial<T>(), T*>
    {
      std::ptrdiff_t n = last - first;
      std::memmove(result, first, n * sizeof(T));
      return result + n;
    }

  template <typename T>
    inline auto move(T* first, T* last, T* result)
      -> Requires<Trivial<T>(), T*>
    {
      std::ptrdiff_t n = last - first;
      std::memmove(result, first, n * sizeof(T));
      return result + n;
    }

    
    
  // Move
  // Move the elements in the range [first, last) into the range 
  // [result, result + (last - first)).
  template <typename I, typename O>
    inline O move(I first, I last, O result)
    {
      static_assert(Move<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_movable_range(result, distance(first, last), *first));
      return move_impl(unwrap(first), unwrap(last), unwrap(result));
    }
  
  
  
  // Move (range)
  template <typename R, typename O>
    inline void move(const R& i, O o)
    {
      static_assert(Range_move<R, O>(), "");
      assume(size(i) <= size(o));
      move(begin(i), end(i), begin(o));
    }





  // Move if
  template <typename I, typename O, typename P>
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
  template <typename R, typename O, typename P>
    inline void move_if(R&& in, O&& out, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      static_assert(Range_move<R, O>(), "");
      return move_if(std::begin(in), std::end(in), std::begin(out), pred);
    }

    
    
  // Move if not
  template <typename I, typename O, typename P>
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
  template <typename R, typename O, typename P>
    inline void move_if_not(R&& in, O&& out, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      static_assert(Range_move<R, O>(), "");
      return move_if_not(std::begin(in), std::end(in), std::begin(out), pred);
    }

    
    
  // Move backward step
  // Decrement both iterators and move the representation of *i to the object 
  // pointed at by o.
  template <typename I, typename O>
    inline void move_backward_step(I& i, O& o)
    {
      --i;
      --o;
      *o = std::move(*i);
    }


  // Move backward implementation
  template <typename I, typename O>
    inline O move_backward_impl(I first, I last, O result)
    {
      while (first != last)
        move_backward_step(first, result);
      return result;
    }

  template <typename T>
    inline auto move_backward_impl(const T* first, const T* last, T* result)
      -> Requires<Trivial<T>(), T*>
    {
      std::ptrdiff_t n = last - first;
      result -= n;
      std::memmove(result, first, n * sizeof(T));
      return result;
    }

  template <typename T>
    inline auto move_backward_impl(T* first, T* last, T* result)
      -> Requires<Trivial<T>(), T*>
    {
      std::ptrdiff_t n = last - first;
      result -= n;
      std::memmove(result, first, n * sizeof(T));
      return result;
    }


  // Move backward
  //
  // FIXME: Write type type requirements and preconditions of this algorithm.
  template <typename I, typename O>
    inline O move_backward(I first, I last, O result)
    {
      return move_backward_impl(unwrap(first), unwrap(last), unwrap(result));
    }


  // Move backward (range)
  template <typename R, typename O>
    inline O move_backward(const R& range, O result)
    {
      return move_backward(begin(range), end(range), begin(result));
    }




  // Iterator swap
  //
  // FIXME: I am just about 100% sure that Value_type<I1> and Value_type<I2>
  // must be the same type to guarantee correct behavior. 
  //
  // FIXME: Do not call std::swap! Call origin's swap.
  template <typename I1, typename I2>
    inline void iter_swap(I1 i, I2 j)
    {
      static_assert(Move_writable<I1, Value_type<I2>>(), "");
      static_assert(Move_writable<I2, Value_type<I1>>(), "");

      std::swap(*i, *j);
    }


    
  // Exchange step
  template <typename I1, typename I2>
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
  template <typename I1, typename I2>
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
  template <typename I1, typename I2, typename P>
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
