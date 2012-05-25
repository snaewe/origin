// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/iterator/reverse.hpp>

using namespace std;
using namespace origin;

int main()
{
  int a[] = {1, 2, 3, 4, 5};

  auto f = reversed(a + 5);
  auto l = reversed(a);
  while (f != l) {
    std::cout << *f << ' ';
    ++f;
  }
  std::cout << '\n';
}
