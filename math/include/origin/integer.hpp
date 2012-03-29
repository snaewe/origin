// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_INTEGER_HPP
#define ORIGIN_INTEGER_HPP

#if ORIGIN_USE_GMP
#  include <origin/integer/gmp.hpp>
#else
#  error No integer implementation has been configured.
#endif

namespace origin
{
  // Integer
  // The integer class implements an dynamic, arbitrary precision integer.
  class integer
  {
    integer_impl impl;

    // Adoption
    // This constructor adopts the underlying implementation, moving it into
    // the constructed integer object.
    integer(integer_impl&& x) : impl(std::move(x)) { }
  
  public:
    integer() : impl() { }

    // Movable
    integer(integer&& x) : impl(std::move(x.impl)) { }
    integer& operator=(integer&& x) { impl = std::move(x.impl); return *this; }

    // Copyable
    integer(const integer& x) : impl(x.impl) { }
    integer& operator=(const integer& x) { impl = x.impl; return *this; }

    // Coversion
    //
    // FIXME: These should be long longs or intmax_t. Unfortunately, GMP does
    // not support constructors for integers of that size.
    integer(signed int n) : impl(static_cast<signed long>(n)) { }
    integer(unsigned int n) : impl(static_cast<unsigned long>(n)) { }
    integer(signed long n) : impl(n) { }
    integer(unsigned long n) : impl(n) { }

    // String construction
    explicit integer(const char* str, int base = 10) : impl(str, base) { }
    explicit integer(const std::string& str, int base = 10) : impl(str, base) { }

    // Eqaulity_comparable
    bool operator==(const integer& x) const { return impl == x.impl; }
    bool operator!=(const integer& x) const { return impl != x.impl; }

    // Totally_ordered
    bool operator<(const integer& x) const { return impl < x.impl; }
    bool operator>(const integer& x) const { return impl > x.impl; }
    bool operator<=(const integer& x) const { return impl <= x.impl; }
    bool operator>=(const integer& x) const { return impl >= x.impl; }

    // Arithmetic operations

    // Addition
    integer& operator+=(const integer& x) { impl += x.impl; return *this; }
    integer operator+(const integer& x) const { return impl + x.impl; }
    
    // Subtraction
    integer& operator-=(const integer& x) { impl -= x.impl; return *this; }
    integer operator-(const integer& x) const { return impl - x.impl;} 

    // Multiplication
    integer& operator*=(const integer& x) { impl *= x.impl; return *this; }
    integer operator*(const integer& x) const { return impl * x.impl; }

    // Division
    integer& operator/=(const integer& x) { impl /= x.impl; return *this; }
    integer operator/(const integer& x) const { return impl / x.impl; }

    // Modulus
    integer& operator%=(const integer& x) { impl %= x.impl; return *this; }
    integer operator%(const integer& x) const { return impl % x.impl; }

    // Negation and absolute value
    integer operator-() const { return -impl; }

    // FIXME: Why oh why can't I declare abs as an operator?
    integer abs() const { return impl.abs(); }

    // Incrementable
    integer& operator++() { ++impl; return *this; }
    integer operator++(int) { return impl++; }

    // Decrementable
    integer& operator--() { --impl; return *this; }
    integer operator--(int) { return impl--; }

    // Return a refernce to the base implementation.
    integer_impl&       base()       { return impl; }
    const integer_impl& base() const { return impl; }
  };



  // Numeric algorithms
  inline integer abs(const integer& n)
  {
    return n.abs();
  }


  // Output streamable
  template <typename C, typename T>
    std::basic_ostream<C, T>& 
    operator<<(std::basic_ostream<C, T>& os, const integer& i)
    {
      return os << i.base();
    }


  // Input streamable
  template <typename C, typename T>
    std::basic_istream<C, T>& 
    operator<<(std::basic_istream<C, T>& is, integer& i)
    {
      return is >> is.base();
    }

} // namespace origin

#endif
