// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_UTILITY_HPP
#define ORIGIN_UTILITY_HPP

#include <cassert>

#include <origin/utility/meta.hpp>
#include <origin/utility/typestr.hpp>

#include <iosfwd>

// The assume macro is used in conjunction with assert. It is used to state
// a preconditon that is not intended to be evaluated or even compiled. This is 
// useful for documenting uncheckable preconditions or those that would induce
// complex dependencies, etc.
#define assume(X)

// The assert_if macro is used to conditionally assert an expression. If the
// expression cond is true, then the expression expr is evaluated. 

// Note that if NDEBUG is defined, the assertion expands into an empty 
// statement.
#ifdef NDEBUG
#  define assert_if(cond, expr)
#else
#  define assert_if(Cond, Expr) if(Cond) assert(Expr)
#endif

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



  // Streamable<empty_t>
  //
  // TODO: Should we actually read and/or write something for this type? It
  // might be kind of nice.
  template <typename Char, typename Traits>
    inline std::basic_ostream<Char, Traits>& 
    operator<<(std::basic_ostream<Char, Traits>& os, empty_t)
    { 
      return os; 
    }

  template <typename Char, typename Traits>
    inline std::basic_istream<Char, Traits>& 
    operator>>(std::basic_istream<Char, Traits>& is, empty_t&)
    { 
      return is; 
    }
    
    
  // Laze false (trait)
  // The lazy false function always evaluates to false. However, being a
  // template, any calls to this function are inherently dependent on the
  // argument type. Therefore, the compiler cannot eagerly replace the function
  // call with the false literal. This is sometimes used to defer static
  // assertions that are known to be failure cases.
  template <typename T>
    constexpr bool Lazy_false()
    {
      return false;
    }



  // Swap (utility)
  // A wrapper for the use of std::swap.
  template <typename T>
    void o_swap(T& a, T& b)
    {
      using std::swap;
      swap(a, b);
    }

} // namespace origin

#endif
