// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_TRNSFORM_HPP
#define ORIGIN_ITERATOR_TRNSFORM_HPP

#include <origin/iterator/facade.hpp>

namespace origin
{
  template <typename I, typename F>
    class transform_iterator;


  // An alias to the iterator facade used by transform iterators. The reference
  // type is the result of apply f to *i.
  template <typename I, typename F>
    using Transform_iterator_facade
      = iterator_facade<
        transform_iterator<I, F>, I, Result_of<F(Iterator_reference<I>)>
      >;


  // A transform iterator applies a unary transform (function) to the
  // referenced elements of an underlying iterator.  Note that transformed 
  // references are assumed to be temporaries. As a result, pointer syntax is 
  // not supported for transform iterators.
  //
  // Transform iterators have the same traversal properties as their underlying
  // iterators. 
  template <typename I, typename F>
    class transform_iterator : public Transform_iterator_facade<I, F>
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Function<F, Value_type<I>>() ,"");
    public:
      using Base = Transform_iterator_facade<I, F>;
      using Ref = Result_of<F(Iterator_reference<I>)>;

      // The value type may be unrelated to the reference type.
      using value_type = Auto<Ref>;


      transform_iterator(I i, F f)
        : data{i, f}
      { }
      
      // Return the underlying iterator.
      I base() const { return iter(); }
      
      // Return the tranform function.
      F transform() const { return func(); }

      // Readable (no pointer syntax)
      Ref operator*() const { return func()(*iter()); }
      using Base::operator[];

      // Increment
      transform_iterator& operator++();
      using Base::operator++;
      
      // Decrement
      transform_iterator& operator--();
      using Base::operator--;

      // Advance
      transform_iterator& operator+=(Difference_type<I> n);
      transform_iterator& operator-=(Difference_type<I> n);
      
    private:
      I&       iter()       { return std::get<0>(data); }
      const I& iter() const { return std::get<0>(data); }
      
      const F& func() const { return std::get<1>(data); }
    
    private:
      std::tuple<I, F> data;
    };

  template <typename I, typename F>
    inline auto transform_iterator<I, F>::operator++() -> transform_iterator&
    { 
      ++iter(); 
      return *this; 
    }

  template <typename I, typename F>
    inline auto transform_iterator<I, F>::operator--() -> transform_iterator&
    { 
      --iter(); 
      return *this; 
    }

  template <typename I, typename F>
    inline auto transform_iterator<I, F>::operator+=(Difference_type<I> n) 
      -> transform_iterator&
    { 
      iter() += n; 
      return *this; 
    }
  
  template <typename I, typename F>  
    inline auto transform_iterator<I, F>::operator-=(Difference_type<I> n) 
      -> transform_iterator&
    { 
      iter() -= n; 
      return *this; 
    }

  
  // Equality_comparable
  // Two transform iterators compare equal when they refer to the same element
  // and their transform functions compare equal
  //
  // FIXME: For stateless transform functions, we can omit the computation, but
  // we can't really do that for stateful functions unless they are equality
  // comparable.
  template <typename I, typename F>
    inline bool operator==(const transform_iterator<I, F>& a, const transform_iterator<I, F>& b)
    {
      return a.base() == b.base(); 
    }
    
  template <typename I, typename F>
    inline bool operator!=(const transform_iterator<I, F>& a, const transform_iterator<I, F>& b)
    {
      return !(a == b);
    }
    
  
  // Totally_ordered
  // A transform iterator i compares less than a transform iterator j when
  // i.base() compares less than j.base.
  //
  // FIXME: Is this class totally ordered or is it weakly ordered because there
  // can be no ordering of functions?
  template <typename I, typename F>
    inline bool operator<(const transform_iterator<I, F>& a, const transform_iterator<I, F>& b) 
    { 
      return a.base() < b.base(); 
    }
    
  template <typename I, typename F>
    inline bool operator>(const transform_iterator<I, F>& a, const transform_iterator<I, F>& b) 
    { 
      return a.base() > b.base(); 
    }

  template <typename I, typename F>
    inline bool operator<=(const transform_iterator<I, F>& a, const transform_iterator<I, F>& b) 
    { 
      return a.base() <= b.base(); 
    }

  template <typename I, typename F>
    inline bool operator>=(const transform_iterator<I, F>& a, const transform_iterator<I, F>& b) 
    { 
      return a.base() >= b.base(); 
    }


  // Difference
  template <typename I, typename F>
    inline Difference_type<I> 
    operator-(const transform_iterator<I, F>& a, const transform_iterator<I, F>& b)
    {
      return a.iter() - b.iter();
    }



  // Make transform iterator
  //
  // Return a transform iterator.
  template <typename I, typename F>
    inline transform_iterator<I, F> make_transform_iterator(I i, F f)
    {
      return {i, f};
    }
    
} // namespace origin

#endif
