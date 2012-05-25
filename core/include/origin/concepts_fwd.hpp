// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_FWD_HPP
#define ORIGIN_CONCEPTS_FWD_HPP

namespace origin
{
  // Fundamental concepts
  template <typename T, typename U = T> constexpr bool Equality_comparable();
  template <typename T, typename U = T> constexpr bool Weakly_ordered();
  template <typename T, typename U = T> constexpr bool Totally_ordered();

  template <typename T> constexpr bool Movable();
  template <typename T> constexpr bool Copyable();
  template <typename T> constexpr bool Regular();
  
  // Function concepts
  template <typename F, typename... Args> constexpr bool Function();
  template <typename F, typename... Args> constexpr bool Regular_function();
  template <typename P, typename... Args> constexpr bool Predicate();
  template <typename R, typename T, typename U = T> constexpr bool Relation();

  // Streaming
  template <typename T, typename U = default_t> constexpr bool Input_streamable();
  template <typename T, typename U = default_t> constexpr bool Output_streamable();
  template <typename T, typename U = default_t> constexpr bool Streamable();
  
  // Concepts for random values
  template <typename Eng> constexpr bool Random_number_engine();
  template <typename Gen> constexpr bool Random_bit_generator();
  template <typename Gen> constexpr bool Random_value_generator();
  template <typename Gen> constexpr bool Random_number_generator();

} // namespace origin

#endif
