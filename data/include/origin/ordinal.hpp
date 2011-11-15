// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ORDINAL_HPP
#define ORIGIN_ORDINAL_HPP

#include <limits>

#include <origin/traits.hpp>

namespace origin
{
  // Semiordinal types
  // A semiordinal type defines a mapping between its values and the ordinal
  // numbers via the ord() function.
  
  // For unsigned integers, ord is just an identity function.
  template<typename T>
    inline Requires<Unsigned<T>(), T> ord(T x)
    {
      return x;
    }
    
  // For signed integers, the first ordinal (0) is the minimum value encoded
  // by the type (numeric_limits<Ord>::min. Values increase from there.
  template<typename T>
    inline Requires<Signed<T>(), Make_unsigned<T>> ord(T x)
    {
      return x + Make_unsigned<T>(std::numeric_limits<T>::max()) + 1u;
    }
    
  // For classes defining an ord member function, just return that.
  template<typename T>
    inline auto ord(T const& x) -> decltype(x.ord())
    {
      return x.ord();
    }

    
    
  // Safely get the result of the expression ord(t).
  template<typename T>
    struct ord_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(ord(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // The ordinal type of an ordinal object is the result of its associate
  // ord() operation.
  template<typename T>
    using Ordinal_type = typename ord_result<T>::type;
    
  // Specification of the Semiordinal concept.
  // FIXME: Is there more to this concept?
  template<typename T>
    struct Semiordinal_concept
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Ordinal_type<T>>();
      }
    };

  // Returns true if T is semiordinal.
  template<typename T>
    constexpr bool Semiordinal()
    {
      return Semiordinal_concept<T>::check();
    }
    
    

  // Ordinal cast
  // The ordinal cast operator returns a value of type T that corresponds to
  // the ordinal value n.
    
  // For unsigned cast targets, just static cast the ordinal value.
  template<typename T>
    inline Requires<Unsigned<T>(), T> ordinal_cast(Ordinal_type<T> n) 
    {
      return n;
    }
    
  // For signed cast targets, subtract off the max signed value before
  // returning.
  template<typename T>
    inline Requires<Signed<T>(), T> ordinal_cast(Ordinal_type<T> n)
    {
      return Ordinal_type<T>(std::numeric_limits<T>::max()) - n + 1;
    }

  // For non-integral classes, we can compute an ordinal cast if T is
  // constructible over its ordinal type.
  //
  // FIXME: This may be problematic. What if T has another constructor over
  // an integral type that is *not* an ordinal constructor?
  template<typename T>
    inline auto ordinal_cast(Ordinal_type<T> n)
      -> Requires<!Integral<T>() && Constructible<T, Ordinal_type<T>>(), T>
    {
      return {n};
    }


  // Ordinal types
  // An ordinal type is a semiordinal type whose ordinal values can be mapped
  // to values of the original type.
  
  // Safely get the result of the expression ord<T>(u).
  template<typename T, typename U>
    struct ordinal_cast_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&&, Y const& y) -> decltype(ordinal_cast<X>(y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };
    
  // The ordinal type of an ordinal object is the result of its associate
  // ord() operation.
  template<typename T, typename U>
    using Ordinal_cast_result = typename ordinal_cast_result<T, U>::type;
    
  // Returns true if the expression ordinal_cast<T>(u) is valid.
  template<typename T, typename U>
    constexpr bool Has_ordinal_cast()
    {
      return Subst_succeeded<Ordinal_cast_result<T, U>>();
    }
    
  // A helper class for checking syntactic requirements.
  template<bool Prereqs, typename T>
    struct Ordinal_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename T>
    struct Ordinal_requirements<true, T>
    {
      static constexpr bool check()
      {
        return Has_ordinal_cast<T, Ordinal_type<T>>();
      }
    };
    
  // Specification of the ordinal concept.
  template<typename T>
    struct Ordinal_concept
    {
      static constexpr bool check()
      {
        return Ordinal_requirements<Semiordinal<T>(), T>::check();
      }
      
      static bool test(T a, T b)
      {
        return ordinal_cast<T>(ord(a)) == a
            && (a == b ? ord(a) == ord(b) : true);
      }
    };
    
  // Returns true if T is an ordinal type.
  template<typename T>
    constexpr bool Ordinal()
    {
      return Ordinal_concept<T>::check();
    }
    
    
} // namespace origin

#endif
