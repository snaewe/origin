// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/iterator/counting_iterator.hpp>
#include <origin/range/filter_range.hpp>
#include <origin/range/enumerate_range.hpp>
#include <origin/range/container_range.hpp>

using namespace std;
using namespace origin;

int main()
{
  vector<int> v = {1, 0, 2, 0, 3, 0, 4, 0, 5};

  { // Standard test with a vector
    for(auto x : enumerate(v)) {
      cout << x.first << "," << x.second << '-';
    }
    cout << '\n';
  }

  { // Test with const iterator
    vector<int> const& cv = v;
    for(auto x : enumerate(cv)) {
      cout << x.first << ' ';
    }
    cout << '\n';
  }

  { // Test const ranges
    auto const cr = get_range(v);
    for(auto x : enumerate(cr)) {
      cout << x.first << ' ';
    }
    cout << '\n';
  }

 { // Layering.
    for(auto x : enumerate(filter(v, [](int x) { return x != 0; }))) {
      cout << x.first << "," << x.second << '-';
    }
    cout << '\n';
  }

  {
    auto x = make_counting_iterator(0), y = make_counting_iterator(10);
    for( ; x != y; ++x) {
      cout << *x << "\n";
    }
  }
}

