// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_COUNTING_HPP
#define ORIGIN_COUNTING_HPP

#include <cassert>

namespace origin
{
  // FIXME: How do we represent identity elements?
  
  // PROJECT: Implement faster factorial algorithms. This seems like a nice
  // place to start looking for related information.
  // http://www.luschny.de/math/factorial/FastFactorialFunctions.htm
  //
  // It might also be useful to consider approximation algorithms instead of
  // precise computation.
 
  
  // Return n! (the factorial of n).
  //
  // requires: Semiring<T>
  // precondition: n >= 0
  template<typename T>
    T factorial(T n)
    {
      assert(( n >= T{0} ));
      T result = 1;
      while(n > T{0}) {
        result = result * n;
        --n;
      }
      return result;
    }
    
  // PROJECT: Implement double and triple factorial and the more general
  // form of multifactorial.

  // Return n to the power of k falling.
  //
  // requires: Semiring<T>
  // precondition: 0 <= k <= n
  template<typename T>
    T falling_factorial(T n, T k)
    {
      assert(( T{0} <= k && k <= n ));
      T result = 1;
      while(k >= T{1}) {
        result = result * n;
        --n;
        --k;
      }
      return result;
    }
    

  // Retrun n to the power of k rising.
  //
  // requires: Semiring<T>
  // precondition: 0 <= k
  template<typename T>
    T rising_factorial(T n, T k)
    {
      assert(( T{0} <= k ));
      T result = 1;
      while(k >= T{1}) {
        result = result * n;
        ++n;
        --k;
      }
      return result;
    }



  // Return the kth coefficient of binomial (1 + x) raised to the nth power.
  //
  // requires: Semiring<T>
  // precondition: 0 <= k <= n
  template<typename T>
    T binomial_coefficient(T n, T k)
    {
      assert(( T{0} <= k && k <= n ));
      
      // Take advantage of symmetry, making the loop shorter.
      if(k > n - k)
        k = n - k;
      
      // Divide in each step to reduce the chance of overflow.
      T result = 1;
      T r = 1;
      while(r <= k) {
        result = (result * n) / r;
        --n;
        ++r;
      }
      return result;
    }


} // namespace origin

#endif
