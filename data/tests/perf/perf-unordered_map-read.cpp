// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <unordered_map>

#include "perf-map-read.hpp"

using namespace std;

int main()
{
  default_random_engine eng;
  
  size_t sz = 1000;
  size_t reps = 100000;
  auto t = read_map<unordered_map>(eng, sz, reps);
  std::cout << t.count() << " ms\n";
}
