// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/memory.hpp>


// Memory initializer
// The memory initializer class is responsible for the initialaization of
// global memory resources such as the default allocator.
class memory_initializer
{
public:
    origin::allocator alloc;
};

// Globally initialized memory object.
memory_initializer memory_init;



namespace origin
{
  allocator& default_allocator()
  {
    return memory_init.alloc;
  }
}

