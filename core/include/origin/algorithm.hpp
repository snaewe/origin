// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_HPP
#define ORIGIN_ALGORITHM_HPP

#include <algorithm>

#include <origin/algorithm/combination.hpp>

namespace origin
{
  //--- Quantifiers ---//
  
  // Note that:
  //   !all_of == any_not_of
  //   !none_of == any_of

  // Return true if and only if all of the the elements in the range r satisfy 
  // the given predicate, or if r is empty.
  template<typename R, typename Pred>
    inline bool all_of(R const& range, Pred pred)
    {
      return std::all_of(begin(range), end(range), pred);
    }
    
  // Return true if and only if the range r is non-empty and any of its
  // elements satisfy the given predicate.
  template<typename R, typename Pred>
    inline bool any_of(R const& range, Pred pred)
    {
      return std::any_of(std::begin(range), std::end(range), pred);
    }
    
  // Return true if and only if the range [first, last) is non-empty and any
  // of its elements do not satisfy the given prdicate.
  template<typename Iter, typename Pred>
    bool any_not_of(Iter first, Iter last, Pred pred)
    {
      while(first != last) {
        if(!pred(*first))
          return true;
      }
      return false;
    }
  
  // Return true if and only if the range r is non-empty and any of its 
  // elements do not satisfy the given prdicate.
  template<typename R, typename Pred>
    inline bool any_not_of(R const& range, Pred pred)
    {
      return any_not_of(std::begin(range), std::end(range), pred);
    }

  // Return true if and only if none of the the elements in the range r satisfy
  // the given predicate or r is empty.
  template<typename R, typename Pred>
    inline bool none_of(R const& range, Pred pred)
    {
      return std::none_of(std::begin(range), std::end(range), pred);
    }
 
  //--- Equality Quantifiers ---//

  // Returns true if and only if all elements in [first, last) are equal to
  // value or [first, last) is an empty range.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    bool all_equal(Iter first, Iter last, T const& value)
    {
      while(first != last) {
        if(*first != value)
          return false;
        ++first;
      }
      return true;
    }
   
  template<typename R, typename T>
    inline bool all_equal(R const& range, T const& value)
    {
      return all_equal(std::begin(range), std::end(range), value);
    }
    
  // Returns true if and only if [first, last) is not an empty range and at
  // least one element in [first, last) is equal to value.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    bool any_equal(Iter first, Iter last, T const& value)
    {
      while(first != last) {
        if(*first == value)
          return true;
      }
      return false;
    }

  template<typename R, typename T>
    inline bool any_equal(R const& range, T const& value)
    {
      return any_equal(std::begin(range), std::end(range), value);
    }
    
  template<typename Iter, typename T>
    bool any_not_equal(Iter first, Iter last, T const& value)
    {
      while(first != last) {
        if(!first != value)
          return true;
      }
      return false;
    }
    
  template<typename R, typename T>
    inline bool any_not_equal(R const& range, T const& value)
    {
      return any_not_equal(std::begin(range), std::end(range), value);
    }
    
  // Returns true if and only if [first, last) is an empty range or there
  // are no elements in [first, last) that are equal to value (or equivalently,
  // all elements are not equal to value).
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    bool none_equal(Iter first, Iter last, T const& value)
    {
      while(first != last)
      {
        if(*first == value)
          return false;
      }
      return true;
    }

  //--- Find ---//
  
  // Return the first iterator i in the given range that is equal to value.
  template<typename R, typename T>
    auto find(R& range, T const& value) -> decltype(std::begin(range))
    {
      return std::find(std::begin(range), std::end(range), value);
    }

  template<typename R, typename T>
    auto find(R const& range, T const& value) -> decltype(std::begin(range))
    {
      return std::find(std::begin(range), std::end(range), value);
    }
    
  // Return the first iterator i in the range that satisfies the given 
  // predicate.
  template<typename R, typename Pred>
    auto find_if(R& range, Pred pred) -> decltype(std::begin(range))
    {
      return std::find_if(std::begin(range), std::end(range), pred);
    }

  template<typename R, typename Pred>
    auto find_if(R const& range, Pred pred) -> decltype(std::begin(range))
    {
      return std::find_if(std::begin(range), std::end(range), pred);
    }


  // Return the first element in [first, last) that satisfies the predicate
  // pred, or last if no such element exists.
  //
  // This function, along with next_if, can be used to iterate over the
  // subsequence of values satisfying some predicate.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename Pred>
    inline Iter first_if(Iter first, Iter last, Pred pred)
    {
      return std::find_if(first, last, pred);
    }
    
  // Return the first element in [first, last) that is equal to value or
  // last if no such element exists.
  //
  // This function, along with next_equal, can be used to iterate over the
  // subsequence of values that are equal to another.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    inline Iter first_equal(Iter first, Iter last, const T& value)
    {
      return std::find(first, last, value);
    }

  // Return the next element in [first, last) that satisfies pred, or last if
  // no such element exists.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename Pred>
    inline Iter next_if(Iter first, Iter last, Pred pred)
    {
      if(first != last)
        return std::find_if(first + 1, last, pred);
      else
        return last;
    }
    
  // Return the next element in [first + 1, last) that is equal to value, or
  // last if no such element exists.
  //
  // precondition: readable_range(first, last)
  template<typename Iter, typename T>
    inline Iter next_equal(Iter first, Iter last, T const& value)
    {
      if(first != last)
        return std::find(first + 1, last, value);
      else
        return last;
    }
    
  // Return an iterator to the nth element in [first, last) that satisfies
  // pred, or last if no such element exists.
  //
  // precondition: readable_range(first, last)
  // precondition: count >= 0
  //
  // TODO: Replace Size with DistanceType.
  template<typename Iter, typename Size, typename Pred>
    Iter find_nth_if(Iter first, Iter last, Size count, Pred pred)
    {
      if(count == 0)
        return last;
      
      first = first_if(first, last, pred);
      --count;
      while(count != 0 && first != last) {
        first = next_if(first, last, pred);
        --count;
      }
      return first;
    }

  // Return an iterator to the nth element in [first, last) that is equal to
  // value, or last if no such element exists.
  //
  // precondition: readable_range(first, last)
  // precondition: count >= 0
  template<typename Iter, typename Size, typename T>
    Iter find_nth(Iter first, Iter last, Size count, T const& value)
    {
      if(count == 0)
        return last;
      
      first = first_equal(first, last, value);
      --count;
      while(count != 0 && first != last) {
        first = next_equal(first, last, value);
        --count;
      }
      return first;
    }


  // Extract the elements of [first, last) that are equal to value by moving 
  // them into the output range. This algorithm is similar to remove, except 
  // that the elements are moved into the output range instead of being 
  // overwritten.
  // 
  // requires: Forward_iterator<Iter> && Equality_comparable<Iter, Value_type<Iter>>
  // requires: Value_movable<Out, Iter>
  // precondition: readable_range(first, last)
  // precondition: movable_range(result, count_if(first, last, pred))
  // postcondition: ...
  template<typename Iter, typename Out, typename T>
    std::pair<Iter, Out> extract(Iter first, Iter last, Out result, T const& value)
    {
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
    
  // Extract the elements of [first, last) by moving them into the output
  // range. This algorithm is similar to remove_if, except that the elements
  // are moved into the output range instead of being overwritten.
  // 
  // requires: Forward_iterator<Iter>
  // requires: Value_movable<Out, Iter>
  // precondition: readable_range(first, last)
  // precondition: movable_range(result, count_if(first, last, pred))
  // postcondition: ...
  template<typename Iter, typename Out, typename Pred>
    std::pair<Iter, Out> extract_if(Iter first, Iter last, Out result, Pred pred)
    {
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


  // Permutations and Combinations

  // Compute the next lexicographical permutation of the ranger.
  //
  // requires: BidirectionalIterator<IteratorType<R>>
  template<typename R>
    inline bool next_permutation(R& range)
    {
      return std::next_permutation(std::begin(range), std::end(range));
    }

  template<typename R, typename Comp>
    inline bool next_permutation(R& range, Comp comp)
    {
      return std::next_permutation(std::begin(range), std::end(range), comp);
    }


  // Compute the previous lexicographical permutation of the range r.
  //
  // requires: BidirectionalIterator<IteratorType<R>>
  template<typename R>
    inline bool prev_permutation(R& range)
    {
      return std::prev_permutation(std::begin(range), std::end(range));
    }

  template<typename R, typename Comp>
    inline bool prev_permutation(R& range, Comp comp)
    {
      return std::prev_permutation(std::begin(range), std::end(range), comp);
    }


  // TODO: I'm not wild about the name _partial_*. These are often called
  // k-permutations (and similarly k-combination). In general, I would have
  // have preferred next_permutaiton to increment to k-permutations instead
  // of entire sequences.

  // Compute the next lexicographical permutation of elements in [first, mid) 
  // from r where first is begin(r). Return false if there is no next 
  // permutation.
  //
  // requires: See above.
  // requires: SameType<IteratorType<R>>, Iter>
  // precondition: mid in range
  template<typename R, typename Iter>
    inline bool next_partial_permutation(R& range, Iter mid)
    {
      return next_partial_permutation(std::begin(range), mid, std::end(range));
    }

  template<typename R, typename Iter, typename Comp>
    inline bool next_partial_permutation(R& range, Iter mid, Comp comp)
    {
      return next_partial_permutation(std::begin(range), mid, std::end(range), comp);
    }


  // Compute the next lexicographical permutation of elements in [first, mid) 
  // from r where first is begin(r). Return false if there is no next 
  // permutation.
  //
  // requires: See above.
  // requires: SameType<IteratorType<R>>, Iter>
  // precondition: mid in range
  template<typename R, typename Iter>
    inline bool prev_partial_permutation(R& range, Iter mid)
    {
      return prev_partial_permutation(std::begin(range), mid, std::end(range));
    }

  template<typename R, typename Iter, typename Comp>
    inline bool prev_partial_permutation(R& range, Iter mid, Comp comp)
    {
      return prev_partial_permutation(std::begin(range), mid, std::end(range), comp);
    }


  // Compute the next lexicographical combination of elements in [first, mid)
  // from the range r where first is equal to begin(r). Return false if there
  // is no next combination.
  template<typename R, typename Iter>
    inline bool next_combination(R& range, Iter mid)
    {
      return next_combination(std::begin(range), mid, std::end(range));
    }

  template<typename R, typename Iter, typename Comp>
    inline bool next_combination(R& range, Iter mid, Comp comp)
    {
      return next_combination(std::begin(range), mid, std::end(range), comp);
    }


  // Compute the previous lexicographical combination of elements in 
  // [first, mid) from the range r where first is equal to begin(r). Return 
  // false if there is no next combination.
  template<typename R, typename Iter>
    inline bool prev_combination(R& range, Iter mid)
    {
      return prev_combination(std::begin(range), mid, std::end(range));
    }

  template<typename R, typename Iter, typename Comp>
    inline bool prev_combination(R& range, Iter mid, Comp comp)
    {
      return prev_combination(std::begin(range), mid, std::end(range), comp);
    }


  // PROJECT: Implement multiset permutations.
  
  

} // namespace origin

#endif
