// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_MERGE_HPP
#define ORIGIN_ALGORITHM_MERGE_HPP

namespace origin
{
  // FIXME: Finish and document this module (it's small).
  
  // Merge (relation)
  template<typename I1, typename I2, typename O, typename R>
    O o_merge(I1 first1, I1 last1, I2 first2, I2 last2, O result, R comp)
    {
      static_assert(Merge<I1, I2, O, R>(), "");

      while(first1 != last1 && first2 != last2) {
        if(comp(*first2, *first1))
          copy_step(first2, result);
        else
          copy_step(first1, result);
      }
      return o_copy(first2, last2, o_copy(first1, last1, result));
    }


    
  // Merge (less)
  template<typename I1, typename I2, typename O>
    O o_merge(I1 first1, I1 last1, I2 first2, I2 last2, O result)
    {
      static_assert(Merge<I1, I2, O>(), "");
      
      return o_merge(first1, last1, first2, last2, result, lt());
    }
    
    
    
  // Merge (range, relation)
  template<typename R1, typename R2, typename O, typename Rel>
    void merge(const R1& range1, const R2& range2, O&& result, Rel comp)
    {
      return o_merge(o_begin(range1), o_end(range1),
                     o_begin(range2), o_end(range2),
                     o_begin(result), comp);
    }

    
    
  // Merge (range, less)
  template<typename R1, typename R2, typename O>
    void merge(const R1& range1, const R2& range2, O&& result)
    {
      return o_merge(o_begin(range1), o_end(range1),
                     o_begin(range2), o_end(range2),
                     o_begin(result));
    }

    
  
  // Inplace merge (relation)
  template<typename I, typename R>
    void o_inplace_merge(I first, I middle, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");

      std::inplace_merge(first, middle, last, comp);
    }

    
    
  // Inplace merge (less)
  template<typename I>
    void o_inplace_merge(I first, I middle, I last)
    {
      static_assert(Sort<I>(), "");
      
      std::inplace_merge(first, middle, last);
    }
  
} // namespace origin

#endif
