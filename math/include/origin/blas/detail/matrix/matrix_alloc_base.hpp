// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_BLAS_MATRIX_ALLOC_BASE_HPP
#define ORIGIN_MATH_BLAS_MATRIX_ALLOC_BASE_HPP
#include <cassert>
#include <memory>
#include <algorithm>

namespace origin { namespace blas { namespace detail {

/** The basic matrix base class used for handling 
 *  memory allocations. 
 */
template<typename T, typename Alloc>
class matrix_alloc_base {
public:
    typedef typename Alloc::template rebind<T>::other rebound_alloc_type;
    typedef typename std::allocator_traits<rebound_alloc_type>::allocator_type allocator_type;
    typedef typename std::allocator_traits<rebound_alloc_type>::value_type value_type;
    typedef typename std::allocator_traits<rebound_alloc_type>::pointer pointer;
    typedef typename std::allocator_traits<rebound_alloc_type>::reference reference;
    typedef typename std::allocator_traits<rebound_alloc_type>::const_pointer const_pointer;
    typedef typename std::allocator_traits<rebound_alloc_type>::const_reference const_reference;
    typedef typename std::allocator_traits<rebound_alloc_type>::difference_type difference_type;
    typedef typename std::allocator_traits<rebound_alloc_type>::size_type size_type;

};
    
    
}}} // end origin::blas::detail

#endif