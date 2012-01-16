// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_HPP
#define ORIGIN_RANGE_HPP

#include <origin/iterator.hpp>
#include <origin/iterator/counted_iterator.hpp>
#include <origin/iterator/counter.hpp>

namespace origin
{
  // The Range concept
  //
  // A range is simply a class that exposes a pair of iterators called begin(r)
  // and end(r). It is, in some senses, a very lightweight container.
  //
  // Note that for a range R, the following type aliases are available:
  //
  //    - Iterator_type<R>
  //    - Iterator_type<R const>
  //
  // By const-qualifying R, we can differentiate between const and non-const
  // iterators for the range. This is particularly helpful for containers.
  


  // Begin iterator
  // Ranges provide an operation, begin(r), that returns an iterator to the
  // first element in the range.

  // Safely get the type returned by std::begin(x).
  template<typename T>
    struct begin_result
    {
    private:
      template<typename X>
        static auto check(X&& x) -> decltype(std::begin(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // An alias to the result of the expression empty(x).
  template<typename T>
    using Begin_result = typename begin_result<T>::type;
    
  // Return true if empty(t) is a valid expression.
  template<typename T>
    bool constexpr Has_begin()
    {
      return Subst_succeeded<Begin_result<T>>();
    }
    
    
    
  // End iterator
  // Ranges provide an operation, end(r), that returns an iterator past the
  // last element in the range.

  // Safely get the type returned by std::end(x).
  template<typename T>
    struct end_result
    {
    private:
      template<typename X>
        static auto check(X&& x) -> decltype(std::end(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // An alias to the result of the expression empty(x).
  template<typename T>
    using End_result = typename end_result<T>::type;
    
  // Return true if empty(t) is a valid expression.
  template<typename T>
    bool constexpr Has_end()
    {
      return Subst_succeeded<End_result<T>>();
    }
    
    
    
  // Iterator type
  // Any type that exposes begin()/end() members has an associated iterator
  // type. The actual iterator type depends on the const-ness of the alias'
  // argument type. For example, a const-container will have a constant
  // iterator, while a non-const container will have a non-constant iterator.
  
  // An alias to the iterator type of a range. This is the same as the
  // result of the begin operation on the same type. Note that "const R" may
  // yield a different type than an unqualified "R".
  template<typename R>
    using Iterator_type = Begin_result<R>;
    

    
  // Range (concept)
  // A range is indicated by a pair of iterators called begin(r) and end(r).
    
  // The specification of the Range concept.
  template<typename R>
    struct Range_concept
    {
      static constexpr bool check()
      {
        return Has_begin<R>() 
            && Input_iterator<Begin_result<R>>()
            && Has_end<R>()
            && Input_iterator<End_result<R>>()
            && Same<Begin_result<R>, End_result<R>>();
      }

      static bool test(const R& r)
      {
        // A range encapsulates a (possibly empty) bounded range.
        return is_bounded_range(r.begin(), r.end());
      }
    };
  
  // Returns true if R is a range.
  template<typename R>
    constexpr bool Range()
    {
      return Range_concept<R>::check();
    }
    

    
  // Range concepts
    
  // NOTE: The meaning of saying "is fooable everywhere except its limit" is
  // analogous to asserting the corresponding property for all ranges r of 
  // some Range type R:
  //
  //   is_fooable_range(begin(r), end(r))
  //
  // as an invariant of the type.
    
  // Returns true if R is an input range. An input range is a range of input
  // iterators. An input range is readable everywhere except its limit.
  template<typename R>
    constexpr bool Input_range()
    {
      return Range<R>() && Readable<Iterator_type<R>>();
    }



  // Returns true if R is an output range. An output range is a range of 
  // writable iterators and is writable everywhere except its limit.
  template<typename R, typename T>
    constexpr bool Output_range()
    {
      return Range<R>() && Writable<Iterator_type<R>, T>();
    }


  
  // Returns true if R is a move range. A move range is a range of movable
  // iterators and is movable everywhere except its limit.
  //
  // FIXME: Is there a better name for this?
  template<typename R, typename T>
    constexpr bool Move_range()
    {
      return Range<R>() && Move_writable<Iterator_type<R>, T>();
    }


    
  // Returns true if R is a permutable range. A permutable range is permutable
  // everywhere except its limit.
  template<typename R>
    constexpr bool Permutable_range()
    {
      return Range<R>() && Permutable_iterator<Iterator_type<R>>();
    }



  // Returns true if R is a mutable range.
  template<typename R>
    constexpr bool Mutable_range()
    {
      return Range<R>() && Mutable_iterator<Iterator_type<R>>();
    }


    
  // Returns true if R is a forward range. A forward range is a range whose 
  // iterator type is a forward iterator.
  template<typename R>
    constexpr bool Forward_range()
    {
      return Range<R>() && Forward_iterator<Iterator_type<R>>();
    }


  
  // Returns true if R is a bidirectional range.
  template<typename R>
    constexpr bool Bidirectional_range()
    {
      return Range<R>() && Bidirectional_iterator<Iterator_type<R>>();
    }


    
  // Returns true if R is a random access range.
  template<typename R>
    constexpr bool Random_access_range()
    {
      return Range<R>() && Random_access_iterator<Iterator_type<R>>();
    }
  
  
  
  // Sortable range
  // A sortable range is a permutable range whose values are either totally
  // ordered or weakly ordered by some relation. 
  
  // Requirements for a sortable range.
  template<typename Rng, typename R>
    struct Sortable_range_concept
    {
      static constexpr bool check()
      {
        return Forward_range<Rng>() 
            && Permutable_range<Rng>() 
            && Relation<R, Value_type<Rng>>();
      }
    };
  
  // Totally ordered.
  template<typename Rng>
    struct Sortable_range_concept<Rng, default_t>
    {
      static constexpr bool check()
      {
        return Forward_range<Rng>()
            && Permutable_range<Rng>()
            && Totally_ordered<Value_type<Rng>>();
      }
    };
  
  // Returns true if Rng is sortable (with respect to the relation R).
  template<typename Rng, typename R = default_t>
    constexpr bool Sortable_range()
    {
      return Sortable_range_concept<Rng, R>::check();
    }
    
    

  // The following type traits and type predicates establish the notion of
  // sized type, some type for which size(x) is valid query. Size is defined
  // in 3 ways:
  //    - For user-defined containers, it is x.size().
  //    - For ranges, it is distance(begin(x), end(x))
  //    - For statically sized arrays of type T[N], it is N.


  
  // Size (member)
  // Return the size of x. The meaning of size is dependent on the type of
  // T, and the operation generally returns an unsigned integral type.
  //
  // NOTE: Some containers may have constexpr size (e.g., array).
  template<typename T>
    constexpr auto size(T const& x) -> decltype(x.size())
    {
      return x.size();
    }



  // Size (array)
  // Specialize size for statically sized arrays.
  template<typename T, std::size_t N>
    constexpr std::size_t size(T(&)[N])
    {
      return N;
    }
    
    
  
  // Size (range)
  // Specialize for size for Ranges that are not containers.
  //
  // FIXME: This should be available in the range module.
  template<typename R>
    inline auto size(R const& r)
      -> Requires<Range<R>() && !Has_member_size<R>(), Make_unsigned<Distance_type<R>>>
    {
      return std::distance(std::begin(r), std::end(r));
    }
  
  
  
  // Safely get the result type of the expression size(r).
  template<typename T>
    struct size_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(size(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
  
  // An alias for the result of the size(t) expression. Every type for which
  // size(t) is valid has an associated size type. This includes ranges,
  // containers, matrices, and graphs.
  template<typename T>
    using Size_type = typename size_result<T>::type;

  // Returns true if size(t) is a valid expression.
  template<typename T>
    constexpr bool Has_size()
    {
      return Subst_succeeded<Size_type<T>>();
    }


    
  // Empty
  // A type may support empty queries as in empty(x). Like size(), empty is
  // defined in 3 ways:
  //    - For containers, it is x.empty().
  //    - For ranges, it is begin(x) == end(x)
  //    - For statically sized arrays of type T[N], it is N == 0.
  

  
  // Empty (member)
  // Return true if x is empty. The meaning of empty is dependent on the type
  // of T.
  //
  // NOTE: Some containers may have constexpr empty (e.g., arrays).
  template<typename T>
    constexpr auto empty(T const& x) -> decltype(x.empty())
    {
      return x.empty();
    }



  // Empty (array)
  // Specialization for statically sized arrays.
  template<typename T, std::size_t N>
    constexpr bool empty(T(&a)[N])
    {
      return N == 0;
    }



  // Empty (range)
  // Specialization for ranges, which don't have a member empty.
  template<typename R>
    inline Requires<!Has_member_empty<R>(), bool> empty(R const& r)
    {
      return std::begin(r) == std::end(r);
    }



  // Safely get the result type of the expression empty(r).
  template<typename T>
    struct empty_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(empty(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
  
  // An alias for the result of the empty(t) expression.
  template<typename T>
    using Empty_result = typename empty_result<T>::type;

  // Returns true if empty(t) is a valid expression.
  template<typename T>
    constexpr bool Has_empty()
    {
      return Subst_succeeded<Empty_result<T>>();
    }



  // Range adaptors
  // A range adaptor is a model of a range. These typically wrap other data
  // structures or data elements in such a way that they can be iterated over.
  //
  // Note that a range adaptor must declare its value type and difference type.
  // The latter is required so that Distance_type<R> is a valid expression for
  // ranges (as is Size_type<R>). It is not strictly required that a Range
  // declare a nested iterator name; that is uniquely deduced from the begin()
  // and end() operations.
  
  
  
  // Array Range
  // Wraps an array reference with static bounds and guarantees that it will 
  // behave like an array. This is useful to prevent array types from decaying
  // into pointers. 
  template<typename T, std::size_t N>
    struct array_range
    {
      using value_type = Remove_cv<T>;
      using difference_type = std::ptrdiff_t;
      
      array_range(T(&a)[N]) : array(a) { }
      
      T* begin() const { return array; }
      T* end()   const { return array + N; }

      T(&array)[N];
    };

  // Return a wrapper around an array that makes it behave like a range.
  // This can be used to disambiguate overloads for functions that take arrays
  // as both ranges and pointers (through decay).
  template<typename T, std::size_t N>
    inline array_range<T, N> arr(T(&a)[N])
    {
      return {a};
    }

  // A constant version of the function above.
  template<typename T, std::size_t N>
    inline array_range<T const, N> arr(T const(&a)[N])
    {
      return {a};
    }



  // Counted range
  // A counted range adapts an iterator, distance pair into a bounded range.
  //
  // Note that the semantics of counted ranges differ from the EoP notion. In
  // Origin, a counted range has the same semantics as a weak range in EoP. The
  // acyclic property of EoP counted is considered separately in Origin.
  //
  // FIXME: Finish writing this class.
  template<typename I>
    class counted_range
    {    
    public:
      using value_type = Value_type<I>;
      using difference_type = Distance_type<I>;
      using iterator = counted_iterator<I>;
      
      counted_range(I first, Distance_type<I> n)
        : first{first}, count{n}
      { }

      // Returns the underlying iterator
      I base() const { return first; }
      
      // Returns the number of times the iterator can be incremeted.
      Distance_type<I> distance() const { return count; }
      
      // Range
      iterator begin() const { return {first, count}; }
      iterator end() const { return {first, 0}; }
    
    private:
      I first;
      Distance_type<I> count;
    };


  
  // Bounded range
  // A bounded range encapsulates a pair of iterator and has the 
  // is_bounded_range precondition as an invariant.
  //
  // wraps a pair of iterators. This is essentially the same as the Boost 
  // iterator_range, or pair<I, I> with appropriate overloads.
  //
  // Invariants: 
  //    bounded_range(this->begin(), this->end());
  template<typename I>
    class bounded_range
    {
      static_assert(Weakly_incrementable<I>(), "");
      static_assert(Equality_comparable<I>(), "");
    public:
      using value_type = Value_type<I>;
      using difference_type = Distance_type<I>;

      // Initialize the bounded range so that both values are the same. The
      // range is initially empty.
      bounded_range() : first(), last(first) { }
    
      // Initialize the bounded range over [first, last).
      bounded_range(I first, I last)
        : first(first), last(last)
      { 
        assert(( is_bounded_range(first, last) ));
      }
      
      // Range
      I begin() const { return first; }
      I end() const { return last; }
      
    private:
      I first;
      I last;
    };



  // Iterator range
  // An iterator range defines a bounded range over a set of iterators. This is
  // to say that the elements of an iterator range are iterators. The range is
  // parameterized over the underlying iterator type and an action that 
  // describes how the range is iterated (increment by default).
  template<typename I, typename Act = increment_action<I>>
    class iterator_range
    {
      static_assert(Weakly_incrementable<I>(), "");
      static_assert(Equality_comparable<I>(), "");
      static_assert(Function<Act, I&>(), "");

    public:
      using iterator = counter<I, Act>;
      using value_type = Value_type<iterator>;
      using difference_type = Distance_type<iterator>;

      iterator_range(I first, I last)
        : first{first}, last{last}
      {
        assert(( is_bounded_range(first, last) ));
      }
    
      iterator begin() const { return first; }
      iterator end()   const { return last; }

    private:
      I first;
      I last;
    };



  // Iter range
  // Returns a bounded range over the iterators in the range [first, last).
  // For example:
  //
  //    vector<int> v = {1, 2, 3};
  //    for(auto i : range_over(v)) cout << *i << ' ';
  //
  // prints "1 2 3". Each element in the range over v is vector iterator and
  // not an element in v.
  
  
  
  // Iter range
  // Returns an iterator range over the iterators in [first, last).
  template<typename I>
    inline iterator_range<I> iter_range(I first, I last)
    {
      assert(( is_bounded_range(first, last) ));

      return {first, last};
    }



  // Iter range (range)
  template<typename R>
    inline iterator_range<Iterator_type<R>> iter_range(R& range)
    {
      return {std::begin(range), std::end(range)};
    }



  // Iter range (const range)
  template<typename R>
    inline iterator_range<Iterator_type<const R>> iter_range(const R& range)
    {
      return {std::begin(range), std::end(range)};
    }



  // Range
  // Returns a (right) half-open range [first, last) over the elements in that
  // range. For example:
  //
  //    for(auto i : range(0, 5)) count << i << ' ';
  //
  // prints "0 1 2 3 4". Similarly:
  //
  //    vector<int> v = {1, 2, 3}
  //    for(auto i : range(v.begin(), v.end())) cout << i << ' ';
  //
  // prints "1 2 3".
  //
  // This function tries to be clever about its uses. If the type of the first
  // and last arguments are integer types, then the result will range over the
  // integer values in that set. If the type is an iterator, then the result 
  // ranges over the elements in the underlying range.
  
  
  
  // Range (integral)
  template<typename I>
    inline Requires<Integral<I>(), iterator_range<I>> range(I first, I last)
    {
      assert(( is_bounded_range(first, last) ));

      return {first, last};
    }

  
  // Range (iterator)
  template<typename I>
    inline Requires<Iterator<I>(), bounded_range<I>> range(I first, I last)
    {
      assert(( is_bounded_range(first, last) ));
      
      return {first, last};
    }



  // Closed range
  // Return a closed range over the elements of [first, last]. The behavor
  // of this constructor is analogous to that of the range function. It tries
  // to select an appropriate range based on the argument types provided.
  
  
  
  // Closed range (integral)
  template<typename I>
    inline Requires<Integral<I>(), iterator_range<I>> closed_range(I first, I last)
    {
      assume(( is_bounded_range(first, std_next(last)) ));
      return {first, ++last};
    }



  // Closed range (iterator)
  template<typename I>
    inline Requires<Iterator<I>(), bounded_range<I>> closed_range(I first, I last)
    {
      assume(( is_bounded_range(first, std_next(last)) ));
      return {first, ++last};
    }
    
} // namespace origin

#endif
