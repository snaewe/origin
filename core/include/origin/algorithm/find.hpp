// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_FIND_HPP
#define ORIGIN_ALGORITHM_FIND_HPP

#include <algorithm>

namespace origin
{
  // The find algorithms search a range of elements for an element that 
  // satisfies some property, usually a predicate or an equality. 
  //
  // The following algorithms search for an equal value in a range of elements.
  //
  //    find(first, last, value)
  //    find(range, value)
  //    find_not_equal(first, last, value)
  //    find_not_equal(range, value)
  //    find_next(first, last, value)
  //    find_nth(first, last, value)
  //    find_nth(range, value)
  //
  // These algorithms search for an element in a range of elements that 
  // satisfies a unary predicate.
  //
  //    find_if(first, last, pred)
  //    find_if(range, pred)
  //    find_if_not(first, last, pred)
  //    find_if_not(range, pred)
  //    find_next_if(first, last, value)
  //    find_nth_if(first, last, pred)
  //    find_nth_if(range, pred)


  
  // Find algorithms
  // These algorithms search a range of elements for an element that satisfies
  // some property.
  //
  // The _if family of algorithms search for the first iterator that satisfies
  // some unary predicate.
  


  // Find
  // Return the first iterator i in [first, last) such that *i == value or
  // last if no such iterator exists.
  template<typename I, typename T>
    I std_find(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");
      assert(( is_readable_range(first, last) ));

      return std::find(first, last, value);
    }

  
  
  // Find (range)
  // Returns the first iterator i in r such that *i == value or end(r) if no
  // such iterator exists.
  template<typename R, typename T>
    auto find(R& range, const T& value)
      -> Requires<Input_range<R>() && !Has_member_find<R, T>(), Iterator_type<R>>
    {
      static_assert(Value_searchable_range<R, T>(), "");
      
      return std_find(std::begin(range), std::end(range), value);
    }



  // Find (const range)
  // Returns the first iterator i in r such that *i == value or end(r) if no
  // such iterator exists.
  template<typename R, typename T>
    auto find(const R& range, const T& value)
      -> Requires<Input_range<R>() && !Has_member_find<R>(), Iterator_type<const R>>
    {
      static_assert(Value_searchable_range<R, T>(), "");

      return std_find(std::begin(range), std::end(range), value);
    }
  
  

  // Find (associative container)
  // Returns the first iterator i in c such that *i == value or end(c) if no
  // such iterator exists. The result is comptued in O(log(size(c))).
  template<typename C, typename T>
    auto find(C& c, const T& x) 
      -> Requires<Has_member_find<C, T>(), Iterator_type<C>>
    {
      return c.find(x);
    }



  // Find (const associative container)
  // Returns the first iterator i in c such that *i == value or end(c) if no
  // such iterator exists. The result is comptued in O(log(size(c))).
  template<typename C, typename T>
    auto find(const C& c, T const& x) 
      -> Requires<Has_member_find<C, T>(), Iterator_type<const C>>
    {
      return c.find(x);
    }



  // Find not equal
  // Returns the first iterator i in [first, last) where *i != value or last
  // if no such iterator exists.
  template<typename I, typename T>
    inline I find_not_equal(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");

      while(first != last) {
        if(*first != value)
          return first;
        ++first;
      }
      return last;
    }
  
  
  
  // Find not equal (range)
  // Returns the first iterator i in r where *i != value or end(r) if no such
  // iterator exists.
  template<typename R, typename T>
    inline Iterator_type<R> find_not_equal(R& range, const T& value)
    {
      return find_not_equal(std::begin(range), std::end(range), value);
    }
    
    
  
  // Find not equal (const range)
  // Returns the first iterator i in r where *i != value or end(r) if no such
  // iterator exists.
  template<typename R, typename T>
    inline Iterator_type<const R> find_not_equal(const R& range, const T& value)
    {
      return find_not_equal(std::begin(range), std::end(range), value);
    }

    

  // Find if
  // Returns the first iterator i in [first, last) where pred(*i) is true or
  // last if no such iterator exists.
  template<typename I, typename P>
    inline I std_find_if(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(pred(*first))
          return first;
        ++first;
      }
      return last;
    }



  // Find if (range)
  // Returns the first iterator i in r where pred(*i) is true or end(r) if no 
  // such iterator exists.
  template<typename R, typename P>
    inline Iterator_type<R> find_if(R& range, P pred) 
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return std_find_if(std::begin(range), std::end(range), pred);
    }
    
    

  // Find if (const range)
  // Returns the first iterator i in r where pred(*i) is true or end(r) if no 
  // such iterator exists.
  template<typename R, typename P>
    inline Iterator_type<const R> find_if(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return std_find_if(std::begin(range), std::end(range), pred);
    }
  

  
  // Find if not
  // Returns the first iterator i in [first, last) where !pred(*i) is true or
  // last if no such iterator exists.
  template<typename I, typename P>
    inline I std_find_if_not(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(!pred(*first))
          return first;
        ++first;
      }
      return last;
    }


    
  // Find if not (range)
  // Returns the first iterator i in r where pred(*i) is true or end(r) if no 
  // such iterator exists.
  template<typename R, typename P>
    inline Iterator_type<R> find_if_not(R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return std_find_if_not(std::begin(range), std::end(range), pred);
    }
    
  
  
  // Find if not (const range)
  // Returns the first iterator i in r where pred(*i) is true or end(r) if no 
  // such iterator exists.
  template<typename R, typename P>
    inline Iterator_type<R> find_if_not(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return std_find_if_not(std::begin(range), std::end(range), pred);
    }
  


  // Find next
  // Returns the first iterator i in [first + 1, last) where *i == value or
  // last if no such iterator exists or first == last.
  //
  // Note that there is no corresponding range version of this function. The
  // operation is only intended to be applied to iterators.
  template<typename I, typename T>
    inline I find_next(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");
      assert(( is_readable_range(first, last) ));
      
      if(first != last)
        return std_find(std_next(first), last, value);
      else
        return last;
    }



  // Find next if
  // Returns the first element in [first + 1, last) where pred(*i) is true or
  // last if no iterator exists or first == last.
  //
  // Note that there is no corresponding range version of this function. The
  // operation is only intended to be applied to iterators.
  template<typename I, typename P>
    inline I find_next_if(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      if(first != last)
        return std_find_if(std_next(first), last, pred);
      else
        return last;
    }



  // Find nth
  // Return the nth iterator i in [first, last) where *i == value or last if
  // there are fewer than n elements equal to value.
  template<typename I, typename T>
    inline I find_nth(I first, I last, Distance_type<I> n, const T& value)
    {
      static_assert(Value_searchable<I>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last && n != 0) {
        if(*first == value)
          --n;
        ++first;
      }
      return first;
    }



  // Find nth (range)
  // Return an iterator to the nth element in r that is equal to value.
  template<typename R, typename T>
    inline Iterator_type<R> find_nth(R& range, Distance_type<R> n, T const& value)
    {
      static_assert(Value_searchable_range<R>(), "");
      
      return find_nth(std::begin(range), std::end(range), n, value);
    }
    
    
  
  // Find nth (const range)
  // Return an iterator to the nth element in r that is equal to value.
  template<typename R, typename T>
    inline Iterator_type<const R> find_nth(const R& range, Distance_type<R> n, const T& value)
    {
      static_assert(Value_searchable_range<R>(), "");
      
      return find_nth(std::begin(range), std::end(range), n, value);
    }



  // Find nth if
  // Return the nth iterator i in [first, last) where pred(*i) is true or last 
  // if there are fewer than n elements equal to value.
  template<typename I, typename P>
    I find_nth_if(I first, I last, Distance_type<I> n, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));
      
      while(first != last && n != 0) {
        if(pred(*first))
          --n;
        ++first;
      }
      return first;
    }
    
    
  
  // Find nth if (range)
  // Return an iterator to the nth iterator in r where pred(*i) or end(r) if
  // there are fewer than n elements satisfying pred.
  template<typename R, typename P>
    inline Iterator_type<R> find_nth_if(R& range, Distance_type<R> n, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return find_nth_if(std::begin(range), std::end(range), n, pred);
    }
    
  
  
  // Find nth if (const range)
  // Return an iterator to the nth iterator in r where pred(*i) or end(r) if
  // there are fewer than n elements satisfying pred.
  template<typename R, typename P>
    inline Iterator_type<R const> find_nth_if(R const& range, Distance_type<R> n, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return find_nth_if(std::begin(range), std::end(range), n, pred);
    }



  // Find first of
  // Returns an iterator i in [first1, last1) where any_equal(first2, last, *i)
  // is true.
  template<typename I1, typename I2>
    inline I1 std_find_first_of(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Value_searchable<I1, Value_type<I2>>(), "");
      static_assert(Forward_iterator<I2>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));

      while(first1 != last1) {
        if(any_equal(first2, last2, *first1))
          return first1;
        ++first1;
      }
      return last1;
    }



  // Find first of (range)
  // Returns the first iterator i in r1 where any_equal(r2, *i) is true.
  template<typename R1, typename R2>
    inline Iterator_type<R1> find_first_of(R1& range1, const R2& range2)
    {
      static_assert(Value_searchable_range<R1, Value_type<R2>>(), "");
      static_assert(Forward_range<R2>(), "");
      
      return std_find_first_of(std::begin(range1), std::end(range1),
                               std::begin(range2), std::end(range2));
    }
  
  
  
  // Find first of (const range)
  // Returns an iterator i in r1 that is equal to any of the elements in r2.
  template<typename R1, typename R2>
    inline Iterator_type<const R1> find_first_of(const R1& range1, const R2& range2)
    {
      static_assert(Value_searchable_range<R1, Value_type<R2>>(), "");
      static_assert(Forward_range<R2>(), "");
      
      return std_find_first_of(std::begin(range1), std::end(range1),
                               std::begin(range2), std::end(range2));
    }
    
    
  
  // Find first of (relation)
  // Returns the first iterator i in [first1, last1) such that comp(*i, *j) is 
  // true where j is any iterator in [first2, last2).
  template<typename I1, typename I2, typename R>
    inline I1 std_find_first_of(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      // FIXME: Is there a more succint concept describing the search
      // principle?
      static_assert(Input_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Relation<R, Value_type<I1>, Value_type<I2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));
      
      using namespace std::placeholders;

      while(first1 != last1) {
        if(any_of(first2, last2, std::bind(*first1, _1)))
          return first1;
        ++first1;
      }
      return last1;
    }



  // Find first of (relation, range)
  // Returns the first iterator i in r1 such that comp(*i, *j) is true where j 
  // is any iterator in r2.
  template<typename R1, typename R2, typename Rel>
    inline Iterator_type<R1> find_first_of(R1& range1, const R2& range2, Rel comp)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Forward_range<R2>(), "");
      static_assert(Relation<Rel, Value_type<R1>, Value_type<R2>>(), "");
      
      return std_find_first_of(std::begin(range1), std::end(range1),
                               std::begin(range2), std::end(range2), comp);
    }
    
    
  
  // Find first of (relation, const range)
  // Returns the first iterator i in r1 such that comp(*i, *j) is true where j 
  // is any iterator in r2.
  template<typename R1, typename R2, typename Rel>
    inline Iterator_type<const R1> find_first_of(const R1& range1, const R2& range2, Rel comp)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");
      static_assert(Relation<Rel, Value_type<R1>, Value_type<R2>>(), "");
      
      return std_find_first_of(std::begin(range1), std::end(range1),
                               std::begin(range2), std::end(range2), comp);
    }  

} // namespace origin

#endif
