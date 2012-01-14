// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/range.hpp>

using namespace std;
using namespace origin;

int main()
{
  for(auto i : range(0, 10))
    cout << i << " ";
  cout << "\n";


  for(auto i : closed_range(-3, 3))
    cout << i << ' ';
  cout << '\n';


  vector<int> v = { 1, 2, 3 };
  for(auto i : range(v.begin(), v.end()))
    cout << i << ' ';
  cout << '\n';

  for(auto i : closed_range(v.begin(), --v.end()))
    cout << i << ' ';
  cout << '\n';
}
