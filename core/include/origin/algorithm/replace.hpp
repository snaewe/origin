// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_REPLACE_HPP
#define ORIGIN_ALGORITHM_REPLACE_HPP

#include <algorithm>

namespace origin
{
  // Replace algorithms
  // The replace algorithms search or query an iterator range, replacing 
  // some elements with others. The replacement can be in place or the entire
  // replacements could be written into a new range.
  //
  // Search algorithms:
  //
  //    replace(first, last, old_value, new_value)
  //    replace(first, last, old_value, new_value, comp)
  //    replace(range, old_value, new_value)
  //    replace(range, old_value, new_value, comp)
  //    replace_copy(first, last, result, old_value, new_value)
  //    replace_copy(first, last, result, old_value, new_value, comp)
  //
  // Query algorithms
  //
  //    replace_if(first, last, pred, new_value)
  //    replace_if(range, pred, new_value)
  //    replace_copy_if(first, last, pred, new_value)
  //    replace_copy_if(range, pred, new_value)
  //
  // TODO: Write documentation.
  //
  // FIXME: I think that fill, generate, and transform also belong in this
  // header. All of these are fill or conditional fill algorithms.
  

  // Replace
  template<typename I, typename T>
    void o_replace(I first, I last, const T& old_value, const T& new_value)
    {
      static_assert(Search<I, T>(), "");
      static_assert(Fill<I, T>(), "");
      assert(is_readable_range(first, last));
      assert(is_writable_range(first, last, new_value));

      while(first != last) {
        if(*first == old_value)
          *first = new_value;
        ++first;
      }
    }
    
    
  
  // Replace (relation)
  template<typename I, typename T, typename R>
    void replace(I first, I last, const T& old_value, const T& new_value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      static_assert(Fill<I, T>(), "");
      assert(is_readable_range(first, last));
      assert(is_writable_range(first, last, new_value));

      while(first != last) {
        if(comp(*first, old_value))
          *first = new_value;
        ++first;
      }
    }

    
    
  // Replace (range)
  template<typename R, typename T>
    inline void replace(R&& range, const T& old_value, const T& new_value)
    {
      using Rx = Forwarded<R>;
      static_assert(Range_search<Rx, T>(), "");
      static_assert(Range_fill<Rx, T>(), "");
      
      o_replace(std::begin(range), std::end(range), old_value, new_value);
    }

    
    
  // Replace (range, relation)
  template<typename R, typename T, typename Rel>
    inline void replace(R&& range, const T& old_value, const T& new_value, Rel comp)
    {
      using Rx = Forwarded<R>;
      static_assert(Range_search<Rx, T, Rel>(), "");
      static_assert(Range_copy<R, T>(), "");
      
      o_replace(std::begin(range), std::end(range), old_value, new_value, comp);
    }
    
  
    
  // Replace copy
  template<typename I, typename O, typename T>
    O o_replace_copy(I first, I last, O result, const T& old_value, const T& new_value)
    {
      static_assert(Search<I, T>(), "");
      static_assert(Copy<I, O>(), "");
      static_assert(Fill<O, T>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_count(first, last, old_value), new_value));
      
      while(first != last) {
        if(*first == old_value)
          *result = new_value;
        else
          *result = *first;
        ++result;
        ++first;
      }
      return result;
    }
    
    
  
  // Replace copy (relation)
  template<typename I, typename O, typename T, typename R>
    O replace_copy(I first, I last, O result, const T& old_value, const T& new_value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      static_assert(Copy<I, O>(), "");
      static_assert(Fill<O, T>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_count(first, last, old_value), new_value));
      
      while(first != last) {
        if(comp(*first, old_value))
          *result = new_value;
        else
          *result = *first;
        ++result;
        ++first;
      }
      return result;
    }
    
    
    
  // Replace copy (range)
  template<typename R, typename O, typename T>
    inline void replace_copy(R&& range, O&& result, const T& value)
    {
      using Rx = Forwarded<R>;
      using Ox = Forwarded<O>;
      static_assert(Range_search<Rx, T>(), "");
      static_assert(Copy<Rx, Ox>(), "");
      static_assert(Fill<Ox, T>(), "");
      
      return o_replace_copy(std::begin(range), std::end(range), 
                              std::begin(result), value);
    }

    

  // Replace copy (range, relation)
  template<typename R, typename O, typename T, typename Rel>
    inline void replace_copy(R&& range, O&& result, const T& value, Rel comp)
    {
      using Rx = Forwarded<R>;
      using Ox = Forwarded<O>;
      static_assert(Range_search<Rx, T, Rel>(), "");
      static_assert(Range_copy<Rx, Ox>(), "");
      static_assert(Range_fill<Ox, T>(), "");
      
      return o_replace_copy(std::begin(range), std::end(range), 
                              std::begin(result), value, comp);
    }
    
    
    
  // Replace_if
  template<typename I, typename P, typename T>
    inline void o_replace_if(I first, I last, P pred, const T& new_value)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, T>(), "");
      assert(is_readable_range(first, last));
      assert(is_writable_range(first, last, new_value));
      
      while(first != last) {
        if(pred(*first)) 
          *first = new_value;
        ++first;
      }
    }
    
    
  
  // Replace if (range)
  template<typename R, typename P, typename T>
    inline void replace_if(R& range, P pred, const T& new_value)
    {
      static_assert(Range_query<R, P>(), "");
      static_assert(Range_copy<R, T>(), "");
      
      o_replace_if(std::begin(range), std::end(range), pred, new_value);
    }

    

  // Replace copy if
  template<typename I, typename O, typename P, typename T>
    O o_replace_copy_if(I first, I last, O result, P pred, const T& value)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, O>(), "");
      static_assert(Fill<O, T>(), "");
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_count(first, last, old_value), value));
      
      while(first != last) {
        if(pred(*first))
          *result = value;
        else
          *result = *first;
        ++result;
        ++first;
      }
      return result;
    }



  // Replace copy if (range)
  template<typename R, typename O, typename T, typename P>
    inline void replace_copy_if(R&& in, O&& out, P pred, const T& value)
    {
      using Rx = Forwarded<R>;
      using Ox = Forwarded<O>;
      static_assert(Range_query<Rx, P>(), "");
      static_assert(Range_copy<Rx, Ox>(), "");
      static_assert(Range_fill<Ox, T>(), "");
      
      return o_replace_copy_if(std::begin(in), std::end(in), std::begin(out), 
                                 pred, value);
    }
    
} // namespace origin

#endif
