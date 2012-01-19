// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <origin/numeric/clamp.hpp>

using namespace std;
using namespace origin;

int main()
{
  int min = 1, max = 10;
  
  assert((1  == clamp(-200, min, max)));
  assert((10 == clamp( 200, min, max)));
  assert((5  == clamp(   5, min, max)));
  
  return 0;
}