// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/iterator/filter.hpp>

using namespace std;
using namespace origin;

// Return truen if n is not 0.
bool non_zero(int n) { return n != 0; }

int main()
{
  typedef vector<int> Vector;
  Vector v = {0, 1, 2, 0, 3, 4, 0, 5, 6, 0};
  
  typedef filter_iterator<Vector::iterator, bool(*)(int)> Filter;
  Filter i(v.begin(), v.end(), non_zero);
  Filter j(v.end());
  copy(i, j, ostream_iterator<int>(cout, " "));
  cout << "\n";
}
