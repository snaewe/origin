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

bool geq_ten(int n) { return n >= 10; }

int main()
{
  vector<int> v = {1, 11, 2, 12, 3, 13};
  vector<int> out(3);
  auto p = extract_if(v.begin(), v.end(), out.begin(), geq_ten);

  int ones[] = {1, 2, 3};
  int tens[] = {11, 12, 13};
  
  assert(( equal(v.begin(), p.first, ones) ));
  assert(( equal(out.begin(), p.second, tens) ));
}
