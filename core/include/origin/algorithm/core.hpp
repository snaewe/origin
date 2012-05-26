// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_CORE_HPP
#define ORIGIN_ALGORITHM_CORE_HPP

// FIXME: Remove this header!
#include <algorithm>

#include <origin/functional.hpp>
#include <origin/iterator/core.hpp>
#include <origin/range/core.hpp>

namespace origin
{
  template <typename T> class temporary_buffer;


  // NOTE: The * algorithms are so named to prevent name collisions with
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



  // Range copy out (concept)
  // The copy out concept describes algorithms that copy from an input range
  // into an output iterator.
  template <typename R, typename O>
    constexpr bool Range_copy_out()
    {
      return Input_range<R>() && Output_iterator<O, Value_type<R>>();
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
  //
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
  //
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

} // namespace origin

#endif
