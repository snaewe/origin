// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>
#include <set>

#include <origin/algorithm.hpp>

using namespace std;
using namespace origin;

int main()
{
  {
    int a[] = {1, 2, 3, 4, 5};
    using A = decltype(a);
    auto i = find(a, 3);
    // assert(i == a + 3);
  }

  {
    using Vec = vector<int>;
    Vec v = {1, 2, 3, 0, 5, 0, 7};
    const Vec& cv = v;

    auto i = o_find(v.begin(), v.end(), 0);
    assert(i == v.begin() + 3);
    
    auto vi = find(v, 0);
    static_assert(Same<decltype(vi), Vec::iterator>(), "");
    assert(vi == i);

    auto cvi = find(cv, 0);
    static_assert(Same<decltype(cvi), Vec::const_iterator>(), "");
    assert(cvi == i);
  }
  
  {
    using Set = set<int>;
    Set s = {5, 4, 3, 2, 1};
    const Set& cs = s;
    
    auto i = find(s, 3);
    static_assert(Same<decltype(i), Set::iterator>(), "");
    assert(i != s.end());
    assert(*i == 3);
    
    
    auto ci = find(cs, 3);
    static_assert(Same<decltype(ci), Set::const_iterator>(), "");
    assert(i != s.end());
    assert(*i == 3);
  }
}
