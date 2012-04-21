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
    struct dynarray_allocator_test_type
        :public dynarray<T, fake_allocator<T>>
    {
        using fake_alloc_t = fake_allocator<T>;
        using dynarray_t = dynarray<T, fake_alloc_t>;
        using dynarray_base_t = dynarray_base<T, fake_alloc_t>;
        using typename dynarray_t::pointer;
        using typename dynarray_t::const_pointer;
        
        static pointer get_first(dynarray_t& x)
        {
            return static_cast<dynarray_base_t&>(x).first;
        }
        
        static pointer get_last(dynarray_t& x)
        {
            return static_cast<dynarray_base_t&>(x).last;
        }
        
        static const_pointer get_first(dynarray_t const& x)
        {
            return static_cast<dynarray_base_t const&>(x).first;
        }
        
        static const_pointer get_last(dynarray_t const& x)
        {
            return static_cast<dynarray_base_t const&>(x).last;
        }
        
        
        static void run_tests()
        {
            default_constructor();
            reset_allocator_test_variables();
            allocator_constructor();
            reset_allocator_test_variables();
        }
        
        // dynarray()
        static void default_constructor()
        {
            dynarray_t subject;
            assert(fake_allocator__default_constructor_called);
            assert(get_first(subject) == nullptr);
            assert(get_last(subject) == nullptr);
            assert(subject.size() == 0);
            assert(subject.empty());
        }
        
        // dynarray(allocator_type const& alloc)
        static void allocator_constructor()
        {
            fake_alloc_t testAlloc;
            reset_allocator_test_variables();
            dynarray_t subject(testAlloc);
            assert(!fake_allocator__default_constructor_called);
            assert(fake_allocator__copy_constructor_called);
            assert(get_first(subject) == nullptr);
            assert(get_last(subject) == nullptr);
            assert(subject.size() == 0);
            assert(subject.empty());
        }
        
        // n item constructor. explicit dynarray(size_type n)
        
    };

int main()
{
    dynarray_allocator_test_type<float>::run_tests();
    
    /*
    // dynarray test suite.
    {
        
        // default constrcutor. 
        {
            dynarray_t x;
            assert(x.size() == 0);
            assert(x.empty());
            assert(x.begin() == x.end());
        }
        
        // Allocator constructor. 
        {
        }
        
        
        {
            dynarray<string> x(5);
            assert(( x.size() == 5 ));
        }
        
    }
    */
    /*
    // class dynarray
    // Constructors & destructors.
    
    
    dynarray(dynarray const& x)
    dynarray(dynarray&& x)
    template<typename Iter> dynarray(Iter first, Iter last)
    template<typename Iter> dynarray(Iter first, Iter last, allocator_type const& alloc)
    dynarray(std::initializer_list<value_type> list)
    dynarray(std::initializer_list<value_type> list, allocator_type const& alloc = allocator_type{})
    explicit dynarray(size_type n, value_type const& x = value_type{})
    explicit dynarray(size_type n, value_type const& x = value_type{}, allocator_type const& alloc = allocator_type{})
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
