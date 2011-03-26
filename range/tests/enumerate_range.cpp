// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/range/enumerate_range.hpp>

#include "range_test.hpp"

using namespace std;
using namespace origin;

int main()
{
  // Note that the type here must agree with the enumerator's value type,
  // otherwise you can get some weird type errors. Note long == ptrdiff_t
  typedef pair<long, string> Pair;

  string str[3] = {"a", "b", "c"};

  assert((check(enumerate(str), {Pair{0, "a"}, Pair{1, "b"}, Pair{2, "c"}}) ));

  // FIXME: Write more tests
}

