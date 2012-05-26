// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_FIND_HPP
#define ORIGIN_ALGORITHM_FIND_HPP

#include <origin/algorithm/core.hpp>

namespace origin
{
  // The find algorithms search a range of elements for an element that 
  // satisfies some property, usually a predicate or an equality. 
  //
  // The following algorithms search for an equal value in a range of elements.
  //
  //    find(first, last, value)
  //    find(first, last, value, comp)
  //    find_not_equal(first, last, value)
  //    find_not_equal(first, last, value, comp)
  //    find_next(first, last, value)
  //    find_next(first, last, value, comp)
  //    find_nth(first, last, value)
  //    find_nth(first, last, value, comp)
  //
  // These algorithms search for an element in a range of elements that 
  // satisfies a unary predicate.
  //
  //    find_if(first, last, pred)
  //    find_if_not(first, last, pred)
  //    find_next_if(first, last, value)
  //    find_nth_if(first, last, pred)
  //
  // There are range-based overloads for each of these funtions also. Note that
  // there are no range-based overloads for find_next.
  //
  //    find(range, value)
  //    find(range, vaule, comp)
  //    find_not_equal(range, value)
  //    find_not_equal(range, value, comp)
  //    find_nth(range, value)
  //    find_nth(range, value, comp)
  //
  //    find_if(range, pred)
  //    find_if_not(range, pred)
  //    find_nth_if(range, pred)


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
  //
  // Ancilliary find algorithms include:
  //
  //    is_relation_preserving(first, last, comp)
  //    is_relation_preserving(range, comp)
  //
  
  // Find algorithms
  // These algorithms search a range of elements for an element that satisfies
  // some property.
  //
  // The _if family of algorithms search for the first iterator that satisfies
  // some unary predicate.
  


  // Find 
  //
  // Return the first iterator i where *i matches value. The meaning of matching
  // depends on the condition imposed by the relation, comp. There are four
  // overloads of this function:
  //
  //    find(first, last, value, comp) ~> i
  //    find(first, last, value) ~> i
  //    find(range, value, comp) ~> i
  //    find(range, value) ~> comp
  //
  // The algorithm returns the iterator where *i matches value or an iterator
  // past the end of the range.

  template <typename I, typename T, typename R>
    inline I 
    find(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));

      while (first != last && comp(*first, value))
        ++first;
      return last;
    }

  template <typename I, typename T>
    inline I 
    find(I first, I last, const T& value)
    {
      return find(first, last, value, eq());
    }

  template <typename R, typename T, typename C>
    inline auto 
    find(R&& range, const T& value, C comp) -> decltype(begin(range))
    {
      return find(begin(range), end(range), value, comp);
    }

  template <typename R, typename T>
    inline auto 
    find(R&& range, const T& value) 
      -> Requires<!Has_member_find<Forwarded<R>, T>(), decltype(begin(range))>
    {
      return find(begin(range), end(range), value, eq());
    }

  // Optimization for sets and maps.
  template <typename C, typename T>
    inline auto 
    find(C&& cont, const T& value) -> decltype(cont.find(value))
    {
      return cont.find(value);
    }



  // Find not equal
  //
  // Returns the first iterator i in [first, last) where *i != value or last
  // if no such iterator exists.
  //
  // FIXME: The comp versions are not finding based on equality, but they
  // aren't equivalence relations either. Find not?

  template<typename I, typename T, typename R>
    inline I 
    find_not_equal(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(is_bounded_range(first, last));

      while (first != last && !comp(*first, value))
        ++first;
      return last;
    }


  template <typename R, typename T, typename Comp>
    inline auto 
    find_not_equal(R&& range, const T& value, Comp comp) -> decltype(begin(range))
    {
      return find_not_equal(begin(range), end(range), value, comp);
    }
    



  // Find not equal
  //
  // Returns the first iterator i in a range where *i = value. There are two
  // overloads of this function:
  //
  //    find_not_equal(first, last, value) ~> i
  //    find_not_equal(range, value) ~> i

  template <typename I, typename T>
    inline I 
    find_not_equal(I first, I last, const T& value)
    {
      return find_not_equal(first, last, value, eq());
    }

  template <typename R, typename T>
    inline auto 
    find_not_equal(R&& range, const T& value) -> decltype(begin(range))
    {
      return find_not_equal(begin(range), end(range), value, eq());
    }
    
    

  // Find next
  // Returns the first iterator i in [first + 1, last) where comp(*i, value) is
  // or true last if no such iterator exists or first == last.

  template <typename I, typename T, typename R>
    inline I 
    find_next(I first, I last, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(( is_readable_range(first, last) ));
      
      if (first != last)
        return find(next(first), last, value, comp);
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
    inline I 
    find_next(I first, I last, const T& value)
    {
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));

      return find_next(first, last, value, eq());
    }


  
  // Find nth (iterator, relation)
  // Return the nth iterator i in [first, last) where *i == value or last if
  // there are fewer than n elements equal to value.
  template <typename I, typename T, typename R>
    I 
    find_nth(I first, I last, Difference_type<I> n, const T& value, R comp)
    {
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));

      while (first != last) {
        if (comp(*first, value)) {
          --n;
          if (n == 0)
            return first;
        }
        ++first;
      }
      return first;
    }



  // Find nth (iterator)
  template <typename I, typename T>
    inline I find_nth(I first, I last, Difference_type<I> n, const T& value)
    {
      static_assert(Search<I, T, Equal_to>(), "");
      assert(is_readable_range(first, last));
      return find_nth(first, last, n, value, eq());
    }



  // Find nth (range, relation)
  // Return an iterator to the nth element in r that is equal to value.
  template <typename R, typename T, typename C>
    inline auto find_nth(R&& range, Difference_type<R> n, const T& value, C comp)
      -> decltype(begin(range))
    {
      static_assert(Range_search<Forwarded<R>, T, C>(), "");
      return find_nth(begin(range), end(range), n, value, comp);
    }



  // Find nth (range, equality)
  template <typename R, typename T>
    inline auto find_nth(R&& range, Difference_type<R> n, const T& value)
      -> decltype(begin(range))
    {
      static_assert(Range_search<Forwarded<R>, T, Equal_to>(), "");
      return find_nth(begin(range), end(range), n, value, eq());
    }
      

        
  // Find if
  // Returns the first iterator i in [first, last) where pred(*i) is true or
  // last if no such iterator exists.
  template <typename I, typename P>
    inline I find_if(I first, I last, P pred)
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
    inline auto find_if(R&& range, P pred) -> decltype(begin(range))
    {
      static_assert(Range_query<Forwarded<R>, P>(), "");
      return find_if(begin(range), end(range), pred);
    }
    
    

  // Find if not
  // Returns the first iterator i in [first, last) where !pred(*i) is true or
  // last if no such iterator exists.
  template <typename I, typename P>
    inline I find_if_not(I first, I last, P pred)
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
    inline auto find_if_not(R&& range, P pred) -> decltype(begin(range))
    {
      static_assert(Range_query<Unqualified<R>, P>(), "");
      
      return find_if_not(begin(range), end(range), pred);
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
        return find_if(next(first), last, pred);
      else
        return last;
    }


  
  // Find nth if
  // Return the nth iterator i in [first, last) where pred(*i) is true or last 
  // if there are fewer than n elements equal to value.
  template <typename I, typename P>
    I find_nth_if(I first, I last, Difference_type<I> n, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));
      
      while (first != last) {
        if (pred(*first)) {
          --n;
          if (n == 0)
            return first;
        }
        ++first;
      }
      return first;
    }
    
    
  
  // Find nth if (range)
  // Return an iterator to the nth iterator in r where pred(*i) or end(r) if
  // there are fewer than n elements satisfying pred.
  template <typename R, typename P>
    inline auto find_nth_if(R&& range, Difference_type<R> n, P pred)
      -> decltype(begin(range))
    {
      static_assert(Range_query<Unqualified<R>, P>(), "");
      
      return find_nth_if(begin(range), end(range), n, pred);
    }
    

    
  // TODO: The following are Comparison algorithms. Maybe they belong with
  // equal and mismatch -- even though they're find algorithms.
  
  
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
      
      while (first1 != last1) {
        if (some_equal(first2, last2, *first1, comp))
          return first1;
        ++first1;
      }
      return last1;
    }


  // Find mismatch
  // Returns a pair of iterators denoting the position where two ranges do
  // not match. There are four overloads of this algorithm:
  //
  //    find_mismatch(first1, last, first2, last, comp) ~> {i, j}
  //    find_mismatch(first1, last, first2, last) ~> {i, j}
  //    find_mismatch(range1, range2, comp) ~> {i, j}
  //    find_mismatch(range1, range2) ~> {i, j}
  //
  // The first two search for a mismatch in the iterator ranges [first1, last1)
  // and [first2, last), and the last two do the same for bounded ranges range1
  // and range2.
  //
  // The algorithm returns a pair of iterators i and j such that i is in
  // [first1, last], j is in [first2, last2] (note closed ranges), and the
  // values of i and j do not match. The definition of matching is determined by
  // the relation used to compare the those elements. The first overload
  // compares the elements of the two ranges using a relation, comp. The second
  // compares elements for equality.
  //
  // In the standard library, the iterator versions are called mismatch and
  // take only 3 arguments.

  template<typename I1, typename I2, typename Comp>
    inline std::pair<I1, I2> 
    find_mismatch(I1 first1, I1 last1, I2 first2, I2 last2, Comp comp)
    {
      static_assert(Comparison<I1, I2, Comp>(), "");
      assert(is_readable_range(first1, last1));
      assume(is_readable_range(first2, last2));

      while (first1 != last1 && first2 != last2 && comp(*first1, *first2)) {
        ++first1;
        ++first2;
      }
      return {first1, first2};
    }

  template <typename I1, typename I2>
    inline std::pair<I1, I2>
    find_mismatch(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      return find_mismatch(first1, last1, first2, last2, eq());
    }

  template <typename R1, typename R2, typename Comp>
    inline auto 
    find_mismatch(R1&& range1, R2&& range2, Comp comp)
      -> decltype(find_mismatch(begin(range1), end(range1), 
                                begin(range2), end(range2), comp))
    {
      return find_mismatch(begin(range1), end(range1), 
                           begin(range2), end(range2), comp);
    }

  template <typename R1, typename R2>
    inline auto mismatch(R1&& range1, R2&& range2)
      -> decltype(find_mismatch(begin(range1), end(range1), 
                                begin(range2), end(range2), lt()))
    {
      return find_mismatch(begin(range1), end(range1), 
                           begin(range2), end(range2), eq());
    }

    
  // Find first in
  // Returns an iterator i in [first1, last1) where some_equal(first2, last, *i)
  // is true.
  //
  // Note that this is re-implemented from the generalized algorithm to avoid 
  // any incidental overhead from the use of a lambda expression.
  template <typename I1, typename I2>
    inline I1 find_first_in(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));

      while (first1 != last1) {
        if (some_equal(first2, last2, *first1))
          return first1;
        ++first1;
      }
      return last1;
    }



  // Find first in (range)
  // Returns the first iterator i in r1 where some_equal(r2, *i) is true.
  template <typename R1, typename R2>
    inline auto find_first_in(R1&& range1, const R2& range2) 
      -> decltype(begin(range1))
    {
      static_assert(Range_comparison<Unqualified<R1>, R2>(), "");
      static_assert(Forward_range<R2>(), "");
      
      return find_first_in(begin(range1), end(range1),
                           begin(range2), end(range2));
    }
  


  // Find first in (relation, range)
  // Returns the first iterator i in r1 such that comp(*i, *j) is true where j 
  // is any iterator in r2.
  template <typename R1, typename R2, typename R>
    inline auto find_first_in(R1&& range1, const R2& range2, R comp)
      -> decltype(begin(range1))
    {
      static_assert(Range_comparison<Unqualified<R1>, R2, R>(), "");
      static_assert(Forward_range<R2>(), "");
      
      return find_first_in(begin(range1), end(range1),
                           begin(range2), end(range2), comp);
    }
    
    
  
  // Find adjacent (relation)
  // Returns the first iterator i in [first, last) where comp(*i, *(i + 1)) is
  // true.
  //
  // TODO: Write using find_mismatch?
  template <typename I, typename R>
    I find_adjacent(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      
      if (first != last) {
        for (I i = next(first); i != last; ++i) {
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
    inline auto find_adjacent(R&& range) -> decltype(begin(range))
    {
      static_assert(Range_equality_query<Forwarded<R>>(), "");
      return find_adjacent(begin(range), end(range));
    }
  

  
  // Find adjacent (range, relation)
  // Returns the first iterator i in range wheren *i == *(i + 1).
  template <typename R, typename Rel>
    inline auto find_adjacent(R&& range, Rel comp) -> decltype(begin(range))
    {
      static_assert(Range_relational_query<Forwarded<R>, Rel>(), "");

      return find_adjacent(begin(range), end(range), comp);
    }

    
    
  // Returns the first iterator i in range [first, last) where 
  // comp(*i, *(i + 1)) is false.
  template <typename I, typename R>
    I find_not_adjacent(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));

      if (first != last) {
        for (I i = next(first); i != last; ++i) {
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
    inline auto find_not_adjacent(R&& range) -> decltype(begin(range))
    {
      static_assert(Range_equality_query<R>(), "");

      return find_not_adjacent(begin(range), end(range));
    }
    
    
  // Find not adjacent (range, relation)
  template <typename R, typename Rel>
    inline auto find_not_adjacent(R&& range, Rel comp) -> decltype(begin(range))
    {
      static_assert(Range_relational_query<R, Rel>(), "");

      return find_not_adjacent(begin(range), end(range), comp);
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
      static_assert(Range_relational_query<R, Rel>(), "");
      return is_relation_preserving(begin(range), end(range), comp);
    }

} // namespace origin

#endif
