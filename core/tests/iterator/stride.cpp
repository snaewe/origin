// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/iterator/stride.hpp>

using namespace std;
using namespace origin;

int main()
{
  origin::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  
  {
    auto f = make_stride_iterator(v.begin(), 2), 
         l = make_stride_iterator(v.end(), 2);
    static_assert(Random_access_iterator<decltype(f)>(), "");
    for( ; f < l; ++f) {
      cout << *f << ' ';
    }
    cout << '\n';
  }
}
