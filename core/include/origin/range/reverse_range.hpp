// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_REVERSE_RANGE_HPP
#define ORIGIN_RANGE_REVERSE_RANGE_HPP

#include <origin/range.hpp>

namespace origin
{
  // A reverse range is a bounded range adaptor that traverses the elements in
  // reverse order.
  template<typename I>
    class reverse_range
    {
      static_assert(Bidirectional_iterator<I>(), "");
    public:
      using value_type = Value_type<I>;
      using difference_type = Distance_type<I>;
      using iterator = std::reverse_iterator<I>;
      
      reverse_range(I first, I last)
        : first{first}, last{last}
      { }

      iterator begin() const { return iterator{last}; }
      iterator end() const   { return iterator{first}; }
      
    private:
      I first;
      I last;
    };
    
    
  // Reversed
  // The reversed function constructs a reverse range from its range arguments.

  template<typename I>
    inline reverse_range<I> reversed(I first, I last)
    {
      return {first, last};
    }
    
  template<typename R>
    inline reverse_range<Iterator_type<R>> reversed(R& range)
    {
      return {std::begin(range), std::end(range)};
    }
    
  template<typename R>
    inline reverse_range<Iterator_type<const R>> reversed(const R& range)
    {
      return {std::begin(range), std::end(range)};
    }
    
} // namespace origin

#endif
