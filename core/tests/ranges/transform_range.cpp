// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/algorithm.hpp>
#include <origin/range/transform_range.hpp>

using namespace std;
using namespace origin;

// Check transform range
// Copying a transformed range is equivalent to using calling the transform
// algorithm.
//
// transform(in, out) <~> copy(transformed(in), out)
template<typename R, typename F>
  bool check_transform_range(const R& range, F f)
  {
    using V = vector<Value_type<R>>;

    V a(size(range));
    transform(range, a, f);
    
    V b(size(range));
    copy(transformed(range, f), b);
    
    return equal(a, b);
  }

  
// Return 2 * x.
struct twice
{
  int operator()(int x) const { return 2 * x; }
};



int main()
{
  vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
  assert(check_transform_range(v, twice{}));
}
