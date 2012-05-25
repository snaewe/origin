// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_UTILITY_HPP
#define ORIGIN_UTILITY_HPP

#include <type_traits>

// The assume macro is used in conjunction with assert. It is used to state
// a preconditon that is not intended to be evaluated or even compiled. This is 
// useful for documenting uncheckable preconditions or those that would induce
// complex dependencies, etc.
#define assume(X)


namespace origin
{
  // Default type (utility)  
  // The default_t type is a tag class used to indicate the selection of a
  // default value. This is only used to support class template specialization.
  struct default_t { };
  

  // Unspecified type (utility)
  // The unspecified_t type is a tag class used to indicate that an argument
  // for a template parameter has not been specified.
  struct unspecified_t { };
  

  // Empty type (utility)
  // The empty type is an empty, trivial type that is meant to be used as a
  // placeholder for unspecified types in containers or other data structures.
  struct empty_t { };



  // Declval
  // Declval acts as an expression that evaluates to the explicitly specified
  // type. It must only be used in unevaluated contexts such as decltype,
  // sizeof, or noexcept expressions.
  //
  // FIXME: Decouple this definition from the standard's by making my own
  // add_rvalue_reference.
  template <typename T>
    typename std::add_rvalue_reference<T>::type declval() noexcept;



  // Unwrap
  // The unwrap provides an extension point for unwrapping objects from
  // adaptors, allowing an algorithm or data structure to operate directly on
  // the underlying type. For example, some iterator adaptors can be unwrapped
  // to provide direct access to a pointer, allowing move, copy, and fill
  // optimizations.
  //
  // By default, this function just returns the object it was passed. In other
  // words, it is an identity function.
  //
  // TODO: We could actually have this be an "unwrappable" concept for types
  // that can return some unerlying value. The question is, how deep do we
  // unwrap? One level? Fully recursive? For now, unwrappable iterators must
  // overload this function.

  template <typename T>
    inline T& unwrap(T& x) { return x; }

  template <typename T>
    inline const T& unwrap(const T& x) { return x; }


  // Unwrapped type
  // An alias to the unwrapped type.
  template <typename I>
    using Unwrapped_type = decltype(unwrap(declval<I>()));
    


} // namespace origin


// Utility components
#include <origin/utility/typestr.hpp>

#endif
