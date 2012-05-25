// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/range/reverse.hpp>

using namespace origin;

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      std::cout << x << ' ';
    std::cout << '\n';
  }


int main()
{
  vector<int> v = {1, 2, 3, 4, 5}; 

  print(reversed(v.begin(), v.end()));
  print(reversed(v));  
}
