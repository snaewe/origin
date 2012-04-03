// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_FRACTION_HPP
#define ORIGIN_FRACTION_HPP

#include <iosfwd>

#include <origin/concepts.hpp>
#include <origin/math.hpp>

namespace origin
{
  // FIXME: What are the actual requirements of T. Euclidean_domain?
  //
  // FIXME: Do I need to build a separate specialization of fractions for
  // rational numbers? What about rational functions?
  //
  // FIXME: User-defined literals.
  template <typename T>
    class fraction
    {
    public:
      using value_type = T;
    
      fraction() = default;

      // Copy and move provided by default.


      // Move conversion
      // If N can be converted to the value type, construct the fraction n/0.
      template<typename N, typename = Requires<Convertible<N, T>()>>
        fraction(N&& n) : num(std::move(n)), den(0)
        { }

      template <typename N, typename = Requires<Convertible<N, T>()>>  
        fraction& operator=(N&& n)
        {
          fraction tmp(std::move(n));
          swap(tmp);
          return *this;
        }

      // Copy conversion
      // If N can be converted to the value type, construct the fraction n/0.
      template<typename N, typename = Requires<Convertible<N, T>()>>
        fraction(const N& n) : num(n), den(0)
        { }

      template <typename N, typename = Requires<Convertible<N, T>()>>
        fraction& operator=(const N& n)
        {
          fraction tmp(n);
          swap(tmp);
          return *this;
        }


      // Value initialization
      fraction(value_type n, value_type d)
        : num(n), den(d)
      { reduce(); }


      void swap(fraction& x)
      {
        using std::swap;
        swap(num);
        swap(den);
      }

      // Accessors
      const value_type& numerator() const { return num; }
      const value_type& denominator() const { return den; }

      // Equality comparable
      bool operator==(const fraction& x) const;
      bool operator!=(const fraction& x) const { return !(*this == x); }


      // Totally ordered
      bool operator<(const fraction& x) const;
      bool operator>(const fraction& x) const  { return x < *this; }
      bool operator<=(const fraction& x) const { return !(x < *this); }
      bool operator>=(const fraction& x) const { return !(*this < x); }


      // Arithmetic operators
      fraction& operator+=(const fraction& x);
      fraction operator+(const fraction& x) const;

      fraction& operator-=(const fraction& x);
      fraction operator-(const fraction& x) const;

      fraction& operator*=(const fraction& x);
      fraction operator*(const fraction& x) const;

      fraction& operator/=(const fraction& x);
      fraction operator/(const fraction& x) const;

      fraction operator-() const;

      fraction abs() const;

    private:
      struct force { };

      // Forced initialization
      // Create a fraction with exactly the given numerator and denominator.
      // Do not not normalize the results. This is useful when need to create
      // fractions with specific values that are known to be in their normal
      // form.
      fraction(const value_type& n, const value_type& d, force)
        : num(n), den(d)
      { }

      // Return a scaling fractor for the given multiple. The scaling factor
      // is the number that we need to multiply by in order to write the
      // fraction in terms of the least common multiple, mul.
      value_type scale(const value_type& mul) const { return mul / den; }

      // Scale the numerator and denominator in terms of  the given multiple.
      value_type scale_num(const value_type& mul) const { return num * scale(mul); }
      value_type scale_den(const value_type& mul) const { return den * scale(mul); }

      void reduce();

    private:
      value_type num;
      value_type den;
    };



  template <typename T>
    inline bool fraction<T>::operator==(const fraction& x) const 
    { 
      return num == x.num && den == x.den; 
    }

  template <typename T>
    inline bool fraction<T>::operator<(const fraction& x) const
    {
      // FIXME: This is a somewhat expensive computation. Surely, there
      // must be a more efficient one.
      T m = lcm(den, x.denom);
      return scale_num(m) < x.scale_num(m);
    }

  template <typename T>
    auto fraction<T>::operator+=(const fraction& x) -> fraction&
    {
      // FIXME: This is a very expensive computation. Can we do better?
      T m = lcm(den, x.den);
      num = scale_num(m) + x.scale_num(m);
      den = scale_den(m);
      reduce();
      return *this;
    }

  template <typename T>
    auto fraction<T>::operator+(const fraction& x) const -> fraction
    {
      fraction tmp {*this};
      tmp += x;
      return tmp;
    }

  template <typename T>
    auto fraction<T>::operator-=(const fraction& x) -> fraction&
    {
      // FIXME: This is a very expensive computation. Can we do better?
      T m = lcm(den, x.den);
      num = scale_num(m) - x.scale_num(m);
      den = scale_den(m);
      reduce();
      return *this;
    }

  template <typename T>
    auto fraction<T>::operator-(const fraction& x) const -> fraction
    {
      fraction tmp {*this};
      tmp -= x;
      return tmp;
    }

  template <typename T>
    auto fraction<T>::operator*=(const fraction& x) -> fraction&
    {
      num *= x.num;
      den *= x.den;
      reduce();
      return *this;
    }

  template <typename T>
    auto fraction<T>::operator*(const fraction& x) const -> fraction
    {
      fraction tmp {*this};
      tmp *= x;
      return tmp;
    }

  template <typename T>
    auto fraction<T>::operator/=(const fraction& x) -> fraction&
    {
      num *= x.den;
      den *= x.num;
      reduce();
      return *this;
    }

  template <typename T>
    auto fraction<T>::operator/(const fraction& x) const -> fraction
    {
      fraction tmp {*this};
      tmp /= x;
      return tmp;
    }

  template <typename T>
    auto fraction<T>::operator-() const -> fraction
    {
      return {-num, den, force {}};
    }

  template <typename T>
    auto fraction<T>::abs() const -> fraction
    {
      using std::abs;
      return {abs(num), den, force {}};
    }



  // Put the fraction into a normalized form by reducing it and ensuring that
  // the denominator is always positive. A denominator of 0 results in 
  // undefined behavior.
  template <typename T>
    void fraction<T>::reduce()
    {
      assert(den != 0);

      // Reduce the fraction.
      T n = gcd(num, den);
      num /= n;
      den /= n;

      // Guarantee that the denominator is positive by swapping signs if
      // the denominator is negative.
      if (is_negative(den)) {
        num = -num;
        den = -den;
      }
    }


  
  // Numeric operations
  // Specializations of numeric operations for size.


  template <typename T>
    inline fraction<T> abs(const fraction<T>& x)
    {
      return x.abs();
    }

  template <typename T>
    inline int sgn(const fraction<T>& x)
    {
      return sgn(x.numerator());
    }

  template <typename T>
    inline bool is_negative(const fraction<T>& x)
    {
      return is_negative(x.numerator());
    }

  template <typename T>
    inline bool is_positive(const fraction<T>& x)
    {
      return is_positive(x.numerator());
    }

  template <typename T>
    inline bool is_non_negative(const fraction<T>& x)
    {
      return is_non_negative(x.numerator());
    }

  template <typename T>
    inline bool is_non_positive(const fraction<T>& x)
    {
      return is_non_positive(x.numerator());
    }

  // Output streamable
  template <typename C, typename CT, typename T>
    std::basic_ostream<C, CT>& 
    operator<<(std::basic_ostream<C, CT>& os, const fraction<T>& f)
    {
      return os << f.numerator() << '/' << f.denominator();
    }


  // Input streamable
  // FIXME: Implement me!
  template <typename C, typename CT, typename T>
    std::basic_istream<C, CT>& 
    operator<<(std::basic_istream<C, CT>& is, fraction<T>& f)
    {
      return is;
    }

} // namespace origin

#endif
