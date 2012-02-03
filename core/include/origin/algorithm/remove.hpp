// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_REMOVE_HPP
#define ORIGIN_ALGORITHM_REMOVE_HPP

#include <algorithm>

namespace origin
{
  // Remove algorithms
  // The remove algorithms remove elements from an iterator range by permuting
  // a range, splitting into kept and remove elements, or by copying the
  // kept elements into a new range. There are three kinds of removal 
  // algorithms:
  //
  //    remove -- search or query for elements to be removed
  //    retain -- like remove, but negating the search or query
  //    extract -- move kept elements into a new range
  //    unique -- removes duplicate elements
  //
  // The remove algorithms are:
  //
  //    remove_if(first, last, pred)
  //    remove_if(range, pred)
  //    remove_copy_if(first, last, result, pred)
  //    remove_copy_if(in, out, pred)
  //
  //    ratain_if(first, last, pred)
  //    retain_if(range, pred)
  //    retain_copy_if(first, last, result, pred)
  //    retain_copy_if(in, out, pred)
  //
  //    remove(first, last, value)
  //    remove(first, last, value, comp)
  //    remove(range, value)
  //    remove(range, value, comp)
  //    remove_copy(first, last, result, value)
  //    remove_copy(first, last, result, value, comp)
  //    remove_copy(in, out, value)
  //    remove_copy(in, out, value, comp)
  //
  // TODO: Write the rest of the retain algorithms.
  //
  // The extract algorithms are similar to remove_copy except that the kept
  // elements are moved instead of copied, and the "removed" elements are
  // moved into a valid sub range of the origional.
  //
  //    extract(first, last, result, value)
  //    extract(first, last, result, value, comp)
  //    extract(in, out, value)
  //    extract(in, out, value, comp)
  //    extract_if(first, last, result, pred)
  //    extract_if(in, out, pred)
  //
  // TODO: Write the rest of the extract algorithms.
  //
  // The unique algorithms...
  //
  //
  // TODO: Write documentation
  
  
  // Remove if
  template<typename I, typename P>
    I o_remove_if(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Permutation<I>(), "");
      assert(is_permutable_range(first, last));

      first = o_find_if(first, last, pred);
      if(first != last)
        move_if_not(o_next(first), last, first);
      return first;
    }
    
    
    
  // Remove if (range)
  template<typename R, typename P>
    void remove_if(R&& range, P pred)
    {
      using Rx = Forwarded<R>;
      static_assert(Range_query<Rx, P>(), "");
      static_assert(Range_permutation<Rx>(), "");
      
      return o_remove_if(std::begin(range), std::end(range), pred);
    }
    
    
    
  // Remove copy if
  //
  // Note that this algorithm is equivalent to copy_if_not.
  template<typename I, typename O, typename P>
    O o_remove_copy_if(I first, I last, O result, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_distance(first, last), *first));

      return copy_if_not(first, last, result, pred);
    }
    
    
    
  // Remove copy if (range)
  template<typename R, typename O, typename P>
    void remove_copy_if(R&& range, O&& result, P pred)
    {
      using Rx = Forwarded<R>;
      using Ox = Forwarded<O>;
      static_assert(Range_query<Rx, P>(), "");
      static_assert(Range_copy<Rx, Ox>(), "");
      
      return o_remove_copy_if(std::begin(range), std::end(range),
                                std::begin(result), pred);
    }
  
  
  
  // Retain if
  // Retains the elements in [first, last) where pred(x) is true.
  template<typename I, typename P>
    I retain_if(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Permutation<I>(), "");
      assert(is_permutable_range(first, last));

      first = find_if(first, last, pred);
      if(first != last)
        move_if(o_next(first), last, first);
      return first;
    }
    
    
    
  // Retain if (range)
  template<typename R, typename P>
    inline void retain_if(R&& range, P pred)
    {
      using Rx = Forwarded<R>;
      static_assert(Range_query<Rx, P>(), "");
      static_assert(Range_permutation<Rx>(), "");
      
      retain_if(std::begin(range), std::end(range), pred);
    }
       
  

  // Retain copy if
  //
  // Note that this algorithm is equivalent to copy_if.
  template<typename I, typename O, typename P>
    O retain_copy_if(I first, I last, O result, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_distance(first, last), *first));

      return copy_if(first, last, result, pred);
    }
    
    
    
  // Remove copy if (range)
  template<typename R, typename O, typename P>
    inline void retain_copy_if(R&& range, O&& result, P pred)
    {
      using Rx = Forwarded<R>;
      using Ox = Forwarded<O>;
      static_assert(Range_query<Rx, P>(), "");
      static_assert(Range_copy<Rx, Ox>(), "");
      
      retain_copy_if(std::begin(range), std::end(range), std::begin(result), pred);
    }  
  
  
  
  // Remove
  template<typename I, typename T>
    I o_remove(I first, I last, const T& value)
    {
      static_assert(Search<I, T>(), "");
      static_assert(Permutation<I>(), "");
      assert(is_permutable_range(first, last));
  
      auto p = [&](const Value_type<I>& x) { return x != value; };
      first = find(first, last, value);
      if(first != last)
        move_if(o_next(first), last, first, p);
      return first;
    }
    
    
    
  // Remove (relation)
  template<typename I, typename T, typename R>
    I remove(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      static_assert(Permutation<I>(), "");
      assert(is_permutable_range(first, last));
      
      auto p = [&](const Value_type<I>& x) { return !comp(x, value); };
      first = find(first, last, value);
      if(first != last) {
        move_if(o_next(first), last, first, p);
      }
      return first;
    }

    
    
  // Remove (range)
  template<typename R, typename T>
  inline void remove(R&& range, const T& value)
  {
    using Rx = Forwarded<R>;
    static_assert(Range_search<Rx, T>(), "");
    static_assert(Range_permutation<Rx>(), "");
    
    return o_remove(std::begin<R>(range), std::end<R>(range), value);
  }
  
  
  
  // Remove (range, relation)
  template<typename R, typename T, typename Rel>
  inline void remove(R&& range, const T& value, Rel comp)
  {
    using Rx = Forwarded<R>;
    static_assert(Range_search<Rx, T, Rel>(), "");
    static_assert(Range_permutation<Rx>(), "");
    
    return remove(std::begin<R>(range), std::end<R>(range), value, comp);
  }

  
  
  // Remove copy  
  template<typename I, typename O, typename T>
    O o_remove_copy(I first, I last, O result, const T& value)
    {
      static_assert(Search<I, T>(), "");
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_distance(first, last), *first));

      auto p = [&](const Value_type<I>& x) { return x != value; };
      return copy_if(first, last, result, p);
    }
    
  
  
  // Remove copy  
  template<typename I, typename O, typename T, typename R>
    O remove_copy(I first, I last, O result, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_distance(first, last), *first));

      auto p = [&](const Value_type<I>& x) { !comp(x, value); };
      return copy_if(first, last, result, p);
    }
    
    
    
  // Remove copy (range)
  template<typename R, typename O, typename T>
    inline void remove_copy(R&& in, O&& out, const T& value)
    {
      using Rx = Forwarded<R>;
      using Ox = Forwarded<O>;
      static_assert(Range_search<Rx, T>(), "");
      static_assert(Range_copy<Rx, Ox>(), "");
      
      return o_remove_copy(std::begin(in), std::end(in), std::begin(out), value);
    }
  
  
  
  // Remove copy (range, relation)
  template<typename R, typename O, typename T, typename Rel>
    inline void remove_copy(R&& in, O&& out, const T& value, Rel comp)
    {
      using Rx = Forwarded<R>;
      using Ox = Forwarded<O>;
      static_assert(Range_search<Rx, T>(), "");
      static_assert(Range_copy<Rx, Ox>(), "");
      
      return o_remove_copy(std::begin(in), std::end(in), std::begin(out), value);
    }

  
  
  // Extract
  //
  // Extract the elements of [first, last) that are equal to value by moving 
  // them into the output range [result, last - first). The algorithm returns
  // a pair {new_last, result_last} where the range [first, new_last) is a
  // bounded range containing the elements not extracted and the output range
  // [result, result_last) contains the extracted elements.
  template<typename I, typename O, typename T>
    std::pair<I, O> extract(I first, I last, O result, const T& value)
    {
      static_assert(Search<I, T>(), "");
      static_assert(Move<I, O>(), "");
      static_assert(Permutation<I>(), "");
      assume(is_readable_range(first, last));
      assume(is_movable_range(result, o_count(first, last, value), *first));

      first = o_find(first, last, value);
      if(first != last) {
        *result = std::move(*first);
        ++result;
        I hole = first;
        ++first;
        while(first != last) {
          if(*first != value) {
            *hole = std::move(*first);
            ++hole;
          } else {
            *result = std::move(*first);
            ++result;
          }
          ++first;
        }
        return {hole, result};
      }
      return {first, result};
    }
    
    
    
  // Extract
  //
  // Remove the elements of [first, last) that satisfy pred by moving them
  // into the output range [result, last - first). The algorithm returns
  // a pair {new_last, result_last} where the range [first, new_last) is a
  // bounded range containing the elements not extracted and the output range
  // [result, result_last) contains the extracted elements.
  template<typename I, typename O, typename Pred>
    std::pair<I, O> extract_if(I first, I last, O result, Pred pred)
    {
      static_assert(Query<I, Pred>(), "");
      static_assert(Move<I, O>(), "");
      static_assert(Permutation<I>(), "");
      assert(( is_readable_range(first, last) ));
      assume(( is_movable_range(result, o_count_if(first, last, pred), *first) ));

      first = o_find_if(first, last, pred);
      if(first != last) {
        *result = std::move(*first);
        ++result;
        I hole = first;
        ++first;
        while(first != last) {
          if(!pred(*first)) {
            *hole = std::move(*first);
            ++hole;
          } else {
            *result = std::move(*first);
            ++result;
          }
          ++first;
        }
        return {hole, result};
      }
      return {first, result};
    }



  // Unique algorithms
  
  // Unique
  template<typename I>
    I o_unique(I first, I last)
    {
      static_assert(Equality_query<I>(), "");
      static_assert(Permutation<I>(), "");
      assert(is_permutable_range(first, last));

      first = find_adjacent(first, last);
      if(first != last) {
        // FIXME: This inner loop is similar to the remove loops above, but
        // increments before the assignment.
        for(I i = o_next(first, 2); i != last; ++i) {
          if(*first != *i) {
            ++first;
            *first = std::move(*i);
          }
          ++i;
        }
        return next(first);
      }
      return last;
    }
    
  
  
  // Unique (relation)
  //
  // FIXME: I'd like to improve the Search requirement, but I'm not sure what
  // name to use. See find_adjacent for more discussion.
  template<typename I, typename R>
    I o_unique(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      static_assert(Permutation<I>(), "");
      assert(is_permutable_range(first, last));

      first = find_adjacent(first, last, comp);
      if(first != last) {
        for(I i = o_next(first, 2); i != last; ++i) {
          if(!comp(*first, *i)) {
            ++first;
            *first = std::move(*i);
          }
          ++i;
        }
        return next(first);
      }
      return last;
    }
    
    
    
  // FIXME: Rewrite this algorithm in terms of its weakest requirements:
  // Input_iterator<I> and Output_iterator<I, Value_type<T>>. The algorithm
  // is currently implemented in terms of its strongest requirements. I think
  // the copy steps would need to be modified to accept the cached calue in
  // both cases.
    
  // Unique copy step
  // Copy the value of first into the next position of result if 
  // *first != *result.
  template<typename I, typename O>
    void unique_copy_step(I& first, O& result)
    {
      static_assert(Comparison<I, O>(), "");
      static_assert(Copy<I, O>(), "");

      if(*first != *result) {
        ++result;
        *result = *first;
      }
      ++first;
    }
  
  
  
  // Unique copy step (relation)
  // Implements the comparison, copy, and increment of the unique copy
  // algorithm.
  template<typename I, typename O, typename R>
    void unique_copy_step(I& first, O& result, R comp)
    {
      static_assert(Comparison<I, O, R>(), "");
      static_assert(Copy<I, O>(), "");
      
      if(!comp(*first, *result)) {
        ++result;
        *result = *first;
      }
      ++first;
    }

    
    
  // Unique copy
  //
  // FIXME: Implement count_unique as the precontion on result.
  template<typename I, typename O>
    O o_unique_copy(I first, I last, O result)
    {
      static_assert(Comparison<I, O>(), "");
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, num_unique(first, last)));
      
      if(first != last) {
        *result = *first;
        ++first;
        while(first != last)
          unique_copy_step(first, result);
      }
      return result;
    }
    
    
    
  // Unique copy (relation)
  //
  // FIXME: Implement count_unique as the precontion on result.
  template<typename I, typename O, typename R>
    O o_unique_copy(I first, I last, O result, R comp)
    {
      static_assert(Comparison<I, O>(), "");
      static_assert(Copy<I, O>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, count_unique(first, last)));
      
      if(first != last) {
        *result = *first;
        ++first;
        while(first != last)
          unique_copy_step(first, result, comp);
      }
      return result;
    }

    
} // namespace origin

#endif
