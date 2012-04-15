// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/blas/detail/matrix/matrix_alloc_base.hpp>
#include <cassert>
#include <memory>
#include <iostream>

using namespace origin;

// Global variables associated with the fake
// allocator.
void* fake_allocator__ptr_allocation_requested;
bool fake_allocator__allocate_called = false;
bool fake_allocator__allocator_copy_called = false;
bool fake_allocator__default_constructor_called = false;
bool fake_allocator__copy_other_allocator_called = false;
std::size_t fake_allocator__allocation_size = 0;
void* fake_allocator__ptr_deallocation_address = 0;
std::size_t fake_allocator__deallocation_size = 0;
bool fake_allocator__destructor_called = false;
bool fake_allocator__deallocate_called = false;
bool fake_allocator__allocator_move_called = false;

void reset_allocator_test_variables()
{
    fake_allocator__ptr_allocation_requested = (void*)0xdeadbeef;
    fake_allocator__allocator_move_called = false;
    fake_allocator__allocator_copy_called = false;
    fake_allocator__default_constructor_called = false;
    fake_allocator__copy_other_allocator_called = false;
    fake_allocator__allocation_size = 0;
    fake_allocator__ptr_deallocation_address = (void*)0;
    fake_allocator__deallocation_size = 0;
    fake_allocator__destructor_called = false;
    fake_allocator__allocate_called = false;
    fake_allocator__deallocate_called = false;
}

template <class T>
    class fake_allocator
    {
    public:
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T*        pointer;
        typedef const T*  const_pointer;
        typedef T&        reference;
        typedef const T&  const_reference;
        typedef T         value_type;

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
            fake_allocator__allocator_move_called = true;
        }
        
        fake_allocator(const fake_allocator&) throw()
        {
            fake_allocator__allocator_copy_called = true;
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
        
        // Functions not used during testing.
        size_type max_size() const throw();
        pointer address(reference x) const;
        const_pointer address(const_reference x) const;
        void construct(pointer p, const T& val);
        void destroy(pointer p);
    };

// Testing Type.
template<typename T, typename Alloc>
    struct matrix_alloc_base_impl_test
        :matrix_alloc_base<T, Alloc>
    {
        typedef matrix_alloc_base<T, Alloc> base;
        typedef typename base::matrix_alloc_impl_base base_impl_type;
        typedef typename base::rebound_alloc_type allocator_test_type;
        
        static void run_tests()
        {
            reset_allocator_test_variables();
            impl_test__default_ctor();
            reset_allocator_test_variables();
            impl_test__alloc_copy_ctor();
            reset_allocator_test_variables();
            impl_test__alloc_move_ctor();
            impl_test__swap();
        }
        
        // Impl Tests
        static void impl_test__default_ctor()
        {
            base_impl_type temp;
            assert(fake_allocator__default_constructor_called);
            assert(temp.start == 0);
            assert(temp.finish == 0);
        }
        
        static void impl_test__alloc_copy_ctor()
        {
            allocator_test_type temp_alloc;
            base_impl_type temp(temp_alloc);
            assert(fake_allocator__allocator_copy_called);
            assert(temp.start == 0);
            assert(temp.finish == 0);
        }
        
        static void impl_test__alloc_move_ctor()
        {
            allocator_test_type temp_alloc;
            base_impl_type temp(std::move(temp_alloc));
            assert(fake_allocator__allocator_move_called);
            assert(temp.start == 0);
            assert(temp.finish == 0);
        }
        
        static void impl_test__swap()
        {
            base_impl_type temp1;
            temp1.start = (T*)0xdeadbeef;
            temp1.finish = (T*)0xdeadb0de;
            base_impl_type temp2;
            temp2.start = (T*)0xdef1c8e;
            temp2.finish = (T*)0xc0ffee;
            temp1.swap(temp2);
            assert(temp1.start == (T*)0xdef1c8e);
            assert(temp1.finish == (T*)0xc0ffee);
            assert(temp2.start == (T*)0xdeadbeef);
            assert(temp2.finish == (T*)0xdeadb0de);
        }
    };

template<typename T, typename Alloc>
    struct matrix_alloc_base_test
        :matrix_alloc_base<T, Alloc>
    {
        typedef matrix_alloc_base<T, Alloc> testing_type;
        using pointer = typename testing_type::pointer;
        
        // typedef typename base::matrix_alloc_impl_base base_impl_type;
        // typedef typename base::rebound_alloc_type allocator_test_type;
        
        static void run_tests()
        {
            reset_allocator_test_variables();
            alloc_base__default_ctor();
            
            reset_allocator_test_variables();
            alloc_base__alloc_copy_ctor();
            
            reset_allocator_test_variables();
            alloc_base__n_items_ctor();
            
            reset_allocator_test_variables();
            alloc_base__n_items_alloc_copy_ctor();
            
            reset_allocator_test_variables();
            alloc_base__move_ctor();
            
            reset_allocator_test_variables();
            alloc_base__copy_ctor();
            
            reset_allocator_test_variables();
            alloc_base__move_copy_alloc_ctor();

            reset_allocator_test_variables();
            alloc_base__destructor();
            
            reset_allocator_test_variables();
            alloc_base__allocate();
            
            reset_allocator_test_variables();
            alloc_base__deallocate();

        }
        
        static void alloc_base__default_ctor()
        {
            testing_type temp;
            assert(fake_allocator__default_constructor_called);
            assert(temp.base_impl.start == 0);
            assert(temp.base_impl.finish == 0);
        }
        
        static void alloc_base__alloc_copy_ctor()
        {
            testing_type temp;
            reset_allocator_test_variables();
            
            // Create copy instance
            testing_type temp2(temp.get_allocator());
            assert(fake_allocator__allocator_copy_called);
            assert(!fake_allocator__allocate_called);
            assert(temp2.base_impl.start == 0);
            assert(temp2.base_impl.finish == 0);
        }
        
        static void alloc_base__n_items_ctor()
        {
            std::size_t allocation_size = 4;
            testing_type temp(allocation_size);
            assert(fake_allocator__default_constructor_called);
            assert(fake_allocator__allocate_called);
            assert(fake_allocator__allocation_size == allocation_size);
            assert(temp.base_impl.start == (pointer)0xdeadbeef);
            assert(temp.base_impl.finish == ((pointer)0xdeadbeef) + allocation_size);
        }
        
        static void alloc_base__n_items_alloc_copy_ctor()
        {
            std::size_t allocation_size = 4;
            testing_type temp(allocation_size);
            assert(fake_allocator__default_constructor_called);
            assert(fake_allocator__allocate_called);
            assert(fake_allocator__allocation_size == allocation_size);
            assert(temp.base_impl.start == (pointer)0xdeadbeef);
            assert(temp.base_impl.finish == ((pointer)0xdeadbeef) + allocation_size);
        }
        
        static void alloc_base__move_ctor()
        {
            testing_type temp;
            std::size_t allocation_size = 4;
            typename testing_type::pointer startValue = (typename testing_type::pointer)0x5550000;
            typename testing_type::pointer finishValue = startValue + allocation_size ;
            temp.base_impl.start = startValue;
            temp.base_impl.finish = finishValue;
            reset_allocator_test_variables();
            
            // Create copy instance
            testing_type temp2(std::move(temp));
            assert(fake_allocator__allocator_move_called);
            assert(!fake_allocator__allocate_called);
            assert(!fake_allocator__deallocate_called);
            assert(temp2.base_impl.start != 0);
            assert(temp2.base_impl.finish != 0);
            assert(temp.base_impl.start == 0);
            assert(temp.base_impl.finish == 0);
            assert(std::size_t(temp2.base_impl.finish - temp2.base_impl.start) == allocation_size);
            assert(temp2.base_impl.start == startValue);
            assert(temp2.base_impl.finish == finishValue);
        }
        
        static void alloc_base__copy_ctor()
        {
            testing_type temp;
            std::size_t allocation_size = 4;
            typename testing_type::pointer startValue = (typename testing_type::pointer)0x5550000;
            typename testing_type::pointer finishValue = startValue + allocation_size ;
            temp.base_impl.start = startValue;
            temp.base_impl.finish = finishValue;
            reset_allocator_test_variables();
            
            // Create copy instance
            testing_type temp2(temp);
            assert(fake_allocator__allocator_copy_called);
            assert(fake_allocator__allocate_called);
            assert(fake_allocator__allocation_size == allocation_size);
            assert(temp2.base_impl.start != 0);
            assert(temp2.base_impl.finish != 0);
            assert(std::size_t(temp2.base_impl.finish - temp2.base_impl.start) == allocation_size);
        }
        
        static void alloc_base__move_copy_alloc_ctor()
        {
            testing_type temp;
            std::size_t allocation_size = 4;
            typename testing_type::pointer startValue = (typename testing_type::pointer)0x5550000;
            typename testing_type::pointer finishValue = startValue + allocation_size ;
            temp.base_impl.start = startValue;
            temp.base_impl.finish = finishValue;
            reset_allocator_test_variables();
            
            // Create copy instance
            testing_type temp2(std::move(temp), temp.get_allocator());
            assert(fake_allocator__allocator_copy_called);
            assert(!fake_allocator__allocator_move_called);
            assert(!fake_allocator__allocate_called);
            assert(!fake_allocator__deallocate_called);
            assert(temp2.base_impl.start != 0);
            assert(temp2.base_impl.finish != 0);
            assert(temp.base_impl.start == 0);
            assert(temp.base_impl.finish == 0);
            assert(std::size_t(temp2.base_impl.finish - temp2.base_impl.start) == allocation_size);
            assert(temp2.base_impl.start == startValue);
            assert(temp2.base_impl.finish == finishValue);        
        }

        static void alloc_base__destructor()
        {
            {
                testing_type temp(4);
            }
            assert(fake_allocator__destructor_called);
            assert(fake_allocator__deallocation_size == 4);
            assert(fake_allocator__deallocate_called);
        }
        
        static void alloc_base__allocate()
        {
            testing_type temp;
            temp.allocate(4);
            assert(fake_allocator__allocate_called);
            assert(fake_allocator__allocation_size == 4);
        }
        
        static void alloc_base__deallocate()
        {
            testing_type temp;
            typename testing_type::pointer tempPtr =  temp.allocate(4);
            assert(fake_allocator__allocate_called);
            assert(fake_allocator__allocation_size == 4);
            temp.deallocate(tempPtr, 4);
            assert(fake_allocator__deallocate_called);
            assert(fake_allocator__deallocation_size == 4);
        }
    };

int main()
{
    
    {
        typedef matrix_alloc_base_impl_test<float, fake_allocator<float>> allocator_test_type;
        allocator_test_type::run_tests();
    }
    
    {
        typedef matrix_alloc_base_test<float, fake_allocator<float>> test_type;
        test_type::run_tests();
    }
    return 0;
    
}