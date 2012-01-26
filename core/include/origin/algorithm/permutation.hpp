// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_PERMUTATION_HPP
#define ORIGIN_ALGORITHM_PERMUTATION_HPP

#include <origin/algorithm/boost_combination.hpp>

namespace origin
{
  // Is permutation (impl, sorting)
  // Returns true if the range [first1, last1) is a permutation of the range
  // [first2, last2). The algorithm copies both input ranges into temporary 
  // buffers and sorts them. The runtime of this implementation is O(n log n), 
  // although there are 2n copies and 2 executions of the sort.
  template<typename I1, typename I2>
    bool is_sorted_permutation(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Sort<I1>(), "");
      static_assert(Sort<I2>(), "");
      static_assert(Comparison<I1, I2>(), "");
      
      o_sort(first1, last1);
      o_sort(first2, last2);
      return o_equal(first1, last1, first2);
    }
    
  
    
  // Is permutation (impl, sorting, relation)
  // Returns true if the range [first1, last1) is a permutation of the range
  // [first2, last2). The algorithm copies both input ranges into temporary 
  // buffers and sorts them. The runtime of this implementation is O(n log n), 
  // although there are 2n copies and 2 executions of the sort.
  //
  // The comp relation is required to be a strict weak ordering. Equality is 
  // checked using the symmetric complement of comp. Note that the Comparison
  // requirement below covers the case where equal is used with its symmetric
  // complement.
  template<typename I1, typename I2, typename R>
    bool is_sorted_permutation(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      static_assert(Sort<I1, R>(), "");
      static_assert(Sort<I2, R>(), "");
      static_assert(Comparison<I1, I2, R>(), "");
      // static_assert(is_strict_weak_ordering(comp));
      
      o_sort(first1, last1);
      o_sort(first2, last2);
      return o_equal(first1, last1, first2, symmetric_complement<R>{comp});
    }



  // Is permutation
  // Returns true if the range [first1, last1) is a permutation of the range
  // [first2, first2 + (last1 - first1)). The complexity of this algorithm
  // is O(nlog n) if sufficient memory is available, otherwise O(n^2).
  template<typename I1, typename I2>
    bool o_is_permutation(I1 first1, I1 last1, I2 first2)
    {
      // FIXME: Is there a concept for Semiregular + Totally ordered?
      static_assert(Copyable<Value_type<I1>>(), "");
      static_assert(Copyable<Value_type<I2>>(), "");
      static_assert(Totally_ordered<Value_type<I1>>(), "");
      static_assert(Totally_ordered<Value_type<I2>>(), "");
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, distance(first1, last1)));

      auto n = o_distance(first1, last1);
      temporary_buffer<Value_type<I1>> b1(first1, n);
      temporary_buffer<Value_type<I2>> b2(first2, n);
      if(b1 && b2) {
        return is_sorted_permutation(b1.begin(), b1.end(), b2.begin(), b2.end());
      } else {
        I2 last2 = o_next(first2, n);
        return equal_elements(first1, last1, first2, last2);
      }
    }

    
    
  // Is permutation (relation)
  // Returns true if the range [first1, last1) is a permutation of the range
  // [first2, first2 + (last1 - first1)). The complexity of this algorithm
  // is O(nlog n) if sufficient memory is available, otherwise O(n^2).
  //
  // Note that R is a relaiton on the value types of I1 and I2 and the common
  // type of those types. If the value types are different, then R will most 
  // likely need to be polymorphic in order to avoid conversions.
  template<typename I1, typename I2, typename R>
    bool o_is_permutation(I1 first1, I1 last1, I2 first2, R comp)
    {
      static_assert(Copyable<Value_type<I1>>(), "");
      static_assert(Copyable<Value_type<I2>>(), "");
      static_assert(Relation<R, Value_type<I1>>(), "");
      static_assert(Relation<R, Value_type<I2>>(), "");
      static_assert(Comparison<I1, I2, R>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, distance(first1, last1)));

      auto n = o_distance(first1, last1);
      temporary_buffer<Value_type<I1>> b1(first1, n);
      temporary_buffer<Value_type<I2>> b2(first2, n);      
      if(b1 && b2) {
        return is_sorted_permutation(b1.begin(), b1.end(), b2.begin(), b2.end(), comp);
      } else {
        I2 last2 = o_next(first2, n);
        return equal_elements(first1, last1, first2, last2, symmetric_complement<R>{comp});
      }
    }

  

  // Is permutation (range)
  // Returns true if a is a permutation of b.
  //
  // Note that the minimum requirement for is_permutation is Comparison,
  // but the the optiized version requires Sort, with the weaker 
  // requirements acting as a fallback.
  template<typename R1, typename R2>
    inline bool is_permutation(const R1& a, const R2& b)
    {
      static_assert(Range_sort<R1>(), "");
      static_assert(Range_sort<R2>(), "");
      static_assert(Range_comparison<R1, R2>(), "");
      assert(size(a) == size(b));

      return o_is_permutation(std::begin(a), std::end(a), std::begin(b));
    }

  

  // Is permutaiton (range, relation)
  // Returns true if a is a permutation of b.
  //
  // Note that R is a relaiton on the value types of I1 and I2 and the common
  // type of those types. If the value types are different, then R will most 
  // likely need to be polymorphic in order to avoid conversions.
  template<typename R1, typename R2, typename Rel>
    inline bool is_permutation(const R1& a, const R2& b, Rel comp)
    {
      static_assert(Range_sort<R1, Rel>(), "");
      static_assert(Range_sort<R2, Rel>(), "");
      static_assert(Range_comparison<R1, R2, Rel>(), "");
      assert(size(a) == size(b));
      assert(strict_weak_ordering(comp));

      return o_is_permutation(std::begin(a), std::end(a), std::begin(b), comp);
    }



  // Next permutation
  // Compute the next lexicographical permutation of [first, last).
  template<typename I>
    inline bool o_next_permutation(I first, I last)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Sort<I>(), "");
      assert(is_permutable_range(first, last));

      return std::next_permutation(first, last);
    }
    
    
    
  // Next permutation (range)
  // Compute the next lexicographical permutaiotn of range.
  template<typename R>
    inline bool next_permutation(R& range)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R>(), "");

      return o_next_permutation(std::begin(range), std::end(range));
    }



  // Next permutation (relation)
  // Compute the next lexicographical permutation of [first, last).
  template<typename I, typename R>
    inline bool o_next_permutation(I first, I last, R comp)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Relation<Value_type<I>>(), "");
      assert(is_permutable_range(first, last));
      assert(strict_weak_ordering(comp));
      
      return _next_permutation(first, last);
    }



  // Next permutation (range, relation)
  // Compute the next lexicographical permutation of range.
  template<typename R, typename Rel>
    inline bool next_permutation(R& range, Rel comp)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R, Rel>(), "");
    
      return std::next_permutation(std::begin(range), std::end(range), comp);
    }



  // Previous permutation
  // Compute the previous lexicographical permutation of [first, last).
  template<typename I>
    inline bool o_prev_permutation(I first, I last)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Sort<I>(), "");
      assert(( is_permutable_range(first, last) ));

      return std::prev_permutation(first, last);
    }
    
    
    
  // Previous permutation (range)
  // Compute the previous lexicographical permutation of range.
  template<typename R>
    inline bool prev_permutation(R& range)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R>(), "");

      return std::prev_permutation(std::begin(range), std::end(range));
    }



  // Previous permutation (relation)
  // Compute the previous lexicographical permutation of [first, last).
  template<typename I, typename R>
    inline bool o_prev_permutation(I first, I last, R comp)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      static_assert(Relation<Value_type<I>>(), "");
      assert(( is_permutable_range(first, last) ));
      assert(( strict_weak_ordering(comp) ));
      
      return std::prev_permutation(first, last);

    }



  // Previous permutation (range, relation)
  // Compute the previous lexicographical permutation of range.
  template<typename R, typename Rel>
    inline bool prev_permutation(R& range, Rel comp)
    {
      static_assert(Range_sort<R, Rel>(), "");
      static_assert(Bidirectional_range<R>(), "");

      return std::prev_permutation(std::begin(range), std::end(range), comp);
    }



  // NOTE: I'm not wild about the name _partial_*. These are often called
  // k-permutations (and similarly k-combination). In general, I would have
  // have preferred next_permutation to increment to k-permutations instead
  // of entire sequences in order to make it look like the combination
  // algorithms below.
  //
  // The definitions of [next|prev]_partial_permutation and the the 
  // [next|prev]_combination algorithms is in algorithm/boost_combination.hpp. 
  // The algorithms because they have a different license.



  // Next partial permutation
  // Compute the next lexicographical permutation of elements in [first, mid) 
  // from r where first is begin(r). Return false if there is no next 
  // permutation.
  template<typename R>
    inline bool next_partial_permutation(R& range, Iterator_type<R> mid)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R>(), "");

      return next_partial_permutation(std::begin(range), mid, std::end(range));
    }



  // Next partial permutation (relation)
  template<typename R, typename Rel>
    inline bool next_partial_permutation(R& range, Iterator_type<R> mid, Rel comp)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R, Rel>(), "");

      return next_partial_permutation(std::begin(range), mid, std::end(range), comp);
    }


  // Previous partial permutation
  // Compute the next lexicographical permutation of elements in [first, mid) 
  // from r where first is begin(r). Return false if there is no next 
  // permutation.
  template<typename R>
    inline bool prev_partial_permutation(R& range, Iterator_type<R> mid)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R>(), "");

      return prev_partial_permutation(std::begin(range), mid, std::end(range));
    }



  // Previous partial permutaiton (relation)
  template<typename R, typename Rel>
    inline bool prev_partial_permutation(R& range, Iterator_type<R> mid, Rel comp)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R, Rel>(), "");

      return prev_partial_permutation(std::begin(range), mid, std::end(range), comp);
    }


  // Next combination
  // Compute the next lexicographical combination of elements in [first, mid)
  // from the range r where first is equal to begin(r). Return false if there
  // is no next combination.
  template<typename R>
    inline bool next_combination(R& range, Iterator_type<R> mid)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R>(), "");

      return next_combination(std::begin(range), mid, std::end(range));
    }



  // Next combination (relation)
  template<typename R, typename Rel>
    inline bool next_combination(R& range, Iterator_type<R> mid, Rel comp)
    {
      static_assert(Bidirectional_range<R>(), "");
      static_assert(Range_sort<R, Rel>(), "");

      return next_combination(std::begin(range), mid, std::end(range), comp);
    }


  
  // Previous combination
  // Compute the previous lexicographical combination of elements in 
  // [first, mid) from the range r where first is equal to begin(r). Return 
  // false if there is no next combination.
  template<typename R>
    inline bool prev_combination(R& range, Iterator_type<R> mid)
    {
      return prev_combination(std::begin(range), mid, std::end(range));
    }



  // Previous combination (relation)
  template<typename R, typename Rel>
    inline bool prev_combination(R& range, Iterator_type<R> mid, Rel comp)
    {
      return prev_combination(std::begin(range), mid, std::end(range), comp);
    }



  // TODO: Implement multiset permutations.
  
} // namespace origin

#endif
