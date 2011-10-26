// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_NUMBER_HPP
#define ORIGIN_NUMBER_HPP

#include <limits>

namespace origin
{
  // This is a bit of hack around the std::numeric_traits. It forces a
  // substitution failure if numeric_limits isn't defined for the template
  // argument. The only time this is actually defined is when the argument
  // is a built-in arithmetic type.
  template<typename T> struct std_numeric_limits;
  template<> struct std_numeric_limits<char> : std::numeric_limits<char> { };
  template<> struct std_numeric_limits<signed char> : std::numeric_limits<signed char> { };
  template<> struct std_numeric_limits<unsigned char> : std::numeric_limits<unsigned char> { };
  template<> struct std_numeric_limits<short> : std::numeric_limits<short> { };
  template<> struct std_numeric_limits<unsigned short> : std::numeric_limits<unsigned short> { };
  template<> struct std_numeric_limits<int> : std::numeric_limits<int> { };
  template<> struct std_numeric_limits<unsigned int> : std::numeric_limits<unsigned int> { };
  template<> struct std_numeric_limits<long> : std::numeric_limits<long> { };
  template<> struct std_numeric_limits<unsigned long> : std::numeric_limits<unsigned long> { };
  template<> struct std_numeric_limits<float> : std::numeric_limits<float> { };
  template<> struct std_numeric_limits<double> : std::numeric_limits<double> { };
  template<> struct std_numeric_limits<long double> : std::numeric_limits<long double> { };
  
  // Return the maximum representable value for numeric type T.
  template<typename T>
    constexpr auto max() -> decltype(std_numeric_limits<T>::max())
    {
      return std::numeric_limits<T>::max();
    }

  // Overload for user-defined numeric types. These must define the
  // static member function T::max().
  template<typename T>
    constexpr auto max() -> decltype(T::max())
    {
      return T::max();
    }
    
  // Return the minimum representable value for the numeric type T.
  template<typename T>
    constexpr auto min() -> decltype(std_numeric_limits<T>::min())
    {
      return std::numeric_limits<T>::min();
    }
    
  // Overload for user-defined numeric types. These must define the
  // static member function T::min().
  template<typename T>
    constexpr auto min() -> decltype(T::min())
    {
      return T::min();
    }

}

#endif
