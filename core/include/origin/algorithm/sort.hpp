// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_SORT_HPP
#define ORIGIN_ALGORITHM_SORT_HPP

#include <algorithm>

namespace origin
{
  // The sort family is comprised of a number of algorithms concerning the 
  // ordered permutation of iterator ranges.
  //
  // There are two properties used throughout this library:
  //
  //    is_sortable_range(first, last)
  //    is_sortable_range(first, last, comp)
  //
  // Sorting related queries are:
  //
  //    is_sorted(first, last)
  //    is_sorted(first, last, comp)
  //    is_sorted(range)
  //    is_sorted(range, comp)
  

  // Is sortable range (property)
  // Returns true if [first, last) satisfy the prerequisites for sorting. A
  // sortable range is a permutable range.
  template<typename I>
    bool is_sortable_range(I first, I last) 
    { 
      static_assert(Order_query<I>(), "");
      return is_permutable_range(first, last); 
    }


  
  // Is sortable range (property, relation)
  // Returns true if [first, last) satisfies the prerequisites for soring using
  // the relation comp. The iterator range must be permutable and comp must be
  // a strict weak ordering.
  template<typename I, typename R>
    bool is_sortable_range(I first, I last, R comp) 
    { 
      static_assert(Relational_query<R>(), "");
      return is_permutable_range(first, last) && is_strict_weak_ordering(comp);
    }

    
    
  // Is sorted
  // Returns true if *i <= *(i + 1) for all iterators i in [first, last).
  template<typename I>
    bool o_is_sorted(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      
      std::less<Value_type<I>> comp;
      return is_relation_preserving(first, last, comp);
    }
  
  
  
  // Is sorted (relation)
  // Returns true if !comp(*(i + 1), *i) for all iterators i in [first, last).
  template<typename I, typename R>
    bool o_is_sorted(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_strict_weak_ordering(comp));
      
      return relation_preserving(first, last, comp);
    }
    
    

  // Is sorted (range)
  // Returns true if *i <= *(i + 1) for all iterators i in range.
  template<typename R>
    bool is_sorted(const R& range)
    {
      static_assert(Range_order_query<R>(), "");
      
      return o_is_sorted(o_begin(range), o_end(range));
    }
  
  
  
  // Is sorted (range, relation)
  // Returns true if !comp(*(i + 1), *i) for all iterators i in range.
  template<typename R, typename Rel>
    bool is_sorted(const R& range, Rel comp)
    {
      static_assert(Range_relational_query<R, Rel>(), "");
      assert(is_strict_weak_ordering(comp));
      
      return o_is_sorted(o_begin(range), o_end(range), comp);
    }
  


  // Is sorted until
  template<typename I>
    I o_is_sorted_until(I first, I last)
    {
      static_assert(Order_query<I>(), "");

      std::less<Value_type<I>> comp;
      return find_not_adjacent(first, last, comp);
    }
  
  
  
  // Is sorted until (relation)
  template<typename I, typename R>
    I o_is_sorted_until(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_strict_weak_ordering(comp));
      
      return find_not_adjacent(first, last, comp);
    }


  
  // Is sorted until (range)
  template<typename R>
    auto is_sorted_until(const R& range) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");
      
      return o_is_sorted_until(o_begin(range), o_end(range));
    }
    
    
    
  // Is sorted until (range)
  template<typename R, typename Rel>
    auto is_sorted_until(const R& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_relational_query<R, Rel>(), "");
      assert(is_strict_weak_ordering(comp));
      
      return o_is_sorted_until(o_begin(range), o_end(range), comp);
    }
    
    

  // Sort
  template<typename I>
    void o_sort(I first, I last)
    {
      static_assert(Sort<I>(), "");
      assert(is_permutable_range(first, last));
      
      return std::sort(first, last);
    }
    
    
    
  // Sort (relation)
  template<typename I, typename R>
    void o_sort(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      assert(is_permutable_range(first, last));
      assert(is_strict_weak_order(comp));
      
      return std::sort(first, last, comp);
    }
    
    
    
  // Sort (range)
  template<typename R>
    void sort(R&& range)
    {
      static_assert(Range_sort<Forwarded<R>>(), "");
      
      return o_sort(o_begin(range), o_end(range));
    }
  
  
  // Sort (range, relation)
  template<typename R, typename Rel>
    void sort(R&& range, Rel comp)
    {
      static_assert(Range_sort<Forwarded<R>, Rel>(), "");
      assert(is_strict_weak_ordering(comp));
      
      return o_sort(o_begin(range), o_end(range), comp);
    }

    
    
  // Stable sort
  template<typename I>
    void o_stable_sort(I first, I last)
    {
      static_assert(Sort<I>(), "");
      assert(is_permutable_range(first, last));
      
      return std::stable_sort(first, last);
    }
    
    
    
  // Stable sort (relation)
  template<typename I, typename R>
    void o_stable_sort(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      assert(is_permutable_range(first, last));
      assert(is_strict_weak_ordering(comp));

      return std::stable_sort(first, last, comp);
    }

    
    
  // Stable sort (range)
  template<typename R>
    void stable_sort(R&& range)
    {
      static_assert(Range_sort<Forwarded<R>>(), "");
      
      return o_stable_sort(o_begin(range), o_end(range));
    }


  
  // Stable sort (range, relation)
  template<typename R, typename Rel>
    void stable_sort(R&& range, Rel comp)
    {
      static_assert(Range_sort<Forwarded<R>, Rel>(), "");
      assert(is_strict_weak_ordering(comp));
      
      return o_stable_sort(o_begin(range), o_end(range), comp);
    }


    
  // NOTE: The partial sort algorithms do not have a range equivalent. It would
  // be possible to write a version that accepts partial_sort(range, i), but
  // that seems a little lopsided. A more interesting option might be to
  // provide partial_sort(range1, range2) where range1 and range2 are contiguous
  // sub-ranges.
    
    
    
  // Partial sort
  template<typename I>
    void o_partial_sort(I first, I middle, I last)
    {
      static_assert(Random_access_iterator<I>(), "");
      static_assert(Sort<I>(), "");
      assert(is_permutable_range(first, last));

      return std::partial_sort(first, last);
    }
    
    
    
  // Partial sort (relation)
  template<typename I, typename R>
    void o_partial_sort(I first, I middle, I last, R comp)
    {
      static_assert(Random_access_iterator<I>(), "");
      static_assert(Sort<I, R>(), "");
      assert(is_permutable_range(first, last));
      assert(is_strict_weak_ordering(comp));

      return std::partial_sort(first, last, comp);
    }
    
    
    
  // Partial sort copy
  template<typename I1, typename I2>
    void o_partial_sort_copy(I1 first, I1 last, I2 result_first, I2 result_last)
    {
      static_assert(Random_access_iterator<I2>(), "");
      static_assert(Copy<I1, I2>(), "");
      static_assert(Sort<I2>(), "");
      assert(is_readable_range(first, last));
      assert(is_permutable_range(result_first, result_last));

      return std::partial_sort_copy(first, last, result_first, result_last);
    }
    
    
    
  // Partial sort copy (relation)
  template<typename I1, typename I2, typename R>
    void o_partial_sort_copy(I1 first, I1 last, I2 result_first, I2 result_last, R comp)
    {
      static_assert(Random_access_iterator<I2>(), "");
      static_assert(Copy<I1, I2>(), "");
      static_assert(Sort<I2, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_permutable_range(result_first, result_last));

      return std::partial_sort_copy(first, last, result_first, result_last, comp);
    }

    
    
  // Partial sort copy (range)
  template<typename R1, typename R2>
    void partial_sort_copy(const R1& in, R2&& out)
    {
      static_assert(Random_access_range<Forwarded<R2>>(), "");
      static_assert(Range_copy<R1, Forwarded<R2>>(), "");
      static_assert(Range_sort<Forwarded<R2>>(), "");

      return o_partial_sort_copy(o_begin(in), o_end(in), o_begin(out), o_end(out));
    }



  // Partial sort copy (range, relation)
  template<typename R1, typename R2, typename Rel>
    void partial_sort_copy(const R1& in, R2&& out, Rel comp)
    {
      static_assert(Random_access_range<Forwarded<R2>>(), "");
      static_assert(Range_copy<R1, Forwarded<R2>>(), "");
      static_assert(Range_sort<Forwarded<R2>, Rel>(), "");

      return o_partial_sort_copy(o_begin(in), o_end(in), o_begin(out), o_end(out), comp);
    }

  
  
  // NOTE: There is not range version of the nth element algorithm. See the
  // comments for partial_sort for additional information.
  
  
  
  // Nth element
  template<typename I>
    void o_nth_element(I first, I nth, I last)
    {
      static_assert(Random_access_iterator<I>(), "");
      static_assert(Sort<I>(), "");

      return std::nth_element(first, last);
    }



  // Nth element (relation)
  template<typename I, typename R>
    void o_nth_element(I first, I nth, I last, R comp)
    {
      static_assert(Random_access_iterator<I>(), "");
      static_assert(Sort<I, R>(), "");
      
      return std::nth_element(first, last);
    }
    
} // namespace origin

#endif
    