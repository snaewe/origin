// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_ZIP_RANGE_HPP
#define ORIGIN_RANGE_ZIP_RANGE_HPP

#include <origin/range.hpp>
#include <origin/iterator/zip_iterator.hpp>

namespace origin
{
  // A zip range is bounded range that ranges over the elements of multiple
  // sub-ranges in lockstep.
  template<typename... Iters>
    class zip_range
    {
    public:
      using value_type = std::tuple<Value_type<Iters>...>;
      using difference_type = std::ptrdiff_t;   // FIXME
      using iterator = zip_iterator<Iters...>;
      
      zip_range(iterator first, iterator last)
        : first{first}, last{last}
      { }

      iterator begin() const { return first; }
      iterator end() const   { return last; }
      
    private:
      iterator first;
      iterator last;
    };
    
    
  // Zip
  // The zip range constructor returns a range over the argument ranges that
  // is traversed in lockstep. Note that the constness of range arguments is
  // preserved so that if the ith range argument is constant, then the ith
  // referenced object will also be constant.
  template<typename... Ranges>
    inline zip_range<Iterator_type<Ranges>...> zip(Ranges&&... ranges)
    {
      using I = zip_iterator<Iterator_type<Ranges>...>;
      return {I{std::begin(ranges)...}, I{std::end(ranges)...}};
    }
    
} // namespace origin

#endif
