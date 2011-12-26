// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_RANGE_HPP
#define ORIGIN_ITERATOR_RANGE_HPP

#include <origin/iterator.hpp>

namespace origin
{
  // Deduce the iterator category. If I is an iterator, then use the known
  // iterator category. Otherwise if I is integral, the category is
  // a random access iterator.
  /*
  template<typename I>
    using Range_iterator_category = 
      If<Iterator<I>, 
         Iterator_category<I>, 
         If<Integral<I>, std::random_access_iterator_tag, subst_failure>
      >;
  */
  
  // A range iterator adapts any incrementable type into an iterator.
  // Advancing the iterator causes the wrapped object to be advanced. 
  // Dereferencing a range iterator returns (a const reference to the) wrapped 
  // object. 
  //
  // A range iterator has the same traversal properties as its underlying
  // type.
  template<typename Iter>
    class range_iterator
    {
      static_assert(Weakly_incrementable<Iter>(), "");
    public:
      using value_type = Iter;
      using reference = Iter const&;
      using pointer = Iter const*;
      using difference_type = Distance_type<Iter>;
      using iterator_category = Iterator_category<Iter>;
      
    
      // Initialize...
      range_iterator(Iter i) : iter(i) { }
      
      // Equality_comparable
      bool operator==(range_iterator const& x) const { return iter == x.iter; }
      bool operator!=(range_iterator const& x) const { return iter != x.iter; }
      
      // Readable
      reference operator*() const { return iter; }
      pointer operator->() const { return &iter; }
      
      // Incrementable
      range_iterator& operator++() { ++iter; return *this; }
      range_iterator  operator++(int) { range_iterator x(*this); ++*this; return x; }
      
      // Decrementable (if supported)
      range_iterator& operator--() { --iter; return *this; }
      range_iterator  operator--(int) { range_iterator x(*this); ++*this; return x; }
      
      // Advance (if supported)
      range_iterator& operator+=(difference_type n) { iter += n; return *this; }
      range_iterator& operator-=(difference_type n) { iter -= n; return *this; }
      
      friend range_iterator operator+(range_iterator x, difference_type n)
      {
        x += n;
        return x;
      }

      friend range_iterator operator+(difference_type n, range_iterator x)
      {
        x += n;
        return x;
      }

      friend range_iterator operator-(range_iterator x, difference_type n)
      {
        x -= n;
        return x;
      }
      
      // Distance (if supported)
      friend difference_type operator-(range_iterator const& a, range_iterator const& b)
      {
        return a - b;
      }
      
      // Subscript
      reference operator[](difference_type n) const { return *(iter + n); }

    private:
      Iter iter;
    };

} // namespace origin

#endif
