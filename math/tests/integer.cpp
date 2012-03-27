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


int main()
{
	integer i = 10;
	cout << i << '\n';
	cout << -i << '\n';
	cout <<  i + integer(3) << '\n';

	cout << factorial(integer {100}) << '\n';
}
