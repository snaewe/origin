// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif

namespace matrix_impl
{
  ////////////////////////////////////////////////////////////////////////////
  // Operations
  //
  // The following function objects extend the usual set found in the 
  // <functional> library and provide abstractions over assignment and 
  // arithmetic compound assignment operators.
  //
  // TODO: These should probably be moved into the functional module.
  ////////////////////////////////////////////////////////////////////////////

  template <typename T>
    struct assign
    {
      T& operator()(T& a, T const& b) const { return a = b; }
    };

  template <typename T>
    struct plus_assign
    {
      T& operator()(T& a, T const& b) const { return a += b; }
    };

  template <typename T>
    struct minus_assign
    {
      T& operator()(T& a, T const& b) const { return a -= b; }
    };

  template <typename T>
    struct multiplies_assign 
    {
      T& operator()(T& a, T const& b) const { return a *= b; }
    };

  template <typename T>
    struct divides_assign
    {
      T& operator()(T& a, T const& b) const { return a /= b; }
    };

  template <typename T>
    struct modulus_assign
    {
      T& operator()(T& a, T const& b) const { return a %= b; }
    };



  ////////////////////////////////////////////////////////////////////////////
  // Apply
  //
  // Apply a value, x, to each element of a mutable range. There are two
  // overloads of this function:
  //
  //    apply(first, last, value, f)
  //    apply(first1, last1, first2, last2, f)
  //
  // The first overload applies a single value to each element in the range
  // [first, last), while the second applies 


  // Apply f(*i, value) to each iterator i in the mutable range [first, last).
  // Note that f may (and is generally expected to) modify the object referred
  // to by *i.
  //
  // Parameters:
  //    first, last -- A bounded, mutable range
  //    value -- The value being applied to each element in [first, last)
  //
  // Requires:
  //    Mutable_iterator<I1>
  //    Input_iterator<I2>
  //    Function<F, Value_type<I1>&, Value_type<I2>
  //
  // Returns:
  //    The function object f.
  template <typename I, typename T, typename F>
    inline F 
    apply(I first, I last, const T& value, F f)
    {
      while (first != last) {
        f(*first, value);
        ++first;
      }
      return f;
    }


  // Apply f(*i, *j) to each iterator i in the mutable range [first1, last1) and
  // each corresponding j in the input range [first2, last2). Note that f may
  // (and is generally expected to) modify the object referred to by *i.
  //
  // Parameters:
  //    first1, last1 -- A bounded, mutable range
  //    first2, last2 -- A bounded input range
  //
  // Requires:
  //    Mutable_iterator<I1>
  //    Input_iterator<I2>
  //    Function<F, Value_type<I1>&, Value_type<I2>
  //
  // Returns:
  //    The function object f.  
  template <typename I1, typename I2, typename F>
    inline F 
    apply_each(I1 first1, I1 last1, I2 first2, F f)
    {
      while (first1 != last1) {
        f(*first1, *first2);
        ++first1;
        ++first2;
      }
    }



  //////////////////////////////////////////////////////////////////////////////
  // Matrix Support
  //
  // The following features support the generic implementation of the matrix
  // and matrix_ref classes.
  //////////////////////////////////////////////////////////////////////////////


  // TODO: This algorithm could be generalized to flatten an arbitrary
  // initializer list structure.

  // For iterators over the leaf nodes, insert elements into the back of the
  // vector. We generally assume that the vector has sufficient capacity for
  // all such insertions, but insert guarantees that it will resize if needed.
  template <typename T, typename Vec>
    void 
    initialize(const T* first, const T* last, Vec& vec)
    {
      vec.insert(vec.end(), first, last);
    }

  // For iterators into nested initializer lists, recursively intiailize each
  // sub-initializeer.
  template <typename T, typename Vec>
    void initialize(const std::initializer_list<T>* first,
                    const std::initializer_list<T>* last,
                    Vec& vec)
    {
      while (first != last) {
        initialize(first->begin(), first->end(), vec);
        ++first;
      }
    }

  // Copy the elements from the initializer list nesting into contiguous
  // elements in the vector.
  template <typename T, typename Vec>
    void initialize(const std::initializer_list<T>& list, Vec& vec)
    {
      initialize(list.begin(), list.end(), vec);
    }

} // namespace matrix_impl

