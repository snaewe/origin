// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <algorithm>

#include <origin/fraction.hpp>
#include <origin/rational.hpp>

using namespace std;
using namespace origin;

int main()
{
	rational a {1, 3};
	rational b {1, 6};
	
	cout << a + b << '\n';
	cout << a - b << '\n';
	cout << a * b << '\n';
	cout << a / b << '\n';

	rational c {-3, 4};
	cout << -c << '\n';
	cout << abs(c) << '\n';
}
