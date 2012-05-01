// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <iostream>
#include <origin/dynarray/dynarray.hpp>
#include "fake_allocator.hpp"

using namespace origin;
using namespace std;


template <typename T>
    struct dynarray_test_type
        :public dynarray<T, allocator_wrapper<T>>
    {
        using self = dynarray_test_type<T>;
        using alloc_type = allocator_wrapper<T>;
        using base = dynarray<T, alloc_type>;
        using typename base::pointer;
        using typename base::const_pointer;
        using typename base::value_type;
        // forwarding constructor for testing.
        template<typename... Args>
            explicit dynarray_test_type(Args...  args)
                :base(std::forward<Args>(args)...)
            { }

        static void run_tests()
        {
            default_constructor();
            reset_static_alloc_helper();
            
            allocator_constructor();
            reset_static_alloc_helper();
            
            n_item_constructor();
            reset_static_alloc_helper();
            
            copy_constructor();
            reset_static_alloc_helper();
            
            move_constructor();
            reset_static_alloc_helper();
            
            range_constructor();
            reset_static_alloc_helper();
            
            range_alloc_constructor();
            reset_static_alloc_helper();
            
            initializer_list_constructor();
            reset_static_alloc_helper();
            
            initializer_list_alloc_constructor();
            reset_static_alloc_helper();
        }
        
        // -----------------------------------------------------------------
        // dynarray()
        static void default_constructor()
        {
            self subject;
            subject.default_constructor_test();
        }

        void default_constructor_test()
        {
            assert(static_alloc_helper::default_ctor_called);
            assert(static_alloc_helper::static_alloc_helper::allocated_memory_ptr == nullptr);
            assert(this->first == nullptr);
            assert(this->last == nullptr);
            assert(this->size() == 0);
            assert(this->empty());
        }

        // -----------------------------------------------------------------
        // dynarray(allocator_type const& alloc)
        static void allocator_constructor()
        {
            alloc_type testAlloc;
            reset_static_alloc_helper();
            self subject(testAlloc);
            subject.allocator_constructor_test();
        }
        
        void allocator_constructor_test()
        {
            assert(static_alloc_helper::copy_ctor_called);
            assert(!static_alloc_helper::copy_other_ctor_called);
            assert(this->first == nullptr);
            assert(this->last == nullptr);
            assert(this->size() == 0);
            assert(this->empty());
        }
        
        // -----------------------------------------------------------------
        // explicit dynarray(size_type n)
        static void n_item_constructor()
        {
            {
                self subject(5);
                subject.n_item_constructor_test();
            }
            assert(static_alloc_helper::destroy_called);
            assert(static_alloc_helper::destroy_call_count == 5);
            assert(static_alloc_helper::dtor_called);
        }
        
        void n_item_constructor_test()
        {
            assert(static_alloc_helper::default_ctor_called);
            assert(this->first != nullptr);
            assert(this->last != nullptr);
            for(pointer p = this->first; p != this->last;++p)
                assert(*p == value_type());
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count == 5);
            assert(this->size() == 5);
            assert(!this->empty());
        }
        
        // -----------------------------------------------------------------
        // copy constructor.
        // dynarray(dynarray const& x)
        static void copy_constructor()
        {
            self subject1(5);
            for(size_t i = 0; i < subject1.size(); ++i) {
                subject1.first[i] = i;
            }
            reset_static_alloc_helper();
            self subject2(subject1);
            subject2.copy_constructor_test(subject1);
        }
        
        void copy_constructor_test(self const& copyOf)
        {
            assert(this->first != nullptr);
            assert(this->last != nullptr);
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count == 5);
            assert(this->size() == 5);
            assert(!this->empty());
        }
        
        // -----------------------------------------------------------------
        // move constructor.
        // dynarray(dynarray&& x)
        static void move_constructor()
        {
            {
                self subject1(5);
                
                for(size_t i = 0; i < subject1.size(); ++i) {
                    subject1.first[i] = i;
                }
                assert(static_alloc_helper::allocate_called);
                reset_static_alloc_helper();
                self subject2(std::move(subject1));
                subject2.move_constructor_test();
            }
            assert(static_alloc_helper::deallocate_called);
        }
        
        void move_constructor_test()
        {
            assert(static_alloc_helper::move_ctor_called);
            assert(!static_alloc_helper::allocate_called);
            assert(this->first != nullptr);
            assert(this->last != nullptr);
            assert(!static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count == 0);
            assert(this->size() == 5);
            assert(!this->empty());
            for(size_t i = 0; i < this->size(); ++i) {
                assert(this->first[i] == i);
            }
        }
        
        // -----------------------------------------------------------------
        // range constructor.
        // template<typename Iter> dynarray(Iter first, Iter last)
        static const size_t range_size = 6;
        
        static void range_constructor()
        {
            value_type val[range_size];
            pointer start_pos = val;
            pointer end_pos = val + range_size;
            for(pointer val_pos = start_pos; val_pos != end_pos; ++val_pos) {
                *val_pos = 100 - (end_pos - val_pos);
            }
            {
                self subject(start_pos, end_pos);
                subject.range_constructor_test();
            }
            assert(static_alloc_helper::destroy_called);
            assert(static_alloc_helper::destroy_call_count == range_size);
            assert(static_alloc_helper::deallocate_called);
        }
        
        void range_constructor_test()
        {
            assert(static_alloc_helper::allocate_called);
            assert(this->first != nullptr);
            assert(this->last != nullptr);
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count == range_size);
            assert(this->size() == range_size);
            assert(!this->empty());
            for(pointer val_pos = this->first; val_pos != this->last; ++val_pos) {
                assert((*val_pos == 100 - (this->last - val_pos)));
            }
        }

        // -----------------------------------------------------------------
        // range + allocator constructor.
        // template<typename Iter> dynarray(Iter first, Iter last, allocator_type const& alloc)
        static void range_alloc_constructor()
        {
            value_type val[range_size];
            pointer start_pos = val;
            pointer end_pos = val + range_size;
            for(pointer val_pos = start_pos; val_pos != end_pos; ++val_pos) {
                *val_pos = 100 - (end_pos - val_pos);
            }
            {
                self subject(start_pos, end_pos, alloc_type());
                subject.range_constructor_test();
                subject.range_alloc_constructor_test();
            }
        }
        
        void range_alloc_constructor_test()
        {
            assert(static_alloc_helper::copy_ctor_called);
        }
        
        // -----------------------------------------------------------------
        // initializer list constructor
        // dynarray(std::initializer_list<value_type> list)
        static void initializer_list_constructor()
        {
            std::initializer_list<T> temp_init_list({1.0, 6.0, 29.1,94.93});
            {
                self subject(temp_init_list);
                subject.initializer_list_constructor_test(temp_init_list);
            }
            assert(static_alloc_helper::destroy_called);
            assert(static_alloc_helper::destroy_call_count == temp_init_list.size());
            assert(static_alloc_helper::deallocate_called);
            
        }
        
        void initializer_list_constructor_test(std::initializer_list<T> const& init_list)
        {
            assert(static_alloc_helper::allocate_called);
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count = init_list.size());
            auto current_init_list_item = init_list.begin();
            for(pointer p = this->first; p != this->last; ++p) {
                assert(*p == *current_init_list_item);
                ++current_init_list_item;
            }
        }
        
        // -----------------------------------------------------------------
        // initializer list + alloc constructor
        // dynarray(std::initializer_list<value_type> list, allocator_type const& alloc)
        static void initializer_list_alloc_constructor()
        {
            std::initializer_list<T> temp_init_list({1.0, 6.0, 29.1,94.93});
            {
                self subject(temp_init_list, alloc_type());
                subject.initializer_list_constructor_test(temp_init_list);
                subject.initializer_list_alloc_constructor_test();
            }
        }
        
        void initializer_list_alloc_constructor_test()
        {
            assert(static_alloc_helper::copy_ctor_called);
        }
        
        // -----------------------------------------------------------------
        // n item of value constructor
        // explicit dynarray(size_type n, value_type const& x = value_type{})
        static const size_t n_item_count = 3;
        
        static void n_item_of_value_constructor()
        {
            {
                self subject(n_item_count, 28.6);
                subject.n_item_of_value_constructor_test();
            }
        }
        
        void n_item_of_value_constructor_test()
        {
            // assert(static_alloc_helper::copy_ctor_called);
        }        
        
    };

int main()
{
    dynarray_test_type<float>::run_tests();
    /*
    // class dynarray
    // Constructors & destructors.
    
    explicit dynarray(size_type n, value_type const& x, allocator_type const& alloc)
    ~dynarray()
    
    // Member functions + operators.
    dynarray& operator=(dynarray const& x)
    dynarray& operator=(dynarray&& x)
    
    size_type size() const
    constexpr size_type max_size() const
    bool empty() const
    bool operator==(dynarray const& x) const
    bool operator!=(dynarray const& x) const
    bool operator<(dynarray const& x) const
    bool operator>(dynarray const& x) const
    bool operator<=(dynarray const& x) const
    bool operator>=(dynarray const& x) const
    reference operator[](size_type n)
    const_reference operator[](size_type n) const
    reference front()
    const_reference front() const
    reference back()
    const_reference back() const
    reference at(size_type n)
    const_reference at(size_type n) const
    T* data()
    const T* data() const
    iterator begin()
    iterator end()
    const_iterator begin() const
    const_iterator end() const
    const_iterator cbegin() const
    const_iterator cend() const
    reverse_iterator rbegin()
    reverse_iterator rend()
    const_reverse_iterator rbegin() const
    const_reverse_iterator rend() const
    const_reverse_iterator crbegin() const
    const_reverse_iterator crend() const
    void swap(dynarray & x)
    
    // Free Functions.
    template<typename T> void swap(dynarray<T>& a, dynarray<T>& b)
    */
}
