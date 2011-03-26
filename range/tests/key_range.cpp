// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <map>
#include <iostream>

#include <origin/range/key_range.hpp>
#include <origin/range/value_range.hpp>
#include <origin/range/zip_range.hpp>

using namespace std;
using namespace origin;

int main()
{
  map<int, char> m = {
    {0, 'a'}, {1, 'b'}, {2, 'c'}
  };
  for(auto x : keys(m)) {
    cout << x << " ";
  }
  cout << "\n";

  for(auto x : values(m)) {
    cout << x << " ";
  }
  cout << "\n";

  for(auto x : zip(keys(m), values(m))) {
    cout << get<0>(x) << ":" << get<1>(x) << "\n";
  }

}

