// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/range/iterator_range.hpp>

using namespace std;
using namespace origin;

int main()
{
  // Default constructed ranges on trivially constructed iterator types should
  // be empty.
  // FIXME: What about iterators with singular states?
  iterator_range<int*> r;
  assert(( begin(r) == end(r) ));
}
