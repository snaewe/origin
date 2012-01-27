// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_HEAP_HPP
#define ORIGIN_ALGORITHM_HEAP_HPP

namespace origin
{
  // FIXME: Finish implementing me. I really like libc++'s approach to
  // push and pop heap (push_front/pop_back). It would be worthwhile to
  // duplicate that work.
  
  template<typename I, typename R>
    bool o_is_heap(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_reange(first, last));

      return std::is_heap(first, last, comp);
    }
    
  template<typename I>
    bool o_is_heap(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      
      return std::is_heap(first, last);
    }

    
  
  
  template<typename I, typename R>
    I o_is_heap_until(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      return std::is_heap(first, last, comp);
    }
    
  template<typename I>
    I o_is_heap_until(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      return std::is_heap(first, last);
    }

    
    
  template<typename I, typename R>
    void o_push_heap(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      std::push_heap(first, last, comp);
    }
    
  template<typename I>
    void o_push_heap(I first, I last)
    {
      static_assert(Sort<I>(), "");
      std::push_heap(first, last);
    }

    
    
  template<typename I, typename R>
    void o_pop_heap(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      std::pop_heap(first, last, comp);
    }
    
  template<typename I>
    void o_pop_heap(I first, I last)
    {
      static_assert(Sort<I>(), "");
      std::pop_heap(first, last);
    }

    
    
  template<typename I, typename R>
    void o_make_heap(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      std::make_heap(first, last, comp);
    }
    
  template<typename I>
    void o_make_heap(I first, I last)
    {
      static_assert(Sort<I>(), "");
      std::make_heap(first, last);
    }

    
    
  template<typename I, typename R>
    void o_sort_heap(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      std::sort_heap(first, last, comp);
    }

  template<typename I>
    void o_sort_heap(I first, I last)
    {
      static_assert(Sort<I>(), "");
      std::sort_heap(first, last);
    }

} // namespace origin

#endif
