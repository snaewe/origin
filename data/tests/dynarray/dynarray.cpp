// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <stdexcept>

#include <origin/dynarray/dynarray.hpp>

using namespace std;
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
        
        size_type max_size() const throw();
        pointer address(reference x) const;
        const_pointer address(const_reference x) const;
        void construct(pointer p, T const& val);
        void destroy(pointer p);
    };
    
    /*
void fake_allocator_assert__called_default_ctor(string const& msg) {
    if(!fake_allocator__default_constructor_called) {
        throw std::runtime_error(msg);
    }
}
*/

int main()
{
    {
        dynarray<int> x;
        assert(( x.empty() ));
    }

    {
        dynarray<string> x(5);
        assert(( x.size() == 5 ));
    }
    /*
    
    // dynarray_base
    dynarray_base(dynarray_base const& x)
    dynarray_base(dynarray_base&& x)
    dynarray_base(allocator_type const& alloc)
    dynarray_base(dynarray_base&& x, allocator_type const& alloc)
    dynarray_base(size_type n, allocator_type const& alloc)
    ~dynarray_base()
    rebound_alloc_type const& get_rebound_allocator() const
    rebound_alloc_type& get_rebound_allocator()
    size_type size() const
    allocator_type get_alloc() const
    pointer allocate(size_type n)
    void deallocate(pointer p)
    
    
    
    // class dynarray
    // Constructors & destructors.
    dynarray()
    dynarray(allocator_type const& alloc)
    dynarray(dynarray const& x)
    dynarray(dynarray&& x)
    template<typename Iter> dynarray(Iter first, Iter last)
    template<typename Iter> dynarray(Iter first, Iter last, allocator_type const& alloc)
    dynarray(std::initializer_list<value_type> list)
    dynarray(std::initializer_list<value_type> list, allocator_type const& alloc = allocator_type{})
    explicit dynarray(size_type n)
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
