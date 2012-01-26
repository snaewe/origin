// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_MINMAX_HPP
#define ORIGIN_ALGORITHM_MINMAX_HPP

#include <algorithm>

namespace origin
{
  template<typename I, typename R>
    I o_min_element(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_strict_weak_ordering(comp));

      if(first != last) {
        for(I i = o_next(first); i != last; ++i) {
          if(comp(*i, *first))
            first = i;
        }
      }
      return first;
    }
  
  
  // Min element (less)
  template<typename I>
    I o_min_element(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      
      return min_element(first, last, strict_less{});
    }
    
    

  // Min (relation, const)
  template<typename T, typename R>
    const T& o_min(const T& a, const T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      return comp(b, a) ? b : a;
    }
    

  
  // Min (relation, non-const)
  template<typename T, typename R>
    T& min(T& a, T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      return comp(b, a) ? b : a;
    }
  
  
  
  // Min (less, const)
  template<typename T>
    const T& o_min(const T& a, const T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      return b < a ? b : a;
    }
    
  
  // Min (less, non-const)
  template<typename T>
    T& min(T& a, T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      return b < a ? b : a;
    }
      

      
  // Min (initializer list, relation)
  template<typename T, typename R>
    const T& o_min(std::initializer_list<T> list, R comp)
    {
      static_assert(Relation<R, T>(), "");
      assert(is_strict_weak_ordering(comp));
      assert(list.size() != 0);
      
      return *o_min_element(list.begin(), list.end(), comp);
    }
    
      
  // Min (initializer list, less)
  template<typename T>
    const T& o_min(std::initializer_list<T> list)
    {
      static_assert(Totally_ordered<T>(), "");
      assert(list.size() != 0);

      return *o_min_element(list.begin(), list.end());
    }

  
  
  // Max element (relation)
  template<typename I, typename R>
    I o_max_element(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_strict_weak_ordering(comp));

      if(first != last) {
        for(I i = o_next(first); i != last; ++i) {
          if(comp(*first, *i))
            first = i;
        }
      }
      return first;
    }
  
  
  
  // Max element (less)
  template<typename I>
    I o_max_element(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      
      return max_element(first, last, strict_less{});
    }

  
  
  // Max (relation, const)
  template<typename T, typename R>
    const T& o_max(const T& a, const T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      return comp(a, b) ? b : a;
    }
    

  
  // Max (relation, non-const)
  template<typename T, typename R>
    T& max(T& a, T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      return comp(a, b) ? b : a;
    }
  
  
  
  // Max (less, const)
  template<typename T>
    const T& o_max(const T& a, const T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      return a < b ? b : a;
    }
    
  
  // Max (less, non-const)
  template<typename T>
    T& max(T& a, T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      return a < b ? b : a;
    }
      

      
  // Max (initializer list, relation)
  template<typename T, typename R>
    const T& o_max(std::initializer_list<T> list, R comp)
    {
      static_assert(Relation<R, T>(), "");
      assert(is_strict_weak_ordering(comp));
      assert(list.size() != 0);
      
      return *o_max_element(list.begin(), list.end(), comp);
    }
    
      
  // Max (initializer list, less)
  template<typename T>
    const T& o_max(std::initializer_list<T> list)
    {
      static_assert(Totally_ordered<T>(), "");
      assert(list.size() != 0);

      return *o_max_element(list.begin(), list.end());
    }
  
} // namespace origin

#endif
