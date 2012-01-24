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
  //    count(first, last, value, comp)
  //    count(range, value)
  //    count(range, value, comp)
  //    count_not_equal(first, last, value)
  //    count_not_equal(first, last, value, comp)
  //    count_not_equal(range, value)
  //    count_not_equal(range, value, comp)
  //
  // Algorithms that count an aribtrary property are:
  //
  //    count_if(first, last, pred)
  //    count_if(range, pred)
  //    count_if_not(first, last, pred)
  //    count_if_not(range, pred)
  //
  // And for counted ranges:
  //
  //    count_n(first, n, value)
  //    count_n(first, n, value, comp)
  //    count_n_if(first, n, pred)
  //
  // TODO: Write count_in, which should return the number of elements in
  // [first1, last1) that match any value or have some property with those
  // in [first2, last2). This would be analogous to find_first_in.



  // Count
  // Returns the number of elements x in [first, last) where x == value.
  template<typename I, typename T>
    inline Distance_type<I> std_count(I first, I last, const T& value)
    {
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));

     Distance_type<I> n = 0;
      while(first != last) {
        if(*first == value)
          ++n;
        ++first;
      }
      return n;
    }
    
    

  // Count (relation)
  // Returns the number of elements x in [first, last) where x == value.
  template<typename I, typename T, typename R>
    inline Distance_type<I> std_count(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));

     Distance_type<I> n = 0;
      while(first != last) {
        if(comp(*first, value))
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
      static_assert(Range_search<R>(), "");

      return std_count(std::begin(range), std::end(range), value);
    }
   
   
   
  // Count (range)
  // Returns the number of elements x in range where x == value.
  template<typename R, typename T, typename Rel>
    inline Distance_type<R> count(const R& range, const T& value, Rel comp)
    {
      static_assert(Range_search<R, T, Rel>(), "");

      return std_count(std::begin(range), std::end(range), value, comp);
    }



  // Count not equal
  // Returns the number of elements x in [first, last) where x != value.
  template<typename I, typename T>
    inline Distance_type<I> count_not_equal(I first, I last, const T& value)
    {
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));
      
      Distance_type<I> n = 0;
      while(first != last) {
        if(*first != value)
          ++n;
        ++first;
      }
      return n;
    }

    
    
  // Count not equal (relation)
  // Returns the number of elements x in [first, last) where x != value.
  template<typename I, typename T, typename R>
    inline Distance_type<I> count_not_equal(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));
      
      Distance_type<I> n = 0;
      while(comp(first, last)) {
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
      static_assert(Range_search<R, T>(), "");
      
      return count_not_equal(std::begin(range), std::end(range), value);
    }
    
      
        
  // Count not equal (range, relation)
  // Returns the number of elements x in r where comp(x, value) is true.
  template<typename R, typename T, typename Rel>
    inline Distance_type<R> count_not_equal(const R& range, const T& value, Rel comp)
    {
      static_assert(Range_search<R, T, Rel>(), "");
      
      return count_not_equal(std::begin(range), std::end(range), value, comp);
    }



  // Count if
  // Returns the number of elements x in [first, last) where pred(x) is true.
  template<typename I, typename P>
    inline Distance_type<I> std_count_if(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

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
      static_assert(Range_query<R, P>(), "");

      return std_count_if(std::begin(range), std::end(range), pred);
    }

    
  
  // Count if not
  // Returns the number of elements x in [first, last) where !pred(x)
  template<typename I, typename P>
    inline Distance_type<I> count_if_not(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));
      
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
      static_assert(Range_query<R, P>(), "");
      
      return count_if_not(std::begin(range), std::end(range), pred);
    }
    

  
  // Count n
  // Returns the number of elements x in [first, first + n) where x == value.
  template<typename I, typename T>
    inline Distance_type<I> count_n(I first, Distance_type<I> n, const T& value)
    {
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, n));

      Distance_type<I> c = 0;
      while(n != 0) {
        if(*first == value)
          ++c;
        ++first;
        --n;
      }
      return c;
    }

    
  
  // Count n if
  // Returns the number of elements x in [first, first + n) where pred(x) is
  // true.
  template<typename I, typename P>
    inline Distance_type<I> count_n_if(I first, Distance_type<I> n, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, n));

      Distance_type<I> c = 0;
      while(n != 0) {
        if(pred(*first))
          ++c;
        ++first;
        --n;
      }
    }
    
} // namespace origin

#endif
