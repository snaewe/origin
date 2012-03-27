// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_REAL_MPFR_HPP
#define ORIGIN_REAL_MPFR_HPP

#include <mpfr.h>

#include <ios>
#include <cstring>
#include <string>

namespace origin
{
	// The rounding mode describes different rounding policies used by
	// a real number implementation.
	using round_t = mpfr_rnd_t;
	static constexpr round_t round_nearest_even = MPFR_RNDN;
	static constexpr round_t round_zero 			 	= MPFR_RNDZ;
	static constexpr round_t round_up 					= MPFR_RNDU;
	static constexpr round_t round_down 				= MPFR_RNDD;
	static constexpr round_t round_away 				= MPFR_RNDA;
	static constexpr round_t round_faithful  	  = MPFR_RNDF;
	static constexpr round_t round_nearest_away = MPFR_RNDNA;


	// This is defined strangely by GMP/MPFR. It could be short, int, or long.
	using precision_t = mpfr_prec_t;
	static constexpr precision_t single_precision 	 = 24;
	static constexpr precision_t double_precision 	 = 53;
	static constexpr precision_t quadruple_precision = 113;


  // The MPFR real number wraps the MPFR floating point type. This class is
  // parameterized over its default precision and rounding modes. The default
	// precision is quadruple precision, and all operations round to the nearest
	// value by default.
  //
  // FIXME: Parameterize over exceptions in a similar way.
  template <precision_t Precision = quadruple_precision, 
  					round_t Round = round_nearest_even>
		class real_impl
		{
			mpfr_t value;

		public:
			static constexpr precision_t precision = Precision;
			static constexpr round_t 		 rounding  = Round;

			// Default constructible
			real_impl();

			// Movable
			real_impl(real_impl&& x);
			real_impl& operator=(real_impl&& x);

			// Copyable
			real_impl(const real_impl& x);
			real_impl& operator=(const real_impl& x);

			// Conversion
			real_impl(double);
			
			// String construction
			explicit real_impl(const char* str, int base = 10);
			explicit real_impl(const std::string& str, int base = 10);

			// Destructible.
			~real_impl();

			// Equality_comparable
			bool operator==(const real_impl& x) const;
			bool operator!=(const real_impl& x) const;

			// Totally_ordered
			bool operator<(const real_impl& x) const;
			bool operator>(const real_impl& x) const;
			bool operator<=(const real_impl& x) const;
			bool operator>=(const real_impl& x) const;

			// Arithmetic
			real_impl& operator+=(const real_impl& x);
			real_impl operator+(const real_impl& x) const;
			
			real_impl& operator-=(const real_impl& x);
			real_impl operator-(const real_impl& x) const;

			real_impl& operator*=(const real_impl& x);
			real_impl operator*(const real_impl& x) const;

			real_impl& operator/=(const real_impl& x);
			real_impl operator/(const real_impl& x) const;

			real_impl operator-() const;
			
			// Numeric operations
			real_impl abs() const;

			void swap(real_impl& x);

			mpfr_t& 			impl() 			{ return value; }
			const mpfr_t& impl() const { return value; }

			constexpr mpfr_rnd_t rnd() const { return mpfr_rnd_t(rounding); }
		};



	template <precision_t P, round_t R>
		inline real_impl<P, R>::real_impl() 
		{ 
			mpfr_init(value); 
		}

	template <precision_t P, round_t R>
		inline real_impl<P, R>::real_impl(real_impl&& x)
		{
			mpfr_init(value);
			mpfr_swap(value, x.value);
		}

	template <precision_t P, round_t R>
		inline auto real_impl<P, R>::operator=(real_impl&& x) -> real_impl&
		{
			real_impl tmp(std::move(x));
			swap(tmp);
			return *this;
		}

	template <precision_t P, round_t R>
		inline real_impl<P, R>::real_impl(const real_impl& x)
		{
			mpfr_init_set(value, x.value, rnd());
		}
	
	template <precision_t P, round_t R>
		inline auto real_impl<P, R>::operator=(const real_impl& x) -> real_impl&
		{
			real_impl tmp(x);
			swap(tmp);
			return *this;
		}

	template <precision_t P, round_t R>
		inline real_impl<P, R>::real_impl(double n)
		{
			mpfr_init2(value, 128);
			mpfr_set_d(value, n, rnd());
			// mpfr_init_set_d(value, n, rnd());
		}

	template <precision_t P, round_t R>
		inline real_impl<P, R>::real_impl(const char *str, int base)
		{
			mpfr_init_set_str(value, str, base, rnd());
		}

	template <precision_t P, round_t R>
		inline real_impl<P, R>::real_impl(const std::string& str, int base)
		{
			mpfr_init_set_str(value, str.c_str(), base, rnd());
		}

	template <precision_t P, round_t R>
		inline real_impl<P, R>::~real_impl() 
		{
			mpfr_clear(value);
		}

	template <precision_t P, round_t R>
		inline void real_impl<P, R>::swap(real_impl& x)
		{
			mpfr_swap(value, x.value);
		}

	template <precision_t P, round_t R>
		inline bool real_impl<P, R>::operator==(const real_impl& x) const
		{
			return mpfr_cmp(value, x.value) == 0;
		}

	template <precision_t P, round_t R>
		inline bool real_impl<P, R>::operator!=(const real_impl& x) const
		{
			return !(*this == x);
		}

	template <precision_t P, round_t R>
		bool real_impl<P, R>::operator<(const real_impl& x) const
		{
			return mpfr_cmp(value, x.value) < 0;
		}

	template <precision_t P, round_t R>
		bool real_impl<P, R>::operator>(const real_impl& x) const
		{
			return mpfr_cmp(value, x.value) > 0;
		}

	template <precision_t P, round_t R>
		bool real_impl<P, R>::operator<=(const real_impl& x) const
		{
			return !(*this > x);
		}

	template <precision_t P, round_t R>
		bool real_impl<P, R>::operator>=(const real_impl& x) const
		{
			return !(*this < x);
		}

	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator+=(const real_impl& x) -> real_impl&
		{
			mpfr_add(value, value, x.value, rnd());
			return *this;
		}
		
	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator+(const real_impl& x) const -> real_impl
		{
			real_impl tmp;
			mpfr_add(tmp.value, value, x.value, rnd());
			return tmp;
		}
		
	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator-=(const real_impl& x) -> real_impl&
		{
			mpfr_sub(value, value, x.value, rnd());
			return *this;
		}
		
	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator-(const real_impl& x) const -> real_impl
		{
			real_impl tmp;
			mpfr_sub(tmp.value, value, x.value, rnd());
			return tmp;
		}

	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator*=(const real_impl& x) -> real_impl&
		{
			mpfr_mul(value, value, x.value, rnd());
			return *this;
		}

	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator*(const real_impl& x) const -> real_impl
		{
			real_impl tmp;
			mpfr_mul(tmp.value, value, x.value, rnd());
			return tmp;
		}

	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator/=(const real_impl& x) -> real_impl&
		{
			mpfr_div(value, value, x.value, rnd());
			return *this;
		}
		
	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator/(const real_impl& x) const -> real_impl
		{
			real_impl tmp;
			mpfr_div(tmp.value, value, x.value, rnd());
			return tmp;
		}

	template <precision_t P, round_t R>
		auto real_impl<P, R>::operator-() const -> real_impl
		{
			real_impl tmp;
			mpfr_neg(tmp.value, value, rnd());
			return tmp;
		}
		

	template <precision_t P, round_t R>
		auto real_impl<P, R>::abs() const -> real_impl
		{
			real_impl tmp;
			mpfr_abs(tmp.value, value, rnd());
			return tmp;
		}



	// Output_streamable
	//
	// FIXME: Understand io manipulators.
	//
	// FIXME: What if C is wchar_t? The output is going to be skewed.
	template <typename C, typename T, precision_t P, round_t R>
		std::basic_ostream<C, T>& 
		operator<<(std::basic_ostream<C, T>& os, const real_impl<P, R>& i)
		{
			int base = 10;
			if (os.flags() & std::ios::hex)
				base = 16;
			else if(os.flags() & std::ios::oct)
				base = 8;

			// Get and print a textual version of the number.
			mpfr_exp_t exp;
			char* str = mpfr_get_str(nullptr, &exp, base, 0, i.impl(), i.rnd());

			// Print the 1st n digts before the exponent position.
			char* p = str;
			while(exp != 0) {
				os << *p;
				++p;
				--exp;
			}
			
			// Then print the separator, followed by the fractional part.
			// FIXME: Understand locales.
			os << '.';
			while(*p != 0) {
				os << *p;
				++p;
			}

			// Free the allocated memory.
			mpfr_free_str(str);
			return os;
		}


	// Input streamable
	//
	// FIXME: Implement me!
	template <typename C, typename T, precision_t P, round_t R>
		std::basic_istream<C, T>& 
		operator<<(std::basic_istream<C, T>& is, real_impl<P, R>& i)
		{
			return is;
		}

} // namespace origin

#endif
