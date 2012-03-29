// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_INTEGER_GMP_HPP
#define ORIGIN_INTEGER_GMP_HPP

#include <gmp.h>

#include <ios>
#include <cstring>
#include <string>

namespace origin
{
	// TODO: Remove this implementation once we have our own bigint class.

	namespace gmp
	{
		using alloc_func = void *(*)(size_t);
		using realloc_func = void *(*)(void*, size_t, size_t);
		using free_func = void (*)(void*, size_t);
	} // namespace gmp


  // The GMP integer implementation wraps the C-library's gmpz_t structure 
  // with a C++ facade.
	class integer_impl
	{
		mpz_t value;

	public:
		// Default constructible
		integer_impl();

		// Movable
		integer_impl(integer_impl&& x);
		integer_impl& operator=(integer_impl&& x);

		// Copyable
		integer_impl(const integer_impl& x);
		integer_impl& operator=(const integer_impl& x);

		// Conversion
		integer_impl(long n);
		integer_impl(unsigned long);
		
		// String construction
		explicit integer_impl(const char* str, int base = 10);
		explicit integer_impl(const std::string& str, int base = 10);

		// Destructible.
		~integer_impl();

		// Equality_comparable
		bool operator==(const integer_impl& x) const;
		bool operator!=(const integer_impl& x) const;

		// Totally_ordered
		bool operator<(const integer_impl& x) const;
		bool operator>(const integer_impl& x) const;
		bool operator<=(const integer_impl& x) const;
		bool operator>=(const integer_impl& x) const;

		// Arithmetic
		integer_impl& operator+=(const integer_impl& x);
		integer_impl operator+(const integer_impl& x) const;
		
		integer_impl& operator-=(const integer_impl& x);
		integer_impl operator-(const integer_impl& x) const;

		integer_impl& operator*=(const integer_impl& x);
		integer_impl operator*(const integer_impl& x) const;

		integer_impl& operator/=(const integer_impl& x);
		integer_impl operator/(const integer_impl& x) const;

		integer_impl& operator%=(const integer_impl& x);
		integer_impl operator%(const integer_impl& x) const;

		integer_impl operator-() const;
		
		// Incrementable
		integer_impl& operator++();
		integer_impl operator++(int);

		// Decrementable
		integer_impl& operator--();
		integer_impl operator--(int);


		// Binary oerations
		integer_impl& operator&=(const integer_impl& x);
		integer_impl operator&(const integer_impl& x) const;

		integer_impl& operator|=(const integer_impl& x);
		integer_impl operator|(const integer_impl& x) const;

		integer_impl& operator^=(const integer_impl& x);
		integer_impl operator^(const integer_impl& x) const;

		integer_impl operator~() const;

		// Numeric operations
		integer_impl abs() const;

		void swap(integer_impl& x);

		mpz_t& impl() 						{ return value; }
		const mpz_t& impl() const { return value; }
	};

	integer_impl::integer_impl() 
	{ 
		mpz_init(value); 
	}

	integer_impl::integer_impl(integer_impl&& x)
	{
		mpz_init(value);
		mpz_swap(value, x.value);
	}

	integer_impl& integer_impl::operator=(integer_impl&& x)
	{
		integer_impl tmp(std::move(x));
		swap(tmp);
		return *this;
	}

	integer_impl::integer_impl(const integer_impl& x)
	{
		mpz_init_set(value, x.value);
	}
	
	integer_impl& integer_impl::operator=(const integer_impl& x)
	{
		integer_impl tmp(x);
		swap(tmp);
		return *this;
	}

	integer_impl::integer_impl(long n)
	{
		mpz_init_set_si(value, n);
	}

	integer_impl::integer_impl(unsigned long n)
	{
		mpz_init_set_ui(value, n);
	}

	integer_impl::integer_impl(const char *str, int base)
	{
		mpz_init_set_str(value, str, base);
	}

	integer_impl::integer_impl(const std::string& str, int base)
	{
		mpz_init_set_str(value, str.c_str(), base);
	}

	integer_impl::~integer_impl() 
	{
		mpz_clear(value);
	}

	void integer_impl::swap(integer_impl& x)
	{
		mpz_swap(value, x.value);
	}


	inline bool integer_impl::operator==(const integer_impl& x) const
	{
		return mpz_cmp(value, x.value) == 0;
	}

	inline bool integer_impl::operator!=(const integer_impl& x) const
	{
		return !(*this == x);
	}

	// Totally_ordered
	inline bool integer_impl::operator<(const integer_impl& x) const
	{
		return mpz_cmp(value, x.value) < 0;
	}

	inline bool integer_impl::operator>(const integer_impl& x) const
	{
		return mpz_cmp(value, x.value) > 0;
	}

	inline bool integer_impl::operator<=(const integer_impl& x) const
	{
		return !(*this > x);
	}

	inline bool integer_impl::operator>=(const integer_impl& x) const
	{
		return !(*this < x);
	}

	inline integer_impl& integer_impl::operator+=(const integer_impl& x)
	{
		mpz_add(value, value, x.value);
		return *this;
	}
	
	integer_impl integer_impl::operator+(const integer_impl& x) const
	{
		integer_impl tmp {*this};
		return tmp += x;
	}
	
	integer_impl& integer_impl::operator-=(const integer_impl& x)
	{
		mpz_sub(value, value, x.value);
		return *this;
	}
	
	integer_impl integer_impl::operator-(const integer_impl& x) const
	{
		integer_impl tmp {*this};
		return tmp -= x;
	}

	integer_impl& integer_impl::operator*=(const integer_impl& x)
	{
		mpz_mul(value, value, x.value);
		return *this;
	}

	integer_impl integer_impl::operator*(const integer_impl& x) const
	{
		integer_impl tmp {*this};
		return tmp *= x;
	}

	// NOTE: Division and remainder are computed using truncated division, where
	// the quotient is rounded towards 0, and the remainder has the same sign
	// as the dividend. I think.

	integer_impl& integer_impl::operator/=(const integer_impl& x)
	{
		mpz_tdiv_q(value, value, x.value);
		return *this;
	}
	
	integer_impl integer_impl::operator/(const integer_impl& x) const
	{
		integer_impl tmp {*this};
		return tmp /= x;
	}

	integer_impl& integer_impl::operator%=(const integer_impl& x)
	{
		mpz_tdiv_r(value, value, x.value);
		return *this;
	}
	
	integer_impl integer_impl::operator%(const integer_impl& x) const
	{
		integer_impl tmp {*this};
		return tmp %= x;
	}

	integer_impl integer_impl::operator-() const
	{
		integer_impl tmp;
		mpz_neg(tmp.value, value);
		return tmp;
	}
	
	integer_impl& integer_impl::operator++()
	{
		return *this += integer_impl {1l};
	}

	integer_impl integer_impl::operator++(int)
	{
		integer_impl tmp = *this;
		operator++();
		return tmp;
	}

	integer_impl& integer_impl::operator--()
	{
		return *this -= integer_impl {1l};
	}

	integer_impl integer_impl::operator--(int)
	{
		integer_impl tmp = *this;
		operator--();
		return tmp;
	}

	integer_impl& integer_impl::operator&=(const integer_impl& x)
	{
		mpz_and(value, value, x.value);
		return *this;
	}

	integer_impl integer_impl::operator&(const integer_impl& x) const
	{
		integer_impl tmp {*this};
		return tmp &= x;
	}

	integer_impl& integer_impl::operator|=(const integer_impl& x)
	{
		mpz_ior(value, value, x.value);
		return *this;
	}

	integer_impl integer_impl::operator|(const integer_impl& x) const
	{
		integer_impl tmp {*this};
		return tmp |= x;
	}

	integer_impl& integer_impl::operator^=(const integer_impl& x)
	{
		mpz_xor(value, value, x.value);
		return *this;
	}

	integer_impl integer_impl::operator^(const integer_impl& x) const
	{
		integer_impl tmp {*this};
		return tmp ^= x;
	}

	integer_impl integer_impl::operator~() const
	{
		integer_impl tmp;
		mpz_com(tmp.value, value);
		return tmp;
	}

	integer_impl integer_impl::abs() const
	{
		integer_impl tmp;
		mpz_abs(tmp.value, value);
		return tmp;
	}


	// Output_streamable
	//
	// FIXME: Understand io manipulators.
	//
	// FIXME: What if C is wchar_t? The output is going to be skewed.
	template <typename C, typename T>
		std::basic_ostream<C, T>& 
		operator<<(std::basic_ostream<C, T>& os, const integer_impl& i)
		{
			int base = 10;
			if (os.flags() & std::ios::hex)
				base = 16;
			else if(os.flags() & std::ios::oct)
				base = 8;

			// Get and print a textual version of the number.
			char* str = mpz_get_str(nullptr, base, i.impl());
			os << str;

			// Free the allocated memory.
			gmp::free_func f;
			mp_get_memory_functions(nullptr, nullptr, &f);
			f(str, std::strlen(str) + 1);

			return os;
		}


	// Input streamable
	//
	// FIXME: Implement me!
	template <typename C, typename T>
		std::basic_istream<C, T>& 
		operator<<(std::basic_istream<C, T>& is, integer_impl& i)
		{
			return is;
		}

} // namespace origin

#endif
