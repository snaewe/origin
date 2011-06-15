// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/ordinal_map.hpp>

#include "perf-map-build.hpp"

using namespace std;
using namespace origin;

int main()
{
  default_random_engine eng;
  
  int sz = 10000;
  auto t = build_map<ordinal_map>(eng, sz);
  std::cout << t.count() << " ms\n";
}
