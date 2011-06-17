// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/ordinal.hpp>

using namespace std;
using namespace origin;

// Test for enums
enum class value { a, b, c };

size_t ord(value x)
{
  return static_cast<size_t>(x);
}

value from_ord(size_t x, value)
{
  return static_cast<value>(x);
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
};

size_t ord(num n)
{
  return n.val;
}

int main()
{
  assert(( ord(false) == 0 ));
  assert(( ord(true) == 1 ));
  assert(( aOrdinal_Identity(true) ));
  assert(( aOrdinal_Equality(false, false) ));

  assert(( ord(numeric_limits<char>::min()) == 0 ));
  assert(( ord(0) == uint{numeric_limits<int>::max() + 1} ));
  assert(( aOrdinal_Identity(0) ));
  assert(( aOrdinal_Equality(0, 0) ));
  
  assert(( ord(0u) == 0 ));
  assert(( ord(1u) == 1 ));
  assert(( aOrdinal_Identity(0u) ));
  assert(( aOrdinal_Equality(0u, 0u) ));

  // The ordinals of the max of integral values (of the same size) are equal.
  assert(( ord(numeric_limits<int>::max()) == ord(numeric_limits<unsigned int>::max()) ));
  
  assert(( ord(value::a) == 0 ));
  assert(( ord(value::b) == 1 ));
  assert(( ord(value::c) == 2 ));
  assert(( aOrdinal_Identity(value::a) ));
  assert(( aOrdinal_Equality(value::b, value::b) ));

  assert(( ord(num{}) == 0 ));
  assert(( ord(num{1}) == 1 ));
  assert(( aOrdinal_Identity(num{}) ));
}
