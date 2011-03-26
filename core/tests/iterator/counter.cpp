// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/iterator/counter.hpp>

using namespace std;
using namespace origin;

int main()
{
  // Check counter types.
  {
    auto f = make_counter(0u), l = make_counter(10u);
    typedef decltype(f) Counter;
    static_assert(is_same<Counter::value_type, unsigned int>::value, "");
    static_assert(is_same<Counter::difference_type, int>::value, "");
    static_assert(f.step() == 1, "");
    assert(( distance(f, l) == 10 ));
  }

  {
    auto f = make_counter<-1>(10u), l = make_counter<-1>(0u);
    typedef decltype(f) Counter;
    static_assert(is_same<Counter::value_type, unsigned int>::value, "");
    static_assert(is_same<Counter::difference_type, int>::value, "");
    static_assert(f.step() == -1, "");
    assert(( distance(f, l) == 10 ));
  }

  // Check distances for non-unit counters
  {
    auto f = make_counter<2>(0), l = make_counter<2>(10);
    assert(( distance(f, l) == 5 ));
  }
  {
    auto f = make_counter<-2>(10), l = make_counter<-2>(0);
    assert(( distance(f, l) == 5 ));
  }

  // Test step counters also
  {
    auto f = make_step_counter(0), l = make_step_counter(10);
    assert(( distance(f, l) == 10 ));
  }
  {
    auto f = make_step_counter(0), l = make_step_counter(10);
    assert(( distance(f, l) == 10 ));
  }
}
