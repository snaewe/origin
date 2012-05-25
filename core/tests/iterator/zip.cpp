// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/iterator/zip.hpp>

using namespace std;
using namespace origin;

int main()
{
  vector<int> v1 = {1, 2, 3};
  vector<char> v2 = {'a', 'b', 'c'};
  
  using T = tuple<int, char>;
  vector<T> v3 = {T{1, 'a'}, T{2, 'b'}, T{3, 'c'}};
  
  auto i = make_zip_iterator(v1.begin(), v2.begin());
  auto j = make_zip_iterator(v1.end(), v2.end());
  auto k = v3.begin();
  while(i != j) {
    assert(*i == *k);
    ++k;
    ++i;
  }
}
