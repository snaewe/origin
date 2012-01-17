// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_TRNSFORM_ITERATOR_HPP
#define ORIGIN_ITERATOR_TRNSFORM_ITERATOR_HPP

#include <origin/iterator.hpp>
#include <origin/algorithm.hpp>

namespace origin
{
  // A transform iterator applies a unary transform (function) to the
  // referenced elements of an underlying iterator.  Note that transformed 
  // references are assumed to be temporaries. As a result, pointer syntax is 
  // not supported for transform iterators.
  //
  // Transform iterators have the same traversal properties as their underlying
  // iterators. 
  template<typename I, typename F>
    class transform_iterator
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Function<F, Value_type<I>>() ,"");

      using result_type = decltype(std::declval<F>()(*std::declval<I>()));

    public:
      using value_type = Decay<result_type>;
      using reference = result_type;
      using pointer = void;
      using difference_type = Distance_type<I>;
      using iterator_category = Iterator_category<I>;

      transform_iterator(I i, F f)
        : data{i, f}
      { }
      
      // Return the underlying iterator.
      I base() const { return iter(); }
      
      // Return the tranform function.
      F transform() const { return func(); }

      // Readable (no pointer syntax)
      reference operator*() const { return func()(*iter()); }

      // Equality_comparable
      bool operator==(const transform_iterator& x) const { return iter() == x.iter(); }
      bool operator!=(const transform_iterator& x) const { return iter() != x.iter(); }
      
      // Totally_ordered (if supported)
      bool operator<(const transform_iterator& x) const  { return iter() < x.iter(); }
      bool operator>(const transform_iterator& x) const  { return iter() > x.iter(); }
      bool operator<=(const transform_iterator& x) const { return iter() <= x.iter(); }
      bool operator>=(const transform_iterator& x) const { return iter() >= x.iter(); }

      // Increment
      transform_iterator& operator++() { ++iter(); return *this; }
      
      transform_iterator operator++(int) 
      { 
        transform_iterator tmp{*this}; 
        operator++(); 
        return *this; 
      }
      
      // Decrement
      transform_iterator& operator--() { --iter(); return *this; }
      
      transform_iterator operator--(int) 
      { 
        transform_iterator tmp{*this}; 
        operator--(); 
        return *this; 
      }

      // Advance
      transform_iterator& operator+=(difference_type n) { iter() += n; return *this; }
      transform_iterator& operator-=(difference_type n) { iter() -= n; return *this; }
      
      friend transform_iterator operator+(transform_iterator x, difference_type n)
      {
        x += n;
        return x;
      }

      friend transform_iterator operator+(difference_type n, transform_iterator x)
      {
        x += n;
        return x;
      }

      friend transform_iterator operator-(transform_iterator x, difference_type n)
      {
        x -= n;
        return x;
      }

      // Difference
      friend difference_type operator-(transform_iterator a, transform_iterator b)
      {
        return a.iter() - b.iter();
      }

    private:
      I&       iter()       { return std::get<0>(data); }
      const I& iter() const { return std::get<0>(data); }
      
      const F& func() const { return std::get<1>(data); }
    
    private:
      std::tuple<I, F> data;
    };
  
  
  // Return a transform iterator.
  template<typename I, typename F>
    inline transform_iterator<I, F> transform_iter(I i, F f)
    {
      return {i, f};
    }
    
} // namespace origin

#endif
