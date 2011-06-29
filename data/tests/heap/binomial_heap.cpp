// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/heap/binomial_heap.hpp>

#include "check_heap.hpp"

using namespace std;
using namespace origin;

int main()
{
  default_random_engine eng;
  
  check_heap<binomial_heap<int>>(eng);
  check_heap<mutable_binomial_heap<int>>(eng);

  typedef mutable_binomial_heap<int*, indirect_compare<int>> PtrHeap;
  
  // FIXME: This is going to pose a serious usability problem if we really
  // want to make heaps interchangeable...
  typedef mutable_binomial_heap<
    size_t, 
    index_compare<int>, 
    allocator<size_t>, 
    ordinal_map<size_t, mutable_binomial_tree_node<size_t>*>
  > OrdHeap;
  
  vector<int> v = make_data(eng);
  check_indirect_mutable_heap<PtrHeap>(v, eng);
  check_ordinal_mutable_heap<OrdHeap>(v, eng);
}
