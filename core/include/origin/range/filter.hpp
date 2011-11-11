// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_FILTER_HPP
#define ORIGIN_RANGE_FILTER_HPP

#include <origin/algorithm.hpp>
#include <origin/iterator.hpp>
#include <origin/range/traits.hpp>

namespace origin
{
  // A helper iterator for the filter_range.
  // This is a forward iterator.
  template<typename R, typename Iter>
    class filter_range_iterator
    {
    public:
      using value_type = Value_type<Iter>;
      using reference = Iterator_reference<Iter>;
      using pointer = Iterator_pointer<Iter>;
      using difference_type = Distance_type<Iter>;
      using iterator_category = Clamp_iterator_category<Iter, std::forward_iterator_tag>;
    
      filter_range_iterator(R const& r, Iter i)
        : range(const_cast<R&>(r)), iter(i)
      { }
      
      // Equality_comparable
      bool operator==(filter_range_iterator const& x) const { return iter == x.iter; }
      bool operator!=(filter_range_iterator const& x) const { return iter != x.iter; }
      
      // Readable
      reference operator*()  const { return *iter; }
      pointer   operator->() const { return &*iter; }
      
      // Incrementable
      filter_range_iterator& operator++()
      {
        iter = next_if(iter, std::end(range.range), range.pred);
        return *this;
      }
      
      filter_range_iterator operator++(int)
      {
        filter_range_iterator tmp(*this);
        operator++();
        return tmp;
      }
      
      private:
        R& range;
        Iter iter;
    };

  // A filter range...
  //
  // Note that R may be const. 
  //
  // NOTE: We assume R caches its begin and end iterators. If the cost of
  // calling end on R is expensive, this will be inefficient. Even if end
  // computed, this could also be expensive.
  template<typename R, typename Pred>
    class filter_range
    {
      typedef filter_range<R, Pred> this_type;
      typedef typename range_traits<R>::iterator base_iterator;
    public:
      typedef filter_range_iterator<this_type, base_iterator> iterator;
      
      // Construct a filter range over the underlying range. The predicate
      // may be omitted if Pred is Default_constructible.
      filter_range(R& r, Pred pred = Pred{})
        : range(r), pred(pred)
      { }

      iterator begin() const
      {
        return iterator(*this, first_if(std::begin(range), std::end(range), pred));
      }

      iterator end() const
      {
        return {*this, std::end(range)};
      }
      
      // Range properties
      R const& base() const { return range; }
      Pred predicate() const { return pred; }
      
    private:
      R& range;
      Pred pred;
      friend iterator;
    };
    
  // Return an adapted filter over the given range.
  template<typename R, typename Pred>
    inline typename std::enable_if<
      is_range<R>::value, filter_range<R, Pred>
    >::type
    filter(R& range, Pred pred)
    {
      return {range, pred};
    }

  // A constant version of the function above.
  template<typename R, typename Pred>
    inline typename std::enable_if<
      is_range<R>::value, filter_range<R const, Pred>
    >::type
    filter(R const& range, Pred pred)
    {
      return {range, pred};
    }

} // namespace origin

#endif
