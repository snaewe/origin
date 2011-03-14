// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_FWD_HPP
#define ORIGIN_CONCEPTS_FWD_HPP

namespace origin
{
  // Basic.
  template<typename... Args> struct Same;
  template<typename T, typename U> struct Convertible;
  template<typename T, typename... Args> struct Constructible;
  template<typename T, typename U> struct Assignable;

  // Operators
  template<typename T, typename U = T> struct Equal;
  template<typename T, typename U = T> struct Ordered;

  // Regular types
  template<typename T> struct Default;
  template<typename T> struct Copyable;
  template<typename T> struct Moveable;
  template<typename T> struct Regular;

  // Numeric/Algebraic
  template<typename T> struct Boolean;

} // namespace origin

#endif
