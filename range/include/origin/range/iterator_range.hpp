// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_ITERATOR_RANGE_HPP
#define ORIGIN_RANGE_ITERATOR_RANGE_HPP

#include <type_traits>
#include <iterator>

namespace origin
{
  /**
   * @ingroup range
   * 
   * The range class template encapsulates a pair of iterators denoting a
   * bounded range.
   *
   * @tparam Iter An Iterator type.
   */
  template<typename Iter>
    class iterator_range
    {
    public:
      typedef Iter iterator;

      iterator_range(iterator b = iterator{}, iterator e = iterator{})
        : begin_{b}, end_{e}
      { }

      iterator begin() const { return begin_; }
      iterator end() const { return end_; }

    private:
      iterator begin_;
      iterator end_;
    };
  
  template<typename Iter>
    auto begin(iterator_range<Iter>& r) -> decltype(r.begin())
    {
      return r.begin();
    }
    
  template<typename Iter>
    auto end(iterator_range<Iter>& r) -> decltype(r.end())
    {
      return r.end();
    }

} // namespace origin

#endif
