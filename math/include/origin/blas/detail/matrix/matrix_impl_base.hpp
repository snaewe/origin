// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include "matrix_alloc_base.hpp"

namespace origin
{
    template<typename T, std::size_t Dimension, typename Alloc>
    struct matrix_impl_base
        :public matrix_alloc_base<T, Alloc>
    {
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