// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/iterator/stride_iterator.hpp>

using namespace std;
using namespace origin;

int main()
{
  vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  
  {
    auto f = stride_iter(v.begin(), 2), l = stride_iter(v.end(), 2);
    for( ; f < l; ++f) {
      cout << *f << ' ';
    }
    cout << '\n';
  }

  {
    auto f = stride_iter<2>(v.begin()), l = stride_iter<2>(v.end());
    for( ; f < l; ++f) {
      cout << *f << ' ';
    }
    cout << '\n';
  }
       
}
