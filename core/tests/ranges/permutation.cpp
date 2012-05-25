// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/range/permutation.hpp>

using namespace origin;

template<typename R>
  void print(R const& r)
  {
    for(auto x : r)
      std::cout << x << " ";
    std::cout << "\n";
  }

int main()
{
  vector<int> v1 = {1, 2, 3};
  for(auto r : permutations(v1))
    print(r);

  std::cout << '\n';
  
  // FIXME: Is this right?
  vector<int> v = {1, 2, 3, 4, 5};
  for(auto r : partial_permutations(v, 3))
    print(r);
}
