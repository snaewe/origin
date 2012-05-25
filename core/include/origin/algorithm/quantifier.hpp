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
  //    not_all_of(first, last, pred)
  //    some_of(first, last, pred)
  //    none_of(first, last, pred)
  //    one_of(first, last, pred)
  //
  // TODO: Consider providing overloads like all_of(first, last) where the
  // predicate simply converts *i to bool.
  //
  // These algorithms evaluate the equality of elements to some value. 
  //
  //    all_equal(first, last, value)
  //    not_all_equal(first, last, value)
  //    some_equal(first, last, value)
  //    none_equal(first, last, value)
  //    one_equal(first, last, value)
  //
  // And all of those are generalized over the relation used to compare the
  // the values.
  //
  //    all_equal(first, last, value, comp)
  //    not_all_equal(first, last, value, comp)
  //    some_equal(first, last, value, comp)
  //    none_equal(first, last, value, comp)
  //    one_equal(first, last, value, comp)
  //
  // We also support range interfaces for these algorithsm.
  //
  //    all_of(range, pred)
  //    not_all_of(range, pred)
  //    some_of(range, pred)
  //    none_of(range, pred)
  //    one_of(range, pred)
  //
  //    all_equal(range, value)
  //    not_all_equal(range, value)
  //    some_equal(range, value)
  //    none_equal(range, value)
  //    one_equal(range, value)
  //
  //    all_equal(range, value, comp)
  //    not_all_equal(range, value, comp)
  //    some_equal(range, value, comp)
  //    none_equal(range, value, comp)
  //    one_equal(range, value, comp)
  //
  // Some overloads are also adapted for initializer lists.
  //
  //    all_of(list, pred)
  //    not_all(list, pred)
  //    some_of(list, pred)
  //    none_of(list, pred)
  //    one_of(list, pred)
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
  
  
  
  // TODO: I shouldn't need overloads for initializer lists as function
  // arguments. They should be deduced correctly. Unfortunately, none of the
  // compilers that I'm experiment with do this just yet. When the feature
  // is supported, I can remove the overloads.

  

  template<typename I, typename T> I o_find(I, I, const T&);
  template<typename I, typename P> I o_find_if(I, I, P);


  // All of (iterator, query)
  // Returns true if first == last or pred(x) is true for all elements x in 
  // [first, last).
  template<typename I, typename P>
    bool all_of(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

      while (first != last) {
        if (!pred(*first))
          return false;
        ++first;
      }
      return true;
    }


    
  // All of (iterator, bool) 
  // Retruns true x is true for all elements x in [first, last).
  template<typename I>
    inline bool all_of(I first, I last)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Convertible<Value_type<I>, bool>(), "");
      return all_of(first, last, make_to_bool());
    }


    
  // All of (range, query)
  // Returns true if pred(x) is true for all elements x in range.
  template<typename R, typename P>
    inline bool all_of(const R& range, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      return all_of(begin(range), end(range), pred);
    }
    
    
    
  // All of (range, bool)
  // Returns true if x is true for all elements x in range.
  template<typename R>
    inline bool all_of(const R& range)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Convertible<Value_type<R>, bool>(), "");
      return all_of(begin(range), end(range), make_to_bool());
    }



  // All of (list, query)
  // Returns true if pred(x) is true for all elements x in list.
  template<typename T, typename P>
    inline bool all_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");
      return all_of(list.begin(), list.end(), pred);
    }
    
    
  
  // All of (list, bool)
  // Returns true if x is true for all elements x in list.
  template<typename T>
    inline bool all_of(std::initializer_list<T> list)
    {
      static_assert(Convertible<T, bool>(), "");
      return all_of(list.begin(), list.end(), make_to_bool());
    }
    
    
  
  // Not all of (iterator, query)
  // Returns true if pred(x) is false for some element x in [first, last).
  template<typename I, typename P>
    inline bool not_all_of(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

      while (first != last) {
        if (!pred(*first))
          return true;
        ++first;
      }
      return false;
    }
  
  
  
  // Not all of (iterator, bool)
  // Returns true if x is false for some element x in [first, last).
  template<typename I>
    inline bool not_all_of(I first, I last)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Convertible<Value_type<I>, bool>(), "");
      return not_all_of(first, last, make_to_bool());
    }
  
  
  
  // Not all of (range, query)
  // Returns true if pred(x) is false for some element x in range.
  template<typename R, typename P>
    inline bool not_all_of(const R& range, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      return not_all_of(begin(range), end(range), pred);
    }
    
    
    
  // Not all of (range, bool)
  // Returns true if x is false for some element x in range.
  template<typename R>
    inline bool not_all_of(const R& range)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Convertible<Value_type<R>, bool>(), "");
      return not_all_of(begin(range), end(range), make_to_bool());
    }
    


  // Not all of (list, query)
  // Returns true if pred(x) is false for some element x in list.
  template<typename T, typename P>
    inline bool not_all_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");
      return not_all_of(list.begin(), list.end(), pred);
    }
    
    
    
  // Not all (list, bool)
  // Returns true if x is false for some element x in list.
  template<typename T>
    inline bool not_all_of(std::initializer_list<T> list)
    {
      static_assert(Convertible<T, bool>(), "");
      return not_all_of(list.begin(), list.end(), make_to_bool());
    }
    
    
  
  // Some of (iterator, query)
  // Returns true if pred(x) is true for some element x in [first, last).
  template<typename I, typename P>
    inline bool some_of(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

      while (first != last) {
        if (pred(*first))
          return true;
        ++first;
      }
      return false;
    }

    
    
  // Some of (iterator, bool)
  // Returns true if x is true for some element x in [first, last).
  template<typename I>
    inline bool some_of(I first, I last)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Convertible<Value_type<I>, bool>(), "");
      return some_of(first, last, make_to_bool());
    }


    
  // Some of (range, query)
  // Returns true if pred(x) is true for some element x in range.
  template<typename R, typename P>
    inline bool some_of(const R& range, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      return some_of(begin(range), end(range), pred);
    }



  // Some of (range, bool)
  // Returns true if x is true for some element x in range.
  template<typename R>
    inline bool some_of(const R& range)
    {
      static_assert(Input_range<R>(), "");
      static_assert(Convertible<Value_type<R>, bool>(), "");
      return some_of(begin(range), end(range), make_to_bool());
    }
    
    
    
  // Some of (list, query)
  // Returns true pred(x) is true for some element x in list.
  template<typename T, typename P>
    inline bool some_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");
      return some_of(list.begin(), list.end(), pred);
    }


    
  // Some of (list, bool)
  // Returns true if x is true for some eleemnt x in list.
  template<typename T>
    inline bool some_of(std::initializer_list<T> list)
    {
      return some_of(list.begin(), list.end(), make_to_bool());
    }
    
    

  // None of
  // Returns true if pred(x) is false for all elements x in [first, last).
  template<typename I, typename P>
    inline bool none_of(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

      while (first != last) {
        if (pred(*first))
          return false;
        ++first;
      }
      return true;
    }

    
    
  // None of (iterator, bool)
  // Returns true if x is false for all elements x in range.
  template<typename I>
    inline bool none_of(I first, I last)
    {
      return none_of(first, last, make_to_bool());
    }


 
  // None of (range)
  // Returns true if pred(x) is false for all elements x in range.
  template<typename R, typename P>
    inline bool none_of(const R& range, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      
      return none_of(begin(range), end(range), pred);
    }
 
 
 
  // None of (range, bool)
  // Returns true if x is false for all elements x in range.
  template<typename R>
    inline bool none_of(const R& range)
    {
      return none_of(begin(range), end(range), make_to_bool());
    }
    
    

  // None of (list, query)
  // Returns true if pred(x) is false for all elements x in list.
  template<typename T, typename P>
    inline bool none_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return none_of(list.begin(), list.end(), pred);
    }


    
  // None of (list, bool)
  // Returns true if x is false for all elements x in list.
  template<typename T>
    inline bool none_of(std::initializer_list<T> list)
    {
      return none_of(list.begin(), list.end(), make_to_bool());
    }

    
    
  // One of (iterator, query)
  // Returns true if pred(x) is true for one and only one element x in 
  // [first, last).
  template<typename I, typename P>
    inline bool one_of(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));
      
      first = o_find_if(first, last, pred);
      if (first != last)
        return none_of(++first, last, pred);
      else
        return false;
    }
    
  
  
  // One of (iterator, bool)
  // Returns true if x is true for only element element x in [first, last).
  template<typename I>
    inline bool one_of(I first, I last)
    {
      return one_of(first, last, make_to_bool());
    }
  
  
  
  // One of (range, query)
  // Returns true if pred(x) is true for one and only one element x in range.
  template<typename R, typename P>
    inline bool one_of(const R& range, P pred)
    {
      static_assert(Range_query<R, P>(), "");
      
      return one_of(begin(range), end(range), pred);
    }
 
 

  // One of (range, bool)
  // Returns true if x is true for one and only one element x in range.
  template<typename R>
    inline bool one_of(const R& range)
    {
      return one_of(begin(range), end(range), make_to_bool());
    }

    

  // One of (list, query)
  // Returns true if pred(x) is true for one and only one element x in list.
  template<typename T, typename P>
    inline bool one_of(std::initializer_list<T> list, P pred)
    {
      static_assert(Predicate<P, T>(), "");

      return one_of(list.begin(), list.end(), pred);
    }

 
 
  // One of (list, bool)
  // Returns true if x is true for one and only one element x in list.
  template<typename T>
    inline bool one_of(std::initializer_list<T> list)
    {
      return one_of(list.begin(), list.end(), make_to_bool());
    }
 
 

  // All equal (iterator, relation)
  // Returns true if comp(x, value) for all elements x in [first, last).
  template<typename I, typename T, typename R>
    bool all_equal(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");

      while (first != last) {
        if (!comp(*first, value))
          return false;
        ++first;
      }
      return true;
    }
   

   
  // All equal (iterator, equality)
  // Returns true if x == value for all elements x in [first, last).
  template<typename I, typename T>
    inline bool all_equal(I first, I last, const T& value)
    {
      return all_equal(first, last, value, eq());
    }

    

  // All equal (range, relation)
  // Returns true if comp(x, value) for all elements x in range.
  template<typename R, typename T, typename Rel>
    inline bool all_equal(const R& range, const T& value, Rel comp)
    {
      return all_equal(begin(range), end(range), value, comp);
    }
    
    
    
  // All equal (range)
  // Returns true if x == value for all elements x in range.
  template<typename R, typename T>
    inline bool all_equal(const R& range, const T& value)
    {
      static_assert(Range_search<R, T>(), "");
      return all_equal(begin(range), end(range), value);
    }
    
    

  // All equal (list)
  // Returns true if x == value for all elementx x in list.
  template<typename T, typename U>
    inline bool all_equal(std::initializer_list<T> list, const U& value)
    {
      static_assert(Equality_comparable<T, U>(), "");

      return all_equal(list.begin(), list.end(), value);
    }
     
 
  
  // All equal (list, relation)
  // Returns true if comp(x, value) for all elements x in list.
  template<typename T, typename U, typename R>
    inline bool all_equal(std::initializer_list<T> list, const U& value, R comp)
    {
      return all_equal(list.begin(), list.end(), value, comp);
    }

    
    
  // Not all equal (relation)
  // Returns true if comp(x, value) is false for some element x in 
  // [first, last).
  template<typename I, typename T, typename R>
    bool not_all_equal(I first, I last, const T& value, R comp)
    {
      while (first != last) {
        if (!comp(*first, value))
          return true;
        ++first;
      }
      return false;
    }

    
    
  // Not all equal
  // Returns true if x != value for some element x in [first, last).
  template<typename I, typename T>
    bool not_all_equal(I first, I last, const T& value)
    {
      return not_all_equal(first, last, value, eq());
    }
   
    
    
  // Not all equal (range, relation)
  // Returns true if !comp(x, value) for some element x in range.
  template<typename R, typename T, typename Rel>
    inline bool not_all_equal(const R& range, const T& value, Rel comp)
    {
      return not_all_equal(begin(range), end(range), value, comp);
    }

    
    
  // Not all equal (range)
  // Returns true if x != value for some element x in range.
  template<typename R, typename T>
    inline bool not_all_equal(const R& range, const T& value)
    {
      static_assert(Range_search<R, T>(), "");

      return not_all_equal(begin(range), end(range), value);
    }

    
    
  // Not all equal (list, relation)
  // Returns true if !comp(x, value) for some element x in list.
  template<typename T, typename U, typename R>
    inline bool not_all_equal(std::initializer_list<T> list, const U& value, R comp)
    {
      return not_all_equal(list.begin(), list.end(), value, comp);
    }

    
    
  // Not all equal (list)
  // Returns true x != value for some element x in list.
  template<typename T, typename U>
    inline bool not_all_equal(std::initializer_list<T> list, const U& value)
    {
      static_assert(Equality_comparable<T, U>(), "");

      return not_all_equal(list.begin(), list.end(), value);
    }



  // Some equal (iterator, relation)
  // Returns true if comp(x, value) is true for some element x in [first, last).
  template<typename I, typename T, typename R>
    bool some_equal(I first, I last, const T& value, R comp)
    {
      while (first != last) {
        if (comp(*first, value))
          return true;
        ++first;
      }
      return false;
    }

    

  // Some equal (iterator, equal)
  // Returns true if x == value for some element x in [first, last).
  template<typename I, typename T>
    bool some_equal(I first, I last, const T& value)
    {
      return some_equal(first, last, value, eq());
    }



  // Some equal (range, relation)
  // Returns true if comp(x, value) for some element x in range.
  template<typename R, typename T, typename C>
    inline bool some_equal(const R& range, const T& value, C comp)
    {
      static_assert(Range_search<R, T, C>(), "");
      return some_equal(begin(range), end(range), value, comp);
    }

    
    
  // Some equal (range, equality)
  // Returns true if x == value for some element x in range.
  template<typename R, typename T>
    inline bool some_equal(const R& range, const T& value)
    {
      static_assert(Range_search<R, T, Equal_to>(), "");
      return some_equal(begin(range), end(range), value, eq());
    }


    
  // Some equal (list, relation)
  // Returns true if comp(x, value) for some element x in list.
  template<typename T, typename U, typename R>
    inline bool some_equal(std::initializer_list<T> list, const U& value, R comp)
    {
      return some_equal(list.begin(), list.end(), value, comp);
    }

    
    
  // Some equal (list, equality)
  // Returns true if x == value for some element x in list.
  template<typename T, typename U>
    inline bool some_equal(std::initializer_list<T> list, const U& value)
    {
      static_assert(Equality_comparable<T, U>(), "");

      return some_equal(list.begin(), list.end(), value);
    }

    
    
  // None equal
  // Returns true if !comp(x, value) for all elements x in [first, last).
  template<typename I, typename T, typename R>
    bool none_equal(I first, I last, T const& value, R comp)
    {
      while (first != last) {
        if (comp(*first, value))
          return false;
        ++first;
      }
      return true;
    }

    
    
  // None equal
  // Returns true if x != value for all elements x in [first, last).
  template<typename I, typename T>
    bool none_equal(I first, I last, T const& value)
    {
      static_assert(Search<I, T>(), "");
      assert(( is_readable_range(first, last) ));
      return none_equal(first, last, value, eq());
    }
    
    
    
  // None equal (range)
  // Returns true if !comp(x, value) for all elements x in range.
  template<typename R, typename T, typename Rel>
    inline bool none_equal(const R& range, const T& value, Rel comp)
    {
      return none_equal(begin(range), end(range), value, comp);
    }

    
    
  // None equal (range)
  // Returns true if x != value for all elements x in range.
  template<typename R, typename T>
    inline bool none_equal(const R& range, const T& value)
    {
      static_assert(Range_search<R, T>(), "");

      return none_equal(begin(range), end(range), value);
    }


    
  // None equal (list)
  // Returns true if !comp(x, value) for all elements x in list.
  template<typename T, typename U, typename R>
    inline bool none_equal(std::initializer_list<T> list, const U& value, R comp)
    {
      return none_equal(list.begin(), list.end(), value, comp);
    }

    
    
  // None equal (list)
  // Returns true if x != value for all elements x in list.
  template<typename T, typename U>
    inline bool none_equal(std::initializer_list<T> list, const U& value)
    {
      static_assert(Equality_comparable<T, U>(), "");

      return none_equal(list.begin(), list.end(), value);
    }
    

    
  // One equal
  // Returns true if comp(x, value) for exactly one element x in [first, last).
  template<typename I, typename T, typename R>
    inline bool one_equal(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");

      first = find(first, last, value, comp);
      if (first != last)
        return none_equal(++first, last, value, comp);
      else
        return false;
    }

    
    
  // One equal
  // Returns true if x == value for exactly one element x in [first, last).
  template<typename I, typename T>
    inline bool one_equal(I first, I last, const T& value)
    {
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));
      return one_equal(first, last, value, eq());
    }
    


  // One equal (range)
  // Returns true if x == value for one and only one element x in range.
  template<typename R, typename T>
    inline bool one_equal(const R& range, const T& value)
    {
      static_assert(Range_search<R, T>(), "");
      return one_equal(begin(range), end(range), value);
    }



  // One equal (range)
  // Returns true if comp(x, value) for one and only one element x in range.
  template<typename R, typename T, typename Rel>
    inline bool one_equal(const R& range, const T& value, Rel comp)
    {
      static_assert(Range_search<R, T>(), "");
      return one_equal(begin(range), end(range), value, comp);
    }

    
    
  // One equal (list)
  // Returns true if comp(x, value) for one and only one elements x in list.
  template<typename T, typename U, typename R>
    inline bool one_equal(std::initializer_list<T> list, const U& value, R comp)
    {
      return one_equal(list.begin(), list.end(), value, comp);
    }

    
    
  // One equal (list)
  // Returns true if x == value for one and only one elements x in list.
  template<typename T, typename U>
    inline bool one_equal(std::initializer_list<T> list, const U& value)
    {
      static_assert(Equality_comparable<T, U>(), "");
      return one_equal(list.begin(), list.end(), value);
    }

} // namespace origin

#endif
