// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_MINMAX_HPP
#define ORIGIN_ALGORITHM_MINMAX_HPP

namespace origin
{
  // Minimum and maximum
  // There are a number of minimum and maximum algorithms:
  //
  //    min(a, b)
  //    min(a, b, comp)
  //    min(list)
  //    min(list, comp)
  //    min_element(first, last)
  //    min_element(first, last, comp)
  //    min_element(range)
  //    min_element(range, comp)
  //
  //    max(a, b)
  //    max(a, b, comp)
  //    max(list)
  //    max(list, comp)
  //    max_element(first, last)
  //    max_element(first, last, comp)
  //    max_element(range)
  //    max_element(range, comp)
  //
  //    minmax(a, b)
  //    minmax(a, b, comp)
  //    minmax(list)
  //    minmax(list, comp)
  //    minmax_element(first, last)
  //    minmax_element(first, last, comp)
  //    minmax_element(range)
  //    minmax_element(range, comp)
  //
  // TODO: In reality, is not just related to min/max, but order selection in 
  // general. This should include the algorithms from Elements of Programming.
  
  // Min elemenet (relation)
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
      
      return o_min_element(first, last, strict_less{});
    }
    
    
    
  // Min element (range, relation)
  template<typename R, typename Rel>
    auto min_element(R&& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_min_element(o_begin(range), o_end(range), comp);
    }

    
    
  // Min element (range, less)
  template<typename R>
    auto min_element(R&& range) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_min_element(o_begin(range), o_end(range));
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
      
      return o_max_element(first, last, strict_less{});
    }

  
  
  // Max element (range, relation)
  template<typename R, typename Rel>
    auto max_element(R&& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_max_element(o_begin(range), o_end(range), comp);
    }

    
    
  // Max element (range, less)
  template<typename R>
    auto max_element(R&& range) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_max_element(o_begin(range), o_end(range));
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
  

  
  // Minmax element (relation)
  //
  // FIXME: Implemnt this algorithm.
  template<typename I, typename R>
    std::pair<I, I> o_minmax_element(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_strict_weak_ordering(comp));

      return std::minmax_element(first, last, comp);
    }
  
  
  
  // Minmax element (less)
  template<typename I>
    I o_minmax_element(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      
      return o_minmax_element(first, last, strict_less{});
    }

  
  
  // Minmax (relation, const)
  template<typename T, typename R>
    std::pair<const T&, const T&> o_minmax(const T& a, const T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      
      using P = std::pair<const T&, const T&>;
      return comp(b, a) ? P{b, a} : P{a, b};
    }
    

  
  // Minmax element (range, relation)
  template<typename R, typename Rel>
    auto minmax_element(R&& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_minmax_element(o_begin(range), o_end(range), comp);
    }

    
    
  // Minmax element (range, less)
  template<typename R>
    auto minmax_element(R&& range) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_minmax_element(o_begin(range), o_end(range));
    }

    
    
  // Minmax (relation, non-const)
  template<typename T, typename R>
    std::pair<T&, T&> minmax(T& a, T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      
      using P = std::pair<T&, T&>;
      return comp(b, a) ? P{b, a} : P{a, b};
    }
  
  
  
  // Minmax (less, const)
  template<typename T>
    std::pair<const T&, const T&> o_minmax(const T& a, const T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      
      using P = std::pair<const T&, const T&>;
      return b < a ? P{b, a} : P{a, b};
    }
    
  
  // Minmax (less, non-const)
  template<typename T>
    std::pair<T&, T&> minmax(T& a, T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      
      using P = std::pair<T&, T&>;
      return b > a ? P{b, a} : P{a, b};
    }
      

      
  // Minmax (initializer list, relation)
  template<typename T, typename R>
    std::pair<const T&, const T&> o_minmax(std::initializer_list<T> list, R comp)
    {
      static_assert(Relation<R, T>(), "");
      assert(is_strict_weak_ordering(comp));
      assert(list.size() != 0);
      
      return o_minmax_element(list.begin(), list.end(), comp);
    }
    
      
  // Minmax (initializer list, less)
  template<typename T>
    std::pair<const T&, const T&> o_minmax(std::initializer_list<T> list)
    {
      static_assert(Totally_ordered<T>(), "");
      assert(list.size() != 0);

      return *o_minmax_element(list.begin(), list.end());
    }
  
  
  
} // namespace origin

#endif
