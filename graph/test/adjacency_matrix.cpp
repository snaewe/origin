// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.


#include <iostream>
#include <cassert>

#include <origin/range/zip_range.hpp>
#include <origin/graph/adjacency_matrix.hpp>
#include <origin/dynarray/square_dynarray.hpp>

// Test suite
#include "test_adjacency_matrix/test_create.hpp"
#include "test_adjacency_matrix/test_size.hpp"
#include "test_adjacency_matrix/test_data_access.hpp"

/**
 * A simple harness for running a test model. Note that setup and teardown are
 * performed in the (default) constructors of the test model.
 */
template<typename Matrix = origin::square_dynarray<bool>>
void test_copy_create() {
  using namespace origin;

  create_copy_graph<adjacency_matrix<char, Matrix>> t;

  t.test();
};

template<typename Matrix = origin::square_dynarray<bool>>
void test_sizes() {
  using namespace origin;

  graph_sizes<adjacency_matrix<char, Matrix>> t;

  t.test();
};

template<typename Matrix = origin::square_dynarray<bool>>
void test_data_access() {
  using namespace origin;

  graph_data_access<adjacency_matrix<char, Matrix>> t;

  t.test();
};

int main()
{
  test_copy_create();
  test_sizes();
  test_data_access();

  return 0;
}

