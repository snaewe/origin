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
  //    find(first, last, value, comp)
  //    find(range, value)
  //    find(range, vaule, comp)
  //    find_not_equal(first, last, value)
  //    find_not_equal(first, last, value, comp)
  //    find_not_equal(range, value)
  //    find_not_equal(range, value, comp)
  //    find_next(first, last, value)
  //    find_next(first, last, value, comp)
  //    find_nth(first, last, value)
  //    find_nth(first, last, value, comp)
  //    find_nth(range, value)
  //    find_nth(range, value, comp)
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
  //
  // Algorithms for finding the first match in range of possible values. This
  // is the same as find_first_of in the standard library.
  //
  //    find_first_in(first1, last1, first2, last2)
  //    find_first_in(range1, range2)
  //    find_first_in(first1, last1, first2, last2, comp)
  //    find_first_in(range1, range2, comp)
  //
  // TODO: Write find_next_in and find_nth_in
  //
  // And adjacent find. This is the same as adjacent_find in the standard 
  // library.
  //
  //    find_adjacent(first, last)
  //    find_adjacent(first, last, comp)
  //    find_adjacent(range)
  //    find_adjacent(range, comp)
  //    find_not_adjacent(first, last)
  //    find_not_adjacent(first, last, comp)
  //    find_not_adjacent(range)
  //    find_not_adjacent(range, comp)
  //
  // TODO: Write find_next_adjacent and find_nth_adjacent

  
  // Find algorithms
  // These algorithms search a range of elements for an element that satisfies
  // some property.
  //
  // The _if family of algorithms search for the first iterator that satisfies
  // some unary predicate.
  


  // Find (iterator, relation)
  // Return the first iterator i in [first, last) where comp(*i, value) is true
  // or last if no such iterator exists.
  template <typename I, typename T, typename R>
    I find(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));

      while (first != last) {
        if (comp(*first, value))
          return first;
        ++first;
      }
      return last;
    }



  // Find (iterator, equality)
  // Return the first iterator i in [first, last) where *i == value or last if 
  // no such iterator exists.
  template <typename I, typename T>
    inline I o_find(I first, I last, const T& value)
    {
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));

      return find(first, last, value, eq());
    }


    
  // Find (range, relation)
  // Returns the first iterator i in r such that comp(*i, value) is true or 
  // end(r) if no such iterator exists.
  template <typename R, typename T, typename Rel>
    inline auto find(R&& range, const T& value, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_search<Unqualified<R>, T, Rel>(), "");
      
      return find(o_begin(range), o_end(range), value, comp);
    }



  // Find (range, equality)
  // Returns the first iterator i in r such that *i == value or end(r) if no
  // such iterator exists.
  template <typename R, typename T>
    inline auto find(R&& range, const T& value) 
      -> Requires<!Has_member_find<Forwarded<R>, T>(), decltype(o_begin(range))>
    {
      static_assert(Range_search<Forwarded<R>, T>(), "");
      
      return find(o_begin(range), o_end(range), value, eq());
    }



  // Find (associative container)
  // Returns the first iterator i in c such that *i == value or end(c) if no
  // such iterator exists. The result is comptued in O(log(size(c))).
  //
  // NOTE: This implies that Container (or Associative_container) is a 
  // refinement of Range.
  template <typename C, typename T>
    auto find(C&& cont, const T& value) -> decltype(cont.find(value))
    {
      return cont.find(value);
    }



  // Find not equal (iterator, relation)
  // Returns the first iterator i in [first, last) where *i != value or last
  // if no such iterator exists.
  template<typename I, typename T, typename R>
    I find_not_equal(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");

      while (first != last) {
        if (!comp(*first, value))
          return first;
        ++first;
      }
      return last;
    }



  // Find not equal (iterator, equality)
  // Returns the first iterator i in [first, last) where *i != value or last
  // if no such iterator exists.
  template <typename I, typename T>
    inline I find_not_equal(I first, I last, const T& value)
    {
      static_assert(Search<I, T>(), "");

      return find_not_equal(first, last, value, eq());
    }
  
    
  // Find not equal (range, relation)
  // Returns the first iterator i in r where *i != value or end(r) if no such
  // iterator exists.
  template <typename R, typename T, typename Rel>
    inline auto find_not_equal(R&& range, const T& value, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_search<Forwarded<R>, T, Rel>(), "");
  
      return find_not_equal(o_begin(range), o_end(range), value, comp);
    }
    
        
    
  // Find not equal (range, equality)
  // Returns the first iterator i in r where *i != value or end(r) if no such
  // iterator exists.
  template <typename R, typename T>
    inline auto find_not_equal(R&& range, const T& value) -> decltype(o_begin(range))
    {
      static_assert(Range_search<Forwarded<R>, T>(), "");

      return find_not_equal(o_begin(range), o_end(range), value, eq());
    }
    
    

  // Find next
  // Returns the first iterator i in [first + 1, last) where comp(*i, value) is
  // or true last if no such iterator exists or first == last.
  template <typename I, typename T, typename R>
    inline I find_next(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(( is_readable_range(first, last) ));
      
      if (first != last)
        return o_find(o_next(first), last, value, comp);
      else
        return last;
    }
    
      
        
  // Find next
  // Returns the first iterator i in [first + 1, last) where *i == value or
  // last if no such iterator exists or first == last.
  //
  // Note that there is no corresponding range version of this function. The
  // operation is only intended to be applied to iterators.
  template <typename I, typename T>
    inline I find_next(I first, I last, const T& value)
    {
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));

      return find_next(first, last, value, eq());
    }


  
  // Find nth
  // Return the nth iterator i in [first, last) where *i == value or last if
  // there are fewer than n elements equal to value.
  template <typename I, typename T, typename R>
    inline I find_nth(I first, I last, Distance_type<I> n, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(( is_readable_range(first, last) ));

      while (first != last && n != 0) {
        if (comp(*first, value))
          --n;
        ++first;
      }
      return first;
    }



  // Find nth (range)
  // Return an iterator to the nth element in r that is equal to value.
  template <typename R, typename T, typename Rel>
    inline auto find_nth(R&& range, Distance_type<R> n, const T& value, Rel comp)
      -> decltype(o_begin(range))
    {
      static_assert(Range_search<Forwarded<R>, T, Rel>(), "");
      
      return find_nth(o_begin(range), o_end(range), n, value, comp);
    }
    
      
        
  // Find if
  // Returns the first iterator i in [first, last) where pred(*i) is true or
  // last if no such iterator exists.
  template <typename I, typename P>
    inline I o_find_if(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      while (first != last) {
        if (pred(*first))
          return first;
        ++first;
      }
      return last;
    }



  // Find if (range)
  // Returns the first iterator i in r where pred(*i) is true or end(r) if no 
  // such iterator exists.
  template<typename R, typename P>
    inline auto find_if(R&& range, P pred) -> decltype(o_begin(range))
    {
      static_assert(Range_query<Forwarded<R>, P>(), "");
      return o_find_if(o_begin(range), o_end(range), pred);
    }
    
    

  // Find if not
  // Returns the first iterator i in [first, last) where !pred(*i) is true or
  // last if no such iterator exists.
  template <typename I, typename P>
    inline I o_find_if_not(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(( is_readable_range(first, last) ));

      while (first != last) {
        if (!pred(*first))
          return first;
        ++first;
      }
      return last;
    }


    
  // Find if not (range)
  // Returns the first iterator i in r where pred(*i) is true or end(r) if no 
  // such iterator exists.
  template <typename R, typename P>
    inline auto find_if_not(R&& range, P pred) -> decltype(o_begin(range))
    {
      static_assert(Range_query<Unqualified<R>, P>(), "");
      
      return o_find_if_not(o_begin(range), o_end(range), pred);
    }

     
        
  // Find next if
  // Returns the first element in [first + 1, last) where pred(*i) is true or
  // last if no iterator exists or first == last.
  //
  // Note that there is no corresponding range version of this function. The
  // operation is only intended to be applied to iterators.
  template <typename I, typename P>
    inline I find_next_if(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

      if (first != last)
        return o_find_if(o_next(first), last, pred);
      else
        return last;
    }


  
  // Find nth if
  // Return the nth iterator i in [first, last) where pred(*i) is true or last 
  // if there are fewer than n elements equal to value.
  template <typename I, typename P>
    I find_nth_if(I first, I last, Distance_type<I> n, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));
      
      while (first != last && n != 0) {
        if (pred(*first))
          --n;
        ++first;
      }
      return first;
    }
    
    
  
  // Find nth if (range)
  // Return an iterator to the nth iterator in r where pred(*i) or end(r) if
  // there are fewer than n elements satisfying pred.
  template <typename R, typename P>
    inline auto find_nth_if(R&& range, Distance_type<R> n, P pred)
      -> decltype(o_begin(range))
    {
      static_assert(Range_query<Unqualified<R>, P>(), "");
      
      return find_nth_if(o_begin(range), o_end(range), n, pred);
    }
    

    
  // TODO: The following are Comparison algorithms. Maybe they belong with
  // equal and mismatch -- even though they're find algorithms.
  
  
    
  // Find first in
  // Returns an iterator i in [first1, last1) where any_equal(first2, last, *i)
  // is true.
  template <typename I1, typename I2>
    inline I1 find_first_in(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));

      while (first1 != last1) {
        if (any_equal(first2, last2, *first1))
          return first1;
        ++first1;
      }
      return last1;
    }



  // Find first in (range)
  // Returns the first iterator i in r1 where any_equal(r2, *i) is true.
  template <typename R1, typename R2>
    inline auto find_first_in(R1&& range1, const R2& range2) -> decltype(o_begin(range1))
    {
      static_assert(Range_comparison<Unqualified<R1>, R2>(), "");
      static_assert(Forward_range<R2>(), "");
      
      return find_first_in(o_begin(range1), o_end(range1),
                           o_begin(range2), o_end(range2));
    }
  
  
  
  // Find first in (relation)
  // Returns the first iterator i in [first1, last1) such that comp(*i, *j) is 
  // true where j is any iterator in [first2, last2).
  template <typename I1, typename I2, typename R>
    inline I1 find_first_in(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      static_assert(Comparison<I1, I2, R>(), "");
      static_assert(Forward_iterator<I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));
      
      using namespace std::placeholders;

      while (first1 != last1) {
        if (any_of(first2, last2, std::bind(*first1, _1)))
          return first1;
        ++first1;
      }
      return last1;
    }



  // Find first in (relation, range)
  // Returns the first iterator i in r1 such that comp(*i, *j) is true where j 
  // is any iterator in r2.
  template <typename R1, typename R2, typename R>
    inline auto find_first_in(R1&& range1, const R2& range2, R comp)
      -> decltype(o_begin(range1))
    {
      static_assert(Range_comparison<Unqualified<R1>, R2, R>(), "");
      static_assert(Forward_range<R2>(), "");
      
      return find_first_in(o_begin(range1), o_end(range1),
                           o_begin(range2), o_end(range2), comp);
    }
    
    
  
  // Find adjacent (relation)
  // Returns the first iterator i in [first, last) where comp(*i, *(i + 1)) is
  // true.
  template <typename I, typename R>
    I find_adjacent(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last, comp));
      
      if (first != last) {
        for (I i = o_next(first); i != last; ++i) {
          if (comp(*first, *i))
            return first;
          ++first;
        }
      }
      return last;
    }
    
      
  
  // Find adjacent
  // Return the first iterator i in [first, last) where *i == *(i + 1).
  template <typename I>
    I find_adjacent(I first, I last)
    {
      static_assert(Equality_query<I>(), "");
      assert(is_readable_range(first, last));

      return find_adjacent(first, last, eq());
    }
    
    
  
  // Find adjacent (range)
  // Returns the first iterator i in range [first, last) where *i == *(i + 1).
  template <typename R>
    inline auto find_adjacent(R&& range) -> decltype(o_begin(range))
    {
      static_assert(Range_equality_query<Forwarded<R>>(), "");

      return find_adjacent(o_begin(range), o_end(range));
    }
  

  
  // Find adjacent (range, relation)
  // Returns the first iterator i in range wheren *i == *(i + 1).
  template <typename R, typename Rel>
    inline auto find_adjacent(R&& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_relational_query<Forwarded<R>, Rel>(), "");

      return find_adjacent(o_begin(range), o_end(range), comp);
    }

    
    
  // Returns the first iterator i in range [first, last) where 
  // comp(*i, *(i + 1)) is false.
  template <typename I, typename R>
    I find_not_adjacent(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last, comp));

      if (first != last) {
        for (I i = o_next(first); i != last; ++i) {
          if (!comp(*first, *i))
            return first;
          ++first;
        }
      }
      return last;
    }



  // Find not adjacent
  // Returns the first iterator i in range [first, last) where *i != *(i + 1).
  template <typename I>
    I find_not_adjacent(I first, I last)
    {
      static_assert(Equality_query<I>(), "");
      assert(is_readable_range(first, last));

      return find_not_adjacent(first, last, eq());
    }
    
    
  
    
    
  // Find not adjacent (range)
  template <typename R>
    inline auto find_not_adjacent(R&& range) -> decltype(o_begin(range))
    {
      static_assert(Range_equality_query<R>(), "");

      return find_not_adjacent(o_begin(range), o_end(range));
    }
    
    
  // Find not adjacent (range, relation)
  template <typename R, typename Rel>
    inline auto find_not_adjacent(R&& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_relational_query<R, Rel>(), "");

      return find_not_adjacent(o_begin(range), o_end(range), comp);
    }
  
  
  
  // Is relation preserving
  // Returns true if comp(*i, *next(i)) is true for each pair of consecutive
  // iterators in [first, last).
  template <typename I, typename R>
    inline bool is_relation_preserving(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");

      return find_not_adjacent(first, last, comp) == last;
    }
    
    
    
  // Is relation preserving (range)
  // Returns true if comp(*i, *next(i)) is true for each pair of consecutive
  // iterators in range.
  template <typename R, typename Rel>
    inline bool is_relation_preserving(const R& range, Rel comp)
    {
      static_assert(Relational_query<R, Rel>(), "");

      return is_relation_preserving(o_begin(range), o_end(range), comp);
    }

} // namespace origin

#endif
