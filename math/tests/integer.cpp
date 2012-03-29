// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <algorithm>

#include <origin/integer.hpp>
#include <origin/counting.hpp>

using namespace std;
using namespace origin;

template <typename T>
	void check_divmod()
	{
		assert(T {5} / T {2} == T {2});
		assert(T {5} / T {-2} == T {-2});
		assert(T {-5} / T {2} == T {-2});
		assert(T {-5} / T {-2} == T {2});

		// C++11 requires that the result of % have the same sign as the
		// dividend.
		assert(T {5} % T {2} == T {1});
		assert(T {5} % T {-2} == T {1});
		assert(T {-5} % T {2} == T {-1});
		assert(T {-5} % T {-2} == T {-1});
	}


int main()
{
	integer i = 10;
	cout << i << '\n';
	cout << -i << '\n';
	cout <<  i + integer(3) << '\n';

	// Test division/remainder.
	check_divmod<int>();
	check_divmod<integer>();
}
