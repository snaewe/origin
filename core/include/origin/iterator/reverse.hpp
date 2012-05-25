// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_REVERSE_HPP
#define ORIGIN_ITERATOR_REVERSE_HPP

#include <origin/iterator/facade.hpp>

namespace origin
{
  template <typename I> class reverse_iterator;


  // An alias to the facade used by the filter iterator.
  template <typename I>
    using Reverse_iterator_facade
      = iterator_facade<reverse_iterator<I>, I>;


  // Reverse iterator
  //
  // The reverse iterator adaptor reverse the direction of increment and
  // decrement operators in the underlying iterator.
  template <typename I>
    class reverse_iterator
      : public Reverse_iterator_facade<I>
    {
      static_assert(Bidirectional_iterator<I>(), "");

      using Base = Reverse_iterator_facade<I>;
    public:
      reverse_iterator(I i) : iter{i} { }

      // Iterator properties
      I base() const { return iter; }
    
      // Readable
      Iterator_reference<I> operator*() const;
      using Base::operator[];
      
      // Increment
      reverse_iterator& operator++();
      using Base::operator++;

      // Decremment
      reverse_iterator& operator--();
      using Base::operator--;
      
      // Advance
      reverse_iterator& operator+=(Difference_type<I> n);
      reverse_iterator& operator-=(Difference_type<I> n);

    private:
      I iter;
    };

  template <typename I>
    inline Iterator_reference<I> reverse_iterator<I>::operator*() const
    {
      return *prev(iter);
    }

  template <typename I>  
    inline reverse_iterator<I>& reverse_iterator<I>::operator++()
    { 
      --iter;
      return *this;
    }

  template <typename I>
    inline reverse_iterator<I>& reverse_iterator<I>::operator--()
    {
      ++iter;
      return *this;
    }

  template <typename I>  
    inline reverse_iterator<I>& 
    reverse_iterator<I>::operator+=(Difference_type<I> n)
    { 
      iter -= n;
      return *this;
    }

  template <typename I>
    inline reverse_iterator<I>& 
    reverse_iterator<I>::operator-=(Difference_type<I> n)
    {
      iter += n;
      return *this;
    }

  template <typename I>
    inline Difference_type<I> 
    operator-(const reverse_iterator<I>& a,const reverse_iterator<I>& b)
    {
      return b.base() - a.base();
    }


  // Equality comparable
  template <typename I>
    inline bool 
    operator==(const reverse_iterator<I>& a, const reverse_iterator<I>& b)
    {
      return a.base() == b.base();
    }

  template <typename I>
    inline bool 
    operator!=(const reverse_iterator<I>& a, const reverse_iterator<I>& b)
    {
      return !(a == b);
    }


  // Totally ordered
  template <typename I>
    inline bool 
    operator<(const reverse_iterator<I>& a, const reverse_iterator<I>& b)
    {
      return b.base() < a.base();
    }

  template <typename I>
    inline bool 
    operator>(const reverse_iterator<I>& a, const reverse_iterator<I>& b)
    {
      return a.base() < b.base();
    }

  template <typename I>
    inline bool 
    operator<=(const reverse_iterator<I>& a, const reverse_iterator<I>& b)
    {
      return !(a.base() < b.base());
    }

  template <typename I>
    inline bool 
    operator>=(const reverse_iterator<I>& a, const reverse_iterator<I>& b)
    {
      return !(b.base() < a.base());
    }



  // Make reverse iterator
  //
  // Returns a reverse iterator that wraps iter, reversing its direction of
  // traversal.
  template <typename I>
    inline auto reversed(I iter)
      -> Requires<Bidirectional_iterator<I>(), reverse_iterator<I>>
    {
      return {iter};
    }

} // namespace origin

#endif
