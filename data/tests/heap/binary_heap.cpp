// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/heap/binary_heap.hpp>

#include "check_heap.hpp"

using namespace std;
using namespace origin;

int main()
{
  default_random_engine eng;
  
  int const N = 100;
  
  for(int i = 0; i < N; ++i)
    check_heap<binary_heap>(eng);

  for(int i = 0; i < N; ++i) {
    check_heap<mutable_binary_heap>(eng);
    check_mutable_heap<mutable_binary_heap>(eng);
    check_dense_mutable_heap<mutable_binary_heap>(eng);
  }
}
