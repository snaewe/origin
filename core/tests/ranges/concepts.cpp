// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
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
  using V = vector<int>;
  
  cout << typestr<Iterator_type<V>>() << " " 
       << Writable<Iterator_type<V>, int>() << "\n";

  cout << typestr<Iterator_type<V const>>() << " " 
       << Writable<Iterator_type<V const>, int>() << "\n";
}
