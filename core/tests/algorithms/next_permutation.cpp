// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/algorithm.hpp>

using namespace origin;

int main()
{
  vector<int> v = {1, 2, 3};
  do {
    for(auto i = v.begin(); i != v.end(); ++i)
      std::cout << *i << ' ';
    std::cout << '\n';
  } while(next_permutation(v));
}
