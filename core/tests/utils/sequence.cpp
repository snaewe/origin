// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/utility.hpp>

using namespace std;
using namespace origin;

int main()
{
  assert(( Same<Front_type<char, short, int>, char>() ));
  assert(( Same<Back_type<char, short, int>, int>() ));
}
