// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/range/zip_range.hpp>

using namespace std;
using namespace origin;

int main()
{
  vector<int> v1 = {1, 2, 3};
  vector<char> v2 = {'a', 'b', 'c'};
  
  using T = tuple<int, char>;
  vector<T> v3 = {T{1, 'a'}, T{2, 'b'}, T{3, 'c'}};

  auto z = zip(v1, v2);
  auto i = v3.begin();
  for(const auto& x : z) {
    // cout << get<0>(x) << ' ' << get<1>(x) << '\n';
    assert(x == *i);
    ++i;
  }
  
  {
    const vector<int>& cv1 = v1;
    const vector<char>& cv2 = v2;
    
    auto r1 = zip(cv1, v2);
    auto r2 = zip(v1, cv2);
    
    cout << typestr(r1) << "\n";
    cout << typestr(r2) << "\n";
  }
}
