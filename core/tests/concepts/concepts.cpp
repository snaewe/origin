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
#include <origin/concepts/traits.hpp>

#include "check.hpp"

using namespace std;
using namespace origin;

int main()
{
  assert(( has_common_type<int, int, char>::value ));
  assert(( !has_common_type<string, float>::value ));
}
  
