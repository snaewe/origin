// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/functional.hpp>

using namespace std;
using namespace origin;

template<int N> struct dummy { };

int main()
{
  assert(( identity_element(plus<int>{}) == 0 ));
  assert(( identity_element(multiplies<int>{}) == 1 ));

  assert(( extreme_element(less<int>{}) == numeric_limits<int>::max() ));
  assert(( extreme_element(greater<int>{}) == numeric_limits<int>::min() ));
  
  // Make sure that constexpr actually works the way I think it should...
  dummy<order_traits<less<char>>::extreme()> x;
  dummy<order_traits<greater<char>>::extreme()> y;
  // cout << typestr(x) << " " << typestr(y) << "\n";

  assert(( extreme_element(less<double>{}) == numeric_limits<double>::max() ));
  assert(( extreme_element(greater<double>{}) == numeric_limits<double>::min() ));
  
  
  {
    inverse_compare<less<int>> il;
    greater<int> g;
    
    // NOTE: The property is really,
    // for any x, y il(x, y)==g(y, x)
    il(1, 2)==g(1, 2);
    il(2, 1)==g(2, 1);
  }
}
