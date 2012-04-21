// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <origin/dynarray/dynarray.hpp>
#include "fake_allocator.hpp"
using namespace std;
using namespace origin;



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
    

    return 0;
    
}