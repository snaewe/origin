// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_COUNTING_HPP
#define ORIGIN_COUNTING_HPP

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
  // requires: MultiplicativeMonoid
  // precondition: n >= 0
  //
  // TODO: Does * need to be commutative?
  template<typename T>
    T factorial(T n)
    {
      assert(( n >= 0 ));
      T result = T{1};
      while(n >= T{1}) {
        result = result * n;
        --n;
      }
      return result;
    }

  // Return n! using op instead of the usual multiplication.
  //
  // requires: MultiplicativeMonoid
  // precondition: identity_element(T{}, op) == T{1}
  template<typename T, typename Op>
    T factorial(T n, Op op)
    {
      assert(( n >= 0 ));
      T result = T{1};
      while(n >= T{1}) {
        result = op(result, n);
        --n;
      }
      return result;
    }
    
  // PROJECT: Implement double and triple factorial and the more general
  // form of multifactorial.

  // Return n to the power of k falling.
  //
  // requires: MultiplicativeMonoid<T>
  // precondition: 0 <= k <= n
  template<typename T>
    T falling_factorial(T n, T k)
    {
      assert(( 0 <= k && k <= n ));
      T result = T{1};
      while(k >= T{1}) {
        result = result * n;
        --n;
        --k;
      }
      return result;
    }
    
  // Return n to the power of k falling using op to multiply successive terms.
  //
  // requires: Monoid<T, Op>
  // precondition: 0 <= k <= n
  template<typename T, typename Op>
    T falling_factorial(T n, T k, Op op)
    {
      assert(( 0 <= k && k <= n ));
      T result = T{1};
      while(k >= T{1}) {
        result = op(result, n);
        --n;
        --k;
      }
      return result;
    }

  // Retrun n to the power of k rising.
  //
  // requires: MultiplicativeMonoid<T>
  // precondition: 0 <= k
  template<typename T>
    T rising_factorial(T n, T k)
    {
      assert(( 0 <= k ));
      T result = T{1};
      while(k >= T{1}) {
        result = result * n;
        ++n;
        --k;
      }
      return result;
    }

  // Return n to the power of k rising using op to multiply successive terms.
  //
  // requires: Monoid<T, Op>
  // precondition: 0 <= k
  template<typename T, typename Op>
    T rising_factorial(T n, T k, Op op)
    {
      assert(( 0 <= k ));
      T result = T{1};
      while(k >= T{1}) {
        result = result * n;
        ++n;
        --k;
      }
      return result;
    }
    
} // namespace origin

#endif
