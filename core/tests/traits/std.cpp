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
  // FIXME: Write tests.
  struct S {
    int x;
  };

  S s;
  const S cs{};

  cout << typestr<decltype(s.x)>() << "\n";
  cout << typestr<decltype((s.x))>() << "\n";

  cout << typestr<decltype(cs.x)>() << "\n";
  cout << typestr<decltype((cs.x))>() << "\n";
}
