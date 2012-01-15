// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_COUNT_HPP
#define ORIGIN_ALGORITHM_COUNT_HPP

#include <algorithm>

namespace origin
{

  // The count module provides algorithms that return the number of elements
  // in a range that exhibit a property. Algorithms for counting equal values
  // are:
  //
  //    count(first, last, value)
  //    count(range, value)
  //    count_not_equal(first, last, value)
  //    count_not_equal(range, value)
  //
  // Algorithms that count an aribtrary property are:
  //
  //    count_if(first, last, pred)
  //    count_if(range, pred)
  //    count_if_not(first, last, pred)
  //    count_if_not(range, pred)
  //
  // TODO: Write count_in, which should return the number of elements in
  // [first1, last1) that match any value or have some property with those
  // in [first2, last2).


  // Count
  // Returns the number of elements x in [first, last) where x == value.
  template<typename I, typename T>
    inline Distance_type<I> std_count(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");
      assert(( is_readable_range(first, last) ));

     Distance_type<I> n = 0;
      while(first != last) {
        if(*first == value)
          ++n;
        ++first;
      }
      return n;
    }
    
    

  // Count (range)
  // Returns the number of elements x in range where x == value.
  template<typename R, typename T>
    inline Distance_type<R> count(const R& range, const T& value)
    {
      static_assert(Value_searchable_range<R>(), "");

      return std_count(std::begin(range), std::end(range), value);
    }
   
   
   
  // Count not equal
  // Returns the number of elements x in [first, last) where x != value.
  template<typename I, typename T>
    inline Distance_type<I> count_not_equal(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");
      assert(( is_readable_range(first, last) ));
      
      Distance_type<I> n = 0;
      while(first != last) {
        if(*first != value)
          ++n;
        ++first;
      }
      return n;
    }

    
    
  // Count not equal (range)
  // Returns the number of elements x in r where x != value.
  template<typename R, typename T>
    inline Distance_type<R> count_not_equal(const R& range, const T& value)
    {
      static_assert(Value_searchable_range<R, T>(), "");
      
      return count_not_equal(std::begin(range), std::end(range), value);
    }
    
      
        
  // Count if
  // Returns the number of elements x in [first, last) where pred(x) is true.
  template<typename I, typename P>
    inline Distance_type<I> std_count_if(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));

     Distance_type<I> n = 0;
      while(first != last) {
        if(pred(*first))
          ++n;
        ++first;
      }
      return n;
    }
   
   
   
  // Count if (range)
  // Returns the number of elements x in r where pred(x) is true.
  template<typename R, typename P>
    inline Distance_type<R> count_if(R const& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");

      return std_count_if(std::begin(range), std::end(range), pred);
    }

    
  
  // Count if not
  // Returns the number of elements x in [first, last) where !pred(x)
  template<typename I, typename P>
    inline Distance_type<I> count_if_not(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));
      
      Distance_type<I> n = 0;
      while(first != last) {
        if(!pred(*first))
          ++n;
        ++first;
      }
      return n;
    }

  
  
  // Count if not (range)
  // Returns the number of elements x in r where !pred(x).
  template<typename R, typename P>
    inline Distance_type<R> count_if_not(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return count_if_not(std::begin(range), std::end(range), pred);
    }
    

} // namespace origin

#endif
