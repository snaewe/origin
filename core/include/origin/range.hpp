// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_HPP
#define ORIGIN_RANGE_HPP

#include <origin/iterator.hpp>
#include <origin/range/traits.hpp>

namespace origin
{
  // Return the size of the given range by calculating its distance. The size
  // type is a natural (unsigned) number.
  template<typename R>
    inline auto size(R const& r) 
      -> typename std::enable_if<
        !has_member_size<R>::value,
        typename std::make_unsigned<
          decltype(std::distance(std::begin(r), std::end(r)))
        >::type
      >::type
    {
      return std::distance(std::begin(r), std::end(r));
    }
  
  // Return the size of the given range, if the range is counted (i.e., has a
  // size member function). This is expected to be a constant-time operation.
  //
  // FIXME: This operation is extremely general. It applies, for example, to
  // Containers, and Graphs. There's probably a unifying concept (Sized?),
  // but the operation doesn't belong here. Note that this wouldn't be an issue
  // ADL included x.size() as a candidate for size(x). Then, there wouldn't be
  // any overload issues.
  template<typename R>
    inline auto size(R const& r) -> decltype(r.size())
    {
      return r.size();
    }


  // Return true if r is empty. A range is empty if its beginning value
  // is equal to its limit (end).
  template<typename R>
    inline auto empty(R const& r)
      -> typename std::enable_if<!has_member_size<R>::value, bool>::type
    {
      return std::begin(r) == std::end(r);
    }

  // Return true if the counted range r is emtpy.
  //
  // FIXME: See comments on size(), for Sized? types.
  template<typename R>
    inline auto empty(R const& r) -> decltype(r.empty())
    {
      return r.empty();
    }


  // The range traits class provides a facility for accessing the names of
  // of the types associated with a range.
  template<typename R>
    struct range_traits
    {
      typedef decltype(std::begin(std::declval<R&>())) iterator;
      typedef typename std::iterator_traits<iterator>::value_type value_type;
      typedef decltype(size(std::declval<R&>())) size_type;
    };

  // A specialization of the trait above for constant ranges.
  template<typename R>
    struct range_traits<R const>
    {
      typedef decltype(std::begin(std::declval<R const&>()))      iterator;
      typedef typename std::iterator_traits<iterator>::value_type value_type;
      typedef decltype(size(std::declval<R const&>()))            size_type;
    };

} // namespace origin

// Include range constructors
#include <origin/range/array.hpp>
#include <origin/range/bounded.hpp>
// #include <origin/range/filter.hpp>
// #include <origin/range/permutation.hpp>

#endif
