// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#define ORIGIN_MATH_MATRIX_HPP

#include <iostream>
#include <algorithm>
#include <cassert>

#include <origin/type/concepts.hpp>
#include <origin/sequence/algorithm.hpp>

namespace origin
{
  // Declarations
  template <typename T, std::size_t N>
    class matrix;

  template <typename T, std::size_t N>
    class matrix_ref;

  template <typename T, std::size_t N>
    class matrix_shape;



  //////////////////////////////////////////////////////////////////////////////
  // Matrix Shape
  //
  // The shape class describes the shape of an array: its number of dimensions 
  // (or order) and the number of elements (extent) contained in each dimension.
  // The order of the shape is specified by the template parameter N.
  //
  // NOTE: The traditonal C/C++ name for number of dimensions is traditionally
  // called its rank. However, that name has special meaning in the context
  // of linear algebra, so we use the name "order" instead.
  template <typename T, std::size_t N>
    class matrix_shape
    {
      static_assert(Integer<T>(), "");
    public:
      using value_type = T;

      // Construct an "empty" matrix shape such that a matrix with this shape
      // contains 0 elements in each dimension.
      matrix_shape();

      // Construct a matrix shape with the extents given by the sequence of
      // arguments.
      //
      // FIXME: Write type requirements (must be the same as T? Convertible?).
      // Also, sizeof(Exts...) == N?
      template <typename... Exts,
                typename = Requires<All(Convertible<Exts, T>()...)>>
        matrix_shape(Exts... es);

      // Construct a matrix shape with the extents given in the specified
      // initializer list. The size of the list must be the same as the order
      // of the shape (N).
      matrix_shape(std::initializer_list<value_type> list);

      // Construct a matrix over the extents given in the range [first, last).
      // The size of the [first, last) must be the same as the order of the
      // shape (N).
      template <typename I, typename = Requires<Input_iterator<I>()>>
        matrix_shape(I first, I last);


      // Properties

      // Returns the order of the shape.
      static constexpr value_type order() { return N; }

      // Returns the number of sub-matrices contained in the nth dimension. If 
      // a specific dimension is not given, then returns the number of rows.
      value_type extent(std::size_t i = 0) const;


      // Returns the total number of elements contained in a submatrix of the
      // ith dimension. When i == 0, this is the total number of elements in
      // the matrix. When i == 1, this is the total number of elementsm in
      // a row the matrix, etc. If i is not given, the function returns the
      // total number of elements in the table.
      value_type size(std::size_t i = 0) const;


      // Returns the shape of a row described by this shape.
      matrix_shape<value_type, N - 1> inner() const;


      // Data accessors

      // Returns a pointer to the array of extents.
      const value_type* extents() const { return exts; }

      // Returns a pointer to the array of sizes.
      const value_type* sizes() const { return elems; }


      // Mutators
      void swap(matrix_shape& elems);

    private:
      void count();

    private:
    // The extents of each dimension.
    T exts[N];
      

    // Number of elements stored in each sub-extent. Without storing
    // this, we would have to recompute the same products every time
    // we indexed into the array.
    //
    // TODO: We can effectively remove this when the N is small and
    // recompute anyways. It may be end up being faster.
    T elems[N];
    };


  template <typename T, std::size_t N>
    inline
    matrix_shape<T, N>::matrix_shape()
    { }

  template <typename T, std::size_t N>
    template <typename... Exts, typename Req>
      inline
      matrix_shape<T, N>::matrix_shape(Exts... es)
        : exts{T(es)...}
      {
        static_assert(sizeof...(Exts) == N, "");
        count();
      }

  template <typename T, std::size_t N>
    inline 
    matrix_shape<T, N>::matrix_shape(std::initializer_list<value_type> list)
    {
      assert(list.size() == N);
      std::copy(list.begin(), list.end(), exts);
      count();
    }

  template <typename T, std::size_t N>
    template <typename I, typename X>
      inline
      matrix_shape<T, N>::matrix_shape(I first, I last)
      {
        assert(std::distance(first, last) == N);
        std::copy(first, last, exts);
        count();
      }


  template <typename T, std::size_t N>
    inline auto
    matrix_shape<T, N>::extent(std::size_t i) const -> value_type
    { 
      assert(i < N);
      return exts[i]; 
    }

  template <typename T, std::size_t N>
    inline auto
    matrix_shape<T, N>::size(std::size_t i) const -> value_type
    {
      assert(i < N);
      return elems[i];
    }

  // TODO: Is there a better name for this function?
  template <typename T, std::size_t N>
    inline auto
    matrix_shape<T, N>::inner() const -> matrix_shape<value_type, N - 1>
    { 
      static_assert(N > 1, "");
      return {exts + 1, exts + N}; 
    }

  template <typename T, std::size_t N>
    inline void
    matrix_shape<T, N>::swap(matrix_shape& x)
    {
      using std::swap;
      swap(exts, x.exts);
      swap(elems, x.elems);
    }

  // Pre-compute the number of elements for each sub-table described by the
  // shape. Note that N must not be 0.
  template <typename T, std::size_t N>
    inline void 
    matrix_shape<T, N>::count()
    {
      if (N == 0)
        return;
      T i = N - 1;
      elems[i] = exts[i];
      if (N == 1)
        return;      
      do {
        --i;
        elems[i] = exts[i] * elems[i + 1];
      } while (i != 0);
    }


  // Equality comparable
  // Two shapes compare equal when they have the same order and extents.
  //
  // Note that we do not compare sizes since they are computed from the extents
  // of the shape (that array exists only as an optimization).
  //
  // TODO: This definition not allow us to compare shapes of different order.
  // The operation is not defined. Should it be? Obviously, it would just 
  // be false. The implication is that each shape type is part of a local
  // group.
  template <typename T, std::size_t N>
    inline bool
    operator==(const matrix_shape<T, N>& a, const matrix_shape<T, N>& b)
    {
      for (std::size_t i = 0; i < N; ++i)
        if (a.extent(i) != b.extent(i))
          return false;
      return true;
    }

  template <typename T, std::size_t N>
    inline bool
    operator!=(const matrix_shape<T, N>& a, const matrix_shape<T, N>& b)
    {
      return !(a == b);
    }



// Include traits needed to define the following type functions.
#include "matrix.impl/traits.hpp"


  // An alias to the reference-to-row type of the matrix, M. The type denoted
  // by this alias depends on the const-ness and order of M. When the order of
  // M is 1, this type is the same as the matrice's reference-to-eleemnt type.
  // Otherwise, the type is an instance of the matrix_ref class template.
  // Examples of usage and results are:
  //
  //    using M1 = matrix<T, 1>;
  //    using M2 = matrix<T, 2>;
  //    Matrix_row_reference
  //
  // Note that M can be any type modeling the Matrix concept. If M does not
  // satisfy the requirements of the Matrix concept, then the resulting type
  // indicates substitution failure.
  template <typename M>
    using Matrix_row_reference =
      If<Const<M>(),
        typename matrix_impl::get_associated_row_reference<M>::type,
        typename matrix_impl::get_associated_const_row_reference<M>::type>;



  // An alias to the shape type associated with a matrix.
  template <typename M>
    using Matrix_shape_type = typename matrix_impl::get_shape_type<M>::type;


  // Returns true if T is a matrix.
  //
  // FIXME: A matrix is substantially more complex than this. Finish defining
  // and implementing the concept.
  template <typename M>
    constexpr bool Matrix()
    {
      return Subst_succeeded<Matrix_shape_type<M>>();
    }


  // The matrix initializer is a sequence of nested initializer lists that
  // describes the initailization structure of an N dimensional matrix. For
  // example, an initializer for a 2D matrix of ints allows us to construct 
  // the following initializer list:
  //
  //    {{0, 1},
  //     {2, 3},
  //     {4, 5}}
  //
  // Assuming we have a 3x2 matrix.
  template <typename T, std::size_t N>
    using Matrix_initializer = typename matrix_impl::matrix_init<T, N>::type;


// Include additionl support operations and traits.
#include "matrix.impl/support.hpp"



  //////////////////////////////////////////////////////////////////////////////
  // Matrix
  //
  // The matrix template specifies an implementaiton of an N-dimensional
  // matrix parameterized over some value type T.
  //
  // The matrix is also parameterized over an underlying Storage type which
  // is responsible for the allocation and ownership of the underlying value
  // types. Storage is required to be a Random_access_container. 
  //
  // NOTE: The matrix class is not parameterized over an allocator, simply
  // for the reason that that feature has not yet been implemented. 
  template <typename T, std::size_t N>
    class matrix
    {
      using this_type           = matrix<T, N>;
      using store_type          = std::vector<T>;
    public:
      using allocator_type      = typename store_type::allocator_type;
      using value_type          = typename store_type::value_type;
      using reference           = typename store_type::reference;
      using const_reference     = typename store_type::const_reference;
      using pointer             = typename store_type::pointer;
      using const_pointer       = typename store_type::const_pointer;
      using size_type           = typename store_type::size_type;
      using difference_type     = typename store_type::difference_type;

      using shape_type          = matrix_shape<size_type, N>;
      using row_reference       = matrix_impl::Row_type<T, N - 1>;
      using const_row_reference = matrix_impl::Row_type<const T, N - 1>;

      using iterator            = typename store_type::iterator;
      using const_iterator      = typename store_type::const_iterator;


      // Default construction
      // Allocates a 
      explicit matrix(const allocator_type& a = {});


      // Move semantics
      // Moving a matrix transfers the underlying state of the object x into
      // this object, leaving x empty.
      matrix(matrix&& x, const allocator_type& alloc = {});
      matrix& operator=(matrix&& x);


      // Copy semantics
      matrix(matrix const& x, const allocator_type& alloc = {});
      matrix& operator=(matrix const& x);


      // Conversion from matrix_ref
      matrix(const matrix_ref<this_type, N>& x, const allocator_type& alloc = {});
      matrix& operator=(const matrix_ref<this_type, N>& x);


#if ORIGIN_MATRIX_USE_SHAPE_CTOR
      // Construct a matrix of the specified shape.
      explicit
      matrix(const shape_type& shape, 
             const value_type& value = {},
             const allocator_type& alloc = {});

      // Refinement of the shape constructor for 1-D matrices. This allows
      // intialization over a single integer value.
      template <typename Int>
        explicit 
        matrix(Int n,
               const value_type& value = {},
               const allocator_type& alloc = {},
               matrix_impl::Requires_1D<Int, N>* = {});
#else
      // TODO: Create overloads that allow the specification of a default
      // value as the last argument and one that allows the specification of
      // a default value and an allocator as the last pairs of arguments.
      template <typename... Exts>
        explicit
        matrix(Exts... exts);

      // Disable the initializer list constructor.
      template <typename U>
        matrix(std::initializer_list<U> list) = delete;
#endif

      matrix(Matrix_initializer<T, N> init, const allocator_type& alloc = {});
      matrix& operator=(Matrix_initializer<T, N> init);


      // Properties

      // Returns the shape of the matrix.
      const shape_type& shape() const { return dims; }

      // Returns the order of the matrix.
      static constexpr size_type order() { return N; }

      // Returns the extent of the matrix in the nth dimension.
      size_type extent(size_type n) const { return dims.extent(n); }

      // Returns the total number of elements contained in the matrix.
      size_type size() const { return elems.size(); }


      // Element access
      template <typename... Args>
        reference operator()(Args... args);
      template <typename... Args>
        const_reference operator()(Args... args) const;


      // Row access
      row_reference       operator[](size_type n);
      const_row_reference operator[](size_type n) const;


      // Data access
      pointer       data()       { return elems.data(); }
      const_pointer data() const { return elems.data(); }


      // Vector addition
      template <typename M>
        Requires<Matrix<M>(), matrix&> operator+=(const M& x);
      template <typename M>
        Requires<Matrix<M>(), matrix&> operator-=(const M& x);

      // Scalar addition
      matrix& operator=(const value_type& value);
      matrix& operator+=(const value_type& value);
      matrix& operator-=(const value_type& value);

      // Scalar multiplication
      matrix& operator*=(const value_type& value);
      matrix& operator/=(const value_type& value);
      matrix& operator%=(const value_type& value);


      // Iterators
      iterator begin() { return elems.begin(); }
      iterator end()   { return elems.end(); }

      const_iterator begin() const { return elems.begin(); }
      const_iterator end() const   { return elems.end(); }

      // Mutators
      void swap(matrix& x);

      private:
        shape_type dims;
        store_type elems;
      };


  template <typename T, std::size_t N>
    inline
    matrix<T, N>::matrix(const allocator_type& a)
      : dims(), elems()
    { }

  template <typename T, std::size_t N>
    matrix<T, N>::matrix(matrix&& x, const allocator_type& alloc)
      : dims(std::move(x.dims)), elems(std::move(x.elems), alloc)
    { }
    
  template <typename T, std::size_t N>
    inline auto
    matrix<T, N>::operator=(matrix&& x) -> matrix&
    {
      dims = std::move(x.dims);
      elems = std::move(x.elems);
      return *this;
    }

  template <typename T, std::size_t N>
    inline
    matrix<T, N>::matrix(matrix const& x, const allocator_type& alloc)
      : dims(x.dims), elems(x.elems, alloc)
    { }

  template <typename T, std::size_t N>
    inline auto
    matrix<T, N>::operator=(matrix const& x) -> matrix&
    { 
      dims = x.dims;
      elems = x.elems;
      return *this;
    }


  template <typename T, std::size_t N>
    inline
    matrix<T, N>::matrix(const matrix_ref<this_type, N>& x, 
                         const allocator_type& alloc)
      : dims(x.shape()), elems(x.begin(), x.end(), alloc)
    { }

  template <typename T, std::size_t N>
    inline auto
    matrix<T, N>::operator=(const matrix_ref<this_type, N>& x) -> matrix&
    {
      dims = x.shape();
      elems.assign(x.begin(), x.end());
    }

#ifdef ORIGIN_MATRIX_USE_SHAPE_CTOR
  template <typename T, std::size_t N>
    inline
    matrix<T, N>::matrix(const shape_type& shape, 
                         const value_type& value,
                         const allocator_type& alloc)
      : dims(shape), elems(shape.size(), value, alloc)
    { }

  template <typename T, std::size_t N>
    template <typename Int>
      inline
      matrix<T, N>::matrix(Int n, 
                           const value_type& value, 
                           const allocator_type& alloc,
                           matrix_impl::Requires_1D<Int, N>*)
        : dims(size_type(n)), elems(n, value, alloc)
      { }
#else
  template <typename T, std::size_t N>
    template <typename... Exts>
      inline
      matrix<T, N>::matrix(Exts... exts)
        : dims(exts...), elems(dims.size())
      { }
#endif


  // FIXME: This is not as efficient as it could be since it resizes and then
  // copies. It might be done efficiently if we could simply compute the bounds
  // of the flattened initializer structure. The begin iterator is the address
  // of the first element, and the end iterator is referenced by the first plus
  // the product of all the extents.
  template <typename T, std::size_t N>
    inline
    matrix<T, N>::matrix(Matrix_initializer<T, N> init, const allocator_type& alloc)
      : dims(matrix_impl::shape<N, size_type>(init))
    {
      elems.resize(dims.size());
      matrix_impl::assign_init<N>(init, elems.data());
    }

  template <typename T, std::size_t N>
    inline auto
    matrix<T, N>::operator=(Matrix_initializer<T, N> init) -> matrix&
    {
      matrix tmp(init);
      swap(tmp);
      return *this;
    }

  // FIXME: Write type constraints on Args.
  template <typename T, std::size_t N>
    template <typename... Args>
      inline auto
      matrix<T, N>::operator()(Args... args) -> reference
      {
        // Explicitly convert to size_type so that all of the argument types
        // will be the same when calling address. That will make it easy to
        // write optimizations against common cases later on.
        size_type off = matrix_impl::offset(dims.sizes(), size_type(args)...);
        return *(data() + off);
      }

  template <typename T, std::size_t N>
    template <typename... Args>
      inline auto
      matrix<T, N>::operator()(Args... args) const -> const_reference
      {
        size_type off = matrix_impl::offset(dims.sizes(), size_type(args)...);
        return *(data() + off);
      }

  template <typename T, std::size_t N>
    inline auto
    matrix<T, N>::operator[](size_type n) -> row_reference
    {
      return matrix_impl::row(*this, n);
    }


  template <typename T, std::size_t N>
    inline auto
    matrix<T, N>::operator[](size_type n) const -> const_row_reference
    {
      return matrix_impl::row(*this, n);
    }


  // Matrix addition
  // One matrix can be added to another (elementwise) only when they have the
  // same shape.
  template <typename T, std::size_t N>
    template <typename M>
      inline Requires<Matrix<M>(), matrix<T, N>&>
      matrix<T, N>::operator+=(const M& x)
      {
        static_assert(order() == M::order(), "");
        assert(shape() == x.shape());

        matrix_impl::plus_assign<value_type> plus_eq;
        matrix_impl::apply_each(begin(), end(), x.begin(), plus_eq);
        return *this;
      }

  template <typename T, std::size_t N>
    template <typename M>
      inline Requires<Matrix<M>(), matrix<T, N>&>
      matrix<T, N>::operator-=(const M& x)
      {
        static_assert(order() == M::order(), "");
        assert(shape() == x.shape());

        matrix_impl::minus_assign<value_type> minus_eq;
        matrix_impl::apply_each(begin(), end(), x.begin(), minus_eq);
        return *this;
      }


  // Scalar addition
  // A scalar can be added to a matrix, adding that value to each element of
  // the matrix.
  template <typename T, std::size_t N>
    inline matrix<T, N>& 
    matrix<T, N>::operator=(const value_type& value)  
    { 
      matrix_impl::assign<value_type> assign;
      matrix_impl::apply(begin(), end(), value, assign);
      return *this;
    }
      
  template <typename T, std::size_t N>
    inline matrix<T, N>& 
    matrix<T, N>::operator+=(const value_type& value) 
    { 
      matrix_impl::plus_assign<value_type> add_eq;
      matrix_impl::apply(begin(), end(), value, add_eq);
      return *this;
    }
      
  template <typename T, std::size_t N>
    inline matrix<T, N>& 
    matrix<T, N>::operator-=(value_type const& value) 
    {
      matrix_impl::minus_assign<value_type> sub_eq;
      matrix_impl::apply(begin(), end(), value, sub_eq); 
      return *this;
    }
  

  // Scalar multiplication
  // A matrix can be multiplied by a scalar, scaling each element by that
  // value.
  template <typename T, std::size_t N>
    inline matrix<T, N>& 
    matrix<T, N>::operator*=(value_type const& value) 
    { 
      matrix_impl::multiplies_assign<value_type> mul_eq;
      matrix_impl::apply(begin(), end(), value, mul_eq);
      return *this;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>& 
    matrix<T, N>::operator/=(value_type const& value) 
    { 
      matrix_impl::divides_assign<value_type> div_eq;
      matrix_impl::apply(begin(), end(), value, div_eq);
      return *this;
    }
      
  // This operation is only defined when T is a model of Euclidean domain.
  template <typename T, std::size_t N>
    inline matrix<T, N>& 
    matrix<T, N>::operator%=(value_type const& value) 
    { 
      matrix_impl::modulus_assign<value_type> mod_eq;
      matrix_impl::apply(begin(), end(), value, mod_eq);
      return *this;
    }

  template <typename T, std::size_t N>
    inline void
    matrix<T, N>::swap(matrix& x)
    {
      using std::swap;
      swap(dims, x.dims);
      swap(elems, x.elems);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Matrix Reference
  //
  // A matrix ref is a reference to a contiguous sub-matrix with a specified
  // shape. The matrix ref does not own its elements.
  //
  // This is closely related to the notion of a slice or a submatrix except
  // that all elements are contiguously allocated and define a complete matrix.
  // Indexing into a matrix row does not require strides or spans to move from
  // one element to the next.
  //
  // The matrix row class is a Matrix except that assignment writes through
  // (not to) the matrix reference to the referenced data.
  //
  // Template parameters:
  //    T -- The underlying value type of the matrix, possibly const. If T is
  //         const, then the matrix reference becomes read only. All references
  //         and pointers returned from a const matrix reference are also
  //         const.
  //    N -- The number of dimensions of this sub-matrix.
  // 
  // TODO: When matrix<T, N> gets an allocator, this class will need to get
  // an allocator also.
  template <typename T, std::size_t N>
    class matrix_ref
    {
      using this_type = matrix_ref<T, N>;
    public:
      using value_type          = Remove_const<T>;
      using allocator_type      = std::allocator<value_type>;
      using reference           = matrix_impl::Matrix_ref<T, allocator_type>;
      using const_reference     = typename allocator_type::const_reference;
      using pointer             = matrix_impl::Matrix_ptr<T, allocator_type>;
      using const_pointer       = typename allocator_type::const_pointer;
      using size_type           = typename allocator_type::size_type;
      using difference_type     = typename allocator_type::difference_type;

      using shape_type          = matrix_shape<size_type, N>;
      using row_reference       = matrix_impl::Row_type<value_type, N - 1>;
      using const_row_reference = matrix_impl::Row_type<const value_type, N - 1>;

      using iterator            = pointer;
      using const_iterator      = const_pointer;


      // Move and copy semantics
      // Are implicit. Moves are are the same copies, and copies are shallow.
      // This class does not implemnt value semantics.


      // A matrix reference cannot be constructed over a matrix rvalue. That
      // would leak memory (at best).
      //
      // The Remove_const ensures that we don't try to instantiate a matrix
      // with a const value type. That causes compiler errors with allocators.
      matrix_ref(matrix<Remove_const<T>, N>&&) = delete;
      matrix_ref& operator=(matrix<Remove_const<T>, N>&&) = delete;


      // A matrix can be copy construced or assigned to a matrix lvalue. The
      // lifetime of the matrix is longer than that of the reference.
      matrix_ref(matrix<Remove_const<T>, N>& x);
      matrix_ref& operator=(matrix<Remove_const<T>, N>& x);

      matrix_ref(const matrix<Remove_const<T>, N>& x);
      matrix_ref& operator=(const matrix<Remove_const<T>, N>& x);


      // Properties

      // Initialize the submatrix with the given shape.
      matrix_ref(const shape_type& s, pointer p);

      // Returns the shape of the matrix.
      const shape_type& shape() const { return dims; }

      // Returns the order of the matrix
      static constexpr size_type order() { return N; }

      // Returns the extent of the matrix in the nth dimension.
      size_type extent(size_type n) const { return dims.extent(n); }

      // Returns the total number of elements contained in the matrix.
      size_type size() const { return dims.elements(); }


      // Element access
      template <typename... Args>
        reference operator()(Args... args);
      template <typename... Args>
        const_reference operator()(Args... args) const;


      // Row access
      row_reference       operator[](size_type n);
      const_row_reference operator[](size_type n) const;


      // Data access
      pointer       data()       { return ptr; }
      const_pointer data() const { return ptr; }


      // Matrix addition
      template <typename M>
        Requires<Matrix<M>(), matrix_ref&> operator+=(const M& x);
      template <typename M>
        Requires<Matrix<M>(), matrix_ref&> operator-=(const M& x);

      // Scalar addition
      matrix_ref& operator=(const value_type& value);
      matrix_ref& operator+=(const value_type& value);
      matrix_ref& operator-=(const value_type& value);

      // Scalar multiplication
      matrix_ref& operator*=(const value_type& value);
      matrix_ref& operator/=(const value_type& value);
      matrix_ref& operator%=(const value_type& value);

      // Iterators
      iterator begin() { return ptr; }
      iterator end()   { return ptr + dims.size(); }

      const_iterator begin() const { return ptr; }
      const_iterator end() const   { return ptr + dims.size(); }

    private:
      shape_type dims;  // The shape of the matrix
      pointer ptr;      // The 1st element in the matrix
    };

  // Allows binding to non-const references.
  template <typename T, std::size_t N>
    matrix_ref<T, N>::matrix_ref(matrix<Remove_const<T>, N>& x) 
      : dims(x.shape()), ptr(x.data())
    { }

  template <typename T, std::size_t N>
    inline auto 
    matrix_ref<T, N>::operator=(matrix<Remove_const<T>, N>& x) 
      -> matrix_ref& 
    {
      dims = x.shape();
      ptr = x.data();
      return *this;
    }

  // Allows binding to const references.
  template <typename T, std::size_t N>
    matrix_ref<T, N>::matrix_ref(const matrix<Remove_const<T>, N>& x) 
      : dims(x.shape()), ptr(x.data())
    { }

  template <typename T, std::size_t N>
    inline auto 
    matrix_ref<T, N>::operator=(const matrix<Remove_const<T>, N>& x) 
      -> matrix_ref& 
    {
      dims = x.shape();
      ptr = x.data();
      return *this;
    }


  template <typename T, std::size_t N>
    inline
    matrix_ref<T, N>::matrix_ref(const shape_type& s, pointer p)
      : dims(s), ptr(p)
    { }

  // FIXME: Write type requirements.
  template <typename T, std::size_t N>
    template <typename... Args>
      inline auto
      matrix_ref<T, N>::operator()(Args... args) -> reference
      {
        size_type off = matrix_impl::offset(dims.sizes(), size_type(args)...);
        return *(ptr + off);
      }

  template <typename T, std::size_t N>
    template <typename... Args>
      inline auto
      matrix_ref<T, N>::operator()(Args... args) const -> const_reference
      {
        size_type off = matrix_impl::offset(dims.sizes(), size_type(args)...);
        return *(ptr + off);
      }

  template <typename T, std::size_t N>
    inline auto
    matrix_ref<T, N>::operator[](size_type n) -> row_reference
    {
      return matrix_impl::row(*this, n);
    }

  template <typename T, std::size_t N>
    inline auto
    matrix_ref<T, N>::operator[](size_type n) const -> const_row_reference
    {
      return matrix_impl::row(*this, n);
    }


  // Matrix addition
  template <typename T, std::size_t N>
    template <typename M>
      inline Requires<Matrix<M>(), matrix_ref<T, N>&>
      matrix_ref<T, N>::operator+=(const M& x)
      {
        static_assert(order() == M::order(), "");
        assert(shape() == x.shape());

        matrix_impl::plus_assign<value_type> add_eq;
        matrix_impl::apply_each(begin(), end(), x.begin(), add_eq);
        return *this;
      }

  template <typename T, std::size_t N>
    template <typename M>
      inline Requires<Matrix<M>(), matrix_ref<T, N>&>
      matrix_ref<T, N>::operator-=(const M& x)
      {
        static_assert(order() == M::order(), "");
        assert(shape() == x.shape());

        matrix_impl::minus_assign<value_type> sub_eq;
        matrix_impl::apply(begin(), end(), x.begin(), sub_eq);
        return *this;
      }

  // Scalar addition
  template <typename T, std::size_t N>
    inline matrix_ref<T, N>& 
    matrix_ref<T, N>::operator=(const value_type& value)  
    { 
      matrix_impl::assign<value_type> assign;
      matrix_impl::apply(begin(), end(), value, assign);
      return *this;
    }
      
  template <typename T, std::size_t N>
    inline matrix_ref<T, N>& 
    matrix_ref<T, N>::operator+=(const value_type& value) 
    { 
      matrix_impl::apply(
        begin(), end(), value, matrix_impl::plus_assign<value_type>{}); 
      return *this;
    }
      
  template <typename T, std::size_t N>
    inline matrix_ref<T, N>& 
    matrix_ref<T, N>::operator-=(value_type const& value) 
    { 
      matrix_impl::minus_assign<value_type> sub_eq;
      matrix_impl::apply(begin(), end(), value, sub_eq); 
      return *this;
    }


  // Scalar multiplication
  template <typename T, std::size_t N>
    inline matrix_ref<T, N>& 
    matrix_ref<T, N>::operator*=(value_type const& value) 
    { 
      matrix_impl::minus_assign<value_type> mul_eq;
      matrix_impl::apply(begin(), end(), value, mul_eq); 
      return *this;
    }

  template <typename T, std::size_t N>
    inline matrix_ref<T, N>& 
    matrix_ref<T, N>::operator/=(value_type const& value) 
    { 
      matrix_impl::minus_assign<value_type> div_eq;
      matrix_impl::apply(begin(), end(), value, div_eq); 
      return *this;
    }

  // This operation is only defined when T is a model of Euclidean domain.
  template <typename T, std::size_t N>
    inline matrix_ref<T, N>& 
    matrix_ref<T, N>::operator%=(value_type const& value) 
    { 
      matrix_impl::minus_assign<value_type> div_eq;
      matrix_impl::apply(begin(), end(), value, div_eq); 
      return *this;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Matrix Operations
  //
  // The following operations are defined for all Matrix types.
  //////////////////////////////////////////////////////////////////////////////




  // Returns the number of rows in a matrix with rank > 1. The number of rows
  // is the same as the extent in the 1st dimension.
  template <typename M, typename = Requires<Matrix<M>()>>
    inline Size_type<M> rows(const M& m)
    {
      static_assert(M::order() > 0, "");
      return m.extent(0);
    }


  // Returns the number of columns in a matrix with rank > 1. This number of
  // columns is the same as the extent in the 2nd dimension.
  template <typename M, typename = Requires<Matrix<M>()>>
    inline Size_type<M> cols(const M& m)
    {
      static_assert(M::order() > 0, "");
      return m.extent(1);
    }



  // Equality comparable
  // Two matrices compare equal when they have the same shape and elements.
  template <typename M1, typename M2>
    inline Requires<Matrix<M1>() && Matrix<M2>(), bool> 
    operator==(const M1& a, const M2& b)
    { 
      assert(a.shape() == b.shape());
      return range_equal(a, b);
    }
  
  template <typename M1, typename M2>
    inline Requires<Matrix<M1>() && Matrix<M2>(), bool> 
    operator!=(const M1& a, const M2& b)
    { 
      return !(a == b);
    }
      


  //
  // NOTE: This operation is kind of funny because it is heterogeneous in its
  // result type. If we try to concept check Matrix<R, R> (where R is a matrix
  // ref type), we would normally be asking for an operation a homogeneous
  // operator+(R,R)->R. That's not what we have.
  //
  // In order to check this concept, we have to weaken the result type. The
  // C++0x concepts required that the result be convertible to the argument
  // types. That doesn't work here because matrix is not convertible to matrix
  // ref. It's the other way around.
  //
  // The correct way to check this is to say that the result type must share
  // a common type with the domain type. That is, if U is the result type of the
  // expression t + t (with t having type T), then Common<T, U> must be true.
  //
  // NOTE: We may be able to generalize and differentiate the addition of 
  // matrices vs. matrix references in the the future. A fully general matrix
  // slice would have the same properties as matrix_ref w.r.t. to addition
  // and subtraction.

  
  //////////////////////////////////////////////////////////////////////////////
  // Matrix addition
  //
  // Adding two matrices with the same shape adds corresponding elements in
  // each operatand.
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix<T, N>& a, const matrix<T, N>& b)
    {
      assert(a.shape() == b.shape());
      matrix<T, N> result = a;
      return result += b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix_ref<T, N>& a, const matrix_ref<T, N>& b)
    {
      assert(a.shape() == b.shape());
      matrix<T, N> result = a;
      return result += b;
    }

  // Cross-type addition
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix<T, N>& a, const matrix_ref<T, N>& b)
    {
      assert(a.shape() == b.shape());
      matrix<T, N> result = a;
      return result += b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix_ref<T, N>& a, const matrix<T, N>& b)
    {
      assert(a.shape() == b.shape());
      matrix<T, N> result = a;
      return result += b;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Matrix subtraction
  //
  // Subtracting one matrix from another with the same shape subtracts
  // corresponding elements in each operatand.
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix<T, N>& a, const matrix<T, N>& b)
    {
      asssert(a.dim() == b.dim());
      matrix<T, N> result = a;
      return a -= b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix_ref<T, N>& a, const matrix_ref<T, N>& b)
    {
      assert(a.dim() == b.dim());
      matrix<T, N> result = a;
      return result -= b;
    }

  // Cross-type subtraction
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix<T, N>& a, const matrix_ref<T, N>& b)
    {
      assert(a.dim() == b.dim());
      matrix<T, N> result = a;
      return result -= b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix_ref<T, N>& a, const matrix<T, N>& b)
    {
      assert(a.dim() == b.dim());
      matrix<T, N> result = a;
      return result -= b;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Scalar addition
  //
  // Adding a scalar to a matrix adds that value to each element in the matrix.
  // The following operations are supported:
  //
  //    a + n
  //    n + a
  template <typename T, std::size_t N>
    inline matrix<T, N> 
    operator+(const matrix<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result += n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N> 
    operator+(const T& n, const matrix<T, N>& x)
    {
      matrix<T, N> result = x;
      return result += n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result += n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const T& n, const matrix_ref<T, N>& x)
    {
      matrix<T, N> result = x;
      return result += n;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Scalar subtraction
  //
  // A scalar can be subtracted from a matrix. It is equivalent to adding the
  // negation of the scalar. That is:
  //
  //    a - n <=> a + -n;
  //
  // It is not possible to subtract a matrix from a scalar.
  template <typename T, std::size_t N>
    inline matrix<T, N> 
    operator-(const matrix<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result += n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result -= n;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Scalar multiplication
  //
  // Multiplying a matrix by a scalar multiplies each element by the scalar
  // value. The following operations are supported:
  //
  //    a * n
  //    n * a
  //
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator*(const matrix<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result *= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator*(const T& n, const matrix<T, N>& x)
    {
      matrix<T, N> result = x;
      return result *= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator*(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result *= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator*(const T& n, const matrix_ref<T, N>& x)
    {
      matrix<T, N> result = x;
      return result *= n;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Scalar division
  //
  // A matrix can be divided by a scalar value. It is equivalent to multiplying
  // the matrix by the reciprocal of the scalar.
  //
  //    a / n <=> a * 1/n
  //
  // It is not possible to divide a scalar by a matrix.
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator/(const matrix<T, N>& a, const T& n)
    {
      matrix<T, N> result = a;
      return result /= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator/(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result /= n;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Scalar modulus
  //
  // Returns a matrix containing the remainder of each element divided by the
  // given scalar value.
  //
  // This operation is only available when T is an Integer type.
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator%(const matrix<T, N>& a, const T& n)
    {
      matrix<T, N> result = a;
      return result %= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator%(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result %= n;
    }


  // Declarations
  template <typename M1, typename M2, typename M3>
    void matrix_product(const M1&, const M2&, M3&);

  template <typename M1, typename M2, typename M3>
    void hadamard_product(const M1&, const M2&, M3&);



  //////////////////////////////////////////////////////////////////////////////
  // Matrix Multiplication
  //
  // Two 2D matrices a (m x p) and b (p x n) can be multiplied, resulting in a
  // matrix c (m x n). Note that the "inner" dimension of the operands must
  // be the same.
  template <typename T>
    inline matrix<T, 2>
    operator*(const matrix<T, 2>& a, const matrix<T, 2>& b) 
    {
      matrix<T, 2> result (a.dims());
      matrix_product(a, b, result);
      return result;
    }

  template <typename T>
    inline matrix<T, 2>
    operator*(const matrix_ref<T, 2>& a, const matrix_ref<T, 2>& b) 
    {
      matrix<T, 2> result (a.dims());
      matrix_product(a, b, result);
      return result;
    }

  // Cross product multiplication.
  template <typename T>
    inline matrix<T, 2>
    operator*(const matrix<T, 2>& a, const matrix_ref<T, 2>& b) 
    {
      matrix<T, 2> result (a.dims());
      matrix_product(a, b, result);
      return result;
    }

  template <typename T>
    inline matrix<T, 2>
    operator*(const matrix_ref<T, 2>& a, const matrix<T, 2>& b) 
    {
      matrix<T, 2> result (a.dims());
      matrix_product(a, b, result);
      return result;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Matrix Product
  //
  // The usual meaning of the operation.
  //
  // FIXME: This is not terribly efficient. Implement a more efficient algorithm
  // than this brute force version.
  //
  // FIXME: I'm not at all sure that this generalizes to n dimensions. It might
  // be the case that we want all M's to be 2 dimensions (as they are now!).
  template <typename M1, typename M2, typename M3>
    void 
    matrix_product(const M1& a, const M2& b, M3& out)
    {
      static_assert(M1::order() == 2, "");
      static_assert(M2::order() == 2, "");
      static_assert(M3::order() == 2, "");
      assert(cols(a) == rows(b));
      assert(rows(a) == rows(out));
      assert(cols(b) == cols(out));

      using Size = Size_type<M3>;

      for (Size i = 0; i != rows(a); ++i) {
        for (Size j = 0; j < cols(b); ++j) {
          for (Size k = 0; k < rows(b); ++k)
            out(i, j) += a(i, k) * b(k, j);
        }
      }
    }

  
  //////////////////////////////////////////////////////////////////////////////
  // Hadamard Product
  //
  // The hadamard product can be easly generalized to N-dimensional matrices 
  // since the operation is performed elementwise. The operands only need the
  // same shape.
  template <typename M1, typename M2, typename M3>
    void
    hadamard_product(const M1& a, const M2& b, M3& out)
    {
      assert(a.shape() == b.shape());
      assert(a.shape() == out.shape());

      using Mul = std::multiplies<Value_type<M1>>;
      std::transform(a.begin(), a.end(), b.begin(), out.begin(), Mul{});
    }



  //////////////////////////////////////////////////////////////////////////////
  // Streaming
  //
  // Write the matrix to the the given output stream.
  template <typename C, typename T, typename M>
    inline Requires<Matrix<M>(), std::basic_ostream<C, T>&>
    operator<<(std::basic_ostream<C, T>& os, const M& m)
    {
      os << '[';
      for (std::size_t i = 0; i < rows(m); ++i) {
        os << m[i];
        if (i + 1 != rows(m))
          os << ',';
      }
      os << ']';
      return os;
    }

} // namespace origin

#endif
