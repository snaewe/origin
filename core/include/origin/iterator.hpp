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

    
  
  // An iterator is Move_writable if the expression '*i = move(x)' is valid.
  template<typename Iter, typename T>
    struct Move_writable_concept
    {
      static constexpr bool check()
      {
        return Movable<T>()
            && Has_dereference<Iter>()
            && Assignable<Dereference_result<Iter>, T&&>();
      }
      
      static bool test(Iter i, T x)
      {
        *i = std::move(x);  // NOTE: Not an axiom.
        return true;
      }
    };
    
  // Returns true if values of T can be moved through an Iter object.
  //
  // FIXME: To better align this with standard terminology, this should be
  // called Value_movable.
  template<typename Iter, typename T>
    constexpr bool Move_writable()
    {
      return Move_writable_concept<Iter, T>::check();
    }

  
  
  // An iterator is writable if the expression '*i = x' is valid.
  template<typename Iter, typename T>
    struct Writable_concept
    {
      static constexpr bool check()
      {
        return Copyable<T>()
            && Move_writable<Iter, T>()
            && Assignable<Dereference_result<Iter>, T const&>();
      }
      
      static bool test()
      {
        // FIXME: Write semantics
        return true;
      }
    };
    
  // Returns true if values of T can be written through an Iter object.
  //
  // FIXME: To better align this with standard terminology, this should be
  // called Value_writable.
  template<typename Iter, typename T>
    constexpr bool Writable()
    {
      return Writable_concept<Iter, T>::check();
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
  // Note that this design does not require the expression *i++ for input
  // iterators. The semantics of that operation limit the generality of the
  // concept.

  // Weak input iterator
  // A weak input iterator is weakly incrementable and readable.
  template<typename Iter>
    struct Weak_input_iterator_concept
    {
      static constexpr bool check()
      {
        return Weakly_incrementable<Iter>()
            && Readable<Iter>();
      }
    };

  // Returns true if Iter is a weak input iterator.
  template<typename Iter>
    constexpr bool Weak_input_iterator()
    {
      return Weak_input_iterator_concept<Iter>::check();
    }
    
    
    
  // Input iterators
  // An input
  template<typename Iter, bool Prereqs>
    struct Input_iterator_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename Iter>
    struct Input_iterator_requirements<Iter, true>
    {
      static constexpr bool check()
      {
        return Derived<Iterator_category<Iter>, std::input_iterator_tag>()
            && Equality_comparable<Iter>()
            && Weakly_incrementable<Iter>();
      }
    };
    
  // An input iterator...
  template<typename Iter>
    struct Input_iterator_concept
    {
      static constexpr bool check()
      {
        return Input_iterator_requirements<
          Iter, Regular<Iter>() && Readable<Iter>() && Has_iterator_types<Iter>()
        >::check();
      }
      
      static bool test()
      {
        // FIXME: Write semantics.
        return true;
      }
    };
    
  template<typename Iter>
    constexpr bool Input_iterator()
    {
      return Input_iterator_concept<Iter>::check();
    }
    
 
  
  // Forward Iterators
  // A forward iterator is an input iterator with a regular post-increment
  // operation. This guarantees that multiple passes of a range may be made
  // and that multiple iterators into the range may be used.
  
  // A helper class for checking syntactic requirements.
  template<typename Iter, bool Prereqs>
    struct Forward_iterator_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename Iter>
    struct Forward_iterator_requirements<Iter, true>
    {
      static constexpr bool check()
      {
        return Derived<Iterator_category<Iter>, std::forward_iterator_tag>()
            && Incrementable<Iter>()
            && Readable<Iter>();
      }
    };

  // The specification of forward iterators.
  template<typename Iter>
    struct Forward_iterator_concept
    {
      static constexpr bool check()
      {
        return Forward_iterator_requirements<
          Iter, Input_iterator<Iter>()
        >::check();
      }
      
      static bool test()
      {
        // FIXME: Write semantics.
        return true;
      }
    };
    
  // Return true if Iter is a forward iterator.
  template<typename Iter>
    constexpr bool Forward_iterator()
    {
      return Forward_iterator_concept<Iter>::check();
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
    
    
    
  // Permutable and mutable iterators
  // There are two kinds of mutable iterators: Permutable iterators allow
  // values to be exchanged (moved). Mutable iterators allow values to be
  // replaced (copied). Note that mutable iterators are also permutable.
  // Permutable and mutable iterators are forward iterators.

    
  // Permutable Iterators
  // A permutable iterator allows values to be exchanged (moved) between
  // different iterators without copying. This also includes moving values
  // into temporary values.
    
  // A helper function for checking requirements.
  template<typename Iter, bool Prereqs>
    struct Permutable_iterator_requirements
    {
      static bool constexpr check() { return false; }
    };

  template<typename Iter>
    struct Permutable_iterator_requirements<Iter, true>
    {
      static bool constexpr check() 
      {
        return Move_writable<Iter, Value_type<Iter>>();
      }
    };

  // The specification of permutable iterators.
  template<typename Iter>
    struct Permutable_iterator_concept
    {
      static bool constexpr check()
      {
        return Permutable_iterator_requirements<
          Iter, Forward_iterator<Iter>()
        >::check();
      }
    };

  // Return true if Iter is permutable.
  template<typename Iter>
    constexpr bool Permutable_iterator()
    {
      return Permutable_iterator_concept<Iter>::check();
    }
    
    
    
  // Mutable Iterators
  // A mutable iterator allows its referenced values to be re-assigned to new
  // values (through copies). Note that mutable iterators are inherently 
  // permutable.
    
  // A helper function for checking requirements.
  template<typename Iter, bool Prereqs>
    struct Mutable_iterator_requirements
    {
      static bool constexpr check() { return false; }
    };

  template<typename Iter>
    struct Mutable_iterator_requirements<Iter, true>
    {
      static bool constexpr check() 
      {
        return Writable<Iter, Value_type<Iter>>();
      }
    };

  // The specification of mutable iterators.
  template<typename Iter>
    struct Mutable_iterator_concept
    {
      static bool constexpr check()
      {
        return Mutable_iterator_requirements<
          Iter, Permutable_iterator<Iter>()
        >::check();
      }
    };

  // Return true if Iter is mutable.
  template<typename Iter>
    constexpr bool Mutable_iterator()
    {
      return Mutable_iterator_concept<Iter>::check();
    }


    
  // A strict input iterator is at most an input iterator. That is I is not
  // a forward iterator. This is provided for convenience.
  template<typename Iter>
    constexpr bool Strict_input_iterator()
    {
      return Weak_input_iterator<Iter>() && !Forward_iterator<Iter>();
    }
    
  // FIXME: Write Strict_output_iterator.



  // Rearrangements
  //
  // FIXME: These aren't iterator concepts. These are algorithmic concepts.
  // They are specific to a families of algorithms. These concepts really
  // belong in their corresponding algorithm header.
  
  
  
  // Mergeable
  // The mergeable concept describes common requirements on the family of
  // (non-inplace) merge and set operations. There are two mergeable concepts:
  // - Mergeable<Iter1, Iter2, Out>
  // - Mergeable<Iter1, Iter2, Out, R>
  // The first requires the value types of Iter1 and Iter2 to be totally
  // ordered. The second is generalized over a relation.
  
  // This specialization implements the requirements for the generalized
  // version.
  template<typename Iter1, typename Iter2, typename Out, typename R>
    struct Mergeable_concept
    {
      static constexpr bool check()
      {
        return Input_iterator<Iter1>() && 
               Input_iterator<Iter2>() &&
               Weakly_incrementable<Out>() &&
               Writable<Out, Value_type<Iter1>>() &&
               Writable<Out, Value_type<Iter2>>() &&
               Relation<R, Value_type<Iter1>, Value_type<Iter2>>();
      }
    };

  // This specialization implements the requirements for algorithms that 
  // directly use the < operator.
  template<typename Iter1, typename Iter2, typename Out>
    struct Mergeable_concept<Iter1, Iter2, Out, default_t>
    {
      static constexpr bool check()
      {
        return Input_iterator<Iter1>() && 
               Input_iterator<Iter2>() &&
               Weakly_incrementable<Out>() &&
               Writable<Out, Value_type<Iter1>>() &&
               Writable<Out, Value_type<Iter2>>() &&
               Totally_ordered<Value_type<Iter1>, Value_type<Iter2>>();
      }
    };
    
  // Returns true when Iter1 and Iter2 can be merged into Out.
  template<typename Iter1, typename Iter2, typename Out, typename R = default_t>
    constexpr bool Mergeable()
    {
      return Mergeable_concept<Iter1, Iter2, Out, R>::check();
    }



  // Sortable
  // The Sortable concept describes the requiremnts of algorithms that permute
  // ranges sequences in some sorted order. There are two:
  // - Sortable<Iter>
  // - Sortable<Iter, R>
  // The first describes requirements for sorting the < operator. The second
  // is generalized over a Relation R.
  
  // Requrements for the generalized overload.
  template<typename Iter, typename R>
    struct Sortable_concept
    {
      static constexpr bool check()
      {
        return Permutable_iterator<Iter>() && Relation<R, Value_type<Iter>>();
      }
    };
    
  // Requirements using operator <.
  template<typename Iter>
    struct Sortable_concept<Iter, default_t>
    {
      static constexpr bool check()
      {
        return Permutable_iterator<Iter>() && Totally_ordered<Value_type<Iter>>();
      }
    };
    
  // Returns true if Iter is sortable.
  template<typename Iter, typename R = default_t>
    constexpr bool Sortable()
    {
      return Sortable_concept<Iter, R>::check();
    }
  
  
  
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
      static_assert(Move_writable<I, T>(), "");
      return is_weak_range(first, n);
    }
  
  // Returns true if the bounded range [first, last) is movable everywhere except
  // its limit.
  template<typename I, typename T>
    inline bool is_movable_range(I first, I last, T const& value) 
    {
      static_assert(Move_writable<I, T>(), "");
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
    inline void std_advance(Iter& i, Distance_type<Iter> n = 1)
    {
      static_assert(Weakly_incrementable<Iter>(), "");
      assert(( is_weak_range(i, n) ));

      std::advance(i, n);
    }
    


  // Next iterator
  // Return the nth iterator past i.
  template<typename Iter>
    inline Iter std_next(Iter i, Distance_type<Iter> n = 1)
    {
      static_assert(Weakly_incrementable<Iter>(), "");
      assert(( is_weak_range(i, n) ));
      
      return std::next(i, n);
    }
    
    
  // Previous iterator
  // Return the nth iterator before i.
  template<typename Iter>
    inline Iter std_prev(Iter i, Distance_type<Iter> n = 1)
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
    inline Distance_type<Iter> std_distance(Iter first, Iter last)
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
      assume(( is_reachable(first, last, adv) )); // FIXME: Not quite right.
      
      Distance_type<Iter> n = 0;
      while(first != last) {
        ++n;
        adv(first);
      }
      return n;
    }
 
 
   
  // Iterator actions
  // An action function is a void function that modifies one or more of its
  // arguments.
  
  // The increment action increments an object of type T.
  template<typename T>
    struct increment_action
    {
      void operator()(T& x) const
      {
        ++x;
      }
    };
  
  // The decrement action decrements an object of type T.
  template<typename T>
    struct decrement_action
    {
      void operator()(T& x) const
      {
        --x;
      }
    };
  
  // The advance action advances an Iter object n times.
  template<typename Iter>
    struct advance_action
    {
      void operator()(Iter& i, Distance_type<Iter> n)
      {
        std_advance(i, n);
      }
    };

                   
} // namespace origin

// Include default models
#include <origin/iterator/counter.hpp>


#endif
