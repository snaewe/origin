// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <algorithm> // make sure we don't have min/max collisions.

#include <origin/math/number.hpp>

using namespace std;
using namespace origin;

struct num {
  static char max() { return std::numeric_limits<char>::max(); }
};

int main()
{
  cout << max<int>() << "\n";
  cout << (int)max<num>() << "\n";
}
