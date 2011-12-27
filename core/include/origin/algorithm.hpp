// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_HPP
#define ORIGIN_ALGORITHM_HPP

#include <algorithm>

#include <origin/iterator.hpp>
#include <origin/range.hpp>
#include <origin/container.hpp>
#include <origin/algorithm/combination.hpp>

namespace origin
{
  // NOTE: The std_* algorithms simply add Origin-style concept checking to
  // std algorithms. They are primiarly used for testing

  // Quantifiers
  //
  // Note that:
  //   !all_of == any_not_of
  //   !none_of == any_of

  // All of

  template<typename Iter, typename Pred>
    inline bool std_all_of(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      
      return std::all_of(first, last, pred);
    }

  // Return true if and only if all of the the elements in the range r satisfy 
  // the given predicate, or if r is empty.
  template<typename R, typename Pred>
    inline bool all_of(R const& range, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");

      return std_all_of(std::begin(range), std::end(range), pred);
    }
    
    
  
  // Any of
    
  template<typename Iter, typename Pred>
    inline bool std_any_of(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      return std::any_of(first, last, pred);
    }

  // Return true if and only if the range r is non-empty and any of its
  // elements satisfy the given predicate.
  template<typename R, typename Pred>
    inline bool any_of(R const& range, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");

      return std_any_of(std::begin(range), std::end(range), pred);
    }



  // Any not of
  
  // Return true if and only if the range [first, last) is non-empty and any
  // of its elements do not satisfy the given prdicate.
  template<typename Iter, typename Pred>
    bool any_not_of(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(!pred(*first))
          return true;
      }
      return false;
    }
  
  // Return true if and only if the range r is non-empty and any of its 
  // elements do not satisfy the given prdicate.
  template<typename R, typename Pred>
    inline bool any_not_of(R const& range, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return any_not_of(std::begin(range), std::end(range), pred);
    }



  // None of

  template<typename Iter, typename Pred>
    inline bool std_none_of(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      
      return std::none_of(first, last, pred);
    }

  // Return true if and only if none of the the elements in the range r satisfy
  // the given predicate or r is empty.
  template<typename R, typename Pred>
    inline bool none_of(R const& range, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return std_none_of(std::begin(range), std::end(range), pred);
    }
 
 
 
  // All equal

  // Returns true if and only if all elements in [first, last) are equal to
  // value or [first, last) is an empty range.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    bool all_equal(Iter first, Iter last, T const& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(*first != value)
          return false;
        ++first;
      }
      return true;
    }
   
  template<typename R, typename T>
    inline bool all_equal(const R& range, const T& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");

      return all_equal(std::begin(range), std::end(range), value);
    }
    
    
    
  // Any equal
    
  // Returns true if and only if [first, last) is not an empty range and at
  // least one element in [first, last) is equal to value.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    bool any_equal(Iter first, Iter last, const T& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(*first == value)
          return true;
      }
      return false;
    }

  template<typename R, typename T>
    inline bool any_equal(const R& range, const T& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");

      return any_equal(std::begin(range), std::end(range), value);
    }
    
    
    
  // Any not equal
    
  template<typename Iter, typename T>
    bool any_not_equal(Iter first, Iter last, const T& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(!first != value)
          return true;
      }
      return false;
    }
    
  template<typename R, typename T>
    inline bool any_not_equal(const R& range, const T& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");

      return any_not_equal(std::begin(range), std::end(range), value);
    }
    
    
    
  // None equal
    
  // Returns true if and only if [first, last) is an empty range or there
  // are no elements in [first, last) that are equal to value (or equivalently,
  // all elements are not equal to value).
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    bool none_equal(Iter first, Iter last, T const& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last)
      {
        if(*first == value)
          return false;
      }
      return true;
    }
    
  template<typename R, typename T>
    inline bool none_equal(const R& range, const T& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");

      return none_equal(std::begin(range), std::end(range), value);
    }
    

  // Find algorithms
  //
  // NOTE: There are two overloads of find and find_if because we should be
  // able to return non-const iterators for non-const ranges.
  
  // Find
  
  template<typename Iter, typename T>
    Iter std_find(Iter first, Iter last, T const& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      return std::find(first, last, value);
    }
    
  // Return the first iterator i in the given range that is equal to value.
  template<typename R, typename T>
    auto find(R& range, T const& value)
      -> Requires<Input_range<R>() && !Associative_container<R>(), Iterator_type<R>>
    {
      static_assert(Equality_comparable<Value_type<R>, T>(), "");
      
      return std_find(std::begin(range), std::end(range), value);
    }

  // A constant version of the algorithm above.
  template<typename R, typename T>
    auto find(R const& range, T const& value)
      -> Requires<Input_range<R>() && !Associative_container<R>(), Iterator_type<R const>>
    {
      static_assert(Equality_comparable<Value_type<R>, T>(), "");

      return std_find(std::begin(range), std::end(range), value);
    }
    
  // Specialization for associative containers.
  template<typename C, typename T>
    auto find(C& c, T const& x) 
      -> Requires<Associative_container<C>(), Iterator_type<C>>
    {
      return c.find(x);
    }

  // Const-specialization of the above function.
  template<typename C, typename T>
    auto find(C const& c, T const& x) 
      -> Requires<Associative_container<C>(), Iterator_type<C const>>
    {
      return c.find(x);
    }
    
    
    
  // Find_if
  
  template<typename Iter, typename Pred>
    inline Iter std_find_if(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      return std::find_if(first, last, pred);
    }

  // Return the first iterator i in the range that satisfies the given 
  // predicate.
  template<typename R, typename Pred>
    inline Iterator_type<R> find_if(R& range, Pred pred) 
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return std_find_if(std::begin(range), std::end(range), pred);
    }

  // A constant version of the function above.
  template<typename R, typename Pred>
    inline Iterator_type<R const> find_if(R const& range, Pred pred)
    {
      static_assert(Input_range<R const>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return std_find_if(std::begin(range), std::end(range), pred);
    }


  
  // NOTE: first_if and next_if don't have overloads for Ranges. They are
  // helper functions for traversing a range, and aren't really usable as
  // range-based algorithms. I may decide otherwise in the future.

  // Return the first element in [first, last) that is equal to value or
  // last if no such element exists.
  //
  // This function, along with next_equal, can be used to iterate over the
  // subsequence of values that are equal to another.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    inline Iter first_equal(Iter first, Iter last, const T& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      return std::find(first, last, value);
    }

  // Return the next element in [first + 1, last) that is equal to value, or
  // last if no such element exists.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    inline Iter next_equal(Iter first, Iter last, T const& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      if(first != last)
        return std::find(std::next(first), last, value);
      else
        return last;
    }

    
  // Return the first element in [first, last) that satisfies the predicate
  // pred, or last if no such element exists.
  //
  // This function, along with next_if, can be used to iterate over the
  // subsequence of values satisfying some predicate.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename Pred>
    inline Iter first_if(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      return std::find_if(first, last, pred);
    }
    
  // Return the next element in [first, last) that satisfies pred, or last if
  // no such element exists.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename Pred>
    inline Iter next_if(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      if(first != last)
        return std::find_if(std::next(first), last, pred);
      else
        return last;
    }
    
    

  // Return an iterator to the nth element in [first, last) that is equal to
  // value, or last if no such element exists.
  //
  // precondition: readable_range(first, last)
  // precondition: n >= 0
  template<typename Iter, typename T>
    Iter find_nth(Iter first, Iter last, Distance_type<Iter> n, T const& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));
      
      if(n == 0)
        return last;
      
      first = first_equal(first, last, value);
      --n;
      while(n != 0 && first != last) {
        first = next_equal(first, last, value);
        --n;
      }
      return first;
    }
    
  // Return an iterator to the nth element in r that is equal to value.
  template<typename R, typename T>
    inline Iterator_type<R> find_nth(R& range, Distance_type<R> n, T const& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");
      
      return find_nth(std::begin(range), std::end(range), n, value);
    }
    
  template<typename R, typename T>
    inline Iterator_type<R const> find_nth(R const& range, Distance_type<R> n, T const& value)
    {
      static_assert(Input_range<R const>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");
      
      return find_nth(std::begin(range), std::end(range), n, value);
    }



  // Return an iterator to the nth element in [first, last) that satisfies
  // pred, or last if no such element exists.
  //
  // precondition: readable_range(first, last)
  // precondition: n >= 0
  template<typename Iter, typename Pred>
    Iter find_nth_if(Iter first, Iter last, Distance_type<Iter> n, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      
      if(n == 0)
        return last;
      
      first = first_if(first, last, pred);
      --n;
      while(n != 0 && first != last) {
        first = next_if(first, last, pred);
        --n;
      }
      return first;
    }
    
  // Return an iterator to the nth element in r that satisfies pred, or return
  // last if there is no such element.
  template<typename R, typename Pred>
    inline Iterator_type<R> find_nth_if(R& range, Distance_type<R> n, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return find_nth_if(std::begin(range), std::end(range), n, pred);
    }
    
  template<typename R, typename Pred>
    inline Iterator_type<R const> find_nth_if(R const& range, Distance_type<R> n, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return find_nth_if(std::begin(range), std::end(range), n, pred);
    }
  

  
  // Find first of
  // Note that there are two overloads of the range-based find_first_of that
  // are differentiated by the const-ness of the first range. The second range
  // is always passed as a const range because we dont' return an iterator
  // for it.
  
  template<typename Iter1, typename Iter2>
    inline Iter1 std_find_first_of(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
    {
      static_assert(Input_iterator<Iter1>(), "");
      static_assert(Forward_iterator<Iter2>(), "");
      static_assert(Equality_comparable<Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));
      
      return std::find_first_of(first1, last1, first2, last2);
    }
  
  // The range-based find_first_of takes two ranges.
  template<typename R1, typename R2>
    inline Iterator_type<R1> find_first_of(R1& range1, R2 const& range2)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");
      static_assert(Equality_comparable<Value_type<R1>, Value_type<R2>>(), "");
      
      return std_find_first_of(std::begin(range1), std::end(range1),
                               std::begin(range2), std::end(range2));
    }
  
  // Const version of the function above.
  template<typename R1, typename R2>
    inline Iterator_type<R1 const> find_first_of(R1 const& range1, R2 const& range2)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");
      static_assert(Equality_comparable<Value_type<R1>, Value_type<R2>>(), "");
      
      return std_find_first_of(std::begin(range1), std::end(range1),
                               std::begin(range2), std::end(range2));
    }
    
    
    
  // Generalized find first of
  template<typename Iter1, typename Iter2, typename Rel>
    inline Iter1 std_find_first_of(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, Rel r)
    {
      static_assert(Input_iterator<Iter1>(), "");
      static_assert(Forward_iterator<Iter2>(), "");
      static_assert(Relation<Rel, Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));
      
      return std::find_first_of(first1, last1, first2, last2, r);
    }
    
  // Range-based generalized find_first_of.
  template<typename R1, typename R2, typename Rel>
    inline Iterator_type<R1> find_first_of(R1& range1, R2 const& range2, Rel r)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");
      static_assert(Relation<Rel, Value_type<R1>, Value_type<R2>>(), "");
      
      return std_find_first_of(std::begin(range1), std::end(range1),
                               std::begin(range2), std::end(range2), r);
    }
    
  // Const version of the function above.
  template<typename R1, typename R2, typename Rel>
    inline Iterator_type<R1 const> find_first_of(R1 const& range1, R2 const& range2, Rel r)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");
      static_assert(Relation<Rel, Value_type<R1>, Value_type<R2>>(), "");
      
      return std_find_first_of(std::begin(range1), std::end(range1),
                               std::begin(range2), std::end(range2), r);
    }
  
  
  
  // Adjacent find
  
  template<typename Iter>
    inline Iter std_adjacent_find(Iter first, Iter last)
    {
      static_assert(Forward_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      return std::adjacent_find(first, last);
    }
    
  template<typename R>
    inline Iterator_type<R> adjacent_find(R& range)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>>(), "");

      return std_adjacent_find(std::begin(range), std::end(range));
    }
  
  template<typename R>
    inline Iterator_type<R const> adjacent_find(R const& range)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>>(), "");

      return std_adjacent_find(std::begin(range), std::end(range));
    }

    
    
  // Count
  
  template<typename Iter, typename T>
    inline Distance_type<Iter> std_count(Iter first, Iter last, T const& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      return std::count(first, last, value);
    }
    
  // Return the number of elements in r that are equal to value.
  template<typename R, typename T>
    inline Distance_type<R> count(R const& range, T const& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");

      return std_count(std::begin(range), std::end(range), value);
    }
   
   
   
  // Count if
  
  template<typename Iter, typename Pred>
    inline Distance_type<Iter> std_count_if(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      return std::count_if(first, last, pred);
    }
   
  // Return the number of elements in r that satisfy pred.
  template<typename R, typename Pred>
    inline Distance_type<R> count_if(R const& range, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      return std_count_if(std::begin(range), std::end(range), pred);
    }

    
    
  // Count not equal
  
  // Return the number of elements in [first, last) that are not equal to
  // value.
  //
  // requires: InputIterator<Iter>
  // requires: EqualityComparable<ValueType<Iter>, T>
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    inline Distance_type<Iter> count_not_equal(Iter first, Iter last, T const& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));
      
      Distance_type<Iter> n = 0;
      while(first != last) {
        if(*first != value)
          ++n;
        ++first;
      }
      return n;
    }

  // Return the number of elements in r that are not equal to value.
  template<typename R, typename T>
    inline Distance_type<R> count_not_equal(R const& range, T const& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");
      
      return count_not_equal(std::begin(range), std::end(range), value);
    }
    
    
  
  // Count if not
  
  // Return the number of elements in [first, last) that do not satisfy pred.
  //
  // requires: InputIterator<Iter>
  // precondition: readable_range(first, last)
  template<typename Iter, typename Pred>
    inline Distance_type<Iter> count_if_not(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      
      Distance_type<Iter> n = 0;
      while(first != last) {
        if(!pred(*first))
          ++n;
        ++first;
      }
      return n;
    }

  // Return the number of elements in r that do not satisfy pred.
  template<typename R, typename Pred>
    inline Distance_type<R> count_if_not(R const& range, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return count_if_not(std::begin(range), std::end(range), pred);
    }
    
    
    
  // Equal
  // Returns true if, for two ranges a and b, the elements of a are equal to
  // the elements of b. Note that b must have size greater than or equal to a.
  
  template<typename Iter1, typename Iter2>
    inline bool std_equal(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      static_assert(Input_iterator<Iter1>(), "");
      static_assert(Weak_input_iterator<Iter2>(), "");
      assert(( is_readable_range(first1, last1) ));
      assume(( is_readable_range(first2, distance(first1, last1)) ));
      
      return std::equal(first1, last1, first2);
    }

  template<typename R1, typename R2>
    inline bool equal(R1 const& a, R2 const& b)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");
    
      return size(a) <= size(b) 
          && std_equal(std::begin(a), std::end(a), std::begin(b));
    }

  // Generalized version
  template<typename Iter1, typename Iter2, typename Pred>
    inline bool std_equal(Iter1 first1, Iter1 last1, Iter2 first2, Pred pred)
    {
      static_assert(Input_iterator<Iter1>(), "");
      static_assert(Weak_input_iterator<Iter2>(), "");
      static_assert(Predicate<Pred, Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assume(( is_readable_range(first2, distance(first1, last1)) ));
      
      return std::equal(first1, last1, first2, pred);
    }
    
  template<typename R1, typename R2, typename Pred>
    inline bool equal(const R1& a, const R2& b, Pred pred)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");

      return size(a) <= size(b) 
          && std_equal(std::begin(a), std::end(a), std::begin(b), pred);
    }



  // Mismatch
  // For two ranges a and b, return the first element in a where a is not
  // equal the corresponding element in b (i.e., where they do not match).
  //
  // FIXME: I think I need overloads range overloads for const and non-const
  // overloads. Maybe I can use forwarding.

  template<typename Iter1, typename Iter2>
    inline std::pair<Iter1, Iter2> 
    std_mismatch(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      static_assert(Input_iterator<Iter1>(), "");
      static_assert(Weak_input_iterator<Iter2>(), "");
      assert(( is_readable_range(first1, last1) ));
      assume(( is_readable_range(first2, distance(first1, last1)) ));
      
      return std::mismatch(first1, last1, first2);
    }

  template<typename R1, typename R2>
    inline std::pair<Iterator_type<R1>, Iterator_type<R2>> 
    mismatch(const R1& a, const R2& b)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");

      return std_mismatch(std::begin(a), std::end(a), std::begin(b));
    }

  // Generalized version
  template<typename Iter1, typename Iter2, typename Pred>
    inline std::pair<Iter1, Iter2> 
    std_mismatch(Iter1 first1, Iter1 last1, Iter2 first2, Pred pred)
    {
      static_assert(Input_iterator<Iter1>(), "");
      static_assert(Weak_input_iterator<Iter2>(), "");
      static_assert(Predicate<Pred, Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assume(( is_readable_range(first2, distance(first1, last1)) ));
      
      return std::mismatch(first1, last1, first2, pred);
    }
    
  template<typename R1, typename R2, typename Pred>
    inline std::pair<Iterator_type<R1>, Iterator_type<R2>> 
    mismatch(const R1& a, const R2& b, Pred pred)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");

      return std_mismatch(std::begin(a), std::end(a), std::begin(b), pred);
    }



  // Permutation testing
  // Returns true if, for a range a and b, a is a permutation of b.
  
  template<typename Iter1, typename Iter2>
    inline bool std_is_permutation(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      return std::is_permutation(first1, last1, first2);
    }
    
  template<typename R1, typename R2>
    inline bool is_permutation(const R1& a, const R2& b)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");

      return std_is_permutation(a, b);
    }

  // Generalized version
  template<typename Iter1, typename Iter2, typename Pred>
    inline bool std_is_permutation(Iter1 first1, Iter1 last1, Iter2 first2, Pred pred)
    {
      return std::is_permutation(first1, last1, first2, pred);
    }

  template<typename R1, typename R2, typename Pred>
    inline bool is_permutation(const R1& a, const R2& b, Pred pred)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");

      return std_is_permutation(a, b, pred);
    }



  // Search
  //
  // FIXME: Write range variants.

  template<typename Iter1, typename Iter2>
    inline Iter1 std_search(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
    {
      static_assert(Forward_iterator<Iter1>(), "");
      static_assert(Forward_iterator<Iter2>(), "");
      static_assert(Equality_comparable<Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));
      
      return std::search(first1, last1, first2, last2);
    }
    
  template<typename Iter1, typename Iter2, typename Pred>
    inline Iter1 std_search(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, Pred pred)
    {
      static_assert(Forward_iterator<Iter1>(), "");
      static_assert(Forward_iterator<Iter2>(), "");
      static_assert(Equality_comparable<Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));

      return std::search(first1, last1, first2, last2);
    }
    
    
  
  // Search end
  // This is the same as std::find_end, but named more appropriately.
  //
  // FIXME: Write range variants.

  template<typename Iter1, typename Iter2>
    inline Iter1 search_end(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
    {
      static_assert(Forward_iterator<Iter1>(), "");
      static_assert(Forward_iterator<Iter2>(), "");
      static_assert(Equality_comparable<Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));
      
      return std::find_end(first1, last1, first2, last2);
    }
    
  template<typename Iter1, typename Iter2, typename Pred>
    inline Iter1 search_end(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, Pred pred)
    {
      static_assert(Forward_iterator<Iter1>(), "");
      static_assert(Forward_iterator<Iter2>(), "");
      static_assert(Equality_comparable<Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));

      return std::find_end(first1, last1, first2, last2);
    }
    
  
  
  // Search n
  //
  // FIXME: Write range variants.
  template<typename Iter, typename T>
    inline Iter std_search_n(Iter first, Iter last, Distance_type<Iter> n, const T& value)
    {
      static_assert(Forward_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      return std::search_n(first, last, n, value);
    }
    
  template<typename Iter, typename T, typename Pred>
    inline Iter std_search_n(Iter first, Iter last, Distance_type<Iter> n, const T& value, Pred pred)
    {
      static_assert(Forward_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      return std::search_n(first, last, n, value, pred);
    }
    
    

  // Copy
  // Copy the elements in a range a into another range b.
  
  template<typename Iter, typename Out>
    inline Out std_copy(Iter first, Iter last, Out result)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Writable<Out, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      assume(( is_writable_range(result, distance(first, last), *first) ));
      // FIXME: Overlapping requirements
      
      return std::copy(first, last, result);
    }
  
  template<typename In, typename Out>
    inline Iterator_type<Out> copy(const In& i, Out& o)
    {
      static_assert(Input_range<In>(), "");
      static_assert(Output_range<Out, Value_type<In>>(), "");
      assume(( size(i) <= size(o) ));
      // FIXME: Overlapping requirements
      
      return std_copy(std::begin(i), std::end(i), std::begin(o));
    }
    
    
    
  // Copy
  template<typename Iter, typename Out>
    inline Out std_copy_n(Iter first, Distance_type<Iter> n, Out result)
    {
      return std::copy_n(first, n, result);
    }



  // Copy if
  template<typename Iter, typename Out, typename Pred>
    inline Out std_copy_if(Iter first, Iter last, Out result, Pred pred)
    {
      return std::copy_if(first, last, result, pred);
    }
    
    
    
  // Copy backward
  template<typename Iter, typename Out>
    inline Out std_copy_backward(Iter first, Iter last, Out result)
    {
      return std::copy_backward(first, last, result);
    }



  // Move
  // Move the elements in a range a into another range b.

  template<typename Iter, typename Out>
    inline Out std_move(Iter first, Iter last, Out result)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Move_writable<Out, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      assume(( is_movable_range(result, distance(first, last), *first) ));
      
      return std::copy(first, last, result);
    }
  
  template<typename In, typename Out>
    inline Iterator_type<Out> move(const In& i, Out& o)
    {
      static_assert(Input_range<In>(), "");
      static_assert(Move_range<Out, Value_type<In>>(), "");
      assume(( size(i) <= size(o) ));
      
      return std_copy(std::begin(i), std::end(i), std::begin(o));
    }



  // Move backward
  template<typename Iter, typename Out>
    inline Out std_move_backward(Iter first, Iter last, Out result)
    {
      return std::move_backward(first, last, result);
    }



  // Iterator swap
  template<typename Iter1, typename Iter2>
    inline void std_iter_swap(Iter1 i, Iter2 j)
    {
      std::iter_swap(i, j);
    }
    


  // Swap ranges
  template<typename Iter1, typename Iter2>
    inline Iter2 std_swap_ranges(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      return std::swap_ranges(first1, last1, first2);
    }
    
    
  
  // Transform (unary)
  template<typename Iter, typename Out, typename F>
    inline Out std_transform(Iter first, Iter last, Out result, F f)
    {
      return std::transform(first, last, result, f);
    }
    
    
    
  // Transform (binary)
  template<typename Iter1, typename Iter2, typename Out, typename F>
    inline Out std_transform(Iter1 first1, Iter1 last1, Iter2 first2, Out result, F f)
    {
      return std::transform(first1, last1, first2, result, f);
    }



  // Extract
  //
  // Extract the elements of [first, last) that are equal to value by moving 
  // them into the output range. This algorithm is similar to remove_copy, 
  // except that the elements are moved into the output range instead of being 
  // overwritten.
  template<typename Iter, typename Out, typename T>
    std::pair<Iter, Out> extract(Iter first, Iter last, Out result, T const& value)
    {
      static_assert(Forward_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      static_assert(Move_writable<Out, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      if(first != last) 
        assert(( is_movable_range(result, std_count(first, last, value), *first) ));

      first = std::find(first, last, value);
      if(first == last)
        return {first, result};

      *result = std::move(*first);
      ++result;
      Iter hole = first;
      ++first;
      while(first != last) {
        if(*first != value) {
          *hole = std::move(*first);
          ++hole;
        } else {
          *result = std::move(*first);
          ++result;
        }
        ++first;
      }
      return {hole, result};
    }
    
    
    
  // Extract (predicate)
  //
  // Extract the elements of [first, last) by moving them into the output
  // range. This algorithm is similar to remove_if, except that the elements
  // are moved into the output range instead of being overwritten.
  template<typename Iter, typename Out, typename Pred>
    std::pair<Iter, Out> extract_if(Iter first, Iter last, Out result, Pred pred)
    {
      static_assert(Forward_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      static_assert(Move_writable<Out, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      if(first != last)
        assume(( is_movable_range(result, std_count_if(first, last, pred), *first) ));

      first = std::find_if(first, last, pred);
      if(first == last)
        return {first, result};

      *result = std::move(*first);
      ++result;
      Iter hole = first;
      ++first;
      while(first != last) {
        if(!pred(*first)) {
          *hole = std::move(*first);
          ++hole;
        } else {
          *result = std::move(*first);
          ++result;
        }
        ++first;
      }
      return {hole, result};
    }



  // Permutation Generators
  
  // Next permutation
  template<typename Iter>
    inline bool std_next_permutation(Iter first, Iter last)
    {
      static_assert(Bidirectional_iterator<Iter>(), "");
      static_assert(Sortable<Iter>(), "");
      assert(( is_permutable_range(first, last) ));

      return std::next_permutation(first, last);
    }
    
    
    
  // Next permutation (relation)
  template<typename Iter, typename R>
    inline bool std_next_permutation(Iter first, Iter last, R comp)
    {
      static_assert(Bidirectional_iterator<Iter>(), "");
      static_assert(Relation<Value_type<Iter>>(), "");
      assert(( is_permutable_range(first, last) ));
      assert(( strict_weak_ordering(comp) ));
      
      return std::next_permutation(first, last);

    }



  // Next permutation (range)
  template<typename Rng>
    inline bool next_permutation(Rng& range)
    {
      static_assert(Sortable_range<Rng>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return std::next_permutation(std::begin(range), std::end(range));
    }



  // Next permutation (range, relation)
  template<typename Rng, typename R>
    inline bool next_permutation(Rng& range, R comp)
    {
      static_assert(Sortable_range<Rng, R>(), "");
      static_assert(Bidirectional_range<Rng>(), "");
    
      return std::next_permutation(std::begin(range), std::end(range), comp);
    }



  // Previous permutation
  template<typename Iter>
    inline bool std_prev_permutation(Iter first, Iter last)
    {
      static_assert(Bidirectional_iterator<Iter>(), "");
      static_assert(Sortable<Iter>(), "");
      assert(( is_permutable_range(first, last) ));

      return std::prev_permutation(first, last);
    }
    
    
    
  // Previous permutation (relation)
  template<typename Iter, typename R>
    inline bool std_prev_permutation(Iter first, Iter last, R comp)
    {
      static_assert(Bidirectional_iterator<Iter>(), "");
      static_assert(Relation<Value_type<Iter>>(), "");
      assert(( is_permutable_range(first, last) ));
      assert(( strict_weak_ordering(comp) ));
      
      return std::prev_permutation(first, last);

    }



  // Previous permutation (range)
  template<typename Rng>
    inline bool prev_permutation(Rng& range)
    {
      static_assert(Sortable_range<Rng>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return std::prev_permutation(std::begin(range), std::end(range));
    }



  // Previous permutation (range, relation)
  template<typename Rng, typename R>
    inline bool prev_permutation(Rng& range, R comp)
    {
      static_assert(Sortable_range<Rng, R>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return std::prev_permutation(std::begin(range), std::end(range), comp);
    }



  // NOTE: I'm not wild about the name _partial_*. These are often called
  // k-permutations (and similarly k-combination). In general, I would have
  // have preferred next_permutation to increment to k-permutations instead
  // of entire sequences in order to make it look like the combination
  // algorithms below.
  
  // NOTE: The implementation of the *_partial_permutation and the the
  // *_combination algorithms is in algorithm/combination.hpp. The algorithms
  // are separated for licensing issues.

  // Compute the next lexicographical permutation of elements in [first, mid) 
  // from r where first is begin(r). Return false if there is no next 
  // permutation.
  template<typename Rng>
    inline bool next_partial_permutation(Rng& range, Iterator_type<Rng> mid)
    {
      static_assert(Sortable_range<Rng>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return next_partial_permutation(std::begin(range), mid, std::end(range));
    }

  template<typename Rng, typename R>
    inline bool next_partial_permutation(Rng& range, Iterator_type<Rng> mid, R comp)
    {
      static_assert(Sortable_range<Rng, R>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return next_partial_permutation(std::begin(range), mid, std::end(range), comp);
    }


  // Compute the next lexicographical permutation of elements in [first, mid) 
  // from r where first is begin(r). Return false if there is no next 
  // permutation.
  template<typename Rng>
    inline bool prev_partial_permutation(Rng& range, Iterator_type<Rng> mid)
    {
      static_assert(Sortable_range<Rng>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return prev_partial_permutation(std::begin(range), mid, std::end(range));
    }

  template<typename Rng, typename R>
    inline bool prev_partial_permutation(Rng& range, Iterator_type<Rng> mid, R comp)
    {
      static_assert(Sortable_range<Rng, R>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return prev_partial_permutation(std::begin(range), mid, std::end(range), comp);
    }


  // Compute the next lexicographical combination of elements in [first, mid)
  // from the range r where first is equal to begin(r). Return false if there
  // is no next combination.
  template<typename Rng>
    inline bool next_combination(Rng& range, Iterator_type<Rng> mid)
    {
      static_assert(Sortable_range<Rng>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return next_combination(std::begin(range), mid, std::end(range));
    }

  template<typename Rng, typename R>
    inline bool next_combination(Rng& range, Iterator_type<Rng> mid, R comp)
    {
      static_assert(Sortable_range<Rng, R>(), "");
      static_assert(Bidirectional_range<Rng>(), "");

      return next_combination(std::begin(range), mid, std::end(range), comp);
    }


  // Compute the previous lexicographical combination of elements in 
  // [first, mid) from the range r where first is equal to begin(r). Return 
  // false if there is no next combination.
  template<typename Rng>
    inline bool prev_combination(Rng& range, Iterator_type<Rng> mid)
    {
      return prev_combination(std::begin(range), mid, std::end(range));
    }

  template<typename Rng, typename R>
    inline bool prev_combination(Rng& range, Iterator_type<Rng> mid, R comp)
    {
      return prev_combination(std::begin(range), mid, std::end(range), comp);
    }


  // TODO: Implement multiset permutations.
  
  

} // namespace origin

#endif
