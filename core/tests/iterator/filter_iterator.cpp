// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/iterator/filter_iterator.hpp>

using namespace std;
using namespace origin;

// copy(filtered(first), filtered(last), result, pred) 
//   <=> copy_if(first, last, result, pred)
template<typename I, typename P>
  bool check_filter_iterator(I first, I last, P pred)
  {
    using V = vector<Value_type<I>>;
    
    auto n = std_count_if(first, last, pred);
    
    V a(n);
    std_copy_if(first, last, a.begin(), pred);
    
    V b(n);
    auto first2 = filter_iter(first, last, pred);
    auto last2 = filter_iter(last, pred);
    std_copy(first2, last2, b.begin());
    
    return equal(a, b);
  }

  

// Returns true if n is non-zero.
struct non_zero
{
  bool operator()(int n) const { return n != 0; }
};



int main()
{
  vector<int> v = {0, 1, 2, 0, 3, 4, 0, 5, 6, 0};
  assert(check_filter_iterator(v.begin(), v.end(), non_zero{}));
}
