// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/iterator/counter.hpp>

using namespace std;
using namespace origin;

int main()
{
  // FIXME: Write real tests.

  // Check counter types.
  {
    using D = std::ptrdiff_t;

    auto f = make_counter(0u), l = make_counter(10u);
    using C = decltype(f);
    static_assert(Same<Value_type<C>, unsigned int>(), "");
    static_assert(Same<Difference_type<C>, int>(), "");
    assert(distance(f, l) == 10);
  }
}
