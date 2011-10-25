// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/algorithm.hpp>

using namespace std;
using namespace origin;

bool nonneg(int n) { return n >= 0; }
bool neg(int n) { return n < 0; }
bool zero(int n) { return n == 0; }
bool nonzero(int n) { return n != 0; }

int main()
{
  vector<int> v = {1, 0, 2, 0, 3, 0};
  
  assert(( all_of(v.begin(), v.end(), nonneg) ));
  assert(( all_of(v, nonneg) ));
}
