// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/range/iterator_range.hpp>
#include <origin/range/zip_range.hpp>

using namespace std;
using namespace origin;

int main()
{
  {
    vector<int> v1 = {1, 2, 3, 4};
    vector<int> v2 = {2, 4, 6, 8};

    for(auto x : zip(v1, v2)) {
      cout << get<0>(x) << " " << get<1>(x) << "\n";
    }
  }
}
