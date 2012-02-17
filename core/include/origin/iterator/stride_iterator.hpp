// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_STRIDE_ITERATOR_HPP
#define ORIGIN_ITERATOR_STRIDE_ITERATOR_HPP

#include <origin/iterator.hpp>

namespace origin
{
  // The stride iterator adaptor advances and element some number of times
  // during a single increment. Note that the actual advance operation is
  // parameterized by the Act action.
  //
  // The stride iterator has the same traversal and read/write properties as
  // its underlying iterator.
  //
  // The Act parameter is a unary action Function that advances the underlying
  // iterator. Note that stride iterator increments are unchecked. If the action 
  // is to advance by some number > 1, then it is possible to overrun the limit 
  // of the range.
  template<typename I, typename Act = advance_action<I>>
    class stride_iterator
    {
      static_assert(Iterator<I>(), "");
      static_assert(Advance_action<Act, I&>(), "");
    public:
      using value_type = Value_type<I>;
      using reference = Iterator_reference<I>;
      using pointer = Iterator_pointer<I>;
      using difference_type = Distance_type<I>;
      using iterator_category = Iterator_category<I>;

      stride_iterator(I i, Act adv)
        : data{i, adv}
      { }
      
      // Returns the underlying iterator
      I base() const { return iter(); }
      
      // Returns the advance action.
      Act advance_act() const { return adv(); }
      
      // Returns the stride of the iterator.
      constexpr difference_type stride() const { return get_increment(adv()); }
      
      // Readable
      reference operator*() const { return *iter(); }
      pointer operator->() const { return &*iter(); }

      // Equality_comparable
      // Two stride iterators are equal if they point to the same element.
      bool operator==(const stride_iterator& x) const { return iter() == x.iter(); }
      bool operator!=(const stride_iterator& x) const { return iter() != x.iter(); }
      
      // Totally_ordered
      bool operator<(const stride_iterator& x) const { return iter() < x.iter(); }
      bool operator>(const stride_iterator& x) const { return iter() > x.iter(); }
      bool operator<=(const stride_iterator& x) const { return iter() <= x.iter(); }
      bool operator>=(const stride_iterator& x) const { return iter() >= x.iter(); }

      // Incrementable
      stride_iterator& operator++() { o_advance(iter(), stride()); return *this; }
      
      stride_iterator operator++(int) 
      {
        stride_iterator tmp{*this};
        operator++();
        return tmp;
      }

      // Decrement
      stride_iterator& operator--() { o_advance(iter(), -stride()); return *this; }
      
      stride_iterator operator--(int) 
      {
        stride_iterator tmp{*this};
        operator--();
        return tmp;
      }
     
     // Advance
     stride_iterator& operator+=(difference_type n) { iter() += stride() * n; return *this; }
     stride_iterator& operator-=(difference_type n) { iter() -= stride() * n; return *this; }
     
     friend stride_iterator operator+(stride_iterator i, difference_type n)
     {
       return i += n;
     }
     
     friend stride_iterator operator+(difference_type n, stride_iterator i)
     {
       return i += n;
     }
     
     friend stride_iterator operator-(stride_iterator i, difference_type n)
     {
       return i -= n;
     }

    // Difference
    friend difference_type operator-(stride_iterator i, stride_iterator j)
    {
      assert(( i.stride() == j.stride() ));
      return (i.iter() - j.iter()) / stride;
    }

    private:
      I&       iter()       { return std::get<0>(data); }
      const I& iter() const { return std::get<0>(data); }
      
      const Act& adv() const { return std::get<1>(data); }
      
    private:
      std::tuple<I, Act> data;
    };


    
  // Stride iter
  // Construct a stride iterator with some predetermined stride or step.
  //
  // FIXME: These should be called stride(). There are sufficient requirements
  // to disambiguate these overloads from the range overloads, but GCC is
  // failing with an ICE when trying to reject stride<N>(i) for ranges.
    
  // Returns a stride iterator over i with a step of n.
  template<typename I>
    inline auto stride_iter(I i, Distance_type<I> n)
      -> Requires<Iterator<I>(), stride_iterator<I, advance_action<I>>>
    {
      return {i, advance_action<I>{n}};
    }

  // Returns a stride iterator over i with a step of N (with N an explicit 
  // template argument).
  template<std::ptrdiff_t N, typename I>
    inline auto stride_iter(I i) 
      -> Requires<Iterator<I>(), stride_iterator<I, static_advance_action<I, N>>>
    {
      return {i, static_advance_action<I, N>{}};
    }

} // namespace origin

#endif
