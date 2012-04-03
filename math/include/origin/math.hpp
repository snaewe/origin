// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_HPP
#define ORIGIN_MATH_HPP

#include <cmath>

namespace origin
{
  // General mathematic operations.


  // Absolute value
  //
  // FIXME: Requires an ordered ring.
  template <typename T>
    inline T abs(const T& value)
    {
      return (value < T {0}) ? -value : value;
    }



  // Signum
  // Returns the sign of x such that x == sgn(x) * abs(x).
  template <typename T>
    inline T sgn(const T& x)
    {
      if (x < T {0})
        return T {-1};
      else if (x > T {0})
        return T {1};
      else
        return T {0};
    }
  


  // Even
  // Returns true if n is an even number.
  template <typename T>
    inline bool is_even(const T& n)
    {
      return n % T {2} == T {0};
    }



  // Odd
  // Returns true if n is an odd integer.
  template <typename T>
    inline bool is_odd(const T& n)
    {
      return n % T {2} == T {1};
    }



  // Negative
  // Returns true if n is integer.
  template <typename T>
    inline bool is_negative(const T& n)
    {
      return n < T {0};
    }


  // Positive
  // Returns true if n is positive.
  template <typename T>
    inline bool is_positive(const T& n)
    {
      return n > T {0};
    }



  // Non-negative
  // Returns true if n is non-negative.
  template <typename T>
    inline bool is_non_negative(const T& n)
    {
      return n >= T {0};
    }



  // Non-positive
  // Returns true if n is non-positive.
  template <typename T>
    inline bool is_non_positive(const T& n)
    {
      return n <= T {0};
    }



  // Greated common denominator
  // Returns the GCD of a and b using the Euclidean algorithm.
  //
  // FIXME: Write the requiremetns for this algorithm.
  //
  // FIXME: There is a "cancellable" specialization for types that may not
  // support division (Archimedean monoids?). See EoP.
  //
  // FIXME: There is a more efficient implementation for a subset of types
  // like unsigned ints (Stein GCD). Write that.
  template <typename T>
    T gcd(T a, T b)
    {
      while (b != T {0}) {
        T tmp = b;
        b = a % b;
        a = tmp;
      }
      return a;
    }



  // Least common multiple
  // Returns the LCM of a and b.
  template <typename T>
    T lcm(const T& a, const T& b) 
    {
      return (abs(a) / gcd(a, b)) * abs(b);
    }



  // Divmod
  // Return the quotient and remainder of a / b.
  //
  // FIXME: What does this require? A field? A ring?
  template <typename T>
    std::pair<T, T> divmod(const T& a, const T& b)
    {
      return {a / b, a % b};
    }

  // Divmod for builtin types
  //
  // FIXME: divmod('a', 'b') will use the generic algorithm above. Should it?
  // Is std::div more efficient for integral types smaller that int? Probably.
  //
  // TODO: Do I need a divmod for intmax_t?

  inline std::pair<int, int> divmod(int a, int b)
  {
    std::div_t d = std::div(a, b);
    return {d.quot, d.rem};
  }

  inline std::pair<long, long> divmod(long a, long b)
  {
    std::ldiv_t d = std::div(a, b);
    return {d.quot, d.rem};
  }

  inline std::pair<long long, long long> divmod(long long a, long long b)
  {
    std::lldiv_t d = std::div(a, b);
    return {d.quot, d.rem};
  }



  // Power
  // Raise Num to x to exp-th power through successive multiplications.
  // E must be an Integer type
  template <typename T>
    T o_pow(const T& x, int exp)
    {
      return std::pow(x, exp);
    }


} // namespace origin

#endif
