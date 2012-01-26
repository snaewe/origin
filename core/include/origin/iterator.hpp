// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_HPP
#define ORIGIN_ITERATOR_HPP

#include <iterator>

#include <origin/concepts.hpp>

namespace origin
{
  // Separate the checking of static requirements out of the Readable concept.
  // This prevents compiler errors when associated types aren't found. If
  // associated types can't be found, then the requirements will obviously
  // fail to pass.
  template<typename Iter, bool Types>
    struct Readable_requirements
    {
      static constexpr bool check() { return false; }
    };

  template<typename Iter> 
    struct Readable_requirements<Iter, true>
    {
      static constexpr bool check()
      {
        return Has_dereference<Iter>()
            && Convertible<Dereference_result<Iter>, const Value_type<Iter>&>();
      }
    };
  
  // Allows the following expression: Value_type<Iter> const& x = *i.
  template<typename Iter>
    struct Readable_concept
    {
      static constexpr bool check()
      {
        return Readable_requirements<Iter, Has_value_type<Iter>()>::check();
      }
      
      static bool test(Iter i)
      {
        const Value_type<Iter>& x = *i; // NOTE: Not an axiom.
        return true;
      }
    };
    
    
  // Returns true if Iter is readable.
  //
  // FIXME: To better align this with standard terminology, this should be
  // called Value_readable.
  template<typename Iter>
    constexpr bool Readable()
    {
      return Readable_concept<Iter>::check();
    }

    

  // Writable (concept)
  // Returns true if values of T can be written through an iterator of type
  // I using the syntax: *i = value. 
  //
  // Note that if T is an rvalue reference, the concept requires T to support
  // move semantics. If T is not an rvalue reference, then copy semantics are
  // required.
  //
  // It is strongly recommended to include Movable/Copyable requirements
  // in addition to those implied by Writable concept. The Permutable and
  // Mutable concepts do this.
  
  // If T is not an rvalue reference, check for copy assigment.
  template<typename I, typename T>
    struct Writable_concept
    {
      static constexpr bool check()
      {
        return Assignable<Dereference_result<I>, const T&>();
      }
    };
    
  // If T is an rvalue reference, check for move assignment.
  template<typename I, typename T>
    struct Writable_concept<I, T&&>
    {
      static constexpr bool check()
      {
        return Assignable<Dereference_result<I>, T&&>();
      }
    };
  
  // Returns true if the expression *i = value is valid.
  template<typename I, typename T>
    constexpr bool Writable()
    {
      return Has_dereference<I>() && Writable_concept<I, T>::check();
    }


    
  // Mutable (concept)
  // Returns true if I mutable. A mutable iterator is readable and writable,
  // and has a copyable value type.
  template<typename I>
    constexpr bool Mutable()
    {
      return Readable<I>() && Copyable<Value_type<I>>() && Writable<I, Value_type<I>>();
    }
    
    
    
  // Permutable (concept)
  // Returns true if I is permutable. A permutable iterator is readable,
  // move-writable, and has a movable value type.
  template<typename I>
    constexpr bool Permutable()
    {
      return Readable<I>() && Movable<Value_type<I>>() && Writable<I, Value_type<I>&&>();
    }
    
    

  // Incrementable types
  //
  // FIXME: Rewrite the incrementable concepts so that equality comparison is
  // always required. Weakly_incrementable should be Semi_incrementable and
  // Incrementable should be as it is. This will let me get rid of
  // Weak_input_iterator.
  
  
  // A weakly incrementable type is a semiregular type that can be pre- and
  // post-incremented. Neither operation is requireed to be equality
  // preserving, and the result of post-increment is unspecified.
  template<typename I>
    struct Weakly_incrementable_concept
    {
      static constexpr bool check()
      {
        return Semiregular<I>()
            && Has_distance_type<I>()
            
            // I& == { ++i }
            && Has_pre_increment<I>()
            && Same<Pre_increment_result<I>, I&>()
            
            // i++
            && Has_post_increment<I>();
      }
      
      // FIXME: Write semantics? Not sure if it's really possible.
    };

  template<typename I>
    constexpr bool Weakly_incrementable()
    {
      return Weakly_incrementable_concept<I>::check();
    }
    
  
  
  // An Incrementable type is a Regular, weakly incrementable type with
  // equality preserving pre- and post-increment operations.
  template<typename I>
    struct Incrementable_concept
    {
      static constexpr bool check()
      {
        return Regular<I>()
            && Weakly_incrementable<I>()

            // I == { i++ }
            && Same<Post_increment_result<I>, I>();
      }
    };

  template<typename I>
    constexpr bool Incrementable()
    {
      return Incrementable_concept<I>::check();
    }



  // Iterators
  // The following concept classes and predicates are define checking
  // capabilities for iterator concepts.
    
    
    
  // Iterator categories
  //
  // Every iterator type explicitly describes its category, one of the
  // std::*_iterator_tag classes. For user-defined iterators, this is provided
  // as a nested type name (i.e., Iter::iterator_category). For pointers, the
  // category is random access.
  
  // Safely deduce a custom iterator category as a nested type.
  template<typename Iter>
    struct get_iterator_category
    {
    private:
      template<typename X>
        static typename X::iterator_category check(X&&);

      template<typename X>
        static typename std::iterator_traits<X*>::iterator_category check(X*);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<Iter>()));
    };
    
  // An alias for the category of Iter.
  template<typename Iter>
    using Iterator_category = typename get_iterator_category<Iter>::type;
    
  // Return true if the iterator category is valid.
  template<typename Iter>
    constexpr bool Has_iterator_category()
    {
      return Subst_succeeded<Iterator_category<Iter>>();
    }

  // Return an iterator category that is not more derived than the given limit. 
  // For example, if Tag is "random access" and Limit is "forward", this will
  // return limit. 
  //
  // Note that this does not guarantee that limit is actually derived from
  // Tag. That's just assumed.
  //
  // TODO: This seems like it could generalize to any Tag hierarchy. Consider
  // how this might be done (in terms of lattices?) and then move the 
  // generalized solution into  Traits or Concepts.
  template<typename Tag, typename Limit>
    using Clamp_iterator_category = If<
      Derived<Iterator_category<Tag>, Limit>(), Limit, Iterator_category<Tag>
    >;



  // Returns true if Iter has all of the associated iterator types.
  template<typename Iter>
    constexpr bool Has_iterator_types()
    {
      return Has_iterator_category<Iter>() 
          && Has_value_type<Iter>()
          && Has_distance_type<Iter>();
    }

    
    
  // Return true if T is an iterator. Here, T is an iterator if it has all of
  // the requisite associated types.
  template<typename T>
    constexpr bool Iterator()
    {
      return Has_iterator_types<T>();
    }
  


  // An alias for the associated reference type of the iterator. This supports 
  // writing  backwards compatible iterators where the reference type is 
  // actually named even though it should be deduced as decltype(*i).
  template<typename Iter>
    using Iterator_reference = typename std::iterator_traits<Iter>::reference;



  // An alias for the associated pointer type of the iterator. This supports 
  // writing  backwards compatible iterators where the reference type is 
  // actually named even though it is never used in any STL algorithms. Note
  // that the pointer type cannot be deduced as decltype(&*i).
  template<typename Iter>
    using Iterator_pointer = typename std::iterator_traits<Iter>::pointer;
    
      
      
  // Input and output iterators
  //
  // Abstractly, an input iterator like a tape. Elements can be read as the
  // tape advances, but those elements are inaccessible after the increment.
  //
  // An output iterator is similar to an input iterator in that it is an 
  // abstract model of a tape except that elements are written to the tape
  // instead of reading from it. Note that once an element is written, the
  // (conceptually) tape advances.
  //
  // NOTE: We don't need to check the Iterator_category of input or output
  // iterators since they can be statically differentiated. In fact, the only
  // place we need to check the Iterator_category is the Forward_iterator
  // concept.


    
  // Weak input iterator (concept)
  // A weak input iterator is weakly incrementable and readable.
  template<typename I>
    constexpr bool Weak_input_iterator()
    {
      return Weakly_incrementable<I>() && Readable<I>();
    }
    
    
    
  // Input iterator (concept)
  // A input iterator is a weak input iterator that is equality comparable.
  template<typename I>
    constexpr bool Input_iterator()
    {
      return Weak_input_iterator<I>() && Equality_comparable<I>();
    }



  // Weak output iterator (concept)
  // A weak output iterator is a weakly incrementable type that is writable
  // over some expression type T.
  //
  // Note that if T is an rvalue reference, this concept requires T to
  // implement move semantics. If T is not an rvalue reference, the type is
  // required to implement copy semantics.
  template<typename I, typename T>
    constexpr bool Weak_output_iterator()
    {
      return Weakly_incrementable<I>() && Writable<I, T>();
    }
    
    
    
  // Output iterator (concept)
  // An output iterator is a weak output iterator that is equality comparable.
  //
  // Note that if T is an rvalue reference, this concept requires T to
  // implement move semantics. If T is not an rvalue reference, the type is
  // required to implement copy semantics.
  template<typename I, typename T>
    constexpr bool Output_iterator()
    {
      return Weak_output_iterator<I, T>() && Equality_comparable<I>();
    }
 
 
  
  // Forward Iterator (concept)
  // A forward iterator is an input iterator with a regular post-increment
  // operation. This guarantees that multiple passes of a range may be made
  // and that multiple iterators into the range may be used.
  template<typename I>
    constexpr bool Forward_iterator()
    {
      return Input_iterator<I>()
          && Incrementable<I>()
          && Readable<I>()
          && Derived<Iterator_category<I>, std::forward_iterator_tag>();
    }
    

    
  // Bidirectional Iterators
  // A bidirectional iterator is a forward iterator that can also move 
  // backwards using decrement operators.
    
  // A helper class for checking syntactic requirements.
  template<typename Iter, bool Prereqs>
    struct Bidirectional_iterator_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename Iter>
    struct Bidirectional_iterator_requirements<Iter, true>
    {
      static constexpr bool check()
      {
        return Derived<Iterator_category<Iter>, std::bidirectional_iterator_tag>()
            
            // Iter& == { --i }
            && Has_pre_decrement<Iter>()
            && Same<Pre_decrement_result<Iter>, Iter&>()
            
            // Iter == { i-- }
            && Has_post_decrement<Iter>()
            && Same<Post_decrement_result<Iter>, Iter>();
      }
    };
    
  
  
  // A bidirectional iterator is a forward iterator that supports 
  template<typename Iter>
    struct Bidirectional_iterator_concept
    {
      static constexpr bool check()
      {
        return Bidirectional_iterator_requirements<
          Iter, Forward_iterator<Iter>()
        >::check();
      }
      
      static bool test()
      {
        // FIXME: Write semantics.
        return true;
      }
    };

  template<typename Iter>
    constexpr bool Bidirectional_iterator()
    {
      return Bidirectional_iterator_concept<Iter>::check();
    };
    

    
    
  // Random Access Iterators
  // A random access iterator is a bidirectional iterator that can advance 
  // any number of steps in constant time.
    
  // A helper class for checking syntactic requirements.
  template<typename Iter, bool Prereqs>
    struct Random_access_iterator_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename Iter>
    struct Random_access_iterator_requirements<Iter, true>
    {
      static constexpr bool check()
      {
        return Derived<Iterator_category<Iter>, std::random_access_iterator_tag>()
            && Signed<Distance_type<Iter>>()

            // Iter& == { i += n }
            && Has_plus_assign<Iter, Distance_type<Iter>>()
            && Same<Plus_assign_result<Iter, Distance_type<Iter>>, Iter&>()
            
            // Iter& == { i -= n }
            && Has_minus_assign<Iter, Distance_type<Iter>>()
            && Same<Minus_assign_result<Iter, Distance_type<Iter>>, Iter&>()
            
            // Iter == { i + n }
            && Has_plus<Iter, Distance_type<Iter>>()
            && Same<Plus_result<Iter, Distance_type<Iter>>, Iter>()
            
            // Iter == { n + i }
            && Has_plus<Distance_type<Iter>, Iter>()
            && Same<Plus_result<Distance_type<Iter>, Iter>, Iter>()
            
            // Iter == { i - n }
            && Has_minus<Iter, Distance_type<Iter>>()
            && Same<Minus_result<Iter, Distance_type<Iter>>, Iter>()
            
            // Distance_type<Iter> == { i - j }
            && Has_minus<Iter>()
            && Same<Minus_result<Iter>, Distance_type<Iter>>()
            
            // decltype(*i) == { i[n] }
            && Has_subscript<Iter, Distance_type<Iter>>()
            && Same<Subscript_result<Iter, Distance_type<Iter>>, Dereference_result<Iter>>();
      }
    };
    
    
  // The specification of random access iterators.
  template<typename Iter>
    struct Random_access_iterator_concept
    {
      static constexpr bool check()
      {
        return Random_access_iterator_requirements<
          Iter, Bidirectional_iterator<Iter>()
        >::check();
      }
      
      static bool test()
      {
        // FIXME: Write semantics
        return true;
      }
    };

  // Returns true if Iter is random access iterator.
  template<typename Iter>
    constexpr bool Random_access_iterator()
    {
      return Random_access_iterator_concept<Iter>::check();
    };
    
    
  // A strict input iterator is at most an input iterator. That is I is not
  // a forward iterator. This is provided for convenience.
  template<typename I>
    constexpr bool Strict_input_iterator()
    {
      return Weak_input_iterator<I>() && !Forward_iterator<I>();
    }
    
    
    
  // A strict output iterator is at most an output iterator. That is, I is not
  // also an readable.
  template<typename I>
    constexpr bool Strict_output_iterator()
    {
      return Weak_output_iterator<I> && !Readable<I>();
    }
  
  

  // Incrementable properties  
  // These traits help unify some aspects of incrementable types and iterators.
  // If an abstraction can be adapted to incrementable but not necessarily
  // readable types, then these traits can be used instead of the usual 
  // iterator traits.
  
  
  
  // An alias for the difference type of a counter argument. For Integral types,
  // this is the signed value of that type. For Iterator types, it is the 
  // distance type.
  //
  // FIXME: This is not really correct for integral types. It should be the
  // next larger signed representation.
  template<typename I>
    using Incrementable_distance = 
      If<Integral<I>(), Make_signed<I>, Distance_type<I>>;

  // An alias for the iterator category of a counter. If I is an integral type,
  // the category is "random access". Otherwise, it is the iterator's category.
  template<typename I>
    using Incrementable_category = 
      If<Integral<I>(), std::random_access_iterator_tag, Iterator_category<I>>;

  
  
  
  
  // Iterator range properties
  // The following properties are just used to indicate un-checkable
  // preconditions.
  //
  // Even though these could be constexpr function, they are not intended to
  // be used in constant expressions.
  
  // Returns true if [first, n) is a weak range.
  template<typename I>
    inline bool is_weak_range(I first, Distance_type<I> n) 
    { 
      static_assert(Weakly_incrementable<I>(), "");
      return n >= 0;
    }
  
  // Returns true if [first, n) is a counted range.
  template<typename I>
    inline bool is_counted_range(I first, Distance_type<I> n) 
    { 
      static_assert(Weakly_incrementable<I>(), "");
      return n >= 0;
    }
    
  // Returns true if [first, last) is a bounded range.
  template<typename I>
    inline bool is_bounded_range(I first, I last) 
    {
      static_assert(Weakly_incrementable<I>(), "");
      static_assert(Equality_comparable<I>(), "");
      return true; 
    }
    
  // Returns true if the weak range [first, n) is readable everywhere except
  // its limit.
  template<typename I>
    inline bool is_readable_range(I first, Distance_type<I> n) 
    {
      static_assert(Readable<I>(), "");
      return is_weak_range(first, n); 
    }
  
  // Returns true if the bounded range [first, last) is readable everywhere
  // except its limit.
  template<typename I>
    inline bool is_readable_range(I first, I last) 
    { 
      static_assert(Readable<I>(), "");
      return is_bounded_range(first, last); 
    }
    
  // Returns true if the weak range [first, n) is writable everywhere except
  // its limit.
  template<typename I, typename T>
    inline bool is_writable_range(I first, Distance_type<I> n, T const& value)
    {
      static_assert(Writable<I, T>(), "");
      return is_weak_range(first, n);
    }
   
  // Returns true if the bounded range [first, last) is writable everywhere 
  // except its limit.
  template<typename I, typename T>
    inline bool is_writable_range(I first, I last, T const& value) 
    { 
      static_assert(Writable<I, T>(), "");
      return is_bounded_range(first, last);
    }
    
  // Returns true if the weak range [first, n) is movable everywhere except its
  // limit.
  template<typename I, typename T>
    inline bool is_movable_range(I first, Distance_type<I> n, T const& value)
    {
      static_assert(Writable<I, T&&>(), "");
      return is_weak_range(first, n);
    }
  
  // Returns true if the bounded range [first, last) is movable everywhere except
  // its limit.
  template<typename I, typename T>
    inline bool is_movable_range(I first, I last, T const& value) 
    {
      static_assert(Writable<I, T&&>(), "");
      return is_bounded_range(first, last); 
    }
    
  // Returns true if the weak range [first, n) is mutable everywhere except its
  // limit.
  template<typename I>
    inline bool is_mutable_range(I first, Distance_type<I> n)
    {
      return n > 0 ? is_writable_range(first, n, *first) : true;
    }
    
  // Returns true if the bounded range [first, last) is mutable everywhere
  // except its limit.
  template<typename I>
    inline bool is_mutable_range(I first, I last)
    {
      return first != last ? is_writable_range(first, last, *first) : true;
    }

  // Return true if the weak range [first, n) is permutable everywhere except
  // its limit.
  template<typename I>
    inline bool is_permutable_range(I first, Distance_type<I> n)
    {
      return n > 0 ? is_movable_range(first, n, *first) : true;
    }
    
  // Return true if the bounded range [first, n) is permutable everywhere
  // except its limit.
  template<typename I>
    inline bool is_permutable_range(I first, I last)
    {
      return first != last ? is_movable_range(first, last, *first) : true;
    }



  // Iterator operations
  //
  // The standard iterator operations simply assert the minimum requirements
  // before delegating to the usual algorithm. This library does not dispatch
  // based on iterator type.
    


  // Advance interator
  // Advance i by n positions.
  template<typename Iter>
    inline void o_advance(Iter& i, Distance_type<Iter> n = 1)
    {
      static_assert(Weakly_incrementable<Iter>(), "");
      assert(( is_weak_range(i, n) ));

      std::advance(i, n);
    }
    


  // Next iterator
  // Return the nth iterator past i.
  template<typename Iter>
    inline Iter o_next(Iter i, Distance_type<Iter> n = 1)
    {
      static_assert(Weakly_incrementable<Iter>(), "");
      assert(( is_weak_range(i, n) ));
      
      return std::next(i, n);
    }
    
    
  // Previous iterator
  // Return the nth iterator before i.
  template<typename Iter>
    inline Iter o_prev(Iter i, Distance_type<Iter> n = 1)
    {
      static_assert(Bidirectional_iterator<Iter>(), "");
      assume(( bounded_range(prev(i, n), i) ));

      return std::prev(i, n);
    }
  
  
  
  // Iterator distance
  // Return the distance between i and j.
  //
  // FIXME: Because [first, last) is a boundd range, the result of this
  // operation must be non-negative.
  template<typename Iter>
    inline Distance_type<Iter> o_distance(Iter first, Iter last)
    {
      static_assert(Weakly_incrementable<Iter>(), "");
      assert(( is_bounded_range(first, last) ));
      
      return std::distance(first, last);
    }
    
  
  // Iterator distance (action)
  // Return the distance between a and b using action to advance over the
  // range.
  //
  // FIXME: If we generalize over the increment, do we need a precondition that
  // [first, last) forms a bounded range over adv? Probably. This idea seems
  // to be missing from EoP.
  template<typename Iter, typename Act>
    inline Distance_type<Iter> distance(Iter first, Iter last, Act adv)
    {
      assume(( is_reachable(first, last, adv) ));
      
      Distance_type<Iter> n = 0;
      while(first != last) {
        ++n;
        adv(first);
      }
      return n;
    }



  // Advance actions
  // An action function is a void function that modifies one or more of its
  // arguments. These actions increment, decrement, and advance an iterator
  // some number of times.
  
  // The increment action increments an object of type I.
  template<typename I>
    struct increment_action
    {
      void operator()(I& x) const { ++x; }
      
      static constexpr Incrementable_distance<I> increment = 1;
    };

  // The decrement action decrements an object of type I.
  template<typename I>
    struct decrement_action
    {
      void operator()(I& x) const { --x; }

      static constexpr Incrementable_distance<I> increment = -1;
    };
    
  // The advance action advances an iterator a fixed number of times.
  template<typename I>
    struct advance_action
    {
      advance_action(Distance_type<I> n) : increment{n} { }

      void operator()(I& i) const { o_advance(i, increment); }
      
      Distance_type<I> increment;
    };
    
  // The static advance action advance an iterator a statically determined 
  // number of times.
  template<typename I, Incrementable_distance<I> N>
    struct static_advance_action
    {
      void operator()(I& i) const { o_advance(i, N); }
      
      static constexpr Incrementable_distance<I> increment = N;
    };

    

  // Get increment
  // Return the increment value for an increment action. This is used to
  // determine the stride of counters and stride iterators. Note that the
  // result is a constant expression iff the associated member is a constant
  // expression.
  template<typename Act>
    constexpr auto get_increment(const Act& adv) -> decltype(adv.increment)
    {
      return adv.increment;
    }
  
  
  
  // Advance action (concept)
  // An advance action is a function that advances an incrementable object
  // some number of times. It has an associated increment, which can be
  // accessed via the get_increment operation.
  
  // Safely deduce the result of get_increment(f).
  template<typename T>
    struct get_increment_result
    {
    private:
      template<typename X>
        static auto check(const X& x) -> decltype(get_increment(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<const T&>()));
    };
    
  // An alias for the result of get_increment(f)
  template<typename T>
    using Get_increment_result = typename get_increment_result<T>::type;
    
  // Returns true if F is an increment action.
  template<typename F, typename I>
    constexpr bool Advance_action()
    {
      return Function<F, I>() && Subst_succeeded<Get_increment_result<F>>();
    }

    
    
  // Unwrap iterator
  // This facility provides an extension point for unwrapping iterators from
  // some adaptors, allowing us to determine the actual properties of the
  // underlying iterator abstraction. This is particularly important when
  // specializing algorithms for low-level memory operations (e.g., memcopy,
  // memmove, memcmp, etc.).
  //
  // The goal of unwrapping iterators is to determine if they ultimately refer
  // to a pointer.
  
  // Returns i if the I is not an wrapper.
  template<typename I>
    inline I unwrap_iterator(I i) { return i; }

  // Move iterators can be recursively unwrapped.
  template<typename T>
    inline auto unwrap_iterator(std::move_iterator<T*> i) 
      -> decltype(unwrap_iterator(i.base()))
    { 
      return i; 
    }

#if defined(__GNUC__)
  // Unwrap GCC normal iterators. These are never used as adaptors of other
  // iterators.
  template<typename I, typename C>
    inline I unwrap_iterator(__gnu_cxx::__normal_iterator<I, C> i) 
    { 
      return i.base(); 
    }
#endif
    
  // An alias to the iterator base of I, or I if it is not an unwrappable
  // iterator adaptor. Note that for all iterators I:
  //
  //    Value_type<I> == Value_type<Iterator_base<I>>.
  //
  // This guarantee allows use to reason about the kinds of objects referred to
  // indirectly.
  template<typename I>
    using Iterator_base = decltype(unwrap_iterator(std::declval<I>()));
    

    
  // Concepts for memory optimization

  // Two iterators can be memcmp-ared when they are pointers to the same
  // standard layout type, but only if the size and alignment of those types
  // are equal.
  //
  // NOTE: This is generalized for comparisons on objects of different types,
  // but only if the size of their value types is the same. Semantically, this
  // would require the value types of I1 and I2 to share a common type.
  template<typename I1, typename I2>
    constexpr bool Can_memcmp()
    {
      using B1 = Iterator_base<I1>;
      using B2 = Iterator_base<I2>;
      return Pointer<B1>() && Memory_comparable<Value_type<B1>>()
          && Pointer<B2>() && Memory_comparable<Value_type<B2>>()
          && sizeof(Value_type<B1>) == sizeof(Value_type<B2>);
    }
      
} // namespace origin


#endif
