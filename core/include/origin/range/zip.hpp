// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_ZIP_RANGE_HPP
#define ORIGIN_RANGE_ZIP_RANGE_HPP

#include <origin/range/core.hpp>
#include <origin/iterator/zip.hpp>

namespace origin
{

  template <typename... Rs>
    using zip_range = bounded_range<zip_iterator<Iterator_type<Rs>... >>;

  // Zip
  //
  // The zip range constructor returns a range over the argument ranges that
  // is traversed in lockstep. Note that the constness of range arguments is
  // preserved so that if the ith range argument is constant, then the ith
  // referenced object will also be constant.
  //
  // FIXME: GCC is having problems with zip range. The result type should be
  // zip_range<Auto<Rs>...>, but there seems to be a pack expansion problem.

  template <typename... Rs>
    inline auto zip(Rs&&... ranges)
      -> bounded_range<zip_iterator<Iterator_type<Auto<Rs>>... >>
    {
      return {
        make_zip_iterator(begin(ranges)...),
        make_zip_iterator(end(ranges)...)
      };
    }

    
} // namespace origin

#endif
