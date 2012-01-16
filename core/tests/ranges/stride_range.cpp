// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/range/stride_range.hpp>

using namespace std;
using namespace origin;

// Return true if n != 0.
bool non_zero(int n) { return n != 0; }

template<int N, typename R>
  inline auto f(R& r)
    -> Requires<Range<R>(), static_advance_action<Iterator_type<R>, N>>
  { return 0; }


int main()
{
  vector<int> v = {1, 0, 0, 3, 0, 0, 5, 0, 0};
  {
    for(auto x : stride(v, 3))
      cout << x << ' ';
    cout << '\n';
    for(auto& x : stride(v, 3))
      x = 10;
    for(auto x : v)
      cout << x << ' ';
    cout << '\n';
  }
  {
    for(auto x : stride<3>(v))
      cout << x << ' ';
    cout << '\n';
    for(auto& x : stride(v, 3))
      x = 5;
    for(auto x : v)
      cout << x << ' ';
    cout << '\n';
  }
  
  // Make sure we haven't made stride iterator constructors ambiguous
  auto i = stride_iter(v.begin(), 3);
  assert((++i).base() == next(v.begin(), 3));
  
  auto j = stride_iter<3>(v.end()); 
  assert((--j).base() == prev(v.end(), 3));
}
