// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <origin/dynarray/dynarray.hpp>

using namespace std;
using namespace origin;

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

int main()
{
        using fake_alloc = fake_allocator<float>;
    // dynarray_base test suite.
    {
        using dynarray_base_t = dynarray_base<float, fake_alloc>;
        using pointer = typename dynarray_base_t::pointer;
        
        // Default constrcutor
        {
            dynarray_base_t x;
            assert(fake_allocator__default_constructor_called);
            assert(x.first == nullptr);
            assert(x.last == nullptr);
        }
        
        // destructor.
        assert(fake_allocator__destructor_called);
        reset_allocator_test_variables();
        
        // copy constructor.
        {
            dynarray_base_t x;
            x.first = pointer(0xdeadbeef);
            x.last = pointer(0xdeadbeef);
            reset_allocator_test_variables();
            dynarray_base_t xcopy(x);
            assert(!fake_allocator__default_constructor_called);
            assert(fake_allocator__copy_constructor_called);
            assert(xcopy.first == pointer(0xdeadbeef));
            assert(xcopy.last == pointer(0xdeadbeef));
        }
        reset_allocator_test_variables();
        
        // move constructor.
        {
            dynarray_base_t x;
            x.first = pointer(0xdeadbeef);
            x.last = pointer(0xdeadbeef);
            reset_allocator_test_variables();
            dynarray_base_t xmove(move(x));
            assert(!fake_allocator__default_constructor_called);
            assert(!fake_allocator__copy_constructor_called);
            assert(fake_allocator__move_constructor_called);
            assert(xmove.first == pointer(0xdeadbeef));
            assert(xmove.last == pointer(0xdeadbeef));
            assert(x.first == nullptr);
            assert(x.last == nullptr);
        }
        reset_allocator_test_variables();
        
        // move + copy allocator constructor.
        {
            dynarray_base_t x;
            x.first = pointer(0xdeadbeef);
            x.last = pointer(0xdeadbeef);
            reset_allocator_test_variables();
            dynarray_base_t xmove(move(x), x.get_allocator());
            assert(!fake_allocator__default_constructor_called);
            assert(fake_allocator__copy_constructor_called);
            assert(!fake_allocator__move_constructor_called);
            assert(xmove.first == pointer(0xdeadbeef));
            assert(xmove.last == pointer(0xdeadbeef));
            assert(x.first == nullptr);
            assert(x.last == nullptr);
        }
        reset_allocator_test_variables();
        
        // copy allocator constructor.
        {
            fake_alloc y;
            reset_allocator_test_variables();
            dynarray_base_t x(y);
            assert(!fake_allocator__default_constructor_called);
            assert(fake_allocator__copy_constructor_called);
            assert(!fake_allocator__move_constructor_called);            
            assert(x.first == nullptr);
            assert(x.last == nullptr);
        }
        reset_allocator_test_variables();
        
        // n item constructor.
        {
            dynarray_base_t x(3);
            assert(x.first != nullptr);
            assert(x.last != nullptr);
            assert(x.first == pointer(0xdeadbeef));
            assert(x.last == pointer(0xdeadbeef) + 3);
            assert(fake_allocator__default_constructor_called);
            assert(fake_allocator__allocate_called);
            assert(fake_allocator__allocation_size == 3);
            assert(x.size() == 3);
        }
        assert(fake_allocator__deallocate_called);
        assert(fake_allocator__deallocation_size == 3);
        assert(fake_allocator__ptr_deallocation_address == pointer(0xdeadbeef));
        reset_allocator_test_variables();
        
        // dynarray_base(size_type n, allocator_type const& alloc)
        // n items + copy allocator constructor test
        {
            fake_alloc y;
            reset_allocator_test_variables();
            dynarray_base_t x(3, y);
            assert(x.first != nullptr);
            assert(x.last != nullptr);
            assert(x.first == pointer(0xdeadbeef));
            assert(x.last == pointer(0xdeadbeef) + 3);
            assert(!fake_allocator__default_constructor_called);
            assert(fake_allocator__copy_constructor_called);
            assert(fake_allocator__allocate_called);
            assert(fake_allocator__allocation_size == 3);
            assert(x.size() == 3);
        }
        assert(fake_allocator__deallocate_called);
        assert(fake_allocator__deallocation_size == 3);
        assert(fake_allocator__ptr_deallocation_address == pointer(0xdeadbeef));
        reset_allocator_test_variables();
    }
    
    // dynarray test suite.
    {
        using dynarray_t = dynarray<float, fake_alloc>;
        
        // default constrcutor.
        {
            dynarray_t x;
            assert(x.size() == 0);
            assert(x.empty());
        }
        
        // n item constructor.
        {
            dynarray<string> x(5);
            assert(( x.size() == 5 ));
        }
    }
    return 0;
    
}