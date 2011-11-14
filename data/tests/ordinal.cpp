// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/traits.hpp>
#include <origin/ordinal.hpp>

using namespace std;
using namespace origin;

// Test for enums
enum class enu { a, b, c };

size_t ord(enu x)
{
  return static_cast<size_t>(x);
}

// Test for classes.
struct num
{
  num() : val{0} { }
  num(size_t n) : val{n} { }
  size_t val;

  friend bool operator==(num const& x, num const& y)
  {
    return x.val == y.val;
  }
  
  size_t ord() const { return val; }
  static num chr(size_t n) { return num{n}; }
};

int main()
{
  
  assert(( ord(false) == 0 ));
  assert(( ord(true) == 1 ));

  assert(( ord(numeric_limits<char>::min()) == 0 ));
  assert(( ord(0) == uint(numeric_limits<int>::max()) + 1 ));

  int o = ord(char{0});
  int c = ordinal_cast<char>(o);
  
  assert(( ord(0u) == 0 ));
  assert(( ord(1u) == 1 ));

  // The ordinals of the max of integral enus (of the same size) are equal.
  assert(( ord(numeric_limits<int>::max()) == ord(numeric_limits<unsigned int>::max()) ));

  // FIXME: This is currently semi-ordinal.
  assert(( ord(enu::a) == 0 ));
  assert(( ord(enu::b) == 1 ));
  assert(( ord(enu::c) == 2 ));

  num x;
  assert(( ord(x) == 0 ));
  assert(( ord(num{1}) == 1 ));
}
