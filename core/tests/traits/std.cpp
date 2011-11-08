// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/traits.hpp>

using namespace std;
using namespace origin;

int main()
{
  // FIXME: Write common type tests.
  assert(( Common<int, char>() && Same<Common_type<int, char>, int>() ));
  assert(( Common<int, double>() && Same<Common_type<int, double>, double>() ));
  
  assert(( !Common<int, ostream>() ));
  
  assert(( Has_plus<int, int>() ));
  assert(( !Has_plus<int, ostream>() ));
}
