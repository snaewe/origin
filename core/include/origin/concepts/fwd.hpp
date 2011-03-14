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


  // Operators (arithmetic)
  // NOTE: We can't group these together because there is too algebraic
  // "diversity" here. It would be great to assume that if an operator/ is
  // defined then an operator% is also defined. Unfortunately, that's not the
  // case---even for builtin types (floats). Even minus and unary minus have
  // corresponding algebraic structures (the latter depending apparently
  // depending on an inversion operators.
  template<typename T, typename U = T> struct Plus;
  template<typename T, typename U = T> struct Minus;
  template<typename T, typename U = T> struct Multiplies;
  template<typename T, typename U = T> struct Divides;
  template<typename T, typename U = T> struct Modulus;

  // NOTE: These can't be grouped with Plus and Minus either. Not all types
  // supporting subtraction or multiplication define negation. Unary plus is
  // just weird.
  template<typename T> struct Unary_Plus;
  template<typename T> struct Unary_Minus;

  // Operators (bitwise).
  // FIXME: I don't like the name Bitset since valid interfaces may not be
  // Binary at all (e.g., sets with overloaded operators). On the otherhand,
  // it's just describing the operators, not the abstraction.
  template<typename T, typename U = T> struct Bitset;
  template<typename T, typename U = T> struct Shift;
  template<typename T> struct Complement;

  // Operators (relational)
  template<typename T, typename U = T> struct Equal;
  template<typename T, typename U = T> struct Ordered;

  // Operators (logical)
  template<typename T, typename U = T> struct Logical;

  // Regular types
  template<typename T> struct Default;
  template<typename T> struct Copyable;
  template<typename T> struct Moveable;
  template<typename T> struct Regular;

  // Numeric/Algebraic
  template<typename T> struct Boolean;

} // namespace origin

#endif
