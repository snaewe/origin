// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_HPP
#define ORIGIN_ALGORITHM_HPP

#include <algorithm>


#include <origin/algorithm/core.hpp>
#include <origin/algorithm/quantifier.hpp>
#include <origin/algorithm/find.hpp>
#include <origin/algorithm/count.hpp>

namespace origin 
{
  // For each
  // Evaluate func(x) for each element x in a range. The iterator version of
  // has the following syntax:
  //
  //    for_each(first, last, func) ~> func
  //
  // It evaluates func(*i) for each i in [first, last) and returns func.
  //
  // The range version of the algorithm has the following syntax:
  //
  //    for_each(range, func) ~>func
  //
  // It is equivalent to the iterator version when called over the begin and
  // end of range.

  template <typename I, typename F>
    inline F for_each(I first, I last, F func)
    {
      while (first != last) {
        func(*first);
        ++first;
      }
      return func;
    }

  template <typename R, typename F>
    inline F for_each(R&& range, F func)
    {
      return for_each(begin(range), end(range), func);
    }



  // Repeat
  //
  // Calls f(i) exactly n times for each i in [0, n).
  //
  // Requires:
  //    n >= 0
  //
  // Returns:
  //    The function f.
  template <typename Int, typename F>
    inline F repeat(Int n, F f)
    {
      static_assert(Integral<Int>(), "");
      static_assert(Function<F, Int>(), "");
      assert(n >= 0);

      for (int i = 0; i < n; ++i)
        f(i);
      return f;
    }


  // Reduce
  //
  // The reduce algorithm evaluates value = f(*i, value) for each iterator i in
  // a range. There are two overloads of this function: one that takes an
  // iterator range [first, last) and the other that takes a bounded range.
  //
  //    reduce(first, last, value, f) ~> value'
  //    reduce(range, value, f) -> value'
  //
  // The algorithm returns the reduced value as computed over the elements of
  // the range.

  // FIXME: The same type constraint on the result type is too strong.
  template <typename I, typename T, typename F>
    inline T reduce(I first, I last, T value, F f)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Function<F, Value_type<I>, T>, "");
      static_assert(Convertible<Result_of<F(Value_type<I>, T)>, T>, "");
      assert(is_bounded_range(first, last));

      while (first != last) {
        value = f(*first, value);
        ++first;
      }
      return value;
    }

  template <typename R, typename T, typename F>
    inline T reduce(const R& range, T init, F f)
    {
      return reduce(begin(range), end(range), init, f);
    }




  // Lexicographical equivalent
  //
  // Returns true when two ranges have the same size and corresponding values.
  // There are variants of this algorithm for iterators and ranges. The
  // iterator version is:
  //
  //    lexicographical_equivalent(first1, last1, first2, last2, comp) ~> b
  //
  // It returns true when no mismatch is found in the elements of 
  // [first1, last1) and [first2, last2).
  //
  // The range version is:
  //
  //    lexicographical_equivalent(range1, range2, comp) ~> b
  //
  // Its behavior is equivalent to calling the iterator version using begin and
  // end on the input ranges.

  template <typename I1, typename I2, typename Comp>
    inline auto
    lexicographical_equivalent(I1 first1, I1 last1, I2 first2, I2 last2, Comp comp)
      -> Requires<Non_random_access_iterator<I1>() && 
                  Non_random_access_iterator<I2>(), 
                  bool>
    {
      assert(is_bounded_range(first1, last1));
      assert(is_bounded_range(first2, last2));
      assume(is_equivalence_relation(comp));
      auto p = find_mismatch(first1, last1, first2, last2, comp);
      return p.first == last1 && p.second == last2;
    }

  // When I1 and I2 are random access iterators, we can more efficiently detect
  // the case where the sizes differ.
  template <typename I1, typename I2, typename Comp>
    inline auto
    lexicographical_equivalent(I1 first1, I1 last1, I2 first2, I2 last2, Comp comp)
      -> Requires<Random_access_iterator<I1>() && 
                  Random_access_iterator<I2>(), 
                  bool>
    {
      assert(is_bounded_range(first1, last1));
      assert(is_bounded_range(first2, last2));
      assume(is_equivalence_relation(comp));

      if (last1 - first1 == last2 - first2) {
        auto p = find_mismatch(first1, last1, first2, last2, comp);
        return p.first == last1 && p.second == last2;
      } else {
        return false;
      }
    }

  template <typename R1, typename R2, typename Comp>
    inline bool
    lexicographical_equivalent(const R1& range1, const R2& range2, Comp comp)
    {
      return lexicographical_equivalent(begin(range1), end(range1),
                                        begin(range2), end(range2), comp);
    }



  // Lexicographical equal
  //
  // Returns true when two ranges have the same size and equal values. There
  // are two versions of this function. The iterator version is:
  //
  //    lexicographical_equal(first1, last1, first2, last2) ~> b
  //
  // The range version is:
  //
  //    lexicogpraphical_equal(range1, range2) ~> b

  template <typename I1, typename I2>
    inline bool
    lexicographical_equal(I1 first1, I1 last1, I2 first2, I2 last2)
    {
      return lexicographical_equivalent(first1, last1, first2, last2, eq());
    }

  template <typename R1, typename R2>
    inline bool
    lexicographical_equal(const R1& range1, const R2& range2)
    {
      return lexicographical_equivalent(begin(range1), end(range1),
                                        begin(range2), end(range2), eq());
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
        if(c == 0) {
          return false;
        } else if(count(next(i), last1, *i) + 1 != c) {
          // Start at the next i since we already know that *i == *i.
          return false;
        }
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
      std::tie(first1, first2) = find_mismatch(first1, last1, first2, last2);
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

      std::tie(first1, first2) = mismatch(first1, last1, first2, comp);
      if(first1 != last1)
        return equal_elements(first1, last1, first2, last2, comp);
      return true;
    }

  

  // Search
  template<typename I1, typename I2>
    inline I1 search(I1 first1, I1 last1, I2 first2, I2 last2)
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
    inline I1 search(I1 first1, I1 last1, I2 first2, I2 last2, R comp)
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
      
      return search(std::begin(a), std::end(a), std::begin(b), std::end(b));
    }
    
    
  
  // Search (range, relation)
  template<typename R1, typename R2, typename Rel>
    inline auto search(R1&& a, const R2& b, Rel comp) -> decltype(std::begin(a))
    {
      static_assert(Forward_range<R1>(), "");
      static_assert(Forward_range<R2>(), "");
      static_assert(Comparison<R1, R2, Rel>(), "");
      
      return search(std::begin(a), std::end(a), std::begin(b), std::end(b), comp);
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
    inline I search_n(I first, I last, Difference_type<I> n, const T& value)
    {
      static_assert(Forward_iterator<I>(), "");
      static_assert(Search<I, T>(), "");
      assert(is_readable_range(first, last));

      return std::search_n(first, last, n, value);
    }
    
    
    
  // Search n (relation)
  template<typename I, typename T, typename R>
    inline I search_n(I first, I last, Difference_type<I> n, const T& value, R comp)
    {
      static_assert(Forward_iterator<I>(), "");
      static_assert(Search<I, T, R>(), "");
      assert(is_readable_range(first, last));

      return std::search_n(first, last, n, value, comp);
    }
    
    
  
  // Search n (range)
  template<typename R, typename T>
    inline auto search_n(R&& range, Difference_type<R> n, const T& value)
      -> decltype(std::begin(range))
    {
      static_assert(Forward_range<R>(), "");
      static_assert(Range_search<R, T>(), "");
    
      return search_n(std::begin(range), std::end(range), n, value);
    }
    
    
    
  // Search n (range, relation)
  template<typename R, typename T, typename Rel>
    inline auto search_n(R&& range, Difference_type<R> n, const T& value, Rel comp)
      -> decltype(std::begin(range))
    {
      static_assert(Forward_range<R>(), "");
      static_assert(Range_search<R, T, Rel>(), "");
    
      return search_n(std::begin(range), std::end(range), n, value, comp);
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
    inline O transform(I first, I last, O result, F f)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Regular_function<F, Value_type<I>>(), "");
      static_assert(Weak_output_iterator<O, Result_of<F(Value_type<I>)>>(), "");
      
      assert(is_readable_range(first, last));
      assume(is_writable_range(result, distance(first, last)));
      
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

      transform(std::begin(in), std::end(in), std::begin(out), f);
    }
    
    
    
    
  // Transform (binary)
  template<typename I1, typename I2, typename O, typename F>
    O transform(I1 first1, I1 last1, I2 first2, O result, F f)
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

      return transform(std::begin(range1), std::end(range1), 
                           std::begin(range2), std::begin(result), f);
    }
    
    

  // Fill
  template<typename O, typename T>
    void fill(O first, O last, const T& value)
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
      
      return fill(std::begin(range), std::end(range));
    }
    
    
    
  // Fill n
  template<typename O, typename T>
    inline O fill_n(O first, Difference_type<O> n, const T& value)
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
    inline F generate(O first, O last, F gen)
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

      return generate(std::begin(range), std::end(range), gen);
    }
    
    
    
  // Generate n
  template<typename O, typename F>
    std::pair<O, F> generate_n(O first, Difference_type<O> n, F gen)
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
    void reverse(I first, I last)
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
      return reverse(begin(range), end(range));
    }
  

  
  // Reverse copy
  template <typename I, typename O>
    void reverse_copy(I first, I last, O result)
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
    I rotate(I first, I mid, I last)
    {
      static_assert(Permutation<I>(), "");

      return std::rotate(first, mid, last);
    }
    
  
  
  // Rotate copy
  template<typename I, typename O>
    I rotate_copy(I first, I mid, I last, O result)
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
    void random_shuffle(I first, I last)
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
      return random_shuffle(begin(range), end(range));
    }


      
  // Random shuffle (generator)
  template<typename I, typename Gen>
    void random_shuffle(I first, I last, Gen&& rand)
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
      return random_shuffle(begi(range), end(range), rand);
    }



  // Shuffle (iterator)
  //
  // FIXME: Implement me!
  template<typename I, typename Gen>
    void shuffle(I first, I last, Gen&& rand)
    {
      static_assert(Random_access_iterator<I>(), "");
      static_assert(Permutation<I>(), "");
      
      std::shuffle(first, last, std::forward<Gen>(rand));
    }



    // Shuffle (range)
  template <typename R, typename Gen>
    void shuffle(R&& range, Gen&& rand)
    {
      shuffle(begin(range), end(range), rand);
    }
    

  
  // Partitions
  // This family of algorithms deals with partitions of a sequence.
  
  // Is partitioned
  template<typename I, typename P>
    inline bool is_partitioned(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_readable_range(first, last));

      first = find_if_not(first, last, pred);
      if(first != last)
        return none_of(next(first), last, pred);
      else
        return true;
    }
    
    
  
  // Is partitioned (range)
  template<typename R, typename P>
    bool is_partitioned(const R& range, P pred)
    {
      static_assert(Range_query<R, P>(), "");

      return is_partitioned(std::begin(range), std::end(range), pred);
    }
    
  
  
  // Partition point
  // Return an iterator i such that all_of(first, i) is true and 
  // none_of(i, last) is also true.
  //
  // FIXME Actually implement me.
  template<typename I, typename P>
    I partition_point(I first, I last, P pred)
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

      return partition_point(std::begin(range), std::end(range), pred);
    }
    
  
  
  // Paritition
  template<typename I, typename P>
    I partition(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      static_assert(Permutation<I>(), "");

      first = find_if_not(first, last, pred);
      if(first != last)
        exchange_if(next(first), last, first, pred);
      return first;
    }

  
  
  // Partition (range)
  template<typename R, typename P>
    auto partition(R&& range, P pred) -> decltype(std::begin(range))
    {
      using Rx = Forwarded<R>;
      static_assert(Range_query<Rx, P>(), "");
      static_assert(Range_permutation<Rx>(), "");
      
      return partition(std::begin(range), std::end(range), pred);
    }
  
  
  // Stable partition
  template<typename I, typename P>
    I stable_partition(I first, I last, P pred)
    {
      return std::stable_partition(first, last, pred);
    }
    
    
  
  // Partition copy
  template<typename I, typename Out1, typename Out2, typename P>
    std::pair<Out1, Out2> partition_copy(I first, I last, Out1 out_true, Out2 out_false, P pred)
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

// Algorithm depends on memory for temporary buffer.
#include <origin/memory.hpp>

// Include definitions of container concepts needed for some algorithm
// specializations (e.g., find).
#include <origin/container.hpp>


#endif
