// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_REAL_HPP
#define ORIGIN_REAL_HPP

#if ORIGIN_USE_MPFR
#  include <origin/real/mpfr.hpp>
#else
#  error No real number implementation has been configured.
#endif

namespace origin
{
  // TODO: Make a basic_real template that is parameterized over rounding
  // mode and floating point exceptions. Then, make real an alias to the
  // basic real with default properties.


  // Real
  // The real class implements an dynamic, arbitrary precision real.
  class real
  {
    using impl_type = real_impl<>;
    impl_type impl;

    // Adoption
    // This constructor adopts the underlying implementation, moving it into
    // the constructed real object.
    real(impl_type&& x) : impl(std::move(x)) { }
  
  public:
    real() : impl() { }

    // Movable
    real(real&& x) : impl(std::move(x.impl)) { }
    real& operator=(real&& x) { impl = std::move(x.impl); return *this; }

    // Copyable
    real(const real& x) : impl(x.impl) { }
    real& operator=(const real& x) { impl = x.impl; return *this; }

    // Coversion
    real(double n) : impl(n) { }

    // String construction
    explicit real(const char* str, int base = 10) : impl(str, base) { }
    explicit real(const std::string& str, int base = 10) : impl(str, base) { }

    // Eqaulity_comparable
    bool operator==(const real& x) const { return impl == x.impl; }
    bool operator!=(const real& x) const { return impl != x.impl; }

    // Totally_ordered
    bool operator<(const real& x) const { return impl < x.impl; }
    bool operator>(const real& x) const { return impl > x.impl; }
    bool operator<=(const real& x) const { return impl <= x.impl; }
    bool operator>=(const real& x) const { return impl >= x.impl; }

    // Arithmetic operations

    // Addition
    real& operator+=(const real& x) { impl += x.impl; return *this; }
    real operator+(const real& x) const { return impl + x.impl; }
    
    // Subtraction
    real& operator-=(const real& x) { impl -= x.impl; return *this; }
    real operator-(const real& x) const { return impl - x.impl;} 

    // Multiplication
    real& operator*=(const real& x) { impl *= x.impl; return *this; }
    real operator*(const real& x) const { return impl * x.impl; }

    // Division
    real& operator/=(const real& x) { impl /= x.impl; return *this; }
    real operator/(const real& x) const { return impl / x.impl; }

    // Negation
    real operator-() const { return -impl; }

    // Return a refernce to the base implementation.
    impl_type&       base()       { return impl; }
    const impl_type& base() const { return impl; }
  };


  template <typename C, typename T>
    std::basic_ostream<C, T>& 
    operator<<(std::basic_ostream<C, T>& os, const real& i)
    {
      return os << i.base();
    }


  // Input streamable
  //
  // FIXME: Implement me!
  template <typename C, typename T>
    std::basic_istream<C, T>& 
    operator<<(std::basic_istream<C, T>& is, real& i)
    {
      return is;
    }

} // namespace origin

#endif
