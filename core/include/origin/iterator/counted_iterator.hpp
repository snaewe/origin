// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_COUNTED_ITERATOR_HPP
#define ORIGIN_ITERATOR_COUNTED_ITERATOR_HPP

// NOTE: This file is included by iterator.
#include <origin/iterator.hpp>

namespace origin
{
  // A counted iterator is one that can be incremented a set number of times.
  // These are used to construct a bounded range from a weak iterator and a
  // distance value.
  //
  // A counted iterator is either an input iterator or forward iterator. It
  // does not support bidirectional or random access traversal. The read/write
  // properties are derived from the base iterator.
  template<typename I>
    class counted_iterator
    {
      static_assert(Iterator<I>(), "");
    public:
      using value_type        = Value_type<I>;
      using reference         = Iterator_reference<I>;
      using pointer           = Iterator_pointer<I>;
      using difference_type   = Distance_type<I>;
      using iterator_category = Iterator_category<I>;

      counted_iterator(I i, Distance_type<I> n)
        : iter{i}, count{n}
      {
        assert(( is_weak_range(iter, n) ));
      }

      // Returns the underlying iterator.
      reference base() const { return iter; }

      // Readable
      reference operator*() const { return *iter; }
      pointer operator->() const { return &*iter; }

      // Equality_comparable
      // Two counted iterators are equal only when they have the same count.
      // This is not a particularly strong notion of equality in that it it 
      // does not guarantee that *this == *x. However, when used in conjunction
      // with the counted_range adaptor, this property is assured.
      bool operator==(const counted_iterator& x) const { return count == x.count; }
      bool operator!=(const counted_iterator& x) const { return count != x.count; }

      // Increment
      counted_iterator& operator++() 
      {
        if(count != 0) {
          --count;
          ++iter; 
        }
        return *this; 
      }

      counted_iterator operator++(int)
      {
        counted_iterator tmp{*this};
        operator++();
        return tmp;
      }

    private:
      I iter;
      difference_type count;
    };

} // namespace origin

#endif
