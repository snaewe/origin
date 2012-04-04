// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_HPP
#define ORIGIN_ALGORITHM_HPP

#include <algorithm>

#include <origin/memory.hpp>
#include <origin/functional.hpp>
#include <origin/iterator.hpp>
#include <origin/range.hpp>

namespace origin
{
  // NOTE: The o_* algorithms are so named to prevent name collisions with
  // the std algorithms, except that they may have slightly different 
  // implementations and are fully concept checked.
  
  
  // This file contains a number of concepts. Concepts for non-modifying 
  // algorithms are:
  //
  //    Query
  //    Relational_query
  //    Equality_query
  //    Order_query
  //    Search
  //    Comparison
  //
  // The concepts for modifying algorithms are:
  //
  //    Fill
  //    Copy
  //    Move
  //    Permutation
  //
  // Concepts for ordering-related algorithms are:
  //
  //    Sort
  //    Merge


  // Query (concept)
  // A query is an algorithm that iterates over the elements of a range,
  // searching for one or more that satisfy some property. The property is
  // given as a unary predicate.
  template<typename I, typename P>
    constexpr bool Query()
    {
      return Input_iterator<I>() && Predicate<P, Value_type<I>>();
    }

    
    
  // Range query (concept)
  // A range is queryable if its iterator type is queryable.
  template<typename R, typename P>
    constexpr bool Range_query()
    {
      return Input_range<R>() && Query<Iterator_type<R>, P>();
    }

    
    
  // Relational query (concept)
  // A relational query is an algorithm that evaluates the elements of a
  // range with respect to some (binary) relation, often equality and ordering.
  template<typename I, typename R>
    constexpr bool Relational_query()
    {
      return Forward_iterator<I>() && Relation<R, Value_type<I>>();
    }
    
    
    
  // Range relational query (concept)
  template<typename R, typename Rel>
    constexpr bool Range_relational_query()
    {
      return Range<R>() && Relational_query<Iterator_type<R>, Rel>();
    }
    
    
    
  // Equality query (concept)
  // An equality query algorithm is a relational query for equal elements of
  // a range that uses the == operator.
  template<typename I>
    constexpr bool Equality_query()
    {
      return Forward_iterator<I>() && Equality_comparable<Value_type<I>>();
    }
    
    
    
  // Range equality comparable (concept)
  template<typename R>
    constexpr bool Range_equality_query()
    {
      return Range<R>() && Equality_query<Iterator_type<R>>();
    }
    
    
  // Order query (concept)
  // An order query algorithm is a relational query on the ordering of elements
  // of a range using the < operator.
  template<typename I>
    constexpr bool Order_query()
    {
      return Forward_iterator<I>() && Totally_ordered<Value_type<I>>();
    }
    
    
    
  // Range order query (concept)
  template<typename R>
    constexpr bool Range_order_query()
    {
      return Range<R>() && Range_order_query<Iterator_type<R>>();
    }
  
     

  // Search (concept)
  // A search is an algorithm that iterates over the elements of a range,
  // searching for one or more that is equal to a value type. The value type
  // may the same as or different than as the iterator's, and the equality
  // comparison may be generalized to a relation. There are two forms of
  // this concept.
  //
  //    Search<I, T>    Can be searched for a T value using equality
  //    Search<I, T, R> Can be searched for a T value using a relation R

  // Search<I, T, R> implementation.
  template<typename I, typename T, typename R>
    struct Search_concept
    {
      static constexpr bool check()
      {
        return Input_iterator<I>() && Relation<R, Value_type<I>, T>();
      }
    };
    
  // Search<I, T> implementation.
  template<typename I, typename T>
    struct Search_concept<I, T, default_t>
    {
      static constexpr bool check()
      {
        return Input_iterator<I>() && Equality_comparable<Value_type<I>, T>();
      }
    };

  // Returns true if the iterator can be used for searching.
  template<typename I, typename T, typename R = default_t>
    constexpr bool Search()
    {
      return Search_concept<I, T, R>::check();
    }
  
  
  
  // Range seachable (concept)
  // A range is searchable if its underlying iterator type is searchable. Like 
  // the searchable concept, there are two forms of this concept.
  //
  //    Range_search<R, T>      Can be searched for a T value
  //    Range_search<R, T, Rel> Can be searched for a T value using a relation R
  //
  // Returns true if the input iterator can be searched for a value of type T.
  template<typename R, typename T = default_t, typename Rel = default_t>
    constexpr bool Range_search()
    {
      return Input_range<R>() && Search<Iterator_type<R>, T, Rel>();
    }

  
  
  // Binary search (concept)
  // The binary search concept is an alternative form of search that uses the
  // < operator instead of ==. Note that the iterator is also required to be
  // a forward iterator, which is stronger than Search's iterator requirement.
  //
  // There are two forms of this concept:
  //
  //    Binary_search<I, T>
  //    Binary_search<I, T, R>
  //
  // The latter is equivalent to Search<I, T, R> && Forward_iterator<I> except
  // that R is required to be a strict weak ordering.
  
  template<typename I, typename T, typename R>
    struct Binary_search_concept
    {
      static constexpr bool check()
      {
        return Forward_iterator<I> && Relation<R, T>();
      }
    };
    
  template<typename I, typename T>
    struct Binary_search_concept<I, T, default_t>
    {
      static constexpr bool check()
      {
        return Forward_iterator<I>() && Totally_ordered<Value_type<I>, T>();
      }
    };
  
  // Returns true if I supports a binary search for a T value.
  template<typename I, typename T, typename R = default_t>
    constexpr bool Binary_search()
    {
      return Binary_search_concept<I, T, R>::check();
    }
    
    
    
  // Range binary search (concept)
  template<typename R, typename Rel = default_t>
    constexpr bool Range_binary_search()
    {
      return Range<R>() && Binary_search<R, Rel>();
    }
    
  
  // Comparison (concept)
  // A comparison algorithm compares the elements in two different ranges for 
  // equality. The comparison can also be generalized to a relation. There are
  // two forms of this concept:
  //
  //    Comparison<I1, I2>    Compare the elements of I1 and I2 for equality
  //    Comparison<I1, I2, R> Compare the elements of I1 and I2 using the relation R
  
  // Comparison<I1, I2, R> implementation
  template<typename I1, typename I2, typename R>
    struct Comparison_concept
    {
      static constexpr bool check()
      {
        return Input_iterator<I1>()
            && Weak_input_iterator<I2>()
            && Relation<R, Value_type<I1>, Value_type<I2>>();
      }
    };
  
  // Comparison<I1, I2> implementation
  template<typename I1, typename I2>
    struct Comparison_concept<I1, I2, default_t>
    {
      static constexpr bool check()
      {
        return Input_iterator<I1>()
            && Weak_input_iterator<I2>()
            && Equality_comparable<Value_type<I1>, Value_type<I2>>();
      }
    };
  
  // Returns true I1 and I2 can be compared.
  template<typename I1, typename I2, typename R = default_t>
    constexpr bool Comparison()
    {
      return Comparison_concept<I1, I2, R>::check();
    }
    
    

  // Range comparison (concept)
  // Two ranges are comparable if their iterators can be compared. Like the
  // Comparison concept, there are two forms of this concept:
  //
  //    Range_comparison<R1, R2>      Compare the elements of R1 and R2 for equality
  //    Range_comparison<R1, R2, Rel> Compare the elements of R1 and R2 using Rel
  //
  // Returns true if R1 and R2 are ranges that can be compared using value
  // equality or a relation R.
  template<typename R1, typename R2, typename Rel = default_t>
    constexpr bool Range_comparison()
    {
      return Input_range<R1>() 
          && Input_range<R2>() 
          && Comparison<Iterator_type<R1>, Iterator_type<R2>, Rel>();
    }



  // Lexicographical comparison (concept)
  // A lexicographical comparison is a comparison of two iterator ranges by
  // the ordering of the elements, using the < operator (instead of ==). Note
  // that I2 is required to be an input iterator, which is stronger than the
  // weak input iterator requirement of Comparison. There are two forms of
  // Lexicographical_comparison:
  //
  //    Lexicographical_comparison<I1, I2>
  //    Lexicographical_comparison<I1, I2, R>
  //
  // The latter is equivalent to Comparison<I1, I2, R> && Input_iterator<I2>
  // except that R is also required to be a strict weak ordering.
  template<typename I1, typename I2>
    constexpr bool Lexicographical_comparison()
    {
      return Input_iterator<I1>() 
          && Input_iterator<I2>() 
          && Totally_ordered<Value_type<I1>, Value_type<I2>>();
    }
    
    
    
  // Copy (concept)
  // The iterative copy concept defines the requirements of copying values 
  // from an I iterator range into an O iterator range.
  template<typename I, typename O>
    constexpr bool Copy()
    {
      return Input_iterator<I>() && Weak_output_iterator<O, Value_type<I>>();
    }
  
  
  
  // Range copy (concept)
  // The range copy concept defines the requirements of copying values from
  // an input range (R) into output range (O).
  template<typename R, typename O>
    constexpr bool Range_copy()
    {
      return Input_range<R>() && Output_range<O, Value_type<R>>();
    }
    
    
    
  // Move (concept)
  template<typename I, typename O>
    constexpr bool Move()
    {
      return Input_iterator<I>() && Weak_output_iterator<O, Value_type<I>&&>();
    }
    
    
  
  // Range move (concept)
  template<typename R, typename O>
    constexpr bool Range_move()
    {
      return Input_range<R>() && Output_range<O, Value_type<R>&&>();
    }
    
    
    
  // Fill (concept)
  // An iterator O can be filled with a value of type T if T is copyable and
  // O is an output iterator supporting assignment of T.
  template<typename O, typename T>
    constexpr bool Fill()
    {
      return Copyable<T>() && Output_iterator<O, T>();
    }


    
  // Range fill (concept)
  // A range R can be filled with a value of type T if its underlying iterator
  // can be filled with that value.
  template<typename R, typename T>
    constexpr bool Range_fill()
    {
      return Range<R>() && Fill<Iterator_type<R>, T>();
    }
    
    
    
  // Permutation (concept)
  // A permutation allows values to be exchanged (moved) between different 
  // iterators in a range without copying. This also includes moving values
  // into temporary values.
  template<typename I>
    constexpr bool Permutation()
    {
      return Forward_iterator<I>() && Permutable<I>();
    }
    
  
  
  // Range permutation (concept)
  // A range permutation is a range whose iterators can be permuted.
  template<typename R>
    constexpr bool Range_permutation()
    {
      return Range<R>() && Permutation<Iterator_type<R>>();
    }

  
  
  // Merge
  // The merge concept describes common requirements on the family of
  // (non-inplace) merge and set operations. There are two mergeable concepts:
  //
  //    Mergeable<I1, I2, O>
  //    Mergeable<I1, I2, O, R>
  //
  // The first requires the value types of I1 and I2 to be totally ordered. The 
  // second comparison the elemnents of the two iterator ranges using the
  // relation R.
  
  // This specialization implements the requirements for the generalized
  // version.
  template<typename I1, typename I2, typename O, typename R>
    struct Merge_concept
    {
      static constexpr bool check()
      {
        return Comparison<I1, I2, R>() && Copy<I1, O>() && Copy<I2, O>();
      }
    };

  // This specialization implements the requirements for algorithms that 
  // directly use the < operator.
  template<typename I1, typename I2, typename O>
    struct Merge_concept<I1, I2, O, default_t>
    {
      static constexpr bool check()
      {
        return Lexicographical_comparison<I1, I2>() && Copy<I1, O>() && Copy<I2, O>();
      }
    };
    
  // Returns true if I1 and I2 can be merged into O.
  template<typename I1, typename I2, typename O, typename R = default_t>
    constexpr bool Merge()
    {
      return Merge_concept<I1, I2, O, R>::check();
    }

    
  
  // Range merge (concept)
  template<typename R1, typename R2, typename O, typename Rel = default_t>
    constexpr bool Range_merge()
    {
      return Range<R1>()
          && Range<R2>()
          && Merge<Iterator_type<R1>, Iterator_type<R2>, O, Rel>();
    }
    
    
    
  // Sort (concept)
  // The Sort concept describes the requirements of algorithms that permute
  // an iterator range according to the ordering of the elements. There are
  // two forms of this conceopt:
  //
  //    Sort<I>
  //    Sort<I, R>
  
  // Requirements for the generalized overload.
  template<typename I, typename R>
    struct Sort_concept
    {
      static constexpr bool check()
      {
        return Permutation<I>() && Relation<R, Value_type<I>>();
      }
    };
    
  // Requirements using operator <.
  template<typename I>
    struct Sort_concept<I, default_t>
    {
      static constexpr bool check()
      {
        return Permutation<I>() && Totally_ordered<Value_type<I>>();
      }
    };
    
  // Returns true if I is sortable.
  template<typename I, typename R = default_t>
    constexpr bool Sort()
    {
      return Sort_concept<I, R>::check();
    }



  // Range sort (concept)
  // A range can be sorted if its underlying iterators can be sorted. There
  // are two forms of this concept:
  //
  //    Range_sort<R>
  //    Range_sort<R, Rel>
  template<typename R, typename Rel = default_t>
    constexpr bool Range_sort()
    {
      return Range<R>() && Sort<Iterator_type<R>, Rel>();
    }
}

#include <origin/algorithm/reduce.hpp>
#include <origin/algorithm/quantifier.hpp>
#include <origin/algorithm/find.hpp>
#include <origin/algorithm/count.hpp>

namespace origin 
{
  // For each (iterator)
  template <typename I, typename F>
    inline F o_for_each(I first, I last, F func)
    {
      while (first != last) {
        func(*first);
        ++first;
      }
      return func;
    }



  // For each (range)
  template <typename R, typename F>
    inline F for_each(R&& range, F func)
    {
      return o_for_each(o_begin(range), o_end(range), func);
    }



  // Equal
  // Returns true if *i == *j for each iterator i and j in [first1, last1) and
  // [first2, first2 + (last1 - first1)), pairwise.
  template<typename I1, typename I2>
    inline auto o_equal(I1 first1, I1 last1, I2 first2)
      -> Requires<!Can_memcmp<I1, I2>(), bool>
    {
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assume(is_readable_range(first2, distance(first1, last1)));

      while(first1 != last1) {
        if(*first1 != *first2)
          return false;
        ++first1;
        ++first2;
      }
      return true;
    }

    
  // Equal (memcmp)
  // Use memcmp to compare the iterator ranges, but only in the conditions
  // determined by the Can_memcmp trait.
  template<typename I1, typename I2>
    inline auto o_equal(I1 first1, I1 last1, I2 first2)
      -> Requires<Can_memcmp<I1, I2>(), bool>
    {
      return !__builtin_memcmp(unwrap_iterator(first1), 
                               unwrap_iterator(first2), 
                               sizeof(Value_type<I1>) * (last1 - first1));
    }
    
    

  // Equal (range)
  // Returns true if x == y for each x and y in the ranges a and b, pairwise.
  template<typename R1, typename R2>
    inline auto equal(R1 const& a, R2 const& b)
      -> Requires<!(Random_access_range<R1>() && Random_access_range<R2>()), bool>
    {
      static_assert(Range_comparison<R1, R2>(), "");
    
      return o_equal(std::begin(a), std::end(a), std::begin(b));
    }


    
  // Equal (random access range)
  // This optimization for random access ranges returns true if 
  // size(a) <= size(b) and x == y for each x and y in the ranges a and b, 
  // pairwise.
  template<typename R1, typename R2>
    inline auto equal(R1 const& a, R2 const& b)
      -> Requires<Random_access_range<R1>() && Random_access_range<R2>(), bool>
    {
      static_assert(Range_comparison<R1, R2>(), "");
    
      return size(a) <= size(b) 
          && o_equal(std::begin(a), std::end(a), std::begin(b));
    }
  
  
    
  // Equal (relation)
  // Returns true if comp(*i, *j), for each iterator i and j in [first1, last1) 
  // and [first2, first2 + (last1 - first1)) pairwise.
  //
  // Note that this algorithm cannot be optimized by memcmp.
  template<typename I1, typename I2, typename R>
    inline bool o_equal(I1 first1, I1 last1, I2 first2, R comp)
    {
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assume(is_readable_range(first2, distance(first1, last1)));

      while(first1 != last1) {
        if(!comp(*first1, *first2))
          return false;
        ++first1;
        ++first2;
      }
      return true;
    }
    
    
    
  // Equal (range)
  // Returns true if comp(x, y) is true for each x and y in the ranges a and b, 
  // pairwise.
  template<typename R1, typename R2, typename Rel>
    inline auto equal(const R1& a, const R2& b, Rel comp)
      -> Requires<!(Random_access_range<R1>() && Random_access_range<R2>()), bool>
    {
      static_assert(Range_comparison<R1, R2, Rel>(), "");

      return o_equal(std::begin(a), std::end(a), std::begin(b), comp);
    }

    
    
  // Equal (random access range)
  // This specialization for random access ranges returns true if 
  // size(a) <= size(b) and comp(x, y) is true for each x and y in the ranges a 
  // and b, pairwise.
  template<typename R1, typename R2, typename Rel>
    inline auto equal(const R1& a, const R2& b, Rel comp)
      -> Requires<Random_access_range<R1>() && Random_access_range<R2>(), bool>
    {
      static_assert(Range_comparison<R1, R2, Rel>(), "");

      return size(a) <= size(b)
          && o_equal(std::begin(a), std::end(a), std::begin(b), comp);
    }



  // Mismatch
  // Returns the first iterator i in [first1, last1) where *i != *j and *j is
  // the corresponding iterator in j.
  template<typename I1, typename I2>
    inline auto o_mismatch(I1 first1, I1 last1, I2 first2)
      -> std::pair<I1, I2>
    {
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assume(is_readable_range(first2, distance(first1, last1)));

      while(first1 != last1) {
        if(*first1 != *first2)
          return {first1, first2};
        ++first1;
        ++first2;
      }
      return {last1, first2};
    }

    
    
  // Mismatch (range)
  // Returns the first iterator i in a where *i != *j and *j is the 
  // corresponding iterator in b.
  template<typename R1, typename R2>
    inline auto mismatch(const R1& a, const R2& b)
      -> std::pair<Iterator_type<R1>, Iterator_type<R2>> 
    {
      static_assert(Range_comparison<R1>(), "");

      return o_mismatch(std::begin(a), std::end(a), std::begin(b));
    }

    
    
  // Mismatch (relation)
  template<typename I1, typename I2, typename R>
    inline auto o_mismatch(I1 first1, I1 last1, I2 first2, R comp)
      -> std::pair<I1, I2> 
    {
      static_assert(Comparison<I1, I2, R>(), "");
      assert(is_readable_range(first1, last1));
      assume(is_readable_range(first2, distance(first1, last1)));

      while(first1 != last1) {
        if(!comp(*first1, *first2))
          return {first1, first2};
        ++first1;
        ++first2;
      }
      return {first1, first2};
    }
    
    
    
  // Mismatch (range, relation)
  template<typename R1, typename R2, typename Rel>
    inline auto mismatch(const R1& a, const R2& b, Rel comp)
      -> std::pair<Iterator_type<R1>, Iterator_type<R2>>
    {
      static_assert(Range_comparison<R1, R2, Rel>(), "");

      return o_mismatch(std::begin(a), std::end(a), std::begin(b), comp);
    }

  
    
  // Equal elements (impl)
  // Returns true if [first1, last1) is a rearrangement of [first2, last2).
  template<typename I1, typename I2>
    bool equal_elements_impl(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assume(is_readable_range(first2, distance(first1, last1)));

      for(I1 i = first1; i != last1; ++i) {
        // Don't recount the number of times *i appears
        if(some_equal(first1, i, *i))
          continue;

        // Count the number of times *i appears in [first2, first2 + n). It
        // needs to be the same count as [first1, last1). If not, then the
        // ranges are not permutations.
        auto c = count(first2, last2, *i);
        if(c == 0)
          return false;
        else if(o_count(next(i), last1, *i) + 1 != c)
          // Start at the next i since we already know that *i == *i.
          return false;
      }
      return true;
    }

    

  // Equal elements (relation, impl)
  // Returns true if [first1, last1) is a rearrangement of [first2, last2).
  template<typename I1, typename I2, typename R>
    bool equal_elements_impl(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      static_assert(Comparison<I1, I2, R>(), "");
      assert(is_readable_range(first1, last1));
      assume(is_readable_range(first2, distance(first1, last1)));
      assume(is_equivalence_relation(comp));

      for(I1 i = first1; i != last1; ++i) {
        // Don't recount the number of times *i appears
        if(some_equal(first1, i, *i, comp))
          continue;

        // Count the number of times *i appears in [first2, first2 + n). It
        // needs to be the same count as [first1, last1). If not, then the
        // ranges are not permutations.
        auto c = count_equal(first2, last2, *i, comp);
        if(c == 0)
          return false;
        else if(count_equal(next(i), last1, *i, comp) + 1 != c)
          // Start at the next i since we already know that *i == *i.
          return false;
      }
      return true;
    }    



  // Equal elements
  // Returns true if [first1, last1) is a rearrangemnt of [first2, last2).
  //
  // This is the same as std::is_permutation, but renamed to reflect the
  // implementaiton strategy. The is_permutation algorithm is a sorting
  // algorithm.
  template<typename I1, typename I2>
    bool equal_elements(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));

      // Find where [first1, last1) and [first2, ...) differ. Then, count the 
      // number of times each element occurs in the remainder of the two 
      // ranges. Otherwise the two ranges are equal, and we're done.
      std::tie(first1, first2) = o_mismatch(first1, last1, first2);
      if(first1 != last1)
        return equal_elements(first1, last1, first2, last2);
      return true;
    }
    
            
    
  // Equal elements (relation)
  // Returns true if [first1, last1) is a rearrangemnt of [first2, last2).
  //
  // This is the same as std::is_permutation, but renamed to reflect the
  // implementaiton strategy. The is_permutation algorithm is a sorting
  // algorithm.
  template<typename I1, typename I2, typename R>
    bool equal_elements(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      static_assert(Comparison<I1, I2, R>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));
      assume(is_equivalence_relation(comp));

      std::tie(first1, first2) = o_mismatch(first1, last1, first2, comp);
      if(first1 != last1)
        return equal_elements(first1, last1, first2, last2, comp);
      return true;
    }

  

  // Search
  template<typename I1, typename I2>
    inline I1 o_search(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));
      
      return std::search(first1, last1, first2, last2);
    }
    
    
    
  // Search (relation)
  template<typename I1, typename I2, typename R>
    inline I1 o_search(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparison<I1, I2, R>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));

      return std::search(first1, last1, first2, last2, comp);
    }
    
    
  
  // Search (range)
  template<typename R1, typename R2>
    inline auto search(R1&& a, const R2& b) -> decltype(std::begin(a))
    {
      static_assert(Forward_range<R1>(), "");
      static_assert(Forward_range<R2>(), "");
      static_assert(Comparison<R1, R2>(), "");
      
      return o_search(std::begin(a), std::end(a), std::begin(b), std::end(b));
    }
    
    
  
  // Search (range, relation)
  template<typename R1, typename R2, typename Rel>
    inline auto search(R1&& a, const R2& b, Rel comp) -> decltype(std::begin(a))
    {
      static_assert(Forward_range<R1>(), "");
      static_assert(Forward_range<R2>(), "");
      static_assert(Comparison<R1, R2, Rel>(), "");
      
      return o_search(std::begin(a), std::end(a), std::begin(b), std::end(b), comp);
    }


  
  // Search end
  //
  // This is the same as std::find_end, but renamed to reflect its algorithm
  // family more closely.
  template<typename I1, typename I2>
    inline I1 search_end(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));
      
      return std::find_end(first1, last1, first2, last2);
    }
    
    
    
  // Search end (relation)
  template<typename I1, typename I2, typename R>
    inline I1 search_end(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparison<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));

      return std::find_end(first1, last1, first2, last2, comp);
    }
    
  

  // Search end (range)
  template<typename R1, typename R2>
    inline auto search_end(R1&& a, const R2& b) -> decltype(std::begin(a))
    {
      static_assert(Forward_range<R1>(), "");
      static_assert(Forward_range<R2>(), "");
      static_assert(Comparison<R1, R2>(), "");
      
      return search_end(std::begin(a), std::end(a), std::begin(b), std::end(b));
    }
    
    
  
  // Search end (range, relation)
  template<typename R1, typename R2, typename Rel>
    inline auto search_end(R1&& a, const R2& b, Rel comp) -> decltype(std::begin(a))
    {
      static_assert(Forward_range<R1>(), "");
      static_assert(Forward_range<R2>(), "");
      static_assert(Comparison<R1, R2, Rel>(), "");
      
      return search_end(std::begin(a), std::end(a), std::begin(b), std::end(b), comp);
    }


  
  // Search n
  template<typename I, typename T>
    inline I o_search_n(I first, I last, Distance_type<I> n, const T& value)
    {
      static_assert(Forward_iterator<I>(), "");
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));

      return std::search_n(first, last, n, value);
    }
    
    
    
  // Search n (relation)
  template<typename I, typename T, typename R>
    inline I o_search_n(I first, I last, Distance_type<I> n, const T& value, R comp)
    {
      static_assert(Forward_iterator<I>(), "");
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));

      return std::search_n(first, last, n, value, comp);
    }
    
    
  
  // Search n (range)
  template<typename R, typename T>
    inline auto search_n(R&& range, Distance_type<R> n, const T& value)
      -> decltype(std::begin(range))
    {
      static_assert(Forward_range<R>(), "");
      static_assert(Range_search<R, T>(), "");
    
      return o_search_n(std::begin(range), std::end(range), n, value);
    }
    
    
    
  // Search n (range, relation)
  template<typename R, typename T, typename Rel>
    inline auto search_n(R&& range, Distance_type<R> n, const T& value, Rel comp)
      -> decltype(std::begin(range))
    {
      static_assert(Forward_range<R>(), "");
      static_assert(Range_search<R, T, Rel>(), "");
    
      return o_search_n(std::begin(range), std::end(range), n, value, comp);
    }

    
} // namespace origin


#include <origin/algorithm/copy.hpp>


namespace origin 
{
  // Transform (unary)
  //
  // FIXME: The combination of Input_iterator and Regular_function should be
  // called Transform. Are there other useful occurrences of this conceptf?
  template<typename I, typename O, typename F>
    inline O o_transform(I first, I last, O result, F f)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Regular_function<F, Value_type<I>>(), "");
      static_assert(Weak_output_iterator<O, Result_of<F(Value_type<I>)>>(), "");
      
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, o_distance(first, last)));
      
      while(first != last) {
        *result = f(*first);
        ++first;
        ++result;
      }
      return result;
    }
    
    
    
  // Trnsform (unary, range)
  //
  // FIXME: The combination of Input_range and Regular function should be
  // called Range_transform
  template<typename R1, typename R2, typename F>
    inline void transform(const R1& in, R2& out, F f)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Regular_function<F, Value_type<R2>>(), "");
      static_assert(Output_range<R2, Result_of<F(Value_type<R1>)>>(), "");
      assume(size(in) <= size(out));

      o_transform(std::begin(in), std::end(in), std::begin(out), f);
    }
    
    
    
    
  // Transform (binary)
  template<typename I1, typename I2, typename O, typename F>
    O o_transform(I1 first1, I1 last1, I2 first2, O result, F f)
    {
      static_assert(Input_iterator<I1>(), "");
      static_assert(Input_iterator<I2>(), "");
      static_assert(Regular_function<F, Value_type<I1>(), Value_type<I2>>(), "");
      static_assert(Weak_output_iterator<O, Result_of<F(Value_type<I1>, Value_type<I2>)>>(), "");
      assert(is_readable_range(first1, last1));
      assume(is_readable_range(first2, distance(first1, last1)));
      assume(is_writable_range(result, distance(first1, last1)));

      while(first1 != last1) {
        *result = f(*first1, *first2);
        ++first1;
        ++first2;
        ++result;
      }
      return result;
    }

    
    
  template<typename R1, typename R2, typename R3, typename F>
    inline void transform(const R1& range1, const R2& range2, R3& result, F f)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");
      static_assert(Regular_function<F, Value_type<R1>(), Value_type<R2>>(), "");
      static_assert(Output_range<R3, Result_of<F(Value_type<R1>, Value_type<R2>)>>(), "");

      return o_transform(std::begin(range1), std::end(range1), 
                           std::begin(range2), std::begin(result), f);
    }
    
    

  // Fill
  template<typename O, typename T>
    void o_fill(O first, O last, const T& value)
    {
      static_assert(Fill<O, T>(), "");
      assert(is_writable_range(first, last, value));
      
      while(first != last) {
        *first = value;
        ++first;
      }
    }
    
    
    
  // Fill (range)
  template<typename R, typename T>
    inline void fill(R& range, const T& value)
    {
      static_assert(Copyable<T>(), "");
      static_assert(Output_range<T>(), "");
      
      return o_fill(std::begin(range), std::end(range));
    }
    
    
    
  // Fill n
  template<typename O, typename T>
    inline O o_fill_n(O first, Distance_type<O> n, const T& value)
    {
      static_assert(Copyable<T>(), "");
      static_assert(Weak_output_iterator<O, T>(), "");
      assert(is_writable_range(first, n, value));

      while(n != 0) {
        *first = value;
        ++first;
        --n;
      }
    }
    
    
    
  // Generate
  template<typename O, typename F>
    inline F o_generate(O first, O last, F gen)
    {
      static_assert(Function<F>(), "");
      static_assert(Output_iterator<O, Result_of<F()>>(), "");
      assume(is_writable_range(first, last, gen()));
      
      while(first != last) {
        *first = gen();
        ++first;
      }
      return gen;
    }
    
    
    
  // Generate (range)
  template<typename R, typename F>
    inline F generate(R&& range, F gen)
    {
      using Rx = Forwarded<R>;
      static_assert(Function<F>(), "");
      static_assert(Range_fill<Rx, Result_of<F()>>(), "");

      return o_generate(std::begin(range), std::end(range), gen);
    }
    
    
    
  // Generate n
  template<typename O, typename F>
    inline std::pair<O, F> o_generate_n(O first, Distance_type<O> n, F gen)
    {
      static_assert(Function<F>(), "");
      static_assert(Output_iterator<O, Result_of<F()>>(), "");
      assert(is_writable_range(first, n));

      while(n != 0) {
        *first = gen();
        ++first;
        --n;
      }
      return {first, gen};
    }
  
  

  // Reverse and Rotate Permutations
  // The reverse and rotate algorithms define permutations of a sequence.
  //
  // TODO: Implement a (perfect) riffle algorithm.



  // Reverse (iterator)
  template <typename I>
    void o_reverse(I first, I last)
    {
      while (first != last) {
        --last;
        if (first == last)
          break;
        iter_swap(first, last);
        ++first;
      }
    }
  


  // Reverse (range)
  template <typename R>
    void reverse(R&& range)
    {
      static_assert(Bidirectional_range<Forwarded<R>>(), "");
      return o_reverse(o_begin(range), o_end(range));
    }
  

  
  // Reverse copy
  template <typename I, typename O>
    void o_reverse_copy(I first, I last, O result)
    {
      while (first != last) {
        --result;
        *result = *first;
        ++first;
      }
    }
    
} // namespace origin

#include <origin/algorithm/replace.hpp>
#include <origin/algorithm/remove.hpp>

namespace origin {

  // Rotate
  template<typename I>
    I o_rotate(I first, I mid, I last)
    {
      static_assert(Permutation<I>(), "");

      return std::rotate(first, mid, last);
    }
    
  
  
  // Rotate copy
  template<typename I, typename O>
    I o_rotate_copy(I first, I mid, I last, O result)
    {
      static_assert(Permutation<I>(), "");

      return std::rotate_copy(first, mid, last, result);
    }



  // Random Permutations
  // This family of algorithms randomly permute a sequence of elements by
  // shuffling them.
  //
  // TODO: Implement shuffle copy algorithms.
  //
  // TODO: Implement a randomized riffle algorithm.
  

  // Random shuffle
  template <typename I>
    void o_random_shuffle(I first, I last)
    {
      static_assert(Random_access_iterator<I>(), "");
      static_assert(Permutation<I>(), "");

      
      if (first != last) {
        for (I i = first + 1; i != last; ++i)
          iter_swap(i, first + rand() % ((i - first) + 1));
      }
    }



  // Random shuffle (range)
  template <typename R>
    void random_shuffle(R&& range)
    {
      return o_random_shuffle(o_begin(range), o_end(range));
    }


      
  // Random shuffle (generator)
  template<typename I, typename Gen>
    void o_random_shuffle(I first, I last, Gen&& rand)
    {
      static_assert(Random_access_iterator<I>(), "");
      static_assert(Permutation<I>(), "");

      if(first != last) {
        for(I i = first + 1; i != last; ++i)
          iter_swap(i, first + rand((i - first) + 1));
      }
    }
    
  

  // Random shuffle (range)
  template <typename R, typename Gen>
    void random_shuffle(R&& range, Gen&& rand)
    {
      return o_random_shuffle(o_begi(range), o_end(range), rand);
    }



  // Shuffle (iterator)
  //
  // FIXME: Implement me!
  template<typename I, typename Gen>
    void o_shuffle(I first, I last, Gen&& rand)
    {
      static_assert(Random_access_iterator<I>(), "");
      static_assert(Permutation<I>(), "");
      
      std::shuffle(first, last, std::forward<Gen>(rand));
    }



    // Shuffle (range)
  template <typename R, typename Gen>
    void shuffle(R&& range, Gen&& rand)
    {
      o_shuffle(o_begin(range), o_end(range), rand);
    }
    

  
  // Partitions
  // This family of algorithms deals with partitions of a sequence.
  
  // Is partitioned
  template<typename I, typename P>
    inline bool o_is_partitioned(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

      first = find_if_not(first, last, pred);
      if(first != last)
        return none_of(o_next(first), last, pred);
      else
        return true;
    }
    
    
  
  // Is partitioned (range)
  template<typename R, typename P>
    bool is_partitioned(const R& range, P pred)
    {
      static_assert(Range_query<R, P>(), "");

      return o_is_partitioned(std::begin(range), std::end(range), pred);
    }
    
  
  
  // Partition point
  // Return an iterator i such that all_of(first, i) is true and 
  // none_of(i, last) is also true.
  //
  // FIXME Actually implement me.
  template<typename I, typename P>
    I o_partition_point(I first, I last, P pred)
    {
      static_assert(Forward_iterator<I>(), "");
      static_assert(Query<I, P>(), "");

      return std::partition_point(first, last, pred);
    }
    
  
  
  template<typename R, typename P>
    auto partition_point(const R& range, P pred) -> decltype(std::begin(range))
    {
      static_assert(Forward_range<R>(), "");
      static_assert(Range_query<R, P>(), "");

      return o_partition_point(std::begin(range), std::end(range), pred);
    }
    
  
  
  // Paritition
  template<typename I, typename P>
    I o_partition(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Permutation<I>(), "");

      first = find_if_not(first, last, pred);
      if(first != last)
        exchange_if(o_next(first), last, first, pred);
      return first;
    }

  
  
  // Partition (range)
  template<typename R, typename P>
    auto partition(R&& range, P pred) -> decltype(std::begin(range))
    {
      using Rx = Forwarded<R>;
      static_assert(Range_query<Rx, P>(), "");
      static_assert(Range_permutation<Rx>(), "");
      
      return o_partition(std::begin(range), std::end(range), pred);
    }
  
  
  // Stable partition
  template<typename I, typename P>
    I o_stable_partition(I first, I last, P pred)
    {
      return std::stable_partition(first, last, pred);
    }
    
    
  
  // Partition copy
  template<typename I, typename Out1, typename Out2, typename P>
    std::pair<Out1, Out2> o_partition_copy(I first, I last, Out1 out_true, Out2 out_false, P pred)
    {
      return std::partition_copy(first, last, out_true, out_false, pred);
    }

    
    
  // Lexicographical compare (relation)
  template<typename I1, typename I2, typename R>
    bool lexicographical_compare(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      while(true) {
        if(first1 == last1) return false;
        if(first2 == last2) return true;
        if(comp(*first1, *first2)) return true;
        if(comp(*first2, *first1)) return false;
        ++first1;
        ++first2;
      }
    }
    
    
  
  // Lexicographical compare (less)
  template<typename I1, typename I2>
    bool lexicographical_compare(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      while(true) {
        if(first1 == last1) return false;
        if(first2 == last2) return true;
        if(comp(*first1, *first2)) return true;
        if(comp(*first2, *first1)) return false;
        ++first1;
        ++first2;
      }
    }

} // namespace origin

#include <origin/algorithm/sort.hpp>
#include <origin/algorithm/binary_search.hpp>
#include <origin/algorithm/merge.hpp>
#include <origin/algorithm/set.hpp>
#include <origin/algorithm/heap.hpp>
#include <origin/algorithm/minmax.hpp>
#include <origin/algorithm/permutation.hpp>


// Include definitions of container concepts needed for some algorithm
// specializations (e.g., find).
#include <origin/container.hpp>



#endif
