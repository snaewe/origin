// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/concepts.hpp>

using namespace std;
using namespace origin;

struct not_equal { };

template<typename T, typename U = T>
void check_equal(bool result)
{ assert(( Equal<T, U>::value == result )); }


// FIXME: Figure out a way to bootstrap both tests. If the type models the
// the concept, then pass it to a function that causes it to agree. Both tests
// need to pass in order for the test to really work.
template<typename T>
void check_boolean(bool result)
{ assert(( Boolean<T>::value == result )); }

template<typename T>
void assert_boolean()
{ Boolean<T>{}; }

int main()
{
  check_equal<bool>(true);
  check_equal<int>(true);
  check_equal<not_equal>(false);


  check_boolean<bool>(true);
  assert_boolean<bool>();

  check_boolean<char>(true);
  check_boolean<int>(true);
  check_boolean<long>(true);

  check_boolean<int*>(true);
  assert_boolean<bool>();

  check_boolean<string>(false);
}
