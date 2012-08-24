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

  template< typename T>
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


  // A helper class for extracting the extents from a nested sequence of
  // initializer lists. Note that dimensions are recorded left-to right in the
  // dims array such that the greatest index in dims corresponds to the 
  // outermost array size.
  template <std::size_t N>
    struct extent_from_init
    {
      // Get 
      template <typename List, typename Size>
        static void get(const List& list, Size* dims)
        {
          assert(check_list(list));
          *dims = list.size();
          ++dims;
          extent_from_init<N - 1>::get(*list.begin(), dims);
        }

      // Returns true if all the sub-lists are the same size.
      template <typename List>
        static bool check_list(const List& list)
        {
          auto i = list.begin();
          for(auto j = i + 1; j != list.end(); ++j) {
            if (i->size() != j->size())
              return false;
          }
          return true;
        };
    };

  template <>
    struct extent_from_init<1>
    {
      template <typename T, typename Size>
        static void get(const std::initializer_list<T>& list, Size* dims)
        {
          *dims = list.size();
          ++dims;
        }
    };


  // Returns the matrix shape corresponding to a sequence of nested initialize
  // lists.
  template <std::size_t N, typename Size, typename List>
    inline matrix_shape<Size, N> 
    shape(const List& list)
    {
      Size dims[N];
      extent_from_init<N>::get(list, dims);
      return {dims, dims + N};
    }



  template <std::size_t N>
    struct copy_from_init
    {
      template <typename List, typename T>
        static T* put(const List& list, T* p)
        {
          for (const auto& l : list)
            p = copy_from_init<N - 1>::put(l, p);
          return p;
        }
    };

  template <>
    struct copy_from_init<1>
    {
      template <typename List, typename T>
        static T* put(const List& list, T* p)
        {
          std::copy(list.begin(), list.end(), p);
          return p + list.size();
        }
    };


  template <std::size_t N, typename List, typename T>
    void assign_init(const List& list, T* p)
    {
      copy_from_init<N>::put(list, p);
    }


  // Return an element reference for T (possibly const).
  template <typename T>
    using Element_reference = If<Const<T>(), const T&, T&>;


  // Determines the row type of a matrix. If N is 0, then the row type is
  // simply the scalar type T. Otherwise, the row type is a row matrix of
  // the given dimension.
  template <typename T, std::size_t N>
    using Row_type = If<(N == 0), Element_reference<T>, matrix_ref<T, N>>;



  // Offset
  //
  // Returns the address of the nth element in a table whose element counts
  // are given in the range [first, last). That is, each value in that range
  // denotes the number of contiguous elements in that table. 
  //
  // Note that these functions are defined over iterators, but they're
  // expected to be pointers.

  // This overload is specifically for 1D matrices, hence the single
  // index n. Note that size + 1 is PTE of the sizes array owned by the
  // calling Matrix.
  template <typename Size>
    inline Size
    offset(const Size* size, Size n)
    {
      assert(n < *size);
      return n;
    }

  // Here, {n, args...} are the indexes of the requested element. Note that
  // size + 1 + sizeof...(Args) is PTE of the sizes array owned by the shape 
  // of the calling matrix.
  template <typename Size, typename... Args>
    inline Size
    offset(const Size* size, Size n, Args... args)
    {
      assert(n * *(size + 1) < *size);
      ++size;
      return (n * *size) + offset(size, args...);
    }



  // Row
  //
  // Returns a reference to the nth row of the matrix m, with base being the
  // originating matrix.
  //
  // FIXME: It would be nice to use Row_reference<M> and Row_refernce<const M> 
  // in these algorithms.
  template <typename M>
    inline Requires<(M::order() != 1), typename M::row_reference>
    row(M& m, Size_type<M> n)
    {
      return {m.shape().inner(), m.data() + n * m.shape().size(1)};
    }

  template <typename M>
    inline Requires<(M::order() != 1), typename M::const_row_reference>
    row(const M& m, Size_type<M> n)
    {
      return {m.shape().inner(), m.data() + n * m.shape().size(1)};
    }

  // Specializations for the case where N == 1
  template <typename M>
    inline Requires<(M::order() == 1), typename M::row_reference>
    row(M& m, Size_type<M> n)
    {
      return *(m.data() + n);
    }

  template <typename M>
    inline Requires<(M::order() == 1), typename M::const_row_reference>
    row(const M& m, Size_type<M> n)
    {
      return *(m.data() + n);
    }

} // namespace matrix_impl

