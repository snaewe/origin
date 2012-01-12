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

  
  
  // Returns true if the input iterator I can be searched using the predicate P.
  template<typename I, typename P>
    constexpr bool Searchable()
    {
      return Input_iterator<I> && Predicate<P, Value_type<I>>();
    }
    
    

  // Returns true if the input range R can be searched using the predicate P.
  template<typename R, typename Pred>
    constexpr bool Searchable_range()
    {
      return Input_range<R> && Predicate<P, Value_type<I>>();
    }
  
  
  // Find algorithms
  // These algorithms search a range of elements for an element that satisfies
  // some property.
  //
  // The _if family of algorithms search for the first iterator that satisfies
  // some unary predicate.
  
  

  // Find if
  // Returns the first iterator i in [first, last) such that pred(*i) is true.
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
  // Returns the first iterator i in r such that pred(*i) is true.
  template<typename R, typename P>
    inline Iterator_type<R> find_if(R& range, P pred) 
    {
      static_assert(Searchable_range<I, P>(), "");
      
      return std_find_if(std::begin(range), std::end(range), pred);
    }
    
    

  // Find if (const range)
  // Returns the first iterator i in r such that pred(*i) is true.
  template<typename Rng, typename Pred>
    inline Iterator_type<const Rng> find_if(const Rng& r, Pred pred)
    {
      static_assert(Searchable_range<I, P>(), "");
      
      return std_find_if(std::begin(r), std::end(r), pred);
    }
  

  
  // Find if not
  // Returns the first iterator i in [first, last) such that !pred(*i) is true.
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
  // Returns the first iterator i in r such that !pred(*i).
  template<typename R, typename P>
    inline Iterator_type<R> find_if_not(R& r, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return std_find_if_not(std::begin(r), std::end(r), pred);
    }
    
  
  
  // Find if not (const range)
  // Returns the first iterator i in r such that !pred(*i).
  template<typename R, typename P>
    inline Iterator_type<R> find_if_not(const R& r, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return std_find_if_not(std::begin(r), std::end(r), pred);
    }
  


  // Quantifiers
  // There are four quantifiers: all, none, some, nall, and one. Note that
  // some is called "any" in the C++ standard library and nall is a nonce
  // word combining "not" and "all".
  //
  // Note that:
  //   !all == nall
  //   !some == none
  //
  // There are two families of quantifiers. The *_of family evaluates the
  // quantifier for a given unary predicate. The *_equal family evalutates
  // the quantifier with respect to equality to the given value.
  //
  //
  // An interesting discourse on the term "nall" can be found in the 
  // "Operators in the Lexicon: On the Negative Logic of Natural Language" by
  // Dany Jaspers (Chapter 1). The complete text can be found here:
  // http://www.lotpublications.nl/publish/articles/001541/bookpart.pdf
  //
  // NOTE: These quantifiers are easily defined in terms of find/find_if.
  // The table of equivalences is:
  //    all_of  <=> find_if_not == last
  //    nall_of <=> find_if_not != last
  //    some_of <=> find_if != last
  //    none_of <=> find_if == last
  // However, implementing the algorithms in terms of find/find_if incurs one
  // extra iterator comparison, so they are implemented "in the raw".

  // All of
  // Returns true if first == last or pred(x) is true for all elements x in 
  // [first, last).
  template<typename I, typename P>
    inline bool std_all_of(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(!pred(*first))
          return false;
        ++first;
      }
      return true;
    }



  // All of (range)
  // Returns true if empty(r) or pred(x) is true for all elements x in r.
  template<typename R, typename P>
    inline bool all_of(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");

      return std_all_of(std::begin(range), std::end(range), pred);
    }

    
    
  // Nall of
  // Returns true if first != last and pred(x) is true for some element x in 
  // [first, last).
  template<typename I, typename P>
    inline bool nall_of(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(!pred(*first))
          return true;
        ++first;
      }
      return false;
    }
  
  
  
  // Nall of (range)
  // Returns true if !empty(r) and pred(x) is true for soem element x in r.
  template<typename R, typename P>
    inline bool nall_of(const R& r, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");

      return nall_of(std::begin(r), std::end(r), pred);
    }
    
  
  
  // Some of
  // Returns true if first != last and pred(x) is true for some element x in 
  // [first, last).
  template<typename I, typename P>
    inline bool some_of(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(pred(*first))
          return true;
        ++first;
      }
      return false;
    }



  // Any of (range)
  // Returns true if !empty(r) and pred(x) is true for some element x in r.
  template<typename R, typename P>
    inline bool some_of(const Rng& r, Pred pred)
    {
      static_assert(Searchable_range<R, P>(), "");

      return some_of(std::begin(r), std::end(r), pred);
    }



  // None of
  // Returns true if first == last or !pred(x) is true for all elements x in 
  // [first, last).
  template<typename I, typename P>
    inline bool std_none_of(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(pred(*first))
          return false;
        ++first;
      }
      return true;
    }



  // None of (range)
  // Returns true if empty(r) or !pred(x) for all elements x in r.
  template<typename R, typename P>
    inline bool none_of(const R& r, P pred)
    {
      static_assert(Searchable_range<I, P>(), "");
      
      return std_none_of(std::begin(r), std::end(r), pred);
    }
 
 

  // One of
  // Returns true if pred(x) is true for one and only one element x in 
  // [first, last).
  template<typename Iter, typename Pred>
    inline bool one_of(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      
      Iter i = std::find_if(first, last, pred);
      return !any_of(std_next(i), last, pred);
    }
    
  
  
  // One of (range)
  // Returns true if pred(x) is true for one and only one element x in r.
  template<typename Rng, typename Pred>
    inline bool one_of(const Rng& r, Pred pred)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Predicate<Pred, Value_type<Rng>>(), "");
      
      return one_of(std::begin(r), std::end(r), pred);
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

      return all_equal(std::begin(r), std::end(r), value);
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
    inline bool any_equal(const Rng& r, const T& value)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Equality_comparable<Value_type<Rng>, T>(), "");

      return any_equal(std::begin(r), std::end(r), value);
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

    
    
  // One equal
  // Returns true if x == value for exactly one element x in [first, last).
  template<typename Iter, typename T>
    inline bool one_equal(Iter first, Iter last, const T& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      Iter i = std::find(first, last, value);
      return !any_equal(std_next(i), last, value);
    }
    
   
   
  // One equal (range)
  template<typename Rng, typename T>
    inline bool one_equal(const Rng& r, const T& value)
    {
      static_assert(Input_range<Rng>(), "");
      static_assert(Equality_comparable<Value_type<Rng>, T>(), "");

      return one_equal(std::begin(r), std::end(r), value);
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
    
    
    


    
  // Find next
  // Returns the next next iterator i in [first, last) past first where 
  // *i == value.
  //
  // Note that there is no corresponding range version of this function. The
  // operation is only intended to be applied to iterators.
  template<typename Iter, typename T>
    inline Iter find_next(Iter first, Iter last, const T& value)
    {
      if(first != last)
        return std::find(std_next(first), last, value);
      else
        return last;
    }



  // Find next if
  // Return the next element in [first, last) that satisfies pred, or last if
  // no such element exists.
  //
  // Note that there is no corresponding range version of this function. The
  // operation is only intended to be applied to iterators.
  template<typename Iter, typename Pred>
    inline Iter find_next_if(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      if(first != last)
        return std::find_if(std_next(first), last, pred);
      else
        return last;
    }
    
    

  // Find nth
  // Return the nth iterator i in [first, last) where *i == value or last if
  // there are fewer than n elements equal to value.
  template<typename Iter, typename T>
    inline Iter find_nth(Iter first, Iter last, Distance_type<Iter> n, const T& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
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
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");
      
      return find_nth(std::begin(range), std::end(range), n, value);
    }
    
    
  
  // Find nth (const range)
  // Return an iterator to the nth element in r that is equal to value.
  template<typename R, typename T>
    inline Iterator_type<R const> find_nth(R const& range, Distance_type<R> n, T const& value)
    {
      static_assert(Input_range<R const>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");
      
      return find_nth(std::begin(range), std::end(range), n, value);
    }



  // Find nth if
  // Return the nth iterator i in [first, last) where pred(*i) is true or last 
  // if there are fewer than n elements equal to value.
  template<typename Iter, typename Pred>
    Iter find_nth_if(Iter first, Iter last, Distance_type<Iter> n, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));
      
      while(first != last && n != 0) {
        if(pred(*first))
          --n;
        ++first;
      }
      return first;
    }
    
    
  
  // Find nth if (range)
  // Return an iterator to the nth element in r that satisfies pred, or return
  // last if there is no such element.
  template<typename R, typename Pred>
    inline Iterator_type<R> find_nth_if(R& range, Distance_type<R> n, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return find_nth_if(std::begin(range), std::end(range), n, pred);
    }
    
  
  
  // Find nth if (const range)
  template<typename R, typename Pred>
    inline Iterator_type<R const> find_nth_if(R const& range, Distance_type<R> n, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      
      return find_nth_if(std::begin(range), std::end(range), n, pred);
    }
  

  
  // Find first of
  // Returns an iterator i in [first1, last1) that is equal to any of the 
  // elements in [first2, last2).
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



  // Find first of (range)
  // Returns the first iterator i in r1 that is equal to any of the elements in 
  // r2.
  template<typename Rng1, typename Rng2>
    inline Iterator_type<Rng1> find_first_of(Rng1& r1, const Rng2& r2)
    {
      static_assert(Input_range<Rng1>(), "");
      static_assert(Input_range<Rng2>(), "");
      static_assert(Equality_comparable<Value_type<Rng1>, Value_type<Rng2>>(), "");
      
      return std_find_first_of(std::begin(r1), std::end(r1),
                               std::begin(r2), std::end(r2));
    }
  
  
  
  // Find first of (const range)
  // Returns an iterator i in r1 that is equal to any of the elements in r2.
  template<typename Rng1, typename Rng2>
    inline Iterator_type<const Rng1> find_first_of(const Rng1& r1, const Rng2& r2)
    {
      static_assert(Input_range<Rng1>(), "");
      static_assert(Input_range<Rng2>(), "");
      static_assert(Equality_comparable<Value_type<Rng1>, Value_type<Rng2>>(), "");
      
      return std_find_first_of(std::begin(r1), std::end(r1),
                               std::begin(r2), std::end(r2));
    }
    
    
  
  // Find first of (relation)
  // Returns the first iterator i in [first1, last1) such that comp(*i, *j) is 
  // true where j is any iterator in [first2, last2).
  template<typename Iter1, typename Iter2, typename R>
    inline Iter1 std_find_first_of(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, R comp)
    {
      static_assert(Input_iterator<Iter1>(), "");
      static_assert(Forward_iterator<Iter2>(), "");
      static_assert(Relation<R, Value_type<Iter1>, Value_type<Iter2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));
      
      return std::find_first_of(first1, last1, first2, last2, comp);
    }



  // Find first of (relation, range)
  // Returns the first iterator i in r1 such that comp(*i, *j) is true where j 
  // is any iterator in r2.
  template<typename Rng1, typename Rng2, typename R>
    inline Iterator_type<Rng1> find_first_of(Rng1& r1, const Rng2& r2, R comp)
    {
      static_assert(Input_range<Rng1>(), "");
      static_assert(Input_range<Rng2>(), "");
      static_assert(Relation<R, Value_type<Rng1>, Value_type<Rng2>>(), "");
      
      return std_find_first_of(std::begin(r1), std::end(r1),
                               std::begin(r2), std::end(r2), comp);
    }
    
    
  
  // Find first of (relation, const range)
  // Returns the first iterator i in r1 such that comp(*i, *j) is true where j 
  // is any iterator in r2.
  template<typename Rng1, typename Rng2, typename R>
    inline Iterator_type<const Rng1> find_first_of(const Rng1& r1, const Rng2& r2, R comp)
    {
      static_assert(Input_range<Rng1>(), "");
      static_assert(Input_range<Rng2>(), "");
      static_assert(Relation<R, Value_type<Rng1>, Value_type<Rng2>>(), "");
      
      return std_find_first_of(std::begin(r1), std::end(r1),
                               std::begin(r2), std::end(r2), comp);
    }
  
  
  
  // FIXME: Document adjacent find, etc.
  
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
    inline Iterator_type<const R> adjacent_find(const R& range)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>>(), "");

      return std_adjacent_find(std::begin(range), std::end(range));
    }

    
    
  // Count
  // Returns the number of elements x in [first, last) where x == value.
  template<typename Iter, typename T>
    inline Distance_type<Iter> std_count(Iter first, Iter last, T const& value)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Equality_comparable<Value_type<Iter>, T>(), "");
      assert(( is_readable_range(first, last) ));

      return std::count(first, last, value);
    }
    
    

  // Count (range)
  // Returns the number of elements x in r where x == value.
  template<typename R, typename T>
    inline Distance_type<R> count(R const& range, T const& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");

      return std_count(std::begin(range), std::end(range), value);
    }
   
   
   
  // Count if
  // Returns the number of elements x in [first, last) where pred(x) is true.
  template<typename Iter, typename Pred>
    inline Distance_type<Iter> std_count_if(Iter first, Iter last, Pred pred)
    {
      static_assert(Input_iterator<Iter>(), "");
      static_assert(Predicate<Pred, Value_type<Iter>>(), "");
      assert(( is_readable_range(first, last) ));

      return std::count_if(first, last, pred);
    }
   
   
   
  // Count if (range)
  // Returns the number of elements x in r where pred(x) is true.
  template<typename R, typename Pred>
    inline Distance_type<R> count_if(R const& range, Pred pred)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Predicate<Pred, Value_type<R>>(), "");
      return std_count_if(std::begin(range), std::end(range), pred);
    }

    
    
  // Count not equal
  // Returns the number of elements x in [first, last) where x != value.
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

    
    
  // Count not equal (range)
  // Returns the number of elements x in r where x != value.
  template<typename R, typename T>
    inline Distance_type<R> count_not_equal(R const& range, T const& value)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Equality_comparable<Value_type<R>, T>(), "");
      
      return count_not_equal(std::begin(range), std::end(range), value);
    }
    
    
  
  // Count if not
  // Returns the number of elements x in [first, last) where !pred(x)
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

  
  
  // Count if not (range)
  // Returns the number of elements x in r where !pred(x).
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
      return std::lower_bound(first, last, value);
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
