// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_QUANTIFIER_HPP
#define ORIGIN_ALGORITHM_QUANTIFIER_HPP

#include <algorithm>

namespace origin
{
  // Quantifiers
  // The quantifier algorithms evaluate properties of a range of elements.
  // There are five quantifiers: all, none, some, non_all, and one. Note that
  // some is called "any" in the C++ standard library.
  //
  // Note that:
  //    !all == not_all (obviously)
  //    !some == none
  //    !one == some || none
  //
  // These algorithms in this module evaluate a unary predicate over a range
  // of elements.
  //
  //    all_of(first, last, pred)
  //    all_of(range, pred)
  //    all_of(list, pred)
  //    not_all_of(first, last, pred)
  //    not_all_of(range, pred)
  //    not_all(list, pred)
  //    some_of(first, last, pred)
  //    some_of(range, pred)
  //    some_of(list, pred)
  //    none_of(first, last, pred)
  //    none_of(range, pred)
  //    none_of(list, pred)
  //    one_of(first, last, pred)
  //    one_of(range, pred)
  //    one_of(list, pred)
  //
  // These algorithms evaluate the equality of elements to some value.
  //
  //    all_equal(first, last, pred)
  //    all_equal(range, pred)
  //    all_equal(list, pred)
  //    not_all_equal(first, last, pred)
  //    not_all_equal(range, pred)
  //    not_all_equal(list, pred)
  //    some_equal(first, last, pred)
  //    some_equal(range, pred)
  //    some_equal(list, pred)
  //    none_equal(first, last, pred)
  //    none_equal(range, pred)
  //    none_equal(list, pred)
  //    one_equal(first, last, pred)
  //    one_equal(range, pred)
  //    one_equal(list, pred)
  //
  // Note that these algorithms are easily defined in terms of find/find_if.
  // The table of equivalences is:
  //    all_of     <=>  find_if_not == last
  //    not_all_of <=>  find_if_not != last
  //    some_of    <=>  find_if != last
  //    none_of    <=>  find_if == last
  // However, implementing the algorithms in terms of find/find_if incurs one
  // extra iterator comparison, so they are implemented "in the raw". Another
  // reason is that some "advanced" find algorithms can make use of these
  // quantifiers to simplify search implementations (e.g., find_first_of).
  // Breaking that dependency simplifies the organization of the library.


  template<typename I, typename T> I std_find(I, I, const T&);
  template<typename I, typename P> I std_find_if(I, I, P);


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
  // Returns true if empty(range) or pred(x) is true for all elements x in 
  // range.
  template<typename R, typename P>
    inline bool all_of(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");

      return std_all_of(std::begin(range), std::end(range), pred);
    }



  // All of (list)
  // Returns true if empty(list) or pred(x) is true for all elements x in list.
  template<typename T, typename P>
    inline bool all_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return std_all_of(list.begin(), list.end(), pred);
    }
    
    
  // Not all of
  // Returns true if first != last and pred(x) is true for some element x in 
  // [first, last).
  template<typename I, typename P>
    inline bool not_all_of(I first, I last, P pred)
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
  
  
  
  // Not all of (range)
  // Returns true if !empty(range) and pred(x) is true for soem element x in 
  // range.
  template<typename R, typename P>
    inline bool not_all_of(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");

      return nall_of(std::begin(range), std::end(range), pred);
    }
    


  // Not all of (list)
  // Returns true if !empty(list) or !pred(x) is true for some elements x in 
  // list.
  template<typename T, typename P>
    inline bool not_all_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return not_all_of(list.begin(), list.end(), pred);
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



  // Some of (range)
  // Returns true if !empty(range) and pred(x) is true for some element x in 
  // range.
  template<typename R, typename P>
    inline bool some_of(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");

      return some_of(std::begin(range), std::end(range), pred);
    }



  // Some of (list)
  // Returns true !empty(list) and pred(x) fis true for some element x in list.
  template<typename T, typename P>
    inline bool some_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");
      return some_of(list.begin(), list.end(), pred);
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
  // Returns true if empty(range) or !pred(x) for all elements x in range.
  template<typename R, typename P>
    inline bool none_of(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return std_none_of(std::begin(range), std::end(range), pred);
    }
 
 

  // None of (list)
  // Returns true if empty(list) or !pred(x) is true for all elements x in list.
  template<typename T, typename P>
    inline bool none_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return std_none_of(list.begin(), list.end(), pred);
    }



  // One of
  // Returns true if pred(x) is true for one and only one element x in 
  // [first, last).
  template<typename I, typename P>
    inline bool one_of(I first, I last, P pred)
    {
      static_assert(Searchable<I, P>(), "");
      assert(( is_readable_range(first, last) ));
      
      first = std_find_if(first, last, pred);
      if(first != last)
        return none_of(++first, last, pred);
      else
        return false;
    }
    
  
  
  // One of (range)
  // Returns true if pred(x) is true for one and only one element x in r.
  template<typename R, typename P>
    inline bool one_of(const R& range, P pred)
    {
      static_assert(Searchable_range<R, P>(), "");
      
      return one_of(std::begin(range), std::end(range), pred);
    }
 


  // One of (list)
  // Returns true if empty(list) or pred(x) is true for one and only one 
  // elements x in list.
  template<typename T, typename P>
    inline bool one_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return one_of(list.begin(), list.end(), pred);
    }

 
 
  // All equal
  // Returns true first == last or x == value for all elements x in 
  // [first, last).
  template<typename I, typename T>
    bool all_equal(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");
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
  template<typename R, typename T>
    inline bool all_equal(const R& range, const T& value)
    {
      static_assert(Value_searchable_range<R, T>(), "");

      return all_equal(std::begin(range), std::end(range), value);
    }
    
    
  
  // All equal (list)
  // Returns true if x == value for all elementx x in list.
  template<typename T, typename P>
    inline bool all_equal(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return all_equal(list.begin(), list.end(), pred);
    }
     
 

  // Not all equal
  // Returns true x != value for some element x in [first, last).
  template<typename I, typename T>
    bool not_all_equal(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(*first != value)
          return true;
        ++first;
      }
      return false;
    }
   
   
   
  // Not all equal (range)
  // Returns true x != value for some element x in range.
  template<typename R, typename T>
    inline bool not_all_equal(const R& range, const T& value)
    {
      static_assert(Value_searchable_range<R, T>(), "");

      return not_all_equal(std::begin(range), std::end(range), value);
    }
    
    
  
  // Not all equal (list)
  // Returns true x != value for some element x in list.
  template<typename T, typename P>
    inline bool not_all_equal(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return not_all_equal(list.begin(), list.end(), pred);
    }

     
        
  // Some equal
  // Returns true if x == value for some element x in [first, last).
  template<typename I, typename T>
    bool some_equal(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(*first == value)
          return true;
      }
      return false;
    }



  // Some equal (range)
  // Returns true if x == value for some element x in range.
  template<typename R, typename T>
    inline bool some_equal(const R& range, const T& value)
    {
      static_assert(Value_searchable_range<R, T>(), "");

      return some_equal(std::begin(range), std::end(range), value);
    }



  // Some equal (list)
  // Returns true if x == value for some element x in list.
  template<typename T, typename P>
    inline bool some_equal(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return some_equal(list.begin(), list.end(), pred);
    }
    
  
    
  // None equal
  // Returns true if x != value for all elements x in [first, last).
  template<typename I, typename T>
    bool none_equal(I first, I last, T const& value)
    {
      static_assert(Value_searchable<I, T>(), "");
      assert(( is_readable_range(first, last) ));

      while(first != last) {
        if(*first == value)
          return false;
      }
      return true;
    }
    
    
  
  // None equal (range)
  // Returns true if x != value for all elements x in range.
  template<typename R, typename T>
    inline bool none_equal(const R& range, const T& value)
    {
      static_assert(Value_searchable_range<R, T>(), "");

      return none_equal(std::begin(range), std::end(range), value);
    }



  // None equal (list)
  // Returns true if x != value for all elements x in list.
  template<typename T, typename P>
    inline bool none_equal(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return none_equal(list.begin(), list.end(), pred);
    }
    
    
    
  // One equal
  // Returns true if x == value for exactly one element x in [first, last).
  template<typename I, typename T>
    inline bool one_equal(I first, I last, const T& value)
    {
      static_assert(Value_searchable<I, T>(), "");
      assert(( is_readable_range(first, last) ));

      first = find(first, last, value);
      if(first != last)
        return none_equal(++first, last, value);
      else
        return false;
    }
    
   
   
  // One equal (range)
  // Returns true if x == value for one and only one element x in range.
  template<typename R, typename T>
    inline bool one_equal(const R& range, const T& value)
    {
      static_assert(Value_searchable_range<R, T>(), "");

      return one_equal(std::begin(range), std::end(range), value);
    }



  // One equal (list)
  // Returns true if x == value for one and only one elements x in list.
  template<typename T, typename P>
    inline bool one_equal(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return one_equal(list.begin(), list.end(), pred);
    }

} // namespace origin

#endif
