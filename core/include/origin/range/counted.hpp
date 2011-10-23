// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_COUNTED_HPP
#define ORIGIN_RANGE_COUNTED_HPP

#include <origin/iterator/range.hpp>

namespace origin
{
  // FIXME: Finish implementing this. If Iter is equality comparable, then
  // this will essentially be a bounded range. Otherise, we have to have a
  // specialized iterator that compares by count and not position.
  //
  // Really, though... what iterators are not actually equality comparable?

  // A counted range is defined by an iterator and an associated count. The
  // iterator can be incremented at most n times.
  template<typename Iter>
    class counted_range
    {
      typedef typename std::iterator_traits<Iter>::difference_type difference_type;
    public:
      counted_range(Iter first, difference_type n)
        : first(first), count(n)
      { }
      
      // Iter begin() const { return first; }
      // Iter end() const { return last; }
      
    private:
      Iter first;
      difference_type count;
    };

} // namespace origin

#endif
