
// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
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
  // NOTE: The std_* algorithms simply add Origin-style concept checking to
  // std algorithms. They are primiarly used for testing


  // Query (concept)
  // A query is an algorithm that iterates over the elements of a range,
  // searching for one or more that satisfy some property. The property is
  // given as a unary predicate.
  template<typename I, typename P>
    constexpr bool Query()
    {
      return Input_iterator<I>() && Predicate<P, Value_type<I>>();
    }

  // Range queryable (concept)
  // A range is queryable if its iterator type is queryable.
  template<typename R, typename P>
    constexpr bool Range_query()
    {
      return Input_range<R>() && Query<Iterator_type<R>, P>();
    }

     

  // Search (concept)
  // A search is an algorithm that iterates over the elements of a range,
  // searching for one or more that is equal to a value type. The value type
  // may the same as or different than as the iterator's, and the equality
  // comparison may be generalized to a relation. There are three overloads of
  // this concept.
  //
  //    Search<I>       Can be searched for a Value_type<I> value
  //    Search<I, T>    Can be searched for a T value
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

  // Search<I> implementation.
  template<typename I>
    struct Search_concept<I, default_t, default_t>
    {
      static constexpr bool check()
      {
        return Input_iterator<I>() && Equality_comparable<Value_type<I>>();
      }
    };
     
  // Returns true if the iterator can be used for searching.
  template<typename I, typename T = default_t, typename R = default_t>
    constexpr bool Search()
    {
      return Search_concept<I, T, R>::check();
    }
  
  
  
  // Range seachable (concept)
  // A range is searchable if its underlying iterator type is searchable. Like 
  // the searchable concept, there are three forms of this concept.
  //
  //    Range_search<R>         Can be searched for a Value_type<R> value
  //    Range_search<R, T>      Can be searched for a T value
  //    Range_search<R, T, Rel> Can be searched for a T value using a relation R
  
  // Returns true if the input iterator can be searched for a value of type T.
  template<typename R, typename T = default_t, typename Rel = default_t>
    constexpr bool Range_search()
    {
      return Input_range<R>() && Search<Iterator_type<R>, T, Rel>();
    }

  
  
  // Comparable iterators (concept)
  // A comparison algorithm compares the elements in two different ranges for 
  // equality. The comparison can also be generalized to a relation. There are
  // two forms of this concept:
  //
  //    Comparable<I1, I2>    Compare the elements of I1 and I2 for equality
  //    Comparable<I1, I2, R> Compare the elements of I1 and I2 using the relation R
  
  // Comparable<I1, I2, R> implementation
  template<typename I1, typename I2, typename R>
    struct Comparable_concept
    {
      static constexpr bool check()
      {
        return Input_iterator<I1>()
            && Weak_input_iterator<I2>()
            && Relation<R, Value_type<I1>, Value_type<I2>>();
      }
    };
  
  // Comparable<I1, I2> implementation
  template<typename I1, typename I2>
    struct Comparable_concept<I1, I2, default_t>
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
    constexpr bool Comparable()
    {
      return Comparable_concept<I1, I2, R>::check();
    }
    
    

  // Comparable ranges (concept)
  // Two ranges are comparable if their iterators can be compared. Like the
  // Comparable concept, there are two forms of this concept:
  //
  //    Range_comparable<R1, R2>      Compare the elements of R1 and R2 for equality
  //    Range_comparable<R1, R2, Rel> Compare the elements of R1 and R2 using Rel
  
  // Returns true if R1 and R2 are ranges that can be compared using value
  // equality or a relation R.
  template<typename R1, typename R2, typename Rel = default_t>
    constexpr bool Range_comparable()
    {
      return Input_range<R1>() 
          && Input_range<R2>() 
          && Comparable<Iterator_type<R1>, Iterator_type<R2>, Rel>();
    }



  // Mergeable
  // The mergeable concept describes common requirements on the family of
  // (non-inplace) merge and set operations. There are two mergeable concepts:
  // - Mergeable<Iter1, Iter2, O>
  // - Mergeable<Iter1, Iter2, O, R>
  // The first requires the value types of Iter1 and Iter2 to be totally
  // ordered. The second is generalized over a relation.
  
  // This specialization implements the requirements for the generalized
  // version.
  template<typename Iter1, typename Iter2, typename O, typename R>
    struct Mergeable_concept
    {
      static constexpr bool check()
      {
        return Input_iterator<Iter1>() && 
               Input_iterator<Iter2>() &&
               Weakly_incrementable<O>() &&
               Writable<O, Value_type<Iter1>>() &&
               Writable<O, Value_type<Iter2>>() &&
               Relation<R, Value_type<Iter1>, Value_type<Iter2>>();
      }
    };

  // This specialization implements the requirements for algorithms that 
  // directly use the < operator.
  template<typename Iter1, typename Iter2, typename O>
    struct Mergeable_concept<Iter1, Iter2, O, default_t>
    {
      static constexpr bool check()
      {
        return Input_iterator<Iter1>() && 
               Input_iterator<Iter2>() &&
               Weakly_incrementable<O>() &&
               Writable<O, Value_type<Iter1>>() &&
               Writable<O, Value_type<Iter2>>() &&
               Totally_ordered<Value_type<Iter1>, Value_type<Iter2>>();
      }
    };
    
  // Returns true when Iter1 and Iter2 can be merged into O.
  template<typename Iter1, typename Iter2, typename O, typename R = default_t>
    constexpr bool Mergeable()
    {
      return Mergeable_concept<Iter1, Iter2, O, R>::check();
    }



  // Sortable
  // The Sortable concept describes the requirements of algorithms that permute
  // ranges sequences in some sorted order. There are two:
  //
  //    Sortable<I>
  //    Sortable<I, R>
  
  // Requirements for the generalized overload.
  template<typename I, typename R>
    struct Sortable_concept
    {
      static constexpr bool check()
      {
        return Permutable_iterator<I>() && Relation<R, Value_type<I>>();
      }
    };
    
  // Requirements using operator <.
  template<typename I>
    struct Sortable_concept<I, default_t>
    {
      static constexpr bool check()
      {
        return Permutable_iterator<I>() && Totally_ordered<Value_type<I>>();
      }
    };
    
  // Returns true if I is sortable.
  template<typename I, typename R = default_t>
    constexpr bool Sortable()
    {
      return Sortable_concept<I, R>::check();
    }



  // Range sortable
  // The range sortable concept describes common requirements for sorting
  // iterator ranges. There are two forms of this concept:
  //
  //    Range_sortable<R>
  //    Range_sortable<R, Rel>

  // Requirements for the generalized overload.
  template<typename R, typename Rel>
    struct Range_sortable_concept
    {
      static constexpr bool check()
      {
        return Permutable_range<R>() && Relation<Rel, Value_type<R>>();
      }
    };
    
  // Requirements using operator <.
  template<typename R>
    struct Range_sortable_concept<R, default_t>
    {
      static constexpr bool check()
      {
        return Permutable_range<R>() && Totally_ordered<Value_type<R>>();
      }
    };
    
  // Returns true if R is a sortable range.
  template<typename R, typename Rel = default_t>
    constexpr bool Range_sortable()
    {
      return Range_sortable_concept<R, Rel>::check();
    }
}

#include <origin/algorithm/quantifier.hpp>
#include <origin/algorithm/find.hpp>
#include <origin/algorithm/count.hpp>

namespace origin 
{
  // Equal
  // Returns true if *i == *j for each iterator i and j in [first1, last1) and
  // [first2, first2 + (last1 - first1)), pairwise.
  template<typename I1, typename I2>
    inline auto std_equal(I1 first1, I1 last1, I2 first2)
      -> Requires<!Can_memcmp<I1, I2>(), bool>
    {
      static_assert(Comparable<I1, I2>(), "");
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
    inline auto std_equal(I1 first1, I1 last1, I2 first2)
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
      static_assert(Range_comparable<R1, R2>(), "");
    
      return std_equal(std::begin(a), std::end(a), std::begin(b));
    }


    
  // Equal (random access range)
  // This optimization for random access ranges returns true if 
  // size(a) <= size(b) and x == y for each x and y in the ranges a and b, 
  // pairwise.
  template<typename R1, typename R2>
    inline auto equal(R1 const& a, R2 const& b)
      -> Requires<Random_access_range<R1>() && Random_access_range<R2>(), bool>
    {
      static_assert(Range_comparable<R1, R2>(), "");
    
      return size(a) <= size(b) 
          && std_equal(std::begin(a), std::end(a), std::begin(b));
    }
  
  
    
  // Equal (relation)
  // Returns true if comp(*i, *j), for each iterator i and j in [first1, last1) 
  // and [first2, first2 + (last1 - first1)) pairwise.
  //
  // Note that this algorithm cannot be optimized by memcmp.
  template<typename I1, typename I2, typename R>
    inline bool std_equal(I1 first1, I1 last1, I2 first2, R comp)
    {
      static_assert(Comparable<I1, I2>(), "");
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
      static_assert(Range_comparable<R1, R2, Rel>(), "");

      return std_equal(std::begin(a), std::end(a), std::begin(b), comp);
    }

    
    
  // Equal (random access range)
  // This specialization for random access ranges returns true if 
  // size(a) <= size(b) and comp(x, y) is true for each x and y in the ranges a 
  // and b, pairwise.
  template<typename R1, typename R2, typename Rel>
    inline auto equal(const R1& a, const R2& b, Rel comp)
      -> Requires<Random_access_range<R1>() && Random_access_range<R2>(), bool>
    {
      static_assert(Range_comparable<R1, R2, Rel>(), "");

      return size(a) <= size(b)
          && std_equal(std::begin(a), std::end(a), std::begin(b), comp);
    }



  // Mismatch
  // Returns the first iterator i in [first1, last1) where *i != *j and *j is
  // the corresponding iterator in j.
  template<typename I1, typename I2>
    inline auto std_mismatch(I1 first1, I1 last1, I2 first2)
      -> std::pair<I1, I2>
    {
      static_assert(Comparable<I1, I2>(), "");
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
      static_assert(Range_comparable<R1>(), "");

      return std_mismatch(std::begin(a), std::end(a), std::begin(b));
    }

    
    
  // Mismatch (relation)
  template<typename I1, typename I2, typename R>
    inline auto std_mismatch(I1 first1, I1 last1, I2 first2, R comp)
      -> std::pair<I1, I2> 
    {
      static_assert(Comparable<I1, I2, R>(), "");
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
      static_assert(Range_comparable<R1, R2, Rel>(), "");

      return std_mismatch(std::begin(a), std::end(a), std::begin(b), comp);
    }

  
    
  // Equal elements (impl)
  // Returns true if [first1, last1) is a rearrangement of [first2, last2).
  template<typename I1, typename I2>
    bool equal_elements_impl(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Comparable<I1, I2>(), "");
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
        else if(std_count(next(i), last1, *i) + 1 != c)
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
      static_assert(Comparable<I1, I2, R>(), "");
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
      static_assert(Comparable<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));

      // Find where [first1, last1) and [first2, ...) differ. Then, count the 
      // number of times each element occurs in the remainder of the two 
      // ranges. Otherwise the two ranges are equal, and we're done.
      std::tie(first1, first2) = std_mismatch(first1, last1, first2);
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
      static_assert(Comparable<I1, I2, R>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));
      assume(is_equivalence_relation(comp));

      std::tie(first1, first2) = std_mismatch(first1, last1, first2, comp);
      if(first1 != last1)
        return equal_elements(first1, last1, first2, last2, comp);
      return true;
    }

  

  // Search
  template<typename I1, typename I2>
    inline I1 std_search(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparable<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));
      
      return std::search(first1, last1, first2, last2);
    }
    
    
    
  // Search (relation)
  template<typename I1, typename I2, typename R>
    inline I1 std_search(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparable<I1, I2, R>(), "");
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
      static_assert(Comparable<R1, R2>(), "");
      
      return std_search(std::begin(a), std::end(a), std::begin(b), std::end(b));
    }
    
    
  
  // Search (range, relation)
  template<typename R1, typename R2, typename Rel>
    inline auto search(R1&& a, const R2& b, Rel comp) -> decltype(std::begin(a))
    {
      static_assert(Forward_range<R1>(), "");
      static_assert(Forward_range<R2>(), "");
      static_assert(Comparable<R1, R2, Rel>(), "");
      
      return std_search(std::begin(a), std::end(a), std::begin(b), std::end(b), comp);
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
      static_assert(Comparable<I1, I2>(), "");
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
      static_assert(Comparable<I1, I2>(), "");
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
      static_assert(Comparable<R1, R2>(), "");
      
      return search_end(std::begin(a), std::end(a), std::begin(b), std::end(b));
    }
    
    
  
  // Search end (range, relation)
  template<typename R1, typename R2, typename Rel>
    inline auto search_end(R1&& a, const R2& b, Rel comp) -> decltype(std::begin(a))
    {
      static_assert(Forward_range<R1>(), "");
      static_assert(Forward_range<R2>(), "");
      static_assert(Comparable<R1, R2, Rel>(), "");
      
      return search_end(std::begin(a), std::end(a), std::begin(b), std::end(b), comp);
    }


  
  // Search n
  template<typename I, typename T>
    inline I std_search_n(I first, I last, Distance_type<I> n, const T& value)
    {
      static_assert(Forward_iterator<I>(), "");
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));

      return std::search_n(first, last, n, value);
    }
    
    
    
  // Search n (relation)
  template<typename I, typename T, typename R>
    inline I std_search_n(I first, I last, Distance_type<I> n, const T& value, R comp)
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
    
      return std_search_n(std::begin(range), std::end(range), n, value);
    }
    
    
    
  // Search n (range, relation)
  template<typename R, typename T, typename Rel>
    inline auto search_n(R&& range, Distance_type<R> n, const T& value, Rel comp)
      -> decltype(std::begin(range))
    {
      static_assert(Forward_range<R>(), "");
      static_assert(Range_search<R, T, Rel>(), "");
    
      return std_search_n(std::begin(range), std::end(range), n, value, comp);
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
    inline O std_transform(I first, I last, O result, F f)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Regular_function<F, Value_type<I>>(), "");
      static_assert(Weak_output_iterator<O, Result_of<F(Value_type<I>)>>(), "");
      
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, std_distance(first, last)));
      
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

      std_transform(std::begin(in), std::end(in), std::begin(out), f);
    }
    
    
    
    
  // Transform (binary)
  template<typename I1, typename I2, typename O, typename F>
    O std_transform(I1 first1, I1 last1, I2 first2, O result, F f)
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

      return std_transform(std::begin(range1), std::end(range1), 
                           std::begin(range2), std::begin(result), f);
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
    
    
    
  // Fill
  template<typename O, typename T>
    void std_fill(O first, O last, const T& value)
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
      
      return std_fill(std::begin(range), std::end(range));
    }
    
    
    
  // Fill n
  template<typename O, typename T>
    inline O std_fill_n(O first, Distance_type<O> n, const T& value)
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
    inline F std_generate(O first, O last, F gen)
    {
      static_assert(Function<F>(), "");
      static_assert(Output_iterator<O, Result_of<F()>>(), "");
      assert(is_writable_range(first, last));
      
      while(first != last) {
        *first = gen();
        ++first;
      }
      return gen;
    }
    
    
    
  // Generate (range)
  template<typename R, typename F>
    inline F generate(R& range, F gen)
    {
      static_assert(Function<F>(), "");
      static_assert(Output_range<R, Result_of<F()>>(), "");

      return std_generate(std::begin(range), std::end(range));
    }
    
    
  // Generate n
  template<typename O, typename F>
    inline std::pair<O, F> std_generate_n(O first, Distance_type<O> n, F gen)
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
  
  
  
  // The Replace Family

  // Replace
  template<typename I, typename T>
    void std_replace(I first, I last, const T& old_value, const T& new_value)
    {
      static_assert(Search<I, T>(), "");
      static_assert(Copy<I, T>(), "");

      while(first != last) {
        if(*first == old_value)
          *first = new_value;
        ++first;
      }
    }
    
    
    
  // Replace (range)
  template<typename R, typename T>
    inline void replace(R& range, const T& old_value, const T& new_value)
    {
      static_assert(Range_search<R, T>(), "");
      static_assert(Range_copy<R, T>(), "");
      
      std_replace(std::begin(range), std::end(range), old_value, new_value);
    }
    
  

  // Replace_if
  template<typename I, typename P, typename T>
    inline void std_replace_if(I first, I last, P pred, const T& new_value)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, T>(), "");
      
      while(first != last) {
        if(pred(*first)) 
          *first = new_value;
        ++first;
      }
    }
    
    
  
  // Replace if (range)
  template<typename R, typename P, typename T>
    inline void replace_if(R& range, P pred, const T& new_value)
    {
      static_assert(Range_query<R, P>(), "");
      static_assert(Range_copy<R, T>(), "");
      
      std_replace_if(std::begin(range), std::end(range), pred, new_value);
    }

    
    
  // Replace copy
  template<typename I, typename O, typename T>
    inline O std_replace_copy(I first, I last, O result, const T& old_value, const T& new_value)
    {
      static_assert(Search<I, T>(), "");
      static_assert(Copy<I, O>(), "");
      static_assert(Fill<O, T>(), "");
      
      while(first != last) {
        if(*first == old_value)
          *result = new_value;
        else
          *result = *first;
        ++result;
        ++first;
      }
      return result;
    }
    
    
  
  // Replace copy if
  template<typename I, typename O, typename P, typename T>
    inline O std_replace_copy_if(I first, I last, O result, P pred, const T& new_value)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Copy<I, O>(), "");
      static_assert(Fill<O, T>(), "");
      
      while(first != last) {
        if(pred(*first))
          *result = new_value;
        else
          *result = *first;
        ++result;
        ++first;
      }
      return result;
    }



  // The Remove Family

  // Remove
  template<typename I, typename T>
    I std_remove(I first, I last, const T& value)
    {
      return std::remove(first, last, value);
    }
  
  
  
  // Remove if
  template<typename I, typename Pred>
    I std_remove_if(I first, I last, Pred pred)
    {
      return std::remove_if(first, last, pred);
    }
    
    
  // Remove copy  
  template<typename I, typename O, typename T>
    O std_remove_copy(I first, I last, O result, const T& value)
    {
      return std::remove_copy(first, last, result, value);
    }
    
  
  
  // Remove copy if
  template<typename I, typename O, typename Pred>
    O std_remove_copy_if(I first, I last, O result, Pred pred)
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
  template<typename I, typename O, typename T>
    std::pair<I, O> extract(I first, I last, O result, const T& value)
    {
      static_assert(Permutable_iterator<I>(), "");
      static_assert(Search<I, T>(), "");
      static_assert(Move<I, O>(), "");
      assert(( is_readable_range(first, last) ));
      assume(( is_movable_range(result, std_count(first, last, value), *first) ));

      first = std::find(first, last, value);
      if(first == last)
        return {first, result};

      *result = std::move(*first);
      ++result;
      I hole = first;
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
  template<typename I, typename O, typename Pred>
    std::pair<I, O> extract_if(I first, I last, O result, Pred pred)
    {
      static_assert(Permutable_iterator<I>(), "");
      static_assert(Query<I, Pred>(), "");
      static_assert(Move<I, O>(), "");
      assert(( is_readable_range(first, last) ));
      assume(( is_movable_range(result, std_count_if(first, last, pred), *first) ));

      first = std::find_if(first, last, pred);
      if(first == last)
        return {first, result};

      *result = std::move(*first);
      ++result;
      I hole = first;
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
  template<typename I>
    I std_unique(I first, I last)
    {
      return std::unique(first, last);
    }
    
  
  
  // Unique (predicate)
  template<typename I, typename R>
    I std_unique(I first, I last, R comp)
    {
      return std::unique(first, last, comp);
    }
    
    
  
  // Unique copy
  template<typename I, typename O>
    O std_unique_copy(I first, I last, O result)
    {
      return std::unique(first, last, result);
    }
    
    
    
  // Unique copy (predicate)
  template<typename I, typename O, typename R>
    O std_unique_copy(I first, I last, O result, R comp)
    {
      return std::unique_copy(first, last, result, comp);
    }



  // Reverse and Rotate Permutations
  // The reverse and rotate algorithms define permutations of a sequence.
  //
  // TODO: Implement a (perfect) riffle algorithm.

  // Reverse
  template<typename I>
    void std_reverse(I first, I last)
    {
      std::reverse(first, last);
    }
  
  
  
  // Reverse copy
  template<typename I, typename O>
    void std_reverse_copy(I first, I last, O result)
    {
      std::reverse(first, last, result);
    }
    
    

  // Rotate
  template<typename I>
    I std_rotate(I first, I mid, I last)
    {
      return std::rotate(first, mid, last);
    }
    
  
  
  // Rotate copy
  template<typename I, typename O>
    I std_rotate_copy(I first, I mid, I last, O result)
    {
      return std::rotate_copy(first, mid, last, result);
    }



  // Random Permutations
  // This family of algorithmsm randomly permute a sequence of elements by
  // shuffling them.
  //
  // TODO: Implement a randomized riffle algorithm.

  // Random shuffle
  template<typename I>
    void std_random_shuffle(I first, I last)
    {
      return std::random_shuffle(first, last);
    }
    
  
  
  // Random shuffle (generator)
  template<typename I, typename Gen>
    void std_random_shuffle(I first, I last, Gen&& rand)
    {
      return std::random_shuffle(first, last, rand);
    }
    
  
  
  // Shuffle
  template<typename I, typename Gen>
    void std_shuffle(I first, I last, Gen&& rand)
    {
      return std::shuffle(first, last, rand);
    }
    

  
  // Partitions
  // This family of algorithms deals with partitions of a sequence.
  
  // Is partitioned
  template<typename I, typename Pred>
    bool std_is_partitioned(I first, I last, Pred pred)
    {
      return std::is_partitioned(first, last, pred);
    }
    
  
  
  // Partition point
  template<typename I, typename Pred>
    I std_partition_point(I first, I last, Pred pred)
    {
      return std::partition_point(first, last, pred);
    }
    
  
  
  // Paritition
  template<typename I, typename Pred>
    I std_partition(I first, I last, Pred pred)
    {
      return std::partition(first, last, pred);
    }
  
  
  
  // Stable partition
  template<typename I, typename Pred>
    I std_stable_partition(I first, I last, Pred pred)
    {
      return std::stable_partition(first, last, pred);
    }
    
    
  
  // Partition copy
  template<typename I, typename Out1, typename Out2, typename Pred>
    std::pair<Out1, Out2> std_partition_copy(I first, I last, 
                                             Out1 out_true, Out2 out_false, 
                                             Pred pred)
    {
      return std::partition_copy(first, last, out_true, out_false, pred);
    }



  // Sorting
  // This family of algorithms deals with the ordering of the elements of
  // a sequence.
  
  // Is sorted
  template<typename I>
    bool std_is_sorted(I first, I last)
    {
      return std::is_sorted(first, last);
    }
  
  
  
  // Is sorted (relation)
  template<typename I, typename Ord>
    bool std_is_sorted(I first, I last, Ord comp)
    {
      return std::is_sorted(first, last, comp);
    }



  // Is sorted until
  template<typename I>
    I std_is_sorted_until(I first, I last)
    {
      return std::is_sorted(first, last);
    }
  
  
  
  // Is sorted until (relation)
  template<typename I, typename Ord>
    I std_is_sorted_until(I first, I last, Ord comp)
    {
      return std::is_sorted(first, last, comp);
    }



  // Sort
  template<typename I>
    void std_sort(I first, I last)
    {
      return std::sort(first, last);
    }
    
    
    
  // Sort (relation)
  template<typename I, typename R>
    void std_sort(I first, I last, R comp)
    {
      return std::sort(first, last, comp);
    }
    
    
  
  // Stable sort
  template<typename I>
    void std_stable_sort(I first, I last)
    {
      return std::stable_sort(first, last);
    }
    
    
    
  // Stable sort (relation)
  template<typename I, typename Ord>
    void std_stable_sort(I first, I last, Ord comp)
    {
      return std::stable_sort(first, last, comp);
    }



  // Partial sort
  template<typename I>
    void std_partial_sort(I first, I middle, I last)
    {
      return std::partial_sort(first, last);
    }
    
    
    
  // Partial sort (relation)
  template<typename I, typename Ord>
    void std_partial_sort(I first, I middle, I last, Ord comp)
    {
      return std::partial_sort(first, last, comp);
    }
    
    

  // Partial sort copy
  template<typename I1, typename I2>
    void std_partial_sort_copy(I1 first, I1 last, 
                               I2 result_first, I2 result_last)
    {
      return std::partial_sort_copy(first, last, result_first, result_last);
    }
    
    
    
  // Partial sort copy (relation)
  template<typename I1, typename I2, typename Ord>
    void std_partial_sort_copy(I1 first, I1 last, 
                               I2 result_first, I2 result_last, Ord comp)
    {
      return std::partial_sort_copy(first, last, result_first, result_last, comp);
    }



  // Nth element
  template<typename I>
    void std_nth_element(I first, I last)
    {
      return std::nth_element(first, last);
    }



  // Nth element (relation)
  template<typename I, typename Ord>
    void std_nth_element(I first, I last, Ord ord)
    {
      return std::nth_element(first, last);
    }
    
    

  // Binary search
  // This family of algorithms is admitted by sorted sequences.
  
  // Lower bound
  template<typename I, typename T>
    I std_lower_bound(I first, I last, const T& value)
    {
      return std::lower_bound(first, last, value);
    }
    
  
  
  // Lower bound (relation)
  template<typename I, typename T, typename Ord>
    I std_lower_bound(I first, I last, const T& value, Ord comp)
    {
      return std::lower_bound(first, last, value, comp);
    }



  // Upper bound
  template<typename I, typename T>
    I std_upper_bound(I first, I last, const T& value)
    {
      return std::upper_bound(first, last, value);
    }
    
  
  
  // Upper bound (relation)
  template<typename I, typename T, typename Ord>
    I std_upper_bound(I first, I last, const T& value, Ord comp)
    {
      return std::upper_bound(first, last, value, comp);
    }



  // Equal range
  template<typename I, typename T>
    I std_equal_range(I first, I last, const T& value)
    {
      return std::equal_range(first, last, value);
    }
    
  
  
  // Equal range (relation)
  template<typename I, typename T, typename Ord>
    I std_equal_range(I first, I last, const T& value, Ord comp)
    {
      return std::equal_range(first, last, value, comp);
    }
    
  
  
  // Binary search
  template<typename I, typename T>
    I std_binary_search(I first, I last, const T& value)
    {
      return std::binary_search(first, last, value);
    }
    
  
  
  // Binary search (relation)
  template<typename I, typename T, typename Ord>
    I std_binary_search(I first, I last, const T& value, Ord comp)
    {
      return std::binary_search(first, last, value, comp);
    }

} // namespace origin

#include <origin/algorithm/permutation.hpp>


// Include definitions of container concepts needed for some algorithm
// specializations (e.g., find).
#include <origin/container.hpp>



#endif
