// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/blas/detail/matrix/matrix_alloc_base.hpp>
#include <cassert>
using namespace origin::blas::detail;

int main() {
    matrix_alloc_base<float, std::allocator<float>> temp;
    return 0;
    
}