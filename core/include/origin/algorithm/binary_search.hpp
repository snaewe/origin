// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_BINARY_SEARCH_HPP
#define ORIGIN_ALGORITHM_BINARY_SEARCH_HPP

#include <algorithm>

#include <origin/memory.hpp>
#include <origin/functional.hpp>
#include <origin/iterator.hpp>
#include <origin/range.hpp>

namespace origin
{
  // Binary search
  // This family of algorithm...
  //
  // TODO: Finish documenting.

  
  
  // Encapsulates the predicate used to evaluate the lower bound condition
  // in binary search algorithms.
  template<typename R, typename T>
    struct lower_bound_predicate
    {
      lower_bound_predicate(R comp, const T& value) 
        : comp{comp}, value{value}
      { }
      
      template<typename U>
        bool operator()(const U& x) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !comp(x, value);
        }
        
        R comp;
        const T& value;
    };

    
    
  // Encapsulates the predicate used to evaluate the upper bound condition
  // in binary search algorithms.
  template<typename R, typename T>
    struct upper_bound_predicate
    {
      upper_bound_predicate(R comp, const T& value) 
        : comp{comp}, value{value}
      { }
      
      template<typename U>
        bool operator()(const U& x) const
        {
          static_assert(Relation<R, T, U>(), "");
          return comp(value, x);
        }
        
        R comp;
        const T& value;
    };
  
  
    
  // Lower bound (relation)
  template<typename I, typename T, typename R>
    inline I o_lower_bound(I first, I last, const T& value, R comp)
    {
      static_assert(Binary_search<I, T, R>(), "");
      assert(is_readable_range(first, last));
      assert(strict_seak_ordering(comp));

      lower_bound_predicate<R, T> pred{comp, value};
      return o_partition_point(first, last, pred);
    }

    
    
  // Lower bound (less)
  template<typename I, typename T>
    inline I o_lower_bound(I first, I last, const T& value)
    {
      static_assert(Binary_search<I, T>(), "");
      assert(is_readable_range(first, last));

      return o_lower_bound(first, last, value, strict_less{});
    }


    
  // Lower bound (range, relation)
  template<typename R, typename T, typename Rel>
    inline auto lower_bound(R&& range, const T& value, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_binary_search<R, T, Rel>(), "");

      return o_lower_bound(o_begin(range), o_end(range), value, comp);
    }
  
  
  
  // Lower bound (range, less)
  template<typename R, typename T>
    inline auto lower_bound(R&& range, const T& value) -> decltype(o_begin(range))
    {
      static_assert(Range_binary_search<R, T>(), "");

      return o_lower_bound(o_begin(range), o_end(range), value);
    }
    
    

  // Upper bound (relation)
  template<typename I, typename T, typename R>
    inline I o_upper_bound(I first, I last, const T& value, R comp)
    {
      static_assert(Binary_search<I, T, R>(), "");
      assert(is_readable_range(first, last));
      assert(strict_seak_ordering(comp));

      upper_bound_predicate<R, T> pred{comp, value};
      return o_partition_point(first, last, pred);
    }



  // Upper bound (less)
  template<typename I, typename T>
    inline I o_upper_bound(I first, I last, const T& value)
    {
      static_assert(Binary_search<I, T>(), "");
      assert(is_readable_range(first, last));

      return o_upper_bound(first, last, value, strict_less{});
    }

    
    
  // Upper bound (range, relation)
  template<typename R, typename T, typename Rel>
    inline auto upper_bound(R&& range, const T& value, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_binary_search<R, T, Rel>(), "");

      return o_upper_bound(o_begin(range), o_end(range), value, comp);
    }
  
  
  
  // Upper bound (range, less)
  template<typename R, typename T>
    inline auto upper_bound(R&& range, const T& value) -> decltype(o_begin(range))
    {
      static_assert(Range_binary_search<R, T>(), "");

      return o_upper_bound(o_begin(range), o_end(range), value);
    }
    
    

  // Equal range (relation)
  template<typename I, typename T, typename R>
    std::pair<I, I> o_equal_range(I first, I last, const T& value, R comp)
    {
      static_assert(Binary_search<I, T, R>(), "");
      assert(is_readable_range(first, last));
      assert(strict_seak_ordering(comp));

      return std::equal_range(first, last, value, comp);
    }
    
  
  
  // Equal range (less)
  template<typename I, typename T>
    std::pair<I, I> o_equal_range(I first, I last, const T& value)
    {
      static_assert(Binary_search<I, T>(), "");
      assert(is_readable_range(first, last));

      return o_equal_range(first, last, value, strict_less{});
    }
    
  

  // Lower bound (range, relation)
  template<typename R, typename T, typename Rel>
    inline auto equal_range(R&& range, const T& value, Rel comp) 
      -> decltype(equal_range(range, value, comp))
    {
      static_assert(Range_binary_search<R, T, Rel>(), "");
      assert(is_strict_weak_ordering(comp));

      return o_equal_range(range, value, comp);
    }
  
  
  
  // Lower bound (range, less)
  template<typename R, typename T>
    inline auto equal_range(R&& range, const T& value) 
      -> decltype(equal_range(range, value, strict_less{}))
    {
      static_assert(Range_binary_search<R, T>(), "");

      return o_equal_range(range, value, strict_less{});
    }
    
    

  // Binary search (relation)
  // Returns true if value (or an equivalent value) can be found in the range
  // [first, last) using the strict weak ordering comp to compare values.
  template<typename I, typename T, typename R>
    inline I o_binary_search(I first, I last, const T& value, R comp)
    {
      static_assert(Binary_search<I, T, R>(), "");
      assert(is_readable_range(first, last));
      assert(strict_seak_ordering(comp));

      I i = o_lower_bound(first, last, value, comp);
      return i != last && !comp(value, *i);
    }



  // Binary search (less)
  // Returns true if value can be found in [first, last).
  template<typename I, typename T>
    inline I o_binary_search(I first, I last, const T& value)
    {
      static_assert(Binary_search<I, T>(), "");
      assert(is_readable_range(first, last));

      I i = o_lower_bound(first, last, value);
      return i != last && *i == value;
    }
    
    
    
  // Binary search (range, relation)
  template<typename R, typename T, typename Rel>
    inline bool binary_search(const R& range, const T& value, Rel comp)
    {
      static_assert(Range_binary_search<R, T, Rel>(), "");
      assert(is_strict_weak_ordering(comp));

      return o_binary_search(o_begin(range), o_end(range), value, comp);
    }
    
    

  // Binary search (range, less)
  template<typename R, typename T>
    inline bool binary_search(const R& range, const T& value)
    {
      static_assert(Range_binary_search<R, T>(), "");

      return o_binary_search(o_begin(range), o_end(range), value);
    }

} // namespace origin

#endif
