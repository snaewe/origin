// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_DYNARRAY_HPP
#define ORIGIN_DATA_DYNARRAY_HPP
#include <origin/exception.hpp>
#include <origin/memory.hpp>

namespace origin
{
    // The square dynarray base implements the basic structure and memory
    // allocation requirements of a square dynarray. The dynarray is implemented
    // in terms of a pointer its order.
    template<typename T, typename Alloc>
        struct dynarray_base
            : private Alloc::template rebind<T>::other
        {
            using rebound_alloc_type = typename Alloc::template rebind<T>::other;
            using allocator_type = Alloc;
            using value_type = T;
            using reference = typename rebound_alloc_type::reference ;
            using const_reference = typename rebound_alloc_type::const_reference ;
            using pointer = typename rebound_alloc_type::pointer;
            using const_pointer = typename rebound_alloc_type::const_pointer;
            using size_type =typename rebound_alloc_type::size_type ;
            using difference_type = typename rebound_alloc_type::difference_type;
            
            // Default constructor
            dynarray_base()
                : Alloc{},
                first{},
                last{}
            { }
            
            // Copy semantics
            dynarray_base(dynarray_base const& x)
                : Alloc{x.get_allocator()},
                first{allocate(x.size())},
                last{first + x.size()}
            { }

            // Move semantics
            dynarray_base(dynarray_base&& x)
                : Alloc{std::move(x.get_rebound_allocator())},
                first{x.first},
                last{x.last}
            {
                x.first = x.last = nullptr;
            }

            // Move ctor.
            dynarray_base(dynarray_base&& x, allocator_type const& alloc)
                : Alloc{alloc},
                first{x.first},
                last{x.last}
            {
                x.first = x.last = nullptr;
            }
            
            dynarray_base(allocator_type const& alloc)
                : Alloc{alloc},
                first{nullptr},
                last{nullptr}
            { }

            dynarray_base(size_type n)
                : Alloc{},
                first{allocate(n)},
                last{first + n}
            { }
            
            dynarray_base(size_type n, allocator_type const& alloc)
                : Alloc{alloc},
                first{allocate(n)},
                last{first + n}
            { }

            ~dynarray_base()
            {
                get_rebound_allocator().deallocate(first, size());
            }
            
            rebound_alloc_type const& get_rebound_allocator() const
            {
                return *static_cast<rebound_alloc_type const*>(this);
            }
            
            rebound_alloc_type& get_rebound_allocator()
            {
                return *static_cast<rebound_alloc_type*>(this);
            }
            
            size_type size() const
            {
                return last - first;
            }
            
            allocator_type get_allocator() const
            {
                return allocator_type(*this);
            }

            // Allocate n * n elements
            pointer allocate(size_type n)
            {
                return get_rebound_allocator().allocate(n);
            }

            void deallocate(pointer p)
            {
                get_rebound_allocator().deallocate(p, size());
            }

            pointer first;
            pointer last;
        };

    // 
    // A dynamically allocated array with fixed bounds
    // The dynarray (dynamic array) class implements a dynamically allocated
    // array.
    // 
    // The dynarray defined in n2648. Not all of the functionality is implemented,
    // but the interface is similar to that defined in n2648 by Lawrence Crowl and
    // Matt Austern. We have added swap semantics and const reverse iterators.
    //
    // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2648.html
    template<typename T, typename Alloc = std::allocator<T>>
        class dynarray
            :protected dynarray_base<T, Alloc>
        {
            using base_type = dynarray_base<T, Alloc>;
        public:
            using allocator_type = Alloc ;
            using value_type = T;
            using reference = typename base_type::reference;
            using const_reference = typename base_type::const_reference;
            using pointer = typename base_type::pointer;
            using const_pointer = typename base_type::const_pointer;
            using size_type = typename base_type::size_type;
            using difference_type = typename base_type::difference_type;

            using iterator = pointer;
            using const_iterator = const_pointer;

            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;
            
            // Default constructor
            // Construct an empty dynarray.
            //
            // alloc   An allocator object
            explicit dynarray(allocator_type const& alloc = allocator_type{}) noexcept
                : base_type{alloc}
            { }

            // Copy constructor
            // Construct a copy of the dynarray x.
            // 
            // x   A dynarray
            explicit dynarray(dynarray const& x)
                : base_type{x}
            {
                pointer copy_loc = this->first;
                for(pointer current_pos = x.first; current_pos != x.last;++current_pos) {
                    this->get_rebound_allocator().construct(copy_loc, *current_pos);
                    ++copy_loc;
                }
                
            }

            
            // Copy assignment
            // Assign this object as a copy of the dynarray x.
            //
            // x   A dynarray
            dynarray& operator=(dynarray const& x)
            { dynarray tmp{x}; swap(tmp); return *this; }

            // Move semantics
            // Move constructor
            // Construct a dynarray by moving data out of the dynarray x. After
            // construction x is left in a moved-from state.
            // 
            // x   A dynarray
            explicit dynarray(dynarray&& x) noexcept
                : base_type{std::move(x)}
            { }
            
            // Move assignment
            // Move the state of the dynarray x into this object. After assignment
            // x is left in a moved-from state.
            // 
            // x   A dynarray
            dynarray& operator=(dynarray&& x) noexcept
            { dynarray tmp{std::move(x)}; swap(tmp); return *this; }

            
            template<typename Iter>
                dynarray(Iter first_iter, Iter last_iter, allocator_type const& alloc = allocator_type{})
                    : base_type(std::distance(first_iter, last_iter), alloc)
                {
                    pointer current_local_item = this->first;
                    for(pointer current = first_iter; current != last_iter; ++current) {
                        this->get_rebound_allocator().construct(current_local_item, *current);
                        ++current_local_item;
                    }
                }

            // Initializer list constructor
            // Construct a dynarray over an initializer list.
            //
            // list    An initializer list
            // alloc   An allocator object
            explicit dynarray(std::initializer_list<value_type> list,
                        allocator_type const& alloc = allocator_type{})
                : base_type{list.size(), alloc}
            { 
                pointer current_local_item = this->first;
                for(auto current = list.begin(); current != list.end(); ++current) {
                    this->get_rebound_allocator().construct(current_local_item, *current);
                    ++current_local_item;
                }
            }

            // Fill constructor
            // Construct a dynarray with n elements all either default initialized
            // or initialized to the value x.
            // 
            // n       The required size of the dynarray
            // x       The initial value
            // alloc   An allocator object
            explicit dynarray(size_type n,
                                value_type const& x = value_type{},
                                allocator_type const& alloc = allocator_type{})
                : base_type{n, alloc}
            {
                for(pointer current = this->first; current != this->last; ++current) {
                    this->get_rebound_allocator().construct(current, x);
                }
            }

            ~dynarray() noexcept
            {
                clear();
            }

            void clear() noexcept
            {
                for(pointer current = this->first; current != this->last; ++current) {
                    this->get_rebound_allocator().destroy(current);
                }
                this->get_rebound_allocator().deallocate(this->first, size());
                this->first = this->last = nullptr;
            }
            
            size_type size() const noexcept
            { return base_type::size(); }

            constexpr size_type max_size() const noexcept
            { return this->get_rebound_allocator().max_size(); }

            bool empty() const noexcept
            { return base_type::size() == 0; }

            // EqualityComparable
            bool operator==(dynarray const& x) const noexcept
            {
                if(size() != x.size())
                {
                    return false;
                }
                return std::equal(begin(), end(), x.begin());
            }

            bool operator!=(dynarray const& x) const noexcept { return !(*this == x); }

            // TotallyOrdered
            bool operator<(dynarray const& x) const noexcept
            {
                return std::lexicographical_compare(begin(), end(), x.begin(), x.end());
            }

            bool operator>(dynarray const& x) const noexcept { return x < *this; }
            bool operator<=(dynarray const& x) const noexcept { return !(x < *this); }
            bool operator>=(dynarray const& x) const noexcept { return !(*this < x); }

            reference operator[](size_type n) noexcept
            { return *(this->first + n); }

            const_reference operator[](size_type n) const noexcept
            { return *(this->first + n); }

            reference front() noexcept
            { return *this->first; }

            const_reference front() const noexcept
            { return *this->first; }

            reference back() noexcept
            { return *(this->last - 1); }

            const_reference back() const noexcept
            { return *(this->last - 1); }

            reference at(size_type n)
            { range_check(n); return *(this->first + n); }

            const_reference at(size_type n) const
            { range_check(n); return *(this->first + n); }

            T* data() noexcept
            { return this->first; }

            const T* data() const noexcept
            { return this->first; }

            iterator begin() noexcept
            { return this->first; }

            iterator end() noexcept
            { return this->last; }

            const_iterator begin() const noexcept
            { return this->first; }

            const_iterator end() const noexcept
            { return this->last; }

            const_iterator cbegin() const noexcept
            { return this->first; }

            const_iterator cend() const noexcept
            { return this->last; }

            reverse_iterator rbegin() noexcept
            { return reverse_iterator(end()); }

            reverse_iterator rend() noexcept
            { return reverse_iterator(begin()); }

            const_reverse_iterator rbegin() const noexcept
            { return const_reverse_iterator(cend()); }

            const_reverse_iterator rend() const noexcept
            { return const_reverse_iterator(cbegin()); }

            const_reverse_iterator crbegin() const noexcept
            { return const_reverse_iterator(cend()); }

            const_reverse_iterator crend() const noexcept
            { return const_reverse_iterator(cbegin()); }

            void swap(dynarray & x) noexcept
            {
                std::swap(this->first, x.first);
                std::swap(this->last, x.last);
            }

        private:
            // Helper function for bounds checking
            void range_check(size_type n) const 
            { if(n >= size()) throw std::out_of_range("dynarray: out of range"); }
        };

    template<typename T, typename Alloc>
        void swap(dynarray<T, Alloc>& a, dynarray<T, Alloc>& b) noexcept
        { a.swap(b); }

} // namespace origin

#endif
