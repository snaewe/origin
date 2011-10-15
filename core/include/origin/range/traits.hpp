// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_TRAITS_HPP
#define ORIGIN_RANGE_TRAITS_HPP

#include <iterator>

#include <origin/traits.hpp>

namespace origin
{
  // A helper trait for deciding is_range.
  template<typename T>
    struct is_range__
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(std::begin(x));
      static substitution_failure check(...);
    public:
      typedef decltype(check(std::declval<T>())) type;
    };
  
  // Return true if T is a Range.
  template<typename T>
    struct is_range
      : substitution_succeeded<typename is_range__<T>::type>
    { };


  // A helper for determining if a range has a counted size.
  template<typename T>
    struct has_member_size__
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(x.size());
      static substitution_failure check(...);
    public:
      typedef decltype(check(std::declval<T>())) type;
    };

  // Return true if T has a size() member.
  template<typename T>
    struct has_member_size
      : substitution_succeeded<typename has_member_size__<T>::type>
    { };

    
  // Return the iterator type associated with a range.
  template<typename R>
    struct range_iterator
    {
      typedef decltype(std::begin(std::declval<R&>())) type;
    };

  // A specialization of the trait above for constant ranges.
  template<typename R>
    struct range_iterator<R const>
    {
      typedef decltype(std::begin(std::declval<R const&>())) type;
    };


} // namespace origin

#endif
