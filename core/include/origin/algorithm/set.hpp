// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_SET_HPP
#define ORIGIN_ALGORITHM_SET_HPP

namespace origin
{
  // Set algorithms
  //
  //    includes(first1, first2, last1, last2)
  //    includes(first1, first2, last1, last2, comp)
  //    includes(range1, range2)
  //    includes(range1, range2, comp)
  //
  //    set_union(first1, last1, first2, last2, result)
  //    set_union(first1, last1, first2, last2, result, comp)
  //    set_union(range1, range2, result)
  //    set_union(range1, range2, result, comp)
  //    set_intersection(first1, last1, first2, last2, result)
  //    set_intersection(first1, last1, first2, last2, result, comp)
  //    set_intersection(range1, range2, result)
  //    set_intersection(range1, range2, result, comp)
  //    set_difference(first1, last1, first2, last2, result)
  //    set_difference(first1, last1, first2, last2, result, comp)
  //    set_difference(range1, range2, result)
  //    set_difference(range1, range2, result, comp)
  //    set_symmetric_difference(first1, last1, first2, last2, result)
  //    set_symmetric_difference(first1, last1, first2, last2, result, comp)
  //    set_symmetric_difference(range1, range2, result)
  //    set_symmetric_difference(range1, range2, result, comp)


  // Includes (iterator, relation)
  // Returns true if every element in [first2, last2) is contained in 
  // [first1, last1).
  template<typename I1, typename I2, typename R>
    bool o_includes(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      static_assert(Comparison<I1, I2, R>(), "");

      while(first1 != last1 && first2 != last2) {
        if(comp(*first2, *first1))
          return false;
        else if(comp(*first1, first2))
          ++first1;
        else {
          ++first1;
          ++first2;
        }
      }
      return first2 == last2;
    }
  
  
  
  // Includes (iterator, less)
  // Returns true if every element in [first2, last2) is contained in 
  // [first1, last1).
  template<typename I1, typename I2>
    inline bool o_includes(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Lexicographical_comparison<I1, I2>(), "");

      return o_includes(first1, last1, first2, last2, lt{});
    }
    
    
  
  // Includes (range, relation)
  // Returns true if every element in range2 is contained in range1.
  template<typename R1, typename R2, typename Rel>
    inline bool includes(const R1& range1, const R2& range2, Rel comp)
    {
      return o_includes(o_begin(range1), o_end(range1), 
                        o_begin(range2), o_end(range2), comp);
    }
    

    
  // Includes (range, less)
  // Returns true if every element in range2 is contained in range1.
  template<typename R1, typename R2>
    inline bool includes(const R1& range1, const R2& range2)
    {
      return o_includes(o_begin(range1), o_end(range1), 
                        o_begin(range2), o_end(range2), lt{});
    }
    
    
  
  // Set union (iterator, relation)
  // Copy the union of the elements in [first1, last1) and [first2, last2)
  // into the output range [result, n) where n is the multiset cardinality of
  // the union of the two input ranges. 
  //
  // Note that n is at most distance(first1, last1) + distance(first2, last2),
  // but only when the two sets are disjoint.
  template<typename I1, typename I2, typename O, typename R>
    O o_set_union(I1 first1, I1 last1, I2 first2, I2 last2, O result, R comp)
    {
      static_assert(Merge<I1, I2, O, R>(), "");

      while(first1 != last1 && first2 != last2) {
        if(comp(*first1, *first2))
          copy_step(first1, result);
        else if(comp(*first2, *first1))
          copy_step(first2, result);
        else {
          *result = *first1;
          ++first1;
          ++first2;
        }
      }
      return result;
    }



  // Set union (iterator, less)
  // Copy the union of the elements in [first1, last1) and [first2, last2)
  // into the output range [result, n) where n is the multiset cardinality of
  // the union of the two input ranges. 
  template<typename I1, typename I2, typename O>
    inline O o_set_union(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");
      return o_set_union(first1, last1, first2, last2, result, lt{});
    }

    
    
  // Set union (range, relation)
  // Copy the union of the elements in range1 and range2 into the output
  // range [result, n) where n is the multiset cardinality of the two input
  // ranges.
  template<typename R1, typename R2, typename O, typename Rel>
    inline void set_union(const R1& range1, const R2 range2, O result, Rel comp)
    {
      static_assert(Range_merge<R1, R2, O, Rel>(), "");
      return o_set_union(o_begin(range1), o_end(range1),
                         o_begin(range2), o_end(range2), 
                         result, comp);
    }
    
    
    
  // Set union (range, less)
  // Copy the union of the elements in range1 and range2 into the output
  // range [result, n) where n is the multiset cardinality of the two input
  // ranges.
  template<typename R1, typename R2, typename O>
    inline void set_union(const R1& range1, const R2 range2, O result)
    {
      static_assert(Range_merge<R1, R2, O>(), "");
      return o_set_union(o_begin(range1), o_end(range1),
                         o_begin(range2), o_end(range2), result);
    }

    

  // Set intersection (iterator, relation)
  // Copy the intersection of the elements in [first1, last1) and 
  // [first2, last2) into the output range [result, n) where n is the multiset 
  // cardinality of the intersection of the two input ranges. 
  //
  // Note that n is 0 if the input ranges are disjoint and at most
  // max(distance(first1, last1), distance(first2, last2)).
  template<typename I1, typename I2, typename O, typename R>
    O o_set_intersection(I1 first1, I1 last1, I2 first2, I2 last2, O result, R comp)
    {
      while(first1 != last1 && first2 != last2) {
        if(comp(*first1, *first2))
          ++first1;
        else if(comp(*first2, *first1))
          ++first2;
        else {
          *result = *first1;
          ++result;
          ++first1;
          ++first2;
        }
      }
      return result;
    }

  
  
  // Set intersection (iterator, less)
  // Copy the intersection of the elements in [first1, last1) and 
  // [first2, last2) into the output range [result, n) where n is the multiset 
  // cardinality of the intersection of the two input ranges. 
  template<typename I1, typename I2, typename O>
    inline O o_set_intersection(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");
      return o_set_intersection(first1, last1, first2, last2, result, lt{});
    }
    
    
  
  // Set intersection (range, relation)
  // Copy then intersection of the elements in range1 and range2 into the 
  // result range.
  template<typename R1, typename R2, typename O, typename Rel>
    inline void set_intersection(const R1 range1, const R2 range2, O result, Rel comp)
    {
      static_assert(Range_merge<R1, R2, O, Rel>(), "");
      return o_set_intersection(o_begin(range1), o_end(range1), 
                                o_begin(range2), o_end(range2),
                                result, comp);
    }

    
    
  // Set intersection (range, less)
  // Copy then intersection of the elements in range1 and range2 into the 
  // output range [result, n) where n is the multiset cardinality of the
  // intersection of the input ranges.
  template<typename R1, typename R2, typename O>
    inline void set_intersection(const R1 range1, const R2 range2, O&& result)
    {
      static_assert(Range_merge<R1, R2, O>(), "");
      return o_set_intersection(o_begin(range1), o_end(range1),
                                o_begin(range2), o_end(range2), result);
    }

    
    
  // Set difference (iterator, relation)
  // Copy the elements in [first1, last1) but not in [first2, last2) into the 
  // result range [result, n) where n is the multiset cardinality of the
  // [first1, last1) without the elements of [first2, last2).
  //
  // Note that n is 0 if [first2, last2) is equal to [first1, last1) and at 
  // most distance(first1, last1).
  template<typename I1, typename I2, typename O, typename R>
    O o_set_difference(I1 first1, I1 last1, I2 first2, I2 last2, O result, R comp)
    {
      static_assert(Merge<I1, I2, O, R>(), "");
      while(first1 != last1 && first2 != last2) {
        if(comp(*first1, *first2))
          copy_step(first1, result);
        else if(comp(*first2, *first1))
          ++first2;
        else {
          ++first1;
          ++first2;
        }
      }
      return result;
    }

    
    
  // Set difference (iterator, less)
  // Copy the elements in [first1, last1) but not in [first2, last2) into the 
  // result range [result, n) where n is the multiset cardinality of the
  // [first1, last1) without the elements of [first2, last2).
  template<typename I1, typename I2, typename O>
    inline O o_set_difference(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");
      return o_set_difference(first1, last1, first2, last2, result, lt{});
    }

    

  // Set difference (range, relation)
  // Copy the elements in range1 but not in range2 into the output range
  // [result, n) where n is the multiset cardinality of range1 without the
  // elements of range2.
  template<typename R1, typename R2, typename O, typename Rel>
    inline void set_difference(const R1& range1, const R2& range2, O result, Rel comp)
    {
      static_assert(Range_merge<R1, R2, O, Rel>(), "");
      return o_set_difference(o_begin(range1), o_end(range1),
                              o_begin(range2), o_end(range2),
                              result, comp);
    }
    
    

  // Set difference (range, less)
  // Copy the elements in range1 but not in range2 into the output range
  // [result, n) where n is the multiset cardinality of range1 without the
  // elements of range2.
  template<typename R1, typename R2, typename O>
    inline void set_difference(const R1& range1, const R2& range2, O result)
    {
      static_assert(Range_merge<R1, R2, O>(), "");
      return o_set_difference(o_begin(range1), o_end(range1),
                              o_begin(range2), o_end(range2), result);
    }

    

  // Set symmetric difference (iterator, relation)
  // Copy the elements that are not in both [first1, last1) and [first2, last2)
  // into the output range [first, n) where n is the multiset cardinality of
  // set containing all elements not in both input ranges.
  //
  // Note that n is 0 if the input ranges are disjoint and at most
  // max(distance(first1, last1), distance(first2, last2)) if the sets compare
  // equal.
  template<typename I1, typename I2, typename O, typename R>
    O o_set_symmetric_difference(I1 first1, I1 last1, I2 first2, I2 last2, O result, R comp)
    {
      while(first1 != last1 && first2 != last2) {
        if(comp(*first1, *first2))
          copy_step(first1, result);
        else if(comp(*first2, *first1))
          copy_step(first2, result);
        else {
          ++first1;
          ++first2;
        }
      }
      return result;
    }

    
    
  // Set symmetric difference (iterator, relation)
  // Copy the elements that are not in both [first1, last1) and [first2, last2)
  // into the output range [first, n) where n is the multiset cardinality of
  // the set containing all elements not in both input ranges.
  template<typename I1, typename I2, typename O>
    O o_set_symmetric_difference(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");

      return o_set_symmetric_difference(first1, last1, first2, last2, result, lt{});
    }

    

  // Set symmetric difference (range, relation)
  // copy the elements that are not in both range1 and range2 into the output
  // range [result, n) where n is the multiset cardinality of the set containing
  // all elements not in both input ranges.
  template<typename R1, typename R2, typename O, typename Rel>
    O set_symmetric_difference(const R1& range1, const R2& range2, O result, Rel comp)
    {
      static_assert(Merge<R1, R2, O, Rel>(), "");
      return o_set_symmetric_difference(o_begin(range1), o_end(range1),
                                        o_begin(range2), o_end(range2),
                                        result, comp);
    }
  
  
  
  // Set symmetric difference (range, less)
  // copy the elements that are not in both range1 and range2 into the output
  // range [result, n) where n is the multiset cardinality of the set containing
  // all elements not in both input ranges.
  template<typename R1, typename R2, typename O>
    O set_symmetric_difference(const R1& range1, const R2& range2, O result)
    {
      static_assert(Merge<R1, R2, O>(), "");
      return o_set_symmetric_difference(o_begin(range1), o_end(range1),
                                        o_begin(range2), o_end(range2), result);
    }


} // namespace origin

#endif
