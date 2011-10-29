// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/counting.hpp>

using namespace std;
using namespace origin;

int main()
{
  for(int n = 1; n <= 10; ++n) {
    for(int k = 0; k <= n; ++k) {
      cout << binomial_coefficient(n, k) << ' ';
    }
    cout << '\n';
  }
}
