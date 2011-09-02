// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_EXCEPTION_HPP
#define ORIGIN_GRAPH_EXCEPTION_HPP

#include <stdexcept>

namespace origin
{
  // A graph error is thrown to indicate structural errors in a graph such as
  // loops, cycles, etc.
  class graph_error : std::runtime_error
  {
  public:
    graph_error(std::string const& what)
      : std::runtime_error(what)
    { }
  };
  
} // namesapce origin

#endif
