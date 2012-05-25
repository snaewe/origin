// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_CORE_HPP
#define ORIGIN_ITERATOR_CORE_HPP

#include <cassert>

#include <origin/concepts.hpp>

namespace origin
{
  // Readable (concept)
  // A type is readable if it has an associated value type, is dereferenceable,
  // and a constant refernce to the value type can be bound to the result of
  // dereferencing.
  //
  // TODO: Consider calling this Value_readable to better align with names
  // in the standard.
  template <typename I>
    constexpr bool Readable()
    {
      return Has_value_type<I>()
          && Has_dereference<I>() 
          && Convertible<Dereference_result<I>, const Value_type<I>&>();
    }



  // Move writable (concept)
  // A type I is copy writable to a value of type T, if values of T can be
  // copy assigned through a dereferenced value of type I. That is, the 
  // following expression must be valid:
  //
  //    *i = move(t);
  // 
  // where i has type I and t has type T. The operation has the result of
  // moving the representation of t into the object referenced by *i.
  template <typename I, typename T>
    constexpr bool Move_writable()
    {
      return Assignable<Dereference_result<I>, T&&>();
    }



  // Copy writable (concept)
  // A type I is copy writable to a value of type T, if values of T can be
  // copy assigned through a dereferenced value of type I. That is, the 
  // following expression must be valid:
  //
  //    *i = t;
  // 
  // where i has type I and t has type T. If I is also Readable and the 
  // assigned value type is Equality comparable, then *i == t after the
  // assignment.
  template <typename I, typename T>
    constexpr bool Copy_writable()
    {
      return Move_writable<I, T>() 
          && Assignable<Dereference_result<I>, const T&>();
    };


    
  // Permutable (concept)
  // Returns true if I is permutable. A permutable iterator is readable,
  // move-writable, and has a movable value type.
  template <typename I>
    constexpr bool Permutable()
    {
      return Readable<I>() 
          && Movable<Value_type<I>>() 
          && Move_writable<I, Value_type<I>&&>();
    }
    

  // Mutable (concept)
  // Returns true if I mutable. A mutable iterator is readable and writable,
  // and has a copyable value type.
  //
  // Note that all Mutable iterator types are also Permutable.
  template <typename I>
    constexpr bool Mutable()
    {
      return Readable<I>() 
          && Copyable<Value_type<I>>() 
          && Copy_writable<I, Value_type<I>>();
    }
    
    

  // Incrementable types


  // Weakly incrementable (concept)
  // A weakly incrementable type is a semiregular type that can be pre- and
  // post-incremented. Neither operation is requireed to be equality
  // preserving, and the result of post-increment is unspecified.
  //
  // Note that the weakly incrementable type does not include the
  // post-increment operator.
  template <typename I>
    constexpr bool Weakly_incrementable()
    {
      return Copyable<I>()
          && Equality_comparable<I>()
          && Has_difference_type<I>()
          
          // I& == { ++i }
          && Has_pre_increment<I>()
          && Same<Pre_increment_result<I>, I&>();
    }
    
  

  // Incrementable (concept)  
  // An incrementable type is an equality comparable, weakly incrementable 
  // type with a post-increment operator.
  template <typename I>
    constexpr bool Incrementable()
    {
      return Weakly_incrementable<I>()
          && Same<Post_increment_result<I>, I>();
    }


  // Strict weakly incrementable (concept)
  // A type that is weakly incrementable but not (strongly) incrementable.
  template <typename I>
    constexpr bool Strict_weakly_incrementable()
    {
      return Weakly_incrementable<I>() && !Incrementable<I>();
    }




  // Decrementable (concept)
  // A decrementable type is an inrementable type that can also be pre- and
  // post-decremented.
  template <typename I>
      constexpr bool Decrementable()
      {
        return Incrementable<I>()

            // I& == { --i }
            && Has_pre_decrement<I>()
            && Same<Pre_decrement_result<I>, I&>()
            
            // I == { i-- }
            && Has_post_decrement<I>()
            && Same<Post_decrement_result<I>, I>();
      }


  // TODO: Create another concept that mirrors random access iterators, but
  // does not include readability or writability. Just guessing, but might
  // that be called Ordinal?



  // Iterators
  // The following concept classes and predicates are define checking
  // capabilities for iterator concepts. These concepts unify the read/write
  // and incrementable requirements.
  

  // Iterator (concept)
  // An iterator, in the most general sense is a type that can be incremented
  // and decremented, although no claims are made on the behavior or result of
  // dereferencing. In that sense, the concept is incomplete but nonetheless
  // useful to describe very general requirements on iterator types.
  //
  // An Iterator is minimally a weakly incrementable and dereferenceable type.
  // Note that no constraints are placed on the result of the dereference
  // operator.
  template <typename I>
    constexpr bool Iterator()
    {
    return Weakly_incrementable<I>() 
        && Has_dereference<I>();
    }


  // Iterator reference (alias)
  // An alias for the associated reference type of the iterator. This supports 
  // writing  backwards compatible iterators where the reference type is 
  // actually named even though it should be deduced as decltype(*i).
  template <typename I>
    using Iterator_reference = decltype(*declval<I>());


  // Weak input iterator (concept)
  // A weak input iterator is weakly incrementable and readable.
  template <typename I>
    constexpr bool Weak_input_iterator()
    {
      return Weakly_incrementable<I>() && Readable<I>();
    }
    
    
  // Input iterator (concept)
  // A input iterator is a weak input iterator that is equality comparable.
  template <typename I>
    constexpr bool Input_iterator()
    {
      return Weak_input_iterator<I>() && Equality_comparable<I>();
    }


  // FIXME: What is the corresponding move output iterator? Do I need one?


  // Weak output iterator (concept)
  // A weak output iterator is a weakly incrementable type that is writable
  // over some expression type T.
  template <typename I, typename T>
    constexpr bool Weak_output_iterator()
    {
      return Weakly_incrementable<I>() && Copy_writable<I, T>();
    }
    
    
  // Output iterator (concept)
  // An output iterator is a weak output iterator that is equality comparable.
  //
  // Note that if T is an rvalue reference, this concept requires T to
  // implement move semantics. If T is not an rvalue reference, the type is
  // required to implement copy semantics.
  template <typename I, typename T>
    constexpr bool Output_iterator()
    {
      return Weak_output_iterator<I, T>() && Equality_comparable<I>();
    }
 
 
  // Forward Iterator (concept)
  // A forward iterator is an input iterator with a regular post-increment
  // operation. This guarantees that multiple passes of a range may be made
  // and that multiple iterators into the range may be used.
  template <typename I>
    constexpr bool Forward_iterator()
    {
      return Input_iterator<I>()
          && Incrementable<I>()
          && Readable<I>();
    }
    

  // Bidirectional Iterator (concept)
  // A bidirectional iterator is a forward iterator that can also move 
  // backwards using decrement operators.
  template <typename Iter>
    constexpr bool Bidirectional_iterator()
    {
      return Forward_iterator<Iter>()
          
          // Iter& == { --i }
          && Has_pre_decrement<Iter>()
          && Same<Pre_decrement_result<Iter>, Iter&>()
          
          // Iter == { i-- }
          && Has_post_decrement<Iter>()
          && Same<Post_decrement_result<Iter>, Iter>();
    };

    
  // Random Access Iterator (concept)
  // A random access iterator is a bidirectional iterator that can advance 
  // any number of steps in constant time.
  template <typename I>
    constexpr bool Random_access_iterator()
    {
      return Bidirectional_iterator<I>()
            && Signed<Difference_type<I>>()

            // I& == { i += n }
            && Has_plus_assign<I, Difference_type<I>>()
            && Same<Plus_assign_result<I, Difference_type<I>>, I&>()
            
            // I& == { i -= n }
            && Has_minus_assign<I, Difference_type<I>>()
            && Same<Minus_assign_result<I, Difference_type<I>>, I&>()
            
            // I == { i + n }
            && Has_plus<I, Difference_type<I>>()
            && Same<Plus_result<I, Difference_type<I>>, I>()
            
            // I == { n + i }
            && Has_plus<Difference_type<I>, I>()
            && Same<Plus_result<Difference_type<I>, I>, I>()
            
            // I == { i - n }
            && Has_minus<I, Difference_type<I>>()
            && Same<Minus_result<I, Difference_type<I>>, I>()
            
            // Difference_type<I> == { i - j }
            && Has_minus<I>()
            && Same<Minus_result<I>, Difference_type<I>>()
            
            // decltype(*i) == { i[n] }
            && Has_subscript<I, Difference_type<I>>()
            && Same<Subscript_result<I, Difference_type<I>>, Dereference_result<I>>();
    };
    
    
  // Strict input iterator (concept)
  // A strict input iterator is at most an input iterator. That is I is not
  // a forward iterator. This is provided for convenience.
  template <typename I>
    constexpr bool Strict_input_iterator()
    {
      return Weak_input_iterator<I>() && !Forward_iterator<I>();
    }
    
    
  // Strict output iterator (concept)
  // A strict output iterator is at most an output iterator. That is, I is not
  // also an readable.
  template <typename I>
    constexpr bool Strict_output_iterator()
    {
      return Weak_output_iterator<I> && !Readable<I>();
    }
  

  // Non-random access iterator (concept)
  // Returns true if I is an iterator, but not a random access iterator.
  template <typename I>
    constexpr bool Non_random_access_iterator()
    {
      return Weakly_incrementable<I>() && !Random_access_iterator<I>();
    }



  // Incrementable properties  
  // These traits help unify some aspects of incrementable types and iterators.
  // If an abstraction can be adapted to incrementable but not necessarily
  // readable types, then these traits can be used instead of the usual 
  // iterator traits.



  // Iterator range properties
  // The following properties are just used to indicate un-checkable
  // preconditions.
  //
  // TODO: Specialize these operations so that we can actually check the
  // ones that we know about.


  // Returns true if [first, n) is a weak range.
  template <typename I>
    inline bool is_weak_range(I first, Difference_type<I> n) 
    { 
      static_assert(Weakly_incrementable<I>(), "");
      return n >= 0;
    }
  
  // Returns true if [first, n) is a counted range.
  template <typename I>
    inline bool is_counted_range(I first, Difference_type<I> n) 
    { 
      static_assert(Weakly_incrementable<I>(), "");
      return n >= 0;
    }
    
  // Returns true if [first, last) is a bounded range.
  template <typename I>
    inline auto is_bounded_range(I first, I last) 
      -> Requires<Non_random_access_iterator<I>(), bool>
    {
      static_assert(Equality_comparable<I>(), "");
      return true; 
    }

  // Overload for random access iterators. Minimally, we can guarantee that
  // first <= last, even though some or all elements of [first, last) may not
  // be valid iterators.
  template <typename I>
    inline auto is_bounded_range(I first, I last) 
      -> Requires<Random_access_iterator<I>(), bool>
    {
      return first <= last; 
    }
    
  // Returns true if the weak range [first, n) is readable everywhere except
  // its limit.
  template <typename I>
    inline bool is_readable_range(I first, Difference_type<I> n) 
    {
      static_assert(Readable<I>(), "");
      return is_weak_range(first, n); 
    }
  
  // Returns true if the bounded range [first, last) is readable everywhere
  // except its limit.
  template <typename I>
    inline bool is_readable_range(I first, I last) 
    { 
      static_assert(Readable<I>(), "");
      return is_bounded_range(first, last); 
    }
    
  // Returns true if the weak range [first, n) is writable everywhere except
  // its limit.
  template <typename I, typename T>
    inline bool is_writable_range(I first, Difference_type<I> n, T const& value)
    {
      static_assert(Copy_writable<I, T>(), "");
      return is_weak_range(first, n);
    }
   
  // Returns true if the bounded range [first, last) is writable everywhere 
  // except its limit.
  template <typename I, typename T>
    inline bool is_writable_range(I first, I last, T const& value) 
    { 
      static_assert(Copy_writable<I, T>(), "");
      return is_bounded_range(first, last);
    }
    
  // Returns true if the weak range [first, n) is movable everywhere except its
  // limit.
  template <typename I, typename T>
    inline bool is_movable_range(I first, Difference_type<I> n, T const& value)
    {
      static_assert(Move_writable<I, T&&>(), "");
      return is_weak_range(first, n);
    }
  
  // Returns true if the bounded range [first, last) is movable everywhere except
  // its limit.
  template <typename I, typename T>
    inline bool is_movable_range(I first, I last, T const& value) 
    {
      static_assert(Move_writable<I, T&&>(), "");
      return is_bounded_range(first, last); 
    }
    
  // Returns true if the weak range [first, n) is mutable everywhere except its
  // limit.
  template <typename I>
    inline bool is_mutable_range(I first, Difference_type<I> n)
    {
      return n > 0 ? is_writable_range(first, n, *first) : true;
    }
    
  // Returns true if the bounded range [first, last) is mutable everywhere
  // except its limit.
  template <typename I>
    inline bool is_mutable_range(I first, I last)
    {
      return first != last ? is_writable_range(first, last, *first) : true;
    }

  // Return true if the weak range [first, n) is permutable everywhere except
  // its limit.
  template <typename I>
    inline bool is_permutable_range(I first, Difference_type<I> n)
    {
      return n > 0 ? is_movable_range(first, n, *first) : true;
    }
    
  // Return true if the bounded range [first, n) is permutable everywhere
  // except its limit.
  template <typename I>
    inline bool is_permutable_range(I first, I last)
    {
      return first != last ? is_movable_range(first, last, *first) : true;
    }



  // Iterator operations
  //
  // The standard iterator operations simply assert the minimum requirements
  // before delegating to the usual algorithm. This library does not dispatch
  // based on iterator type.


  // Incrementally advance i by n positions where n >= 0.
  template <typename I>
    void iterative_advance(I& i, Difference_type<I> n = 1)
    {
      static_assert(Weakly_incrementable<I>(), "");
      assert(n >= 0);
      while (n != 0) {
        ++i;
        --n;
      }
    }

  // Incrementally decrement i by n positions where n >= 0.
  template <typename I>
    void iterative_retreat(I& i, Difference_type<I> n = 1)
    {
      static_assert(Decrementable<I>(), "");
      assert(n >= 0);
      while (n != 0) {
        --i;
        --n;
      }
    }

  

  // Advance interator
  // Advance i by n positions. 
  //
  // If I is weakly inrementable, then n must be
  // non-neative. If I is bidirectional, then n may be negative. If I is a
  // random access iterator, then the operation completes in constant time.
  //
  // Requires:
  //    Weakly_incrementable<I>
  //    Acceptable values of n depends on the concept modeled by I.
  template <typename I>
    inline auto advance(I& i, Difference_type<I> n = 1)
      -> Requires<Weakly_incrementable<I>() && !Bidirectional_iterator<I>()>
    {
      assert(is_weak_range(i, n));
      iterative_advance(i, n);
    }
  
  template <typename I>
    inline auto advance(I& i, Difference_type<I> n = 1)
      -> Requires<Bidirectional_iterator<I>() && !Random_access_iterator<I>()>
    {
      assert(is_weak_range(i, n));
      if (i > 0)
        iterative_advance(i, n);
      else
        iterative_retreat(i, -n);
     }

  // TODO: Replace Random_access_iterator with a more general concept.
  template <typename I>
    inline auto advance(I& i, Difference_type<I> n = 1)
      -> Requires<Random_access_iterator<I>()>
    {
      i += n;
    }



  // Next iterator
  // Return the nth iterator past i where n >= 0.
  template <typename I>
    inline I next(I i, Difference_type<I> n = 1)
    {
      static_assert(Weakly_incrementable<I>(), "");
      assert(is_weak_range(i, n));
      advance(i, n);
      return i;
    }
    
    
  // Previous iterator
  // Return the nth iterator before i where n >= 0.
  template <typename I>
    inline I prev(I i, Difference_type<I> n = 1)
    {
      static_assert(Decrementable<I>(), "");
      assume(bounded_range(prev(i, n), i));
      advance(i, -n);
      return i;
    }
  


  // Iterative distance
  // Return the distance between first and last by counting the number of times
  // first must be increemnted until it reaches last.
  template <typename I>
    inline Difference_type<I> iterative_distance(I first, I last)
    {
      Difference_type<I> n = 0;
      while (first != last) {
        ++n;
        ++first;
      }
      return n;
    }


  // Distance
  // Return the distance between first and last.
  //
  // FIXME: Because [first, last) is a boundd range, the result of this
  // operation must be non-negative.
  template <typename I>
    inline auto distance(I first, I last)
      -> Requires<Strict_weakly_incrementable<I>(), std::ptrdiff_t>
    {
      assert(is_bounded_range(first, last));
      return iterative_distance(first, last);
    }

  // FIXME: I think this algorithm should be defined for any type whose
  // subtraction operator satisfies the requirement of a 
  template <typename I>
    inline auto distance(I first, I last)
      -> Requires<Random_access_iterator<I>(), decltype(last - first)>
    {
      assert(is_bounded_range(first, last));
      return std::abs(last - first);
    }





  // Iterative bounded next
  // Returns the nth iterator past first. If first + n exceeds last, the
  // operation returns last. This operation increments first exactly
  // min(n, distance(first, last) times.
  template <typename I>
    I iterative_bounded_next(I first, Difference_type<I> n, I last)
    {
      static_assert(Weakly_incrementable<I>(), "");
      assert(is_bounded_range(first, last));
      assert(n >= 0);
      while (n != 0 && first != last) {
        ++first;
        --n;
      }
      return first;
    }



  // Bounded next (operation)
  // Returns the nth iterator past first. If first + n exceeds last, the
  // operation returns last. This operation is specialized for random accesss
  // iterators, returning in constant time.
  template <typename I>
    auto bounded_next(I first, Difference_type<I> n, I last)
      -> Requires<Non_random_access_iterator<I>(), I>
    {
      assert(n >= 0);
      return iterative_bounded_next(first, n, last);
    }

  template <typename I>
    auto bounded_next(I first, Difference_type<I> n, I last)
      -> Requires<Random_access_iterator<I>(), I>
    {
      assert(n >= 0);
      Difference_type<I> d = distance(first, last);
      return first += (n < d ? n : d);
    }

  template <typename I>
    inline I bounded_next(I first, I last)
    {
      return bounded_next(first, 1, last);
    }

  
  // Iterative bounded prev
  // Returns the nth iterator before last. If last - n exceeds first, the
  // operation returns first. This operation decrements last exactly
  // min(n, distance(first, last) times.
  //
  // FIXME: Assert requirements.
  template <typename I>
    I iterative_bounded_prev(I last, Difference_type<I> n, I first)
    {
      static_assert(Decrementable<I>(), "");
      assert(n >= 0);
      while (n != 0 && first != last) {
        --last;
        --n;
      }
    }


  // Bounded prev (operation)
  // Returns the nth iterator before last. If last - n exceeds first, the
  // operation returns first. This operation is specialized for random access
  // iterators, returning in constant time.
  template <typename I>
    auto bounded_prev(I last, Difference_type<I> n, I first)
      -> Requires<Non_random_access_iterator<I>(), I>
    {
      assert(n >= 0);
      return iterative_bounded_prev(last, n, first);
    }

  template <typename I>
    auto bounded_prev(I last, Difference_type<I> n, I first)
      -> Requires<Random_access_iterator<I>(), I>
    {
      assert(n >= 0);
      Difference_type<I> d = distance(first, last);
      return last -= (n < d) ? n : d;
    }

  template <typename I>
    I bounded_prev(I last, I first)
    {
      return bounded_prev(last, 1, first);
    }



  // Next if
  //
  // Returns next(first) only if first != last and pred(*first) is true.
  // Otherwise, returns last.
  template <typename I, typename P>
    I next_if(I first, I last, P pred)
    {
      if (first != last && pred(*first))
        return next(first);
      else
        return last;
    }


  // Next if not
  //
  // Returns next(first) only if first != last and !pred(*first) is true. 
  // Otherwise, returns last.
  template <typename I, typename P>
    I next_if_not(I first, I last, P pred)
    {
      if (first != last)
        return pred(*first) ? last : next(first);
      else
        return last;
    }

  // TODO: Implement prev_if and prev_if_not algorithms.


  // Exceeds upper limit
  //
  // Returns true if first + n will refer to a position past last.
  template <typename I>
    bool exceeds_upper_limit(I first, Difference_type<I> n, I last)
    {
      static_assert(Weakly_incrementable<I>(), "");
      assert(bounded_range(first, last));
      while (n != 0 && first != last) {
        ++first;
        --n;
      }

      // Returns true if we've reached the limit, but would have continued 
      // decrementing.
      return first == last && n != 0;
    }



  // Exceeds lower limit
  // Returns true if last - n will refer to a position before first.
  //
  // FIXME: Assert requirements.
  template <typename I>
    bool exceeds_lower_limit(I last, Difference_type<I> n, I first)
    {
      static_assert(Decrementable<I>(), "");
      while (n != 0 && first != last) {
        --last;
        --n;
      }

      // Returns true if we've reached the limit, but would have continued 
      // decrementing.
      return first == last && n != 0;
    }


  
  // Exceeds limits
  // Returns true if advance(first, n) results in a valid iterator. Note that 
  // n may be postiive or negative.
  template <typename I>
    bool exceeds_limits(I first, Difference_type<I> n, I last)
    {
      if (n > 0)
        return exceeds_upper_limit(first, n, last);
      else if (n < 0)
        return exceeds_lower_limit(last, n, first);
      else
        return true;
    }

} // namespace origin

#endif
