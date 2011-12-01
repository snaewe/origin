// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_HPP
#define ORIGIN_RANGE_HPP

#include <origin/iterator.hpp>
#include <origin/iterator/range.hpp>

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
  // Ranges and containers have begin and end operations that return iterators.

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
    
  // Range concept
    
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
      
      // FIXME Write semantics.
    };
  
  // Returns true if R is a range.
  template<typename R>
    constexpr bool Range()
    {
      return Range_concept<R>::check();
    }

    
    
  // Range adaptors
  
  // Array Range
  // Wraps a C array with static bounds and guarantees that it will behave like
  // an array.
  template<typename T, std::size_t N>
    struct array_range
    {
      using value_type = Remove_cv<T>;
      
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


  
  // Bounded_range
  // A bounded range wraps a pair of iterators. This is essentially the same
  // as the Boost iterator_range, or pair<Iter, Iter> with appropriate
  // overloads.
  //
  // requires: Weakly_incrementable<Iter> && Equality_comparable<Iter>
  // invariant: bounded_range(this->first, this->last);
  template<typename Iter>
    class bounded_range
    {
      static_assert(Weakly_incrementable<Iter>(), "");
      static_assert(Equality_comparable<Iter>(), "");
    public:
      using value_type = Value_type<Iter>;
      using iterator = Iter;

      // Initialize the bounded range so that both values are the same. The
      // range is initially empty.
      bounded_range() : first(), last(first) { }
    
      // Initialize the bounded range.
      //
      // precondition: bounded_range(first, last)
      // postcondition: this->begin() == first && this->end() == last
      bounded_range(iterator first, iterator last)
        : first(first), last(last)
      { }
      
      iterator begin() const { return first; }
      iterator end() const { return last; }
      
    private:
      iterator first;
      iterator last;
    };



  // Wrapped Bounded Range
  // The adapted range class defines a (right) half-open range over some
  // incrementable type, Iter. Each element in the range is an an object in that
  // underlying sequence.
  //
  // requires: Weakly_incrementable<Iter>
  template<typename Iter>
    class wrapped_bounded_range
    {
      static_assert(Weakly_incrementable<Iter>(), "");
      static_assert(Equality_comparable<Iter>(), "");
    public:
      using iterator = range_iterator<Iter>;
      using value_type = Value_type<iterator>;

      wrapped_bounded_range(Iter first, Iter last)
        : first(first), last(last)
      { }
    
      iterator begin() const { return first; }
      iterator end()   const { return last; }

    private:
      Iter first;
      Iter last;
    };

  // Return a (right) half-open range [first, last) over the elements in that
  // range. For example:
  //
  //    for(auto i : range(0, 5)) count << *i << ' ';
  //
  // prints "0 1 2 3 4". Similarly:
  //
  //    vector<int> v = {1, 2, 3}
  //    for(auto i : range(v.begin(), v.end())) cout << **i << ' ';
  //
  // prints "1 2 3". Because the arguments to range are iterators, each value
  // of i is also an iterator (hence the need to write **i).
  //
  // requires: Incrementable<Iter>
  // precondition: bounded_range(first, last);
  template<typename Iter>
    wrapped_bounded_range<Iter> range(Iter first, Iter last)
    {
      return {first, last};
    }
    
  // Return a closed range [first, last] over the incrementable values first
  // and last.
  //
  // requires: Incrementable<Iter>
  // precondition: bounded_range(first, last + 1);
  template<typename Iter>
    wrapped_bounded_range<Iter> closed_range(Iter first, Iter last)
    {
      return {first, std::next(last)};
    }
    
} // namespace origin

// Include range constructors
// #include <origin/range/filter.hpp>
// #include <origin/range/permutation.hpp>

#endif
