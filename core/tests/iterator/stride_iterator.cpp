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
  // FIXME: Write a real test for this.
  vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto f = make_stride_iterator(v.begin(), 2),
       l = make_stride_iterator(v.end());


  for( ; f < l; ++f) {
    cout << *f << " ";
  }
}
