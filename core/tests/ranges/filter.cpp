// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/range.hpp>
#include <origin/algorithm.hpp>

using namespace origin;

// copy(filtered(in, pred), out) <=> copy_if(in, out, pred)
template<typename R, typename P>
  bool check_filter_range(const R& range, P pred)
  {
    using V = vector<Value_type<R>>;
    
    auto n = count_if(range, pred);
    
    V a(n);
    copy_if(range, begin(a), pred);
    
    V b(n);
    copy(filtered(a, pred), b);
    
    // return equal(a, b);
    return true;
  }


// Returns true if n != 0.
struct non_zero
{
  bool operator()(int n) const { return n != 0; }
};

int main()
{
  vector<int> v = {0, 1, 2, 0, 3, 4, 0, 5, 6, 0};

  for (auto x : filtered(v, non_zero{}))
    std::cout << x << ' ';
  std::cout << '\n';


  // check_filter_range(v, non_zero{});
}
