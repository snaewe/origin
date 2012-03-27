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
	// TODO: Why don't I ship this as part of a binary library? Header only
	// doesn't make that much sense for this implementation! In fact,
	// exposing this header doesn't make any sense either. On the other hand,
	// all these functions are quite trivial and should be inlined.


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
	bool integer_impl::operator<(const integer_impl& x) const
	{
		return mpz_cmp(value, x.value) < 0;
	}

	bool integer_impl::operator>(const integer_impl& x) const
	{
		return mpz_cmp(value, x.value) > 0;
	}

	bool integer_impl::operator<=(const integer_impl& x) const
	{
		return !(*this > x);
	}

	bool integer_impl::operator>=(const integer_impl& x) const
	{
		return !(*this < x);
	}

	integer_impl& integer_impl::operator+=(const integer_impl& x)
	{
		mpz_add(value, value, x.value);
		return *this;
	}
	
	integer_impl integer_impl::operator+(const integer_impl& x) const
	{
		integer_impl tmp;
		mpz_add(tmp.value, value, x.value);
		return tmp;
	}
	
	integer_impl& integer_impl::operator-=(const integer_impl& x)
	{
		mpz_sub(value, value, x.value);
		return *this;
	}
	
	integer_impl integer_impl::operator-(const integer_impl& x) const
	{
		integer_impl tmp;
		mpz_sub(tmp.value, value, x.value);
		return tmp;
	}

	integer_impl& integer_impl::operator*=(const integer_impl& x)
	{
		mpz_mul(value, value, x.value);
		return *this;
	}

	integer_impl integer_impl::operator*(const integer_impl& x) const
	{
		integer_impl tmp;
		mpz_mul(tmp.value, value, x.value);
		return tmp;
	}

	// TODO: Am I dividing correctly? Is integer division truncated or is it
	// floor. The GMP difference is a bit subtle.

	integer_impl& integer_impl::operator/=(const integer_impl& x)
	{
		mpz_fdiv_q(value, value, x.value);
		return *this;
	}
	
	integer_impl integer_impl::operator/(const integer_impl& x) const
	{
		integer_impl tmp;
		mpz_fdiv_q(tmp.value, value, x.value);
		return tmp;
	}

	integer_impl& integer_impl::operator%=(const integer_impl& x)
	{
		mpz_mod(value, value, x.value);
		return *this;
	}
	
	integer_impl integer_impl::operator%(const integer_impl& x) const
	{
		integer_impl tmp;
		mpz_mod(tmp.value, value, x.value);
		return tmp;
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
