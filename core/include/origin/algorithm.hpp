
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
#include <origin/iterator.hpp>
#include <origin/range.hpp>

namespace origin
{
  // NOTE: The std_* algorithms simply add Origin-style concept checking to
  // std algorithms. They are primiarly used for testing


  // There are a number of algorithm concepts describing common requirements
  // in algorithms.
  //
  //    Searchable
  //    Value_searchable
  //    Range_searchable
  //    Value_searchable_range

  // Returns true if the input iterator I can be searched using the predicate P.
  template<typename I, typename P>
    constexpr bool Searchable()
    {
      return Input_iterator<I>() && Predicate<P, Value_type<I>>();
    }

  // Returns true if the input range R can be searched using the predicate P.
  template<typename R, typename P>
    constexpr bool Range_searchable()
    {
      return Input_range<R>() && Predicate<P, Value_type<R>>();
    }

     

  // Value searchable (concept)
  // An iterator is value-searchable if it can be searched for a value of a
  // some type. There are two forms of the concepts:
  //
  //    Value_searchable<I>     Can be searched for a Value_type<I> value
  //    Value_searchable<I, T>  Can be searched for a T value
     
  // Value searchable for a type T
  template<typename I, typename T>
    struct Value_searchable_concept
    {
      static constexpr bool check()
      {
        return Input_iterator<I>() && Equality_comparable<Value_type<I>, T>();
      }
    };

  // Value searchable with its own value type.
  template<typename I>
    struct Value_searchable_concept<I, default_t>
    {
      static constexpr bool check()
      {
        return Input_iterator<I>() && Equality_comparable<Value_type<I>>();
      }
    };
     
  // Returns true if the input iterator can be searched for a value of its own
  // value type or a value of another type T.
  template<typename I, typename T = default_t>
    constexpr bool Value_searchable()
    {
      return Value_searchable_concept<I, T>::check();
    }
  
  
  
  // Value searchable range (concept)
  // A range is value searchable if it can be searched for a value of some
  // type. There are two forms of this concept:
  //
  //    Value_searchable_range<R>     Can be searched for a Value_type<R> value
  //    Value_searchabel_range<R, T>  Can be searched for a T value
  
  // Value searchable for a type T
  template<typename R, typename T>
    struct Range_value_searchable_concept
    {
      static constexpr bool check()
      {
        return Input_range<R>() && Equality_comparable<Value_type<R>, T>();
      }
    };

  // Value searchable with its own value type.
  template<typename R>
    struct Range_value_searchable_concept<R, default_t>
    {
      static constexpr bool check()
      {
        return Input_range<R>() && Equality_comparable<Value_type<R>>();
      }
    };
  
  // Returns true if the input iterator can be searched for a value of type T.
  template<typename R, typename T = default_t>
    constexpr bool Range_value_searchable()
    {
      return Range_value_searchable_concept<R, T>::check();
    }

  
  
  // Comparable iterators (concept)
  // Two iterator are comparable if their values can be compared for equality 
  // or with respect to some relation. Both iterators must be input iterators.
  // Note that the second iterator (I2) is only required to be weak input
  // iterator. Any algorithm requiring this conept may need to strengthen the
  // requirements on the 2nd argument (see find_adjacent and find_first_in).
  
  // Comparable with respect to a relation.
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
  
  // Comparable for equality. Note that this is equivalent to Input_iterator<I2> 
  // and Value_searchable<I1, Value_type<I2>>.
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
  
  // Returns true if the iterators values of the two iterators can be
  // compared.
  template<typename I1, typename I2, typename R = default_t>
    constexpr bool Comparable()
    {
      return Comparable_concept<I1, I2, R>::check();
    }
    
    

  // Comparable ranges (concept)
  // Two ranges are comparable if they are both input ranges and their values
  // can be compared for equality or using some relation. 
  
  // Comparable using a relation.
  template<typename R1, typename R2, typename R>
    struct Range_comparable_concept
    {
      static constexpr bool check()
      {
        return Input_range<R1>() 
            && Input_range<R2>()
            && Relation<R, Value_type<R1>, Value_type<R2>>();
      }
    };

  // Comparable for equality.
  template<typename R1, typename R2>
    struct Range_comparable_concept<R1, R2, default_t>
    {
      static constexpr bool check()
      {
        return Input_range<R1>() 
            && Input_range<R2>()
            && Equality_comparable<Value_type<R1>, Value_type<R2>>();
      }
    };
  
  // Returns true if R1 and R2 are ranges that can be compared using value
  // equality or a relation R.
  template<typename R1, typename R2, typename R = default_t>
    constexpr bool Range_comparable()
    {
      return Range_comparable_concept<R1, R2, R>::check();
    }
}


#include <origin/algorithm/quantifier.hpp>
#include <origin/algorithm/find.hpp>
#include <origin/algorithm/count.hpp>

#include <origin/algorithm/combination.hpp>

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


    
  // Equal (random access ranges)
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
  template<typename R1, typename R2, typename R>
    inline auto equal(const R1& a, const R2& b, R comp)
      -> Requires<!(Random_access_range<R1>() && Random_access_range<R2>()), bool>
    {
      static_assert(Range_comparable<R1>(), "");

      return std_equal(std::begin(a), std::end(a), std::begin(b), comp);
    }

    
    
  // Equal (random access ranges)
  // This specialization for random access ranges returns true if 
  // size(a) <= size(b) and comp(x, y) is true for each x and y in the ranges a 
  // and b, pairwise.
  template<typename R1, typename R2, typename R>
    inline auto equal(const R1& a, const R2& b, R comp)
      -> Requires<Random_access_range<R1>() && Random_access_range<R2>(), bool>
    {
      static_assert(Range_comparable<R1>(), "");

      return size(a) <= size(b)
          && std_equal(std::begin(a), std::end(a), std::begin(b), comp);
    }



  // Mismatch
  // Returns the first iterator i in [first1, last1) where *i != *j and *j is
  // the corresponding iterator in j.
  template<typename I1, typename I2>
    inline std::pair<I1, I2> 
    std_mismatch(I1 first1, I1 last1, I2 first2)
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
    inline std::pair<Iterator_type<R1>, Iterator_type<R2>> 
    mismatch(const R1& a, const R2& b)
    {
      static_assert(Range_comparable<R1>(), "");

      return std_mismatch(std::begin(a), std::end(a), std::begin(b));
    }

    
    
  // Mismatch (relation)
  template<typename I1, typename I2, typename R>
    inline std::pair<I1, I2> 
    std_mismatch(I1 first1, I1 last1, I2 first2, R comp)
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
  template<typename R1, typename R2, typename R>
    inline std::pair<Iterator_type<R1>, Iterator_type<R2>> 
    mismatch(const R1& a, const R2& b, R comp)
    {
      static_assert(Range_comparable<R1>(), "");

      return std_mismatch(std::begin(a), std::end(a), std::begin(b), comp);
    }


    
  // Is permutation (impl, counting, all)
  // Returns true if [first1, last1) is a permutation of 
  // [first2, first2 + (last1 - first1)).
  //
  // This algorithm works by ensuring that each unique value in [first1, last1)
  // has the same number of occurrences in [first2, first2 + (last1 - first1)).
  // The performance of this algorithm is O(n^2). 
  //
  // This is the main implementation of is_permutation_count. It should not be
  // used since it does not optimize the case where the two ranges have equal
  // subranges.
  template<typename I1, typename I2>
    bool is_permutation_counting_all(I1 first1, I1 last1, I2 first2, I2 last2)
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
    
    

  // Is permutation (impl, counting)
  // Returns true if [first1, last1) is a permutation of [first2, last2).
  //
  // This algorithm works by ensuring that each unique value in [first1, last1)
  // has the same number of occurrences in [first2, first2 + (last1 - first1)).
  // The performance of this algorithm is O(n^2).
  template<typename I1, typename I2>
    bool is_permutation_counting(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Comparable<I1, I2>(), "");
      assert(is_readable_range(first1, last1));
      assert(is_readable_range(first2, last2));

      // Find where [first1, last1) and [first2, ...) differ. Then, count the 
      // number of times each element occurs in the remainder of the two 
      // ranges. Otherwise the two ranges are equal, and we're done.
      std::tie(first1, first2) = std_mismatch(first1, last1, first2);
      if(first1 != last1)
        return is_permutation_counting_all(first1, last1, first2, last2);
      return true;
    }
    
  
  
  // Forward declarations
  template<typename I> void std_sort(I, I);
    
  
  
  // Is permutation (impl, sorting)
  // Returns true if the range [first1, last1) is a permutation of the range
  // [first2, last2). The algorithm copies both input ranges into temporary 
  // buffers and sorts them. The runtime of this implementation is O(n log n), 
  // although there are 2n copies and 2 executions of the sort.
  template<typename I1, typename I2>
    bool is_permutation_sorting(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Sortable<I1>(), "");
      static_assert(Sortable<I2>(), "");
      static_assert(Comparable<I1, I2>(), "");
      
      std_sort(first1, last1);
      std_sort(first2, last2);
      return std_equal(first1, last1, first2);
    }
    
  
    
  // Is permutation (sorted)
  // Returns true if [first1, last1) is a permutation of 
  // [first2, first2 + (last1 - first1)). 
  //
  // This specialization is valid for Copyable value types. If there is
  // sufficient memory, the algorithm will copy and sort both ranges in 
  // O(log n) time. If not, the property is checked by counted the number of
  // occurrences of each value in O(n^2) time.
  template<typename I1, typename I2>
    auto std_is_permutation(I1 first1, I1 last1, I2 first2)
      -> Requires<Copyable<Value_type<I1>>() &&
                  Copyable<Value_type<I2>>() &&
                  Totally_ordered<Value_type<I1>>() &&
                  Totally_ordered<Value_type<I2>>(),
                  bool>
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Comparable<I1, I2>(), "");

      auto n = std_distance(first1, last1);
      temporary_buffer<Value_type<I1>> b1(first1, n);
      temporary_buffer<Value_type<I2>> b2(first2, n);
      
      // If allocated enough space, use the efficient implementation. 
      // Otherwise, use the less efficient implementation.
      if(b1 && b2) {
        return is_permutation_sorting(b1.begin(), b1.end(), b2.begin(), b2.end());
      } else {
        I2 last2 = std_next(first2, n);
        return is_permutation_counting(first1, last1, first2, last2);
      }
    }

    
    
  // Is permutation (counted)
  // Returns true if, for a range a and b, a is a permutation of b.
  //
  // This specialization is valid when the value types cannot be copied. It
  // counts the number of occurrences of each value in O(n^2) time.
  template<typename I1, typename I2>
    inline auto std_is_permutation(I1 first1, I1 last1, I2 first2)
      -> Requires<!(Copyable<Value_type<I1>>() &&
                    Copyable<Value_type<I2>>() &&
                    Totally_ordered<Value_type<I1>>() &&
                    Totally_ordered<Value_type<I2>>()),
                  bool>
    {
      I2 last2 = std_next(first2, distance(first1, last1));
      return is_permutation_counting(first1, last1, first2, last2);
    }
    
    
    
  template<typename R1, typename R2>
    inline bool is_permutation(const R1& a, const R2& b)
    {
      static_assert(Input_range<R1>(), "");
      static_assert(Input_range<R2>(), "");
      assert(size(a) == size(b));

      return std_is_permutation(std::begin(a), std::end(a), std::begin(b));
    }

  // Generalized version
  template<typename I1, typename I2, typename Pred>
    inline bool std_is_permutation(I1 first1, I1 last1, I2 first2, Pred pred)
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

  template<typename I1, typename I2>
    inline I1 std_search(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Equality_comparable<Value_type<I1>, Value_type<I2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));
      
      return std::search(first1, last1, first2, last2);
    }
    
  template<typename I1, typename I2, typename Pred>
    inline I1 std_search(I1 first1, I1 last1, I2 first2, I2 last2, Pred pred)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Equality_comparable<Value_type<I1>, Value_type<I2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));

      return std::search(first1, last1, first2, last2);
    }
    
    
  
  // Search end
  // This is the same as std::find_end, but named more appropriately.
  //
  // FIXME: Write range variants.

  template<typename I1, typename I2>
    inline I1 search_end(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Equality_comparable<Value_type<I1>, Value_type<I2>>(), "");
      assert(( is_readable_range(first1, last1) ));
      assert(( is_readable_range(first2, last2) ));
      
      return std::find_end(first1, last1, first2, last2);
    }
    
  template<typename I1, typename I2, typename Pred>
    inline I1 search_end(I1 first1, I1 last1, I2 first2, I2 last2, Pred pred)
    {
      static_assert(Forward_iterator<I1>(), "");
      static_assert(Forward_iterator<I2>(), "");
      static_assert(Equality_comparable<Value_type<I1>, Value_type<I2>>(), "");
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
    
} // namespace origin


#include <origin/algorithm/copy.hpp>


namespace origin 
{
  // Transform (unary)
  //
  // FIXME: The combination of Input_iterator and Regular_function should be
  // called Transformable.
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
  // called Transformable_range.
  //
  // TODO: Can we have a Semirange that accepts a single iterator? or should
  // we just have an overload that accepts an iterator. Probably the latter,
  // if needed.
  template<typename Ri, typename Ro, typename F>
    inline void transform(const Ri& in, Ro& out, F f)
    {
      static_assert(Input_range<Ri>(), "");
      static_assert(Regular_function<F, Value_type<Ri>>(), "");
      static_assert(Output_range<Ro, Result_of<F(Value_type<Ri>)>>(), "");
      assume(size(in) <= size(out));

      std_transform(std::begin(in), std::end(in), std::begin(out), f);
    }
    
    
    
    
  // Transform (binary)
  template<typename I1, typename I2, typename O, typename F>
    inline O std_transform(I1 first1, I1 last1, I2 first2, O result, F f)
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


// Include definitions of container concepts needed for some algorithm
// specializations (e.g., find).
#include <origin/container.hpp>



#endif
