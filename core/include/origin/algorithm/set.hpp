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
  //    set_union(first1, last1, first2, last2)
  //    set_union(first1, last1, first2, last2, comp)
  //    set_intersection(first1, last1, first2, last2)
  //    set_intersection(first1, last1, first2, last2, comp)
  //    set_difference(first1, last1, first2, last2)
  //    set_difference(first1, last1, first2, last2, comp)
  //    set_symmetric_difference(first1, last1, first2, last2)
  //    set_symmetric_difference(first1, last1, first2, last2, comp)
  //
  // TODO: Write the range algorithms and the corresponding concepts.

  
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
  
  
  
  template<typename I1, typename I2>
    bool o_includes(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Lexicographical_comparison<I1, I2>(), "");

      return o_includes(first1, last1, first2, last2, strict_less{});
    }
    
    
  
  template<typename R1, typename R2, typename Rel>
    bool includes(const R1& range1, const R2& range2, Rel comp)
    {
      return o_includes(o_begin(range1), o_end(range1), 
                        o_begin(range2), o_end(range2), comp);
    }
    

    
  template<typename R1, typename R2>
    bool includes(const R1& range1, const R2& range2)
    {
      return o_includes(o_begin(range1), o_end(range1), 
                        o_begin(range2), o_end(range2));
    }
    
    
  
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

  
  template<typename I1, typename I2, typename O>
    O o_set_union(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");

      return o_set_union(first1, last1, first2, last2, result);
    }

    
    
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

  
  
  template<typename I1, typename I2, typename O>
    O o_set_intersection(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");

      return o_set_intersection(first1, last1, first2, last2, result);
    }

    
    
  template<typename I1, typename I2, typename O, typename R>
    O o_set_difference(I1 first1, I1 last1, I2 first2, I2 last2, O result, R comp)
    {
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

    
    
  template<typename I1, typename I2, typename O>
    O o_set_difference(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");

      return o_set_difference(first1, last1, first2, last2, result);
    }

    
    
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

    
    
  template<typename I1, typename I2, typename O>
    O o_set_symmetric_difference(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");

      return o_set_symmetric_difference(first1, last1, first2, last2, result);
    }

} // namespace origin

#endif
