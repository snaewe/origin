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
// #include <set>
// #include <vector>

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

template <typename T>
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

struct static_alloc_helper {
    
    // Constructor & destructor invocation monitors.
    static bool default_ctor_called;
    static bool copy_ctor_called;
    static bool copy_other_ctor_called;
    static bool move_ctor_called;
    static bool dtor_called;
    
    // Member invocation monitors.
    static bool allocate_called;
    static void* allocated_memory_ptr;
    static std::size_t allocation_size;
    
    static bool deallocate_called;
    static void* deallocated_memory_ptr;
    static std::size_t deallocation_size;
    
    static bool construct_called;
    static std::size_t construct_call_count;
    
    static bool destroy_called;
    static std::size_t destroy_call_count;
    
    // Member functions
    static void reset()
    {
        default_ctor_called = false;
        copy_ctor_called = false;
        copy_other_ctor_called = false;
        move_ctor_called = false;
        dtor_called = false;
    
        allocate_called = false;
        allocated_memory_ptr = nullptr;
        allocation_size = 0;
    
        deallocate_called = false;
        deallocated_memory_ptr = nullptr;
        deallocation_size = 0;
    
        construct_called = false;
        construct_call_count = 0;
    
        destroy_called = false;
        destroy_call_count = 0;
    }
};

// Constructor & destructor invocation monitors.
bool static_alloc_helper::default_ctor_called = false;
bool static_alloc_helper::copy_ctor_called = false;
bool static_alloc_helper::copy_other_ctor_called = false;
bool static_alloc_helper::move_ctor_called = false;
bool static_alloc_helper::dtor_called = false;

// Member invocation monitors.
bool static_alloc_helper::allocate_called = false;
void* static_alloc_helper::allocated_memory_ptr = nullptr;
std::size_t static_alloc_helper::allocation_size;

bool static_alloc_helper::deallocate_called = false;
void* static_alloc_helper::deallocated_memory_ptr = nullptr;
std::size_t static_alloc_helper::deallocation_size = 0;

bool static_alloc_helper::construct_called = false;
std::size_t static_alloc_helper::construct_call_count = 0;

bool static_alloc_helper::destroy_called = false;
std::size_t static_alloc_helper::destroy_call_count = 0;

void reset_static_alloc_helper()
{
    static_alloc_helper::reset();
}

template<typename T>
    class allocator_wrapper
        :public std::allocator<T>,
        public static_alloc_helper
    {
    public:
        using alloc_base = std::allocator<T>;
        using typename alloc_base::size_type;
        using typename alloc_base::difference_type;
        using typename alloc_base::pointer;
        using typename alloc_base::const_pointer;
        using typename alloc_base::reference;
        using typename alloc_base::const_reference;
        using typename alloc_base::value_type;

        template <class U>
            struct rebind
            {
                typedef allocator_wrapper<U> other;
            };

        allocator_wrapper() noexcept
            :alloc_base()
        {
            default_ctor_called = true;
        }
        
        allocator_wrapper(allocator_wrapper<T>&& x) noexcept
            :alloc_base(x)
        {
            move_ctor_called = true;
        }
        
        allocator_wrapper(allocator_wrapper<T> const& x) noexcept
            :alloc_base(x)
        {
            copy_ctor_called = true;
        }

        template <class U>
            allocator_wrapper(allocator_wrapper<U> const& x) noexcept
                :alloc_base(x)
            {
                copy_other_ctor_called = true;
            }

        ~allocator_wrapper() noexcept
        {
            dtor_called = true;
        }
        
        pointer allocate(size_type n, void* h= 0)
        {
            allocate_called = true;
            allocation_size = n;
            allocated_memory_ptr = alloc_base::allocate(n, h);
            return  pointer(allocated_memory_ptr);
        }

        void deallocate(pointer p, size_type n)
        {
            deallocate_called = true;
            deallocation_size = n;
            deallocated_memory_ptr = pointer(p);
            alloc_base::deallocate(p, n);
        }
        
        using alloc_base::max_size;
        using alloc_base::address;

        template<typename... Args>
            void construct(pointer p, Args... args)
            {
                construct_called = true;
                ++construct_call_count;
                alloc_base::construct(p, std::forward<Args>(args)...);
            }
        
        void destroy(pointer p)
        {
            destroy_called = true;
            ++destroy_call_count;
            alloc_base::destroy(p);
        }
    };
#endif