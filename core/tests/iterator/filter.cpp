// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/iterator/filter.hpp>

using namespace std;
using namespace origin;

// Return true if n is not 0.
bool non_zero(int n) { return n != 0; }

// Returns true if n is non-zero.
struct non_zero_function
{
  bool operator()(int n) const
  {
    return n != 0;
  }
};

int main()
{
  using Vec = vector<int>;
  Vec v = {0, 1, 2, 0, 3, 4, 0, 5, 6, 0};
 
  { 
    typedef filter_iterator<Vec::iterator, bool(*)(int)> Filter;
    Filter i(v.begin(), v.end(), non_zero);
    Filter j(v.end());
    copy(i, j, ostream_iterator<int>(cout, " "));
    cout << "\n";
    
    // This should be the size of two vector iterators plus a function pointer.
    cout << sizeof(Filter) << "\n";
  }
  
  {
    auto i = filtered(v.begin(), v.end(), non_zero_function{});
    auto j = filtered(v.end(), non_zero_function{});
    copy(i, j, ostream_iterator<int>(cout, " "));
    cout << "\n";
    
    // Check EBO. This should be the size of two vector iterators. The 
    // predicate should have no size.
    cout << sizeof(decltype(i)) << "\n";
  }
}
