// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_DYNARRAY_FAKE_ALLOCATOR_HPP
#define ORIGIN_DATA_DYNARRAY_FAKE_ALLOCATOR_HPP

#include <cassert>
#include <string>
#include <memory>

// Global variables associated with the fake
// allocator.
void* fake_allocator__ptr_allocation_requested = (void*)0xdeadbeef;
bool fake_allocator__allocate_called = false;
bool fake_allocator__copy_constructor_called = false;
bool fake_allocator__default_constructor_called = false;
bool fake_allocator__copy_other_allocator_called = false;
std::size_t fake_allocator__allocation_size = 0;
void* fake_allocator__ptr_deallocation_address = 0;
std::size_t fake_allocator__deallocation_size = 0;
bool fake_allocator__destructor_called = false;
bool fake_allocator__deallocate_called = false;
bool fake_allocator__move_constructor_called = false;
std::size_t fake_allocator__allocator_construct_called_count  = 0;
std::size_t fake_allocator__allocator_destroy_called_count = 0;
void* fake_allocator__constructed_ptr = nullptr;
void* fake_allocator__destroyed_ptr = nullptr;


void reset_allocator_test_variables()
{
    fake_allocator__ptr_allocation_requested = (void*)0xdeadbeef;
    fake_allocator__move_constructor_called = false;
    fake_allocator__copy_constructor_called = false;
    fake_allocator__default_constructor_called = false;
    fake_allocator__copy_other_allocator_called = false;
    fake_allocator__allocation_size = 0;
    fake_allocator__ptr_deallocation_address = (void*)0;
    fake_allocator__deallocation_size = 0;
    fake_allocator__destructor_called = false;
    fake_allocator__allocate_called = false;
    fake_allocator__deallocate_called = false;
    fake_allocator__allocator_construct_called_count = 0;
    fake_allocator__allocator_destroy_called_count = 0;
    fake_allocator__constructed_ptr = nullptr;
    fake_allocator__destroyed_ptr = nullptr;
    
}

template <class T>
    class fake_allocator
    {
    public:
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using value_type = T;

        template <class U>
        struct rebind
        {
            typedef fake_allocator<U> other;
        };

        fake_allocator() throw()
        {
            fake_allocator__default_constructor_called = true;
        }

        fake_allocator(fake_allocator&&) throw()
        {
            fake_allocator__move_constructor_called = true;
        }
        
        fake_allocator(const fake_allocator&) throw()
        {
            fake_allocator__copy_constructor_called = true;
        }

        template <class U>
        fake_allocator(fake_allocator<U> const&) throw()
        {
            fake_allocator__copy_other_allocator_called = true;
        }

        ~fake_allocator() throw()
        {
            fake_allocator__destructor_called = true;
        }
        
        pointer allocate(size_type n, void* = 0)
        {
            fake_allocator__allocate_called = true;
            fake_allocator__allocation_size = n;
            return reinterpret_cast<pointer>(fake_allocator__ptr_allocation_requested);
        }

        void deallocate(pointer p, size_type n)
        {
            fake_allocator__deallocation_size = n;
            fake_allocator__deallocate_called = true;
            fake_allocator__ptr_deallocation_address = reinterpret_cast<void*>(p);
        }
        
        size_type max_size() const throw();
        pointer address(reference x) const;
        const_pointer address(const_reference x) const;
        
        void construct(pointer p, T const& val) {
            ++fake_allocator__allocator_construct_called_count;
        }
        
        void destroy(pointer p) {
            ++fake_allocator__allocator_destroy_called_count;
        }
    };
    
#endif