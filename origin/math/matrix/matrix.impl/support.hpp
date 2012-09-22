// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif

// NOTE: Facilities in this header could be moved into more general libraries.

namespace matrix_impl
{
  // ------------------------------------------------------------------------ //
  //                            Operations
  //
  // The following function objects extend the usual set found in the 
  // <functional> library and provide abstractions over assignment and 
  // arithmetic compound assignment operators.
  //
  // TODO: These should probably be moved into the functional module.

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



  // ------------------------------------------------------------------------ //
  //                                Apply
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

  // ------------------------------------------------------------------------ //
  //                          Insert Flattened
  //
  // Insert the elements of a initializer list nesting into a vector such that
  // each subsequent set of "leaf" values are copied into a contiguous memory.

  // TODO: This algorithm could be generalized to flatten an arbitrary
  // initializer list structure.

  // For iterators over the leaf nodes, insert elements into the back of the
  // vector. We generally assume that the vector has sufficient capacity for
  // all such insertions, but insert guarantees that it will resize if needed.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const T* first, const T* last, Vec& vec)
    {
      vec.insert(vec.end(), first, last);
    }

  // For iterators into nested initializer lists, recursively intiailize each
  // sub-initializeer.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const std::initializer_list<T>* first,
                    const std::initializer_list<T>* last,
                    Vec& vec)
    {
      while (first != last) {
        insert_flattened(first->begin(), first->end(), vec);
        ++first;
      }
    }

  // Copy the elements from the initializer list nesting into contiguous
  // elements in the vector.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const std::initializer_list<T>& list, Vec& vec)
    {
      insert_flattened(list.begin(), list.end(), vec);
    }

  // ------------------------------------------------------------------------ //
  //                       Begin/End Argument Pack
  //
  // Return iterators to the beginning and ending of elements in an argument
  // pack. Note that elements must all have the same type.

  template <typename T, typename... Ts>
    auto begin_args(T&& t, Ts&&... ts) -> decltype(&t)
    {
      static_assert(Same<T, Ts...>(), "");
      return &t;
    }


  template <typename T>
    auto end_args(T&& x) -> decltype(&x)
    {
      return &x + 1;
    }

  template <typename T, typename... Ts>
    auto end_args(T&& t, Ts&&... ts) 
      -> decltype(end_args(std::forward<Ts>(ts)...))
    {
      static_assert(Same<T, Ts...>(), "");
      return end_args(std::forward<Ts>(ts)...);
    }



  // ------------------------------------------------------------------------ //
  //                              Algorithms

  // NOTE: Some of these could be moved into an algoirthm or numeric header.

  // A helper function for creating a reverse iterator over a pointer.
  template <typename T>
    inline std::reverse_iterator<T*> rev(T* p) 
    { 
      return std::reverse_iterator<T*>(p); 
    }


  // Compute the product of a sequence of elements.
  template <typename I>
    inline Value_type<I>
    product(I first, I last)
    {
      using T = Value_type<I>;
      std::multiplies<T> mul;
      return std::accumulate(first, last, T(1), mul);
    }

  template <typename R>
    inline Value_type<R>
    product(const R range)
    {
      using std::begin;
      using std::end;
      return product(begin(range), end(range));
    }


  // Compute the inner product of a two vectors.
  template <typename R1, typename R2>
    inline Value_type<R1>
    inner_product(const R1& range1, R2& range2)
    {
      using std::begin;
      using std::end;
      return std::inner_product(begin(range1), end(range1), 
                                begin(range2),
                                Value_type<R1>(0));
    }


  // Compute the a variant of partial product of the in array, storing the
  // results in the out array. This implementation stores the multiplicative
  // identinity in the first element
  //
  // The resulting vector is used to compute indexes in column-major order.
  template <typename Array>
    inline std::size_t
    forward_partial_product(const Array& in, Array& out)
    {
      using std::begin;
      using std::end;
      using T = Value_type<Array>;

      std::multiplies<T> mul;
      auto f = begin(in);
      auto l = end(in);
      auto o = begin(out);
      *o = T(1);
      std::partial_sum(f, --l, ++o, mul);
      return *(--o) * *l;
    }


  // Compute the partial product, in reverse order, of the in array, storing
  // the results in the out array.
  //
  // The resulting vector is used to compute indexes in row-major order.
  template <typename Array>
    inline std::size_t
    reverse_partial_product(const Array& in, Array& out)
    {
      using std::begin;
      using std::end;
      using T = Value_type<Array>;

      std::multiplies<T> mul;
      auto f = rev(end(in));
      auto l = rev(begin(in));
      auto o = rev(end(out));
      *o = T(1);
      o = std::partial_sum(f, --l, ++o, mul);
      return *(--o) * *l;
    }


  // ------------------------------------------------------------------------ //
  //                          Derive Extents


  // Used in derive_extents, returns true if the array is not jagged. That is,
  // all sub-initializers of list must have the same size.
  template <typename List>
    inline bool
    check_non_jagged(const List& list)
    {
      auto i = list.begin();
      for (auto j = i + 1; j != list.end(); ++j) {
        if (i->size() != j->size())
          return false;
      }
      return true;
    }


  // Derive extents implementation, matches when N == 1.
  template <std::size_t N, typename I, typename List>
    inline Requires<(N == 1), void>
    derive_extents(I& first, I last, const List& list)
    {
      assert(first != last);
      *first++ = list.size();
    }

  // Derive extents implementation, matches when N > 1.
  template <std::size_t N, typename I, typename List>
    inline Requires<(N > 1), void>
    derive_extents(I& first, I last, const List& list)
    {
      assert(check_non_jagged(list));
      assert(first != last);
      *first++ = list.size();
      derive_extents<N - 1>(first, last, *list.begin());
    }

  // Returns the matrix shape corresponding to a sequence of nested initialize
  // lists. Here, List is sequence of nested initializer lists. Note that the
  // nested list must be non-jagged; all initializer lists at the same depth
  // must have the same length.
  template <std::size_t N, typename List>
    inline void
    derive_extents(std::array<std::size_t, N>& extents, const List& list)
    {
      auto first = extents.begin();
      auto last = extents.end();
      derive_extents<N>(first, last, list);
      assert(first == last);
    }

  // ------------------------------------------------------------------------ //
  //                          Compute Row

  // Returns a reference to the nth row in a matrix.
  //
  // The function takes a base object (whose order must be 1 greater than N),
  // a pointer p to the start of a matrix or matrix reference descirbed by
  // base, and n, the row to return.

  // When N > 0, we are returning a matrix reference of N dimensions.
  template <typename T, typename Base>
    inline Requires<(Base::order > 1), matrix_ref<T, Base::order - 1>>
    row(const Base& b, T* p, std::size_t n)
    {
      return {b.row(), p + n * b.extents[1]};
    }

  // When N == 0, we are returning an element reference, possibly const
  // qualified.
  template <typename T, typename Base>
    inline Requires<(Base::order == 1), matrix_ref<T, 0>>
    row(const Base& b, T* p, std::size_t n)
    {
      return *(p + n);
    }

} // namespace matrix_impl

