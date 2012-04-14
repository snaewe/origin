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
#include <origin/memory.hpp>

namespace origin {


/** @name matrix_alloc_base
 *  @brief The basic matrix base class used for handling memory allocations. 
 */
template<typename T, typename Alloc>
class matrix_alloc_base
{
protected:
    using rebound_alloc_type = typename Alloc::template rebind<T>::other;
public:
    using allocator_type = Alloc;
    using pointer = typename std::allocator_traits<rebound_alloc_type>::pointer;
    
    /*
    TODO Move to deriving class.
    typedef T value_type;
    typedef typename std::allocator_traits<rebound_alloc_type>::const_pointer const_pointer;
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef typename std::allocator_traits<rebound_alloc_type>::difference_type difference_type;
    typedef typename std::allocator_traits<rebound_alloc_type>::size_type size_type;
    */
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
        
    /** @name Default constructor
     *  @brief Default construct an instance of the matrix_alloc_base.
     */
    matrix_alloc_base() { }
    
    
    /** @name Allocator copy constructor
     *  @brief Creates a copy of the allocator a.
     *  @param a The allocator to copy.
     */
    matrix_alloc_base(allocator_type const& a)
        :base_impl(a)
    { }

    /** @name Allocate n items constructor
     *  @brief Allocates n items.
     *  @param n The numebr of items to allocate.
     */
    matrix_alloc_base(std::size_t n)
        :base_impl()
    {
        create_storage(n);
    }
    
    /** @name Allocate n items and copy allocatr constructor
     *  @brief Allocates n items.
     *  @param n The numebr of items to allocate.
     *  @param a The allocator to copy.
     */
    matrix_alloc_base(std::size_t n, allocator_type const& a)
        :base_impl(a)
    {
        create_storage(n);
    }
    

    /** @name Move Constructor
     *  @brief Moves data from other into this object.
     *  @param other The class whose memory this class is to gain owner ship of.
     */
    matrix_alloc_base(matrix_alloc_base&& other)
        :base_impl(std::move(other.get_rebound_allocator()))
    { }
    
    /** @name Copy Constructor
     *  @brief Creates a copy of the other object.
     *  @param other The class to create a copy of.
     */
    matrix_alloc_base(matrix_alloc_base const& other)
        :base_impl(other.base_impl)
    {
        create_storage(other.base_impl.finish - other.base_impl.start);
    }
    
    /** @name Move with specific allocator
     *  @brief Moves data from other class into owner ship of this object and 
     *  @param other The class whose memory to gain ownership of.
     *  @param a The allocator to copy.
     *  @TODO Implement ME 
     */
    matrix_alloc_base(matrix_alloc_base&& other, allocator_type const& a)
        :base_impl(a)
    {
        // create_storage(n);
    }
    
    /** @name Destructor
     *  @brief Free memory that is currenlty allocated.
     */
    ~matrix_alloc_base() noexcept {
        deallocate(base_impl.start, base_impl.finish - base_impl.start);
    }

    
    /** @name Allocate
     *  @brief Allocates n instances of value type.
     */
    pointer allocate(std::size_t n) {
        return n != 0 ? base_impl.allocate(n) : 0;
    }
    
    /** @name deallocate
     *  @brief Deallocates memory at location p of size n.
     *  @param p The memory to deallocate.
     *  @param n The number of instances of value_type to deallocate.
     */
    void deallocate(pointer p, std::size_t n){
        if (p)
            base_impl.deallocate(p, n);
    }
    
    /// @name Member Variable
    ///{
    matrix_alloc_impl_base base_impl;
    ///}
private:
    
    /** @name create_storage
     *  @brief Private function which helps encapsulate a the allocation
     *  process of the constructors of this class.
     *  @param n is the number of elements to allocate.
     */
    void create_storage(std::size_t n) {
        base_impl.start = allocate(n);
        base_impl.finish = base_impl.start + n;
    }
};

} // end origin

#endif