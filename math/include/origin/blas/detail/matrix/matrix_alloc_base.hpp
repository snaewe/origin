// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_BLAS_MATRIX_ALLOC_BASE_HPP
#define ORIGIN_MATH_BLAS_MATRIX_ALLOC_BASE_HPP
#include <cassert>
#include <memory>
#include <algorithm>

namespace origin {


/** @name matrix_alloc_base
 *  @brief The basic matrix base class used for handling memory allocations. 
 */
template<typename T, typename Alloc>
class matrix_alloc_base
{
protected:
    typedef typename Alloc::template rebind<T>::other rebound_alloc_type;
public:
    typedef Alloc allocator_type;
    typedef T value_type;
    typedef typename std::allocator_traits<rebound_alloc_type>::pointer pointer;
    typedef typename std::allocator_traits<rebound_alloc_type>::const_pointer const_pointer;
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef typename std::allocator_traits<rebound_alloc_type>::difference_type difference_type;
    typedef typename std::allocator_traits<rebound_alloc_type>::size_type size_type;
    
protected:
    
    /** @name matrix_alloc_impl_base 
     *  @brief The matrix_alloc_impl_base is responsible for inheirting the allocator 
     *  and managing the instance variables. This class is NOT responsible for actually
     *  copying things around.
     */
    struct matrix_alloc_impl_base
        :rebound_alloc_type
    {
        
        /** @name Default Constructor
         *  @brief Default constucts the allocator and instance variables.
         */
        matrix_alloc_impl_base()
            :rebound_alloc_type(),
            start(0),
            finish(0)
        { }
        
        /** @name Allocator Constructor
         *  @brief Provides a copy constructor for an allocator provided
         *  another instance of the matrix_alloc_impl_base type.
         */
        matrix_alloc_impl_base(rebound_alloc_type const& other)
            :rebound_alloc_type(other),
            start(0),
            finish(0)
        { }
        
        /** @name Move Constructor
         *  @brief Moves the the rhs into the current memory allocator.
         */
        matrix_alloc_impl_base(rebound_alloc_type&& other)
            :rebound_alloc_type(std::move(other)),
            start(0),
            finish(0)
        { }
        
        /** @name Swap
         *  @brief The swap function used for exchanging two different
         *  matrix_alloc_impl_base's instance variables.
         */
        void swap(matrix_alloc_impl_base& rhs) noexcept {
            using std::swap;
            swap(start, rhs.start);
            swap(finish, rhs.finish);
        }
        pointer start;
        pointer finish;
    };
    
    matrix_alloc_impl_base base_impl;

    rebound_alloc_type& get_rebound_allocator() noexcept {
        return *static_cast<rebound_alloc_type*>(&base_impl);
    }

    rebound_alloc_type const& get_rebound_allocator() const noexcept{
        return *static_cast<rebound_alloc_type*>(&base_impl);
    }

public:
    /** @name get_allocator
     *  @brief Returns the allocator.
     */
    allocator_type const& get_allocator() const noexcept {
        return allocator_type(get_rebound_allocator);
    }
    
    
    /** @name Default Constructor
     *  @brief Default construct an instance of the matrix_alloc_base.
     */
    matrix_alloc_base()
    {}
    /*
    _Vector_base()
    _Vector_base(const allocator_type& __a)
    _Vector_base(size_t __n)
    _Vector_base(size_t __n, const allocator_type& __a)
    _Vector_base(_Tp_alloc_type&& __a)
    _Vector_base(_Vector_base&& __x)
    _Vector_base(_Vector_base&& __x, const allocator_type& __a)
    ~_Vector_base()     
     */
};
    
    
} // end origin::blas::detail

#endif