// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include "matrix_alloc_base.hpp"

namespace origin
{
    template<typename T, typename Alloc>
        struct matrix_impl_base
            :public matrix_alloc_base<T, Alloc>
        {
            
            using alloc_base = matrix_alloc_base<T, Alloc>;
            using value_type = T;
            using typename alloc_base::size_type;
            using typename alloc_base::allocator_type;
            using typename alloc_base::pointer;
        public:
            
            // Default Constructor
            // Default construct an empty matrix.
            matrix_impl_base()
                :alloc_base()
            { }
            
            // Copy Constructor
            // Creates a copy of other.
            explicit matrix_impl_base(matrix_impl_base const& other)
                :alloc_base(other)
            {
                pointer current_pos = this.base_impl.start;
                std::for_each(other.base_impl.start, other.base_impl.finish,
                    [&](pointer p) {
                        new (current_pos) value_type(*p);
                        ++current_pos;
                    });
            }

            // Copy Constructor + Copy Allocator
            // Creates a copy of other and a copy of the allocator alloc.
            matrix_impl_base(matrix_impl_base const& other, allocator_type const& alloc)
                :alloc_base(other, alloc)
            {
                std::copy(other.base_impl.start, other.base_impl.finish, this.base_impl.start);
            }
            
            // Move constructor.
            // Moves owner ship of memory from other to the new instance.
            explicit matrix_impl_base(matrix_impl_base&& other)
                :alloc_base(std::move(other))
            { }

            // Move constructor + Copy Allocator.
            // Moves owner ship of memory from other to the new instance and creates a copy of allocator alloc.
            matrix_impl_base(matrix_impl_base&& other, allocator_type const& alloc)
                :alloc_base(std::move(other), alloc)
            { }
            
            // N size Constructor.
            // Creates a matrix of size N filled with value val.
            explicit matrix_impl_base(size_type n, value_type const& val = value_type())
                :alloc_base()
            { }
            
            
            /*
            matrix_base()
            : first(nullptr), last(first)
            { }
            
            matrix_base(size_type n)
            : first(n > 0 ? new value_type[n] : nullptr), last(first + n)
            { }
            
            // CopyConstructible
            matrix_base(matrix_base const& x)
            : first(new value_type[x.size()]), last(first + x.size())
            { 
            std::copy(x.first, x.last, first); 
            }
            
            // Move Constructible
            matrix_base(matrix_base&& x)
            : first(x.first), last(x.last)
            { 
            x.first = x.last = nullptr; 
            }
            
            // Destructible
            ~matrix_base() { delete [] first; }
                */
        };
    
} // end origin