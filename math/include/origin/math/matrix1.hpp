// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATRIX1_HPP
#define ORIGIN_MATRIX1_HPP

#ifndef ORIGIN_MATRIX_COMMON_HPP
# error Do not include this file directly. Include <origin/matrix.hpp>.
#endif

namespace origin
{
  // The 1d matrix specialization is a row vector.
  //
  // Note that we support a 0-vector (a vector with no elements) as a special
  // case value.
  template<typename T, typename Alloc>
    class matrix<T, 1, Alloc> : protected matrix_base<T, Alloc>
    {
      typedef matrix_base<T, Alloc> base_type;
    public:
      typedef typename base_type::value_type value_type;
      typedef typename base_type::reference reference;
      typedef typename base_type::const_reference const_reference;
      typedef typename base_type::pointer pointer;
      typedef typename base_type::const_pointer const_pointer;
      typedef typename base_type::size_type size_type;

      // The default constructor creates a 0-element vector.
      matrix()
        : base_type()
      { }
      
      // Create an n-element vector
      explicit matrix(size_type n, value_type const& x = value_type{})
        : base_type(n)
      { 
        std::fill(begin(), end(), x); 
      }
      
      // Transform initialization.
      //
      // FIXME: These algorithms seem like they could use the apply() pattern, 
      // but their a little different since they don't pass this values to
      // transform function.
      
      // Initialize this matrix as the result of applying f(e) to each element
      // e in x.
      template<typename F>
        matrix(matrix const& x, F f)
          : base_type(x.size())
        {
          std::transform(x.begin(), x.end(), begin(), f);
        }

      // Initialize this matrix as the result of applying f(e, a) to each 
      // element e in x.
      template<typename F>
        matrix(matrix const& x, F f, value_type const& a)
          : base_type(x.size())
        {
          using namespace std::placeholders;
          std::transform(x.begin(), x.end(), begin(), std::bind(f, _1, a));
        }
      
      // Initialize this matrix as the result of applying f(e1, e2) to the 
      // elements e1, e2, in a and b element-wise. F is a binary operation.
      template<typename F>
        matrix(matrix const& a, matrix const& b, F f)
          : base_type(a.size())
        {
          assert(( a.size() == b.size() ));
          std::transform(a.begin(), a.end(), b.begin(), begin(), f);
        }

      // Movable
      matrix(matrix&& x)
        : base_type(std::move(x))
      { }
      
      matrix& operator=(matrix&& x)
      {
        matrix tmp(std::move(x));
        swap(tmp);
        return *this;
      }

      // Copyable
      matrix(matrix const& x)
        : base_type(x)
      { }
      
      matrix& operator=(matrix const& x) 
      { 
        matrix tmp(x); 
        swap(tmp); 
        return *this; 
      }

      // FIXME: Needs to be specialized for input iterators. I need to track
      // down the traits that were determining iterator properties.
      //
      // FIXME: Write a range constructor.
      template<typename Iter>
        matrix(Iter first, Iter last, 
               typename std::enable_if<is_iterator<Iter>::value>::type* = nullptr)
          : base_type(std::distance(first, last))
        {
          std::copy(first, last, begin());
        }
      
      // Initialize the matrix from the initializer list.
      matrix(std::initializer_list<value_type> list)
        : base_type(list.size())
      { 
        std::copy(list.begin(), list.end(), begin());
      }
      
      // Named values:
      
      // Return an n-element 0 vector where n is the size of this matrix.
      // FIXME: What if value_type{0} does not define the additive identity?
      matrix zero() const { return std::move(matrix(size(), value_type{0})); }
      
      // Properties
      size_type size() const { return base_type::size(); }
      
      // Data access
      pointer       data()       { return this->first; }
      const_pointer data() const { return this->first; }
      
      // Member access
      reference       operator[](size_type n)       { return get(n); }
      const_reference operator[](size_type n) const { return get(n); }
      
      reference       operator()(size_type n)       { return get(n); }
      const_reference operator()(size_type n) const { return get(n); }
      
      // Equality comparable
      bool operator==(matrix const& x) const { return std::equal(begin(), end(), x.begin()); }
      bool operator!=(matrix const& x) const { return !operator==(x); }
      
      // Assignment operations
      matrix& operator=(value_type const& x)  { return apply(assign<value_type>{}, x); }
      matrix& operator+=(value_type const& x) { return apply(plus_assign<value_type>{}, x); }
      matrix& operator-=(value_type const& x) { return apply(minus_assign<value_type>{}, x); }
      matrix& operator*=(value_type const& x) { return apply(multiplies_assign<value_type>{}, x); }
      matrix& operator/=(value_type const& x) { return apply(divides_assign<value_type>{}, x); }
      matrix& operator%=(value_type const& x) { return apply(modulus_assign<value_type>{}, x); }
      
      // Numeric operations: Matrix x Scalar
      //
      // Note that we don't follow the usual convention of writing the binary
      // operators in terms of the assignment operators. Instead, we rely on
      // special constructors to perform the computation during initialization.
      // Using the idiomatic solution, each operation would:
      //  1. allocate and default initialize
      //  2. copy the matrix x
      //  3. apply the operation
      // Using the right constructors, we skip step 2 and just transform a
      // directly into the result matrix.
      
      // Addition
      friend matrix operator+(matrix const& x, value_type const& a)
      {
        return std::move(matrix(x, std::plus<value_type>{}, a));
      }

      friend matrix operator+(value_type const& a, matrix const& x)
      {
        return std::move(matrix(x, std::plus<value_type>{}, a));
      }

      // Subtraction: only x - a
      friend matrix operator-(matrix const& x, value_type const& a)
      {
        return std::move(matrix(x, std::minus<value_type>{}, a));
      }

      // Multiplication: a * x and x * a
      friend matrix operator*(matrix const& x, value_type const& a)
      {
        return std::move(matrix(x, std::multiplies<value_type>{}, a));
      }

      friend matrix operator*(value_type const& a, matrix const& x)
      {
        return std::move(matrix(x, std::multiplies<value_type>{}, a));
      }

      // Division: only x / a
      friend matrix operator/(matrix const& x, value_type const& a)
      {
        return std::move(matrix(x, std::divides<value_type>{}, a));
      }

      // Modulus: only x % a
      friend matrix operator%(matrix const& x, value_type const& a)
      {
        return std::move(matrix(x, std::modulus<value_type>{}, a));
      }

      // Numeric operations: Matrix x Matrix
      //
      // 1d matrices only support addition and subtraction. Multiplication 
      // isn't supported since we don't have a clear specification of
      // dimensions in this specialization. The class needs to be wrapped as
      // either a row or column matrix in order to be multiplied.

      friend matrix operator+(matrix const& x, matrix const& y)
      {
        return std::move(matrix(x, y, std::plus<value_type>{}));
      }

      friend matrix operator-(matrix const& x, matrix const& y)
      {
        return std::move(matrix(x, y, std::minus<value_type>{}));
      }

      // Function application
      
      // for all i, m[i] = f(m[i])
      template<typename F>
        matrix& apply(F f)
        {
          std::transform(begin(), end(), begin(), f);
          return *this;
        }
      
      // for all i, m[i] = f(m[i], a)
      template<typename F>
        matrix& apply(F f, value_type const& a)
        {
          using namespace std::placeholders;
          std::transform(begin(), end(), begin(), std::bind(f, _1, a));
          return *this;
        }

      // Iterators
      pointer begin() { return this->first; }
      pointer end()   { return this->last; }

      const_pointer begin() const { return this->first; }
      const_pointer end() const   { return this->last; }      
      
      // Operations
      void swap(matrix& x) { base_type::swap(x); }
      
    private:
      reference get(size_type n)
      {
        assert(( n < size() ));
        return data()[n];
      }

      const_reference get(size_type n) const
      {
        assert(( n < size() ));
        return data()[n];
      }
    };
    
  // I/O Operators
  template<typename Char, typename Traits, typename T, typename Alloc>
    std::basic_ostream<Char, Traits>& 
    operator<<(std::basic_ostream<Char, Traits>& os, matrix<T, 1, Alloc> const& m)
    {
      typedef matrix<T, 1, Alloc> Matrix;
      os << '[';
      typename Matrix::size_type i = 0;
      while(i < m.size() - 1) {
        os << m[i] << ' ';
        ++i;
      }
      os << m[i] << ']';
      return os;
    }

} // namespace origin

#endif
