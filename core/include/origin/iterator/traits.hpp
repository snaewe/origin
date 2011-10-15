// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_TRAITS_HPP
#define ORIGIN_ITERATOR_TRAITS_HPP

#include <iterator>

#include <origin/traits.hpp>

namespace origin
{
  // A helper trait for deciding is_iterator.
  template<typename T>
    struct is_iterator__
    {
    private:
      template<typename X>
        static typename std::iterator_traits<X>::iterator_category check(X const&);
      static substitution_failure check(...);
    public:
      typedef decltype(check(std::declval<T>())) type;
    };
  
  // Return true if T is an iterator.
  template<typename T>
    struct is_iterator
      : substitution_succeeded<typename is_iterator__<T>::type>
    { };


  // Return an iterator category that is not more derived than the given limit. 
  // For example, if Tag is "random access" and Limit is "forward", this will
  // return limit.
  template<typename Tag, typename Limit>
  struct clamp_iterator_category
    : std::conditional<std::is_base_of<Limit, Tag>::value, Limit, Tag>
  { };
} // namespace origin

#endif
