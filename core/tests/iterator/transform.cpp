// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/iterator/transform.hpp>

using namespace origin;

// Check transform iterator
// Copying a transformed iterator range is equavalent to the transform
// algorithm. That is:
//
//  transform(first, last, result) <=> 
//    copy(transformed(first, f), transformed(last, f), result)
template <typename I, typename F>
  bool check_transform_iterator(I first, I last, F func)
  {
    static_assert(Forward_iterator<I>(), "");
    static_assert(Regular_function<F, Value_type<I>>(), "");
    
    using V = origin::vector<Value_type<I>>;
    
    V a(distance(first, last));
    transform(first, last, a.begin(), func);
    
    V b(distance(first, last));
    auto first2 = make_transform_iterator(first, func);
    auto last2 = make_transform_iterator(last, func);
    copy(first2, last2, b.begin());
    return lexicographical_equal(a, b);
  }



// Returns 2 * x.
struct twice
{
  int operator()(int x) const { return 2 * x; }
};



int main()
{
  using V = origin::vector<int>;
  V v = {1, 2, 3, 5};

  using I = transform_iterator<V::iterator, twice>;
  static_assert(Input_iterator<I>(), "");

  // Check properties.
  assert(check_transform_iterator(v.begin(), v.end(), twice{}));
}
