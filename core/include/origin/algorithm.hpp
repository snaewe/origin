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
  // Returns true if first == last or pred(x) is true for all elements x in 
  // [first, last).
  template<typename Iter, typename Pred>
    inline bool std_all_of(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      
      return std::all_of(first, last, pred);
    }



  // All of (range)
  // Return true empty(r) or pred(x) is true for all elements x in r.
  template<typename Rng, typename Pred>
    inline bool all_of(const Rng& r, Pred pred)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Predicate<Pred, Value_type<Rng>>(), "");

      return std_all_of(std::begin(r), std::end(r), pred);
    }
    
    
  
  // Any of
  // Returns true if first != last and pred(x) is true for some element x in 
  // [first, last).
  template<typename Iter, typename Pred>
    inline bool std_any_of(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      return std::any_of(first, last, pred);
    }



  // Any of (range)
  // Returns true if !empty(r) and pred(x) is true for some element x in r.
  template<typename Rng, typename Pred>
    inline bool any_of(const Rng& r, Pred pred)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Predicate<Pred, Value_type<Rng>>(), "");

      return std_any_of(std::begin(r), std::end(r), pred);
    }



  // Any not of
  // Returns true first != last and !pred(x) is true for some element x in r.
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
  
  
  
  // Any not of (range)
  // Returns true !empty(r) and !pred(x) for some element x in r.
  template<typename Rng, typename Pred>
    inline bool any_not_of(const Rng& r, Pred pred)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Predicate<Pred, Value_type<Rng>>(), "");
      
      return std::any_not_of(std::begin(r), std::end(r), pred);
    }



  // None of
  // Returns true if first == last or !pred(x) is true for all elements x in 
  // [first, last).
  template<typename Iter, typename Pred>
    inline bool std_none_of(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      
      return std::none_of(first, last, pred);
    }



  // None of (range)
  // Returns true if empty(r) or !pred(x) for all elements x in r.
  template<typename Rng, typename Pred>
    inline bool none_of(const Rng& range, Pred pred)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Predicate<Pred, Value_type<Rng>>(), "");
      
      return std_none_of(std::begin(r), std::end(r), pred);
    }
 
 
 
  // All equal
  // Returns true first == last or x == value for all elements x in 
  // [first, last).
  template<typename Iter, typename T>
    bool all_equal(Iter first, Iter last, const T& value)
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
   
   
   
  // All equal (range)
  // Returns true if empty(r) or x == value for all elements x in r.
  template<typename Rng, typename T>
    inline bool all_equal(const Rng& r, const T& value)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Equality_comparable<Value_type<Rng>, T>(), "");

      return std::all_equal(std::begin(r), std::end(r), value);
    }
    
    
    
  // Any equal
  // Returns true if first != last and x == value for some element x in 
  // [first, last).
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



  // Any equal (range)
  // Returns true if !empty(r) and x == value for some element x in r.
  template<typename Rng, typename T>
    inline bool any_equal(const R& r, const T& value)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Equality_comparable<Value_type<Rng>, T>(), "");

      return std::any_equal(std::begin(r), std::end(r), value);
    }
    
    
    
  // Any not equal
  // Returns true if first != last and x != value for some element x in 
  // [first, last).
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
    
    
    
  // Any not of (range)
  // Returns true if !empty(r) and x != value for some element x in r.
  template<typename R, typename T>
    inline bool any_not_equal(const R& r, const T& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");

      return any_not_equal(std::begin(r), std::end(r), value);
    }
    
    
    
  // None equal
  // Returns true if first == last or x != value for all elements x in 
  // [first, last).
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
    
    
  
  // None equal (range)
  // Returns true if !empty(r) or x != value for all elements x in r.
  template<typename Rng, typename T>
    inline bool none_equal(const Rng& r, const T& value)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Equality_comparable<Value_type<Rng>, T>(), "");

      return none_equal(std::begin(r), std::end(r), value);
    }



  // Find algorithms
  // These algorithms search a range of elements for an element that satisfies
  // some property.
  
  
  
  // Find
  // Return the first iterator i in [first, last) such that *i == value or
  // last if no such iterator exists.
  template<typename Iter, typename T>
    Iter std_find(Iter first, Iter last, const T& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      return std::find(first, last, value);
    }
    
  
  
  // Find (range)
  // Returns the first iterator i in r such that *i == value or end(r) if no
  // such iterator exists.
  template<typename Rng, typename T>
    auto find(Rng& r, const T& value)
      -> Requires<Input_range<Rng>() && !Associative_container<Rng>(), Iterator_type<Rng>>
    {
      static_assert(Equality_comparable<Value_type<Rng>, T>(), "");
      
      return std_find(std::begin(r), std::end(r), value);
    }



  // Find (const range)
  // Returns the first iterator i in r such that *i == value or end(r) if no
  // such iterator exists.
  template<typename Rng, typename T>
    auto find(const Rng& r, const T& value)
      -> Requires<Input_range<Rng>() && !Associative_container<Rng>(), Iterator_type<const Rng>>
    {
      static_assert(Equality_comparable<Value_type<Rng>, T>(), "");

      return std_find(std::begin(r), std::end(r), value);
    }
    
    
    
  // Find (associative container)
  // Returns the first iterator i in c such that *i == value or end(c) if no
  // such iterator exists. The result is comptued in O(log(size(c))).
  template<typename C, typename T>
    auto find(C& c, const T& x) 
      -> Requires<Associative_container<C>(), Iterator_type<C>>
    {
      return c.find(x);
    }



  // Find (const associative container)
  // Returns the first iterator i in c such that *i == value or end(c) if no
  // such iterator exists. The result is comptued in O(log(size(c))).
  template<typename C, typename T>
    auto find(const C& c, T const& x) 
      -> Requires<Associative_container<C>(), Iterator_type<const C>>
    {
      return c.find(x);
    }
    
    
    
  // Find_if
  // Returns the first iterator i in [first, last) such that pred(*i) is true.
  template<typename Iter, typename Pred>
    inline Iter std_find_if(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      return std::find_if(first, last, pred);
    }



  // Find_if (range)
  // Returns the first iterator i in r such that pred(*i) is true.
  template<typename R, typename Pred>
    inline Iterator_type<R> find_if(R& range, Pred pred) 
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return std_find_if(std::begin(range), std::end(range), pred);
    }
    
    

  // Find_if (const range)
  // Returns the first iterator i in r such that pred(*i) is true.
  template<typename R, typename Pred>
    inline Iterator_type<R const> find_if(R const& range, Pred pred)
    {
      static_assert(Input_range<R const>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return std_find_if(std::begin(range), std::end(range), pred);
    }



  // Find first and next
  // The first_equal and next_equal algorithms can be used to iterate over
  // subranges of elements that are equal to a given value. Similarly, the
  // first_if and next_if algorithms can be used to iterate over subranges of
  // elements that satisfy a given predicate. These are useful for filtering
  // the elements of a range.
  
  

  // First equal
  // Returns the first iterator i in [first, last) such that *i == value or
  // last if no such iterator exists. 
  //
  // This is equivalent to find(first, last, value).
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

    
    
  // Fill
  template<typename Out, typename T>
    inline void std_fill(Out first, Out last, const T& value)
    {
      return std::fill(first, last, value);
    }
    
    
    
  // Fill n
  template<typename Out, typename T>
    inline Out std_fill_n(Out first, Distance_type<Out> n, const T& value)
    {
      return std::fill(first, n, value);
    }
    
    
    
  // Generate
  template<typename Out, typename F>
    inline F std_generate(Out first, Out last, F gen)
    {
      std::generate(first, last, gen);
      return gen;
    }
    
    
    
  // Generate n
  template<typename Out, typename F>
    inline std::pair<Out, F> std_generate_n(Out first, Distance_type<Out> n, F gen)
    {
      auto result = std::generate(first, n, gen);
      return {result, gen};
    }
  
  
  
  // The Replace Family

  // Replace
  template<typename Iter, typename T>
    inline void std_replace(Iter first, Iter last, const T& old_value, const T& new_value)
    {
      return std::replace(first, last, old_value, new_value);
    }
    
  

  // Replace_if
  template<typename Iter, typename Pred, typename T>
    inline void std_replace_if(Iter first, Iter last, Pred pred, const T& new_value)
    {
      return std::replace_if(first, last, pred, new_value);
    }
    
    
  
  // Replace copy
  template<typename Iter, typename Out, typename T>
    inline Out std_replace_copy(Iter first, Iter last, Out result, const T& old_value, const T& new_value)
    {
      return std::replace_copy(first, last, result, old_value, new_value);
    }
    
    
  
  // Replace copy if
  template<typename Iter, typename Out, typename Pred, typename T>
    inline Out std_replace_copy(Iter first, Iter last, Out result, Pred pred, const T& new_value)
    {
      return std::replace_copy_if(first, last, result, pred, new_value);
    }



  // The Remove Family

  // Remove
  template<typename Iter, typename T>
    Iter std_remove(Iter first, Iter last, const T& value)
    {
      return std::remove(first, last, value);
    }
  
  
  
  // Remove if
  template<typename Iter, typename Pred>
    Iter std_remove_if(Iter first, Iter last, Pred pred)
    {
      return std::remove_if(first, last, pred);
    }
    
    
  // Remove copy  
  template<typename Iter, typename Out, typename T>
    Out std_remove_copy(Iter first, Iter last, Out result, const T& value)
    {
      return std::remove_copy(first, last, result, value);
    }
    
  
  
  // Remove copy if
  template<typename Iter, typename Out, typename Pred>
    Out std_remove_copy_if(Iter first, Iter last, Out result, Pred pred)
    {
      return std::remove_copy_if(first, last, result, pred);
    }
    
  
  
  // Extract
  //
  // Extract the elements of [first, last) that are equal to value by moving 
  // them into the output range [result, last - first). The algorithm returns
  // a pair {new_last, result_last} where the range [first, new_last) is a
  // bounded range containing the elements not extracted and the output range
  // [result, result_last) contains the extracted elements.
  template<typename Iter, typename Out, typename T>
    std::pair<Iter, Out> extract(Iter first, Iter last, Out result, T const& value)
    {
      static_assert(Forward_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      static_assert(Move_writable<Out, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      assume(( is_movable_range(result, std_count(first, last, value), *first) ));

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
    
    
    
  // Remove move if
  //
  // Remove the elements of [first, last) that satisfy pred by moving them
  // into the output range [result, last - first). The algorithm returns
  // a pair {new_last, result_last} where the range [first, new_last) is a
  // bounded range containing the elements not extracted and the output range
  // [result, result_last) contains the extracted elements.
  template<typename Iter, typename Out, typename Pred>
    std::pair<Iter, Out> extract_if(Iter first, Iter last, Out result, Pred pred)
    {
      static_assert(Forward_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      static_assert(Move_writable<Out, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
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



  // The Unique Family
  
  // Unique
  template<typename Iter>
    Iter std_unique(Iter first, Iter last)
    {
      return std::unique(first, last);
    }
    
  
  
  // Unique (predicate)
  template<typename Iter, typename R>
    Iter std_unique(Iter first, Iter last, R comp)
    {
      return std::unique(first, last, comp);
    }
    
    
  
  // Unique copy
  template<typename Iter, typename Out>
    Out std_unique_copy(Iter first, Iter last, Out result)
    {
      return std::unique(first, last, result);
    }
    
    
    
  // Unique copy (predicate)
  template<typename Iter, typename Out, typename R>
    Out std_unique_copy(Iter first, Iter last, Out result, R comp)
    {
      return std::unique_copy(first, last, result, comp);
    }



  // Reverse and Rotate Permutations
  // The reverse and rotate algorithms define permutations of a sequence.
  //
  // TODO: Implement a (perfect) riffle algorithm.

  // Reverse
  template<typename Iter>
    void std_reverse(Iter first, Iter last)
    {
      std::reverse(first, last);
    }
  
  
  
  // Reverse copy
  template<typename Iter, typename Out>
    void std_reverse_copy(Iter first, Iter last, Out result)
    {
      std::reverse(first, last, result);
    }
    
    

  // Rotate
  template<typename Iter>
    Iter std_rotate(Iter first, Iter mid, Iter last)
    {
      return std::rotate(first, mid, last);
    }
    
  
  
  // Rotate copy
  template<typename Iter, typename Out>
    Iter std_rotate_copy(Iter first, Iter mid, Iter last, Out result)
    {
      return std::rotate_copy(first, mid, last, result);
    }



  // Random Permutations
  // This family of algorithmsm randomly permute a sequence of elements by
  // shuffling them.
  //
  // TODO: Implement a randomized riffle algorithm.

  // Random shuffle
  template<typename Iter>
    void std_random_shuffle(Iter first, Iter last)
    {
      return std::random_shuffle(first, last);
    }
    
  
  
  // Random shuffle (generator)
  template<typename Iter, typename Gen>
    void std_random_shuffle(Iter first, Iter last, Gen&& rand)
    {
      return std::random_shuffle(first, last, rand);
    }
    
  
  
  // Shuffle
  template<typename Iter, typename Gen>
    void std_shuffle(Iter first, Iter last, Gen&& rand)
    {
      return std::shuffle(first, last, rand);
    }
    

  
  // Partitions
  // This family of algorithms deals with partitions of a sequence.
  
  // Is partitioned
  template<typename Iter, typename Pred>
    bool std_is_partitioned(Iter first, Iter last, Pred pred)
    {
      return std::is_partitioned(first, last, pred);
    }
    
  
  
  // Partition point
  template<typename Iter, typename Pred>
    Iter std_partition_point(Iter first, Iter last, Pred pred)
    {
      return std::partition_point(first, last, pred);
    }
    
  
  
  // Paritition
  template<typename Iter, typename Pred>
    Iter std_partition(Iter first, Iter last, Pred pred)
    {
      return std::partition(first, last, pred);
    }
  
  
  
  // Stable partition
  template<typename Iter, typename Pred>
    Iter std_stable_partition(Iter first, Iter last, Pred pred)
    {
      return std::stable_partition(first, last, pred);
    }
    
    
  
  // Partition copy
  template<typename Iter, typename Out1, typename Out2, typename Pred>
    std::pair<Out1, Out2> std_partition_copy(Iter first, Iter last, 
                                             Out1 out_true, Out2 out_false, 
                                             Pred pred)
    {
      return std::partition_copy(first, last, out_true, out_false, pred);
    }



  // Sorting
  // This family of algorithms deals with the ordering of the elements of
  // a sequence.
  
  // Is sorted
  template<typename Iter>
    bool std_is_sorted(Iter first, Iter last)
    {
      return std::is_sorted(first, last);
    }
  
  
  
  // Is sorted (relation)
  template<typename Iter, typename Ord>
    bool std_is_sorted(Iter first, Iter last, Ord comp)
    {
      return std::is_sorted(first, last, comp);
    }



  // Is sorted until
  template<typename Iter>
    Iter std_is_sorted_until(Iter first, Iter last)
    {
      return std::is_sorted(first, last);
    }
  
  
  
  // Is sorted until (relation)
  template<typename Iter, typename Ord>
    Iter std_is_sorted_until(Iter first, Iter last, Ord comp)
    {
      return std::is_sorted(first, last, comp);
    }



  // Sort
  template<typename Iter>
    void std_sort(Iter first, Iter last)
    {
      return std::sort(first, last);
    }
    
    
    
  // Sort (relation)
  template<typename Iter, typename Ord>
    void std_sort(Iter first, Iter last, Ord comp)
    {
      return std::sort(first, last, comp);
    }
    
    
  
  // Stable sort
  template<typename Iter>
    void std_stable_sort(Iter first, Iter last)
    {
      return std::stable_sort(first, last);
    }
    
    
    
  // Stable sort (relation)
  template<typename Iter, typename Ord>
    void std_stable_sort(Iter first, Iter last, Ord comp)
    {
      return std::stable_sort(first, last, comp);
    }



  // Partial sort
  template<typename Iter>
    void std_partial_sort(Iter first, Iter middle, Iter last)
    {
      return std::partial_sort(first, last);
    }
    
    
    
  // Partial sort (relation)
  template<typename Iter, typename Ord>
    void std_partial_sort(Iter first, Iter middle, Iter last, Ord comp)
    {
      return std::partial_sort(first, last, comp);
    }
    
    

  // Partial sort copy
  template<typename Iter1, typename Iter2>
    void std_partial_sort_copy(Iter1 first, Iter1 last, 
                               Iter2 result_first, Iter2 result_last)
    {
      return std::partial_sort_copy(first, last, result_first, result_last);
    }
    
    
    
  // Partial sort copy (relation)
  template<typename Iter1, typename Iter2, typename Ord>
    void std_partial_sort_copy(Iter1 first, Iter1 last, 
                               Iter2 result_first, Iter2 result_last, Ord comp)
    {
      return std::partial_sort_copy(first, last, result_first, result_last, comp);
    }



  // Nth element
  template<typename Iter>
    void std_nth_element(Iter first, Iter last)
    {
      return std::nth_element(first, last);
    }



  // Nth element (relation)
  template<typename Iter, typename Ord>
    void std_nth_element(Iter first, Iter last, Ord ord)
    {
      return std::nth_element(first, last);
    }
    
    

  // Binary search
  // This family of algorithms is admitted by sorted sequences.
  
  // Lower bound
  template<typename Iter, typename T>
    Iter std_lower_bound(Iter first, Iter last, const T& value)
    {
      retrn std::lower_bound(first, last, value);
    }
    
  
  
  // Lower bound (relation)
  template<typename Iter, typename T, typename Ord>
    Iter std_lower_bound(Iter first, Iter last, const T& value, Ord comp)
    {
      return std::lower_bound(first, last, value, comp);
    }



  // Upper bound
  template<typename Iter, typename T>
    Iter std_upper_bound(Iter first, Iter last, const T& value)
    {
      return std::upper_bound(first, last, value);
    }
    
  
  
  // Upper bound (relation)
  template<typename Iter, typename T, typename Ord>
    Iter std_upper_bound(Iter first, Iter last, const T& value, Ord comp)
    {
      return std::upper_bound(first, last, value, comp);
    }



  // Equal range
  template<typename Iter, typename T>
    Iter std_equal_range(Iter first, Iter last, const T& value)
    {
      return std::equal_range(first, last, value);
    }
    
  
  
  // Equal range (relation)
  template<typename Iter, typename T, typename Ord>
    Iter std_equal_range(Iter first, Iter last, const T& value, Ord comp)
    {
      return std::equal_range(first, last, value, comp);
    }
    
  
  
  // Binary search
  template<typename Iter, typename T>
    Iter std_binary_search(Iter first, Iter last, const T& value)
    {
      return std::binary_search(first, last, value);
    }
    
  
  
  // Binary search (relation)
  template<typename Iter, typename T, typename Ord>
    Iter std_binary_search(Iter first, Iter last, const T& value, Ord comp)
    {
      return std::binary_search(first, last, value, comp);
    }

  

  // Permutation Generators
  // These algorithms iterate through successive permutations of a sequence.
  
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
