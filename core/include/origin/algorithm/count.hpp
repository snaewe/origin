// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_COUNT_HPP
#define ORIGIN_ALGORITHM_COUNT_HPP

#include <origin/algorithm/core.hpp>

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
  //
  // Returns the number of elements x in a range where x matches value. The
  // meaning of matching depends on the condition imposed by comp. There are
  // four overloads of this function:
  //
  //    count(first, last, value, comp) ~> n
  //    count(first, last, value) ~> n
  //    count(range, value, comp) ~> n
  //    count(range, value) ~> n
  //
  // The first two overloads count matching elements in the iterator range
  // [first, last), while the last two count elements in the bounded range.

  template <typename I, typename T, typename R>
    Difference_type<I> count(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));

     Difference_type<I> n = 0;
      while (first != last) {
        if (comp(*first, value))
          ++n;
        ++first;
      }
      return n;
    }

  template <typename I, typename T>
    Difference_type<I> count(I first, I last, const T& value)
    {
      return count(first, last, value, eq());
    }

  template <typename R, typename T, typename Comp>
    inline Difference_type<R> count(const R& range, const T& value, Comp comp)
    {
      return count(begin(range), end(range), value, comp);
    }

  template <typename R, typename T>
    inline Difference_type<R> count(const R& range, const T& value)
    {
      return count(begin(range), end(range), value, eq());
    }
   
   

  // Count not equal
  // Returns the number of elements x in [first, last) where x != value.
  //
  // FIXME: The name not_equal is incorrect for these algorithms. Should this
  // just be called count_not? count_mismatch?

  template <typename I, typename T, typename R>
    inline Difference_type<I> 
    count_not_equal(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));
      
      Difference_type<I> n = 0;
      while (first != last) {
        if (!comp(*first, value))
          ++n;
        ++first;
      }
      return n;
    }

  template <typename R, typename T, typename Comp>
    inline Difference_type<R> 
    count_not_equal(const R& range, const T& value, Comp comp)
    {
      return count_not_equal(begin(range), end(range), value, comp);
    }


  
  // Count not equal
  //
  // Returns the number of elements x in [first, last) where x != value.

  template <typename I, typename T>
    inline Difference_type<I> count_not_equal(I first, I last, const T& value)
    {
      return count_not_equal(first, last, value, eq());
    }

  template <typename R, typename T>
    inline Difference_type<R> count_not_equal(const R& range, const T& value)
    {
      static_assert(Range_search<R, T>(), "");
      return count_not_equal(begin(range), end(range), value, eq());
    }
    
      
      
  // Count if
  // 
  // Returns the number of iterators i in a range for which pred(*i) evaluates 
  // to true. There are two overloads of this function:
  //
  //    count_if(first, last, pred) ~> n
  //    count_if(range, pred) ~> n

  template <typename I, typename P>
    inline Difference_type<I> count_if(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

     Difference_type<I> n = 0;
      while (first != last) {
        if (pred(*first))
          ++n;
        ++first;
      }
      return n;
    }
   
  template <typename R, typename P>
    inline Difference_type<R> count_if(const R& range, P pred)
    {
      return count_if(std::begin(range), std::end(range), pred);
    }



  // Count if not
  //
  // Returns the number of iterators i in a range where pred(*i) evaluates to
  // false. There are two overloads of this function:
  //
  //    count_if_not(first, last, pred) ~> n
  //    count_if_not(range, pred) ~> n

  template <typename I, typename P>
    inline Difference_type<I> count_if_not(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));
      
      Difference_type<I> n = 0;
      while (first != last) {
        if (!pred(*first))
          ++n;
        ++first;
      }
      return n;
    }

  template <typename R, typename P>
    inline Difference_type<R> count_if_not(const R& range, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      
      return count_if_not(std::begin(range), std::end(range), pred);
    }
    
} // namespace origin

#endif
