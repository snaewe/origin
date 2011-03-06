// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_COLORING_COMMON_HPP
#define ORIGIN_GRAPH_ALGORITHM_COLORING_COMMON_HPP

#include <origin/graph/algorithm/coloring/common.hpp>

namespace origin
{
  /**
   * A graph coloring error is a runtime error that indicates that a requested 
   * graph coloring cannot be computed.
   */
  class graph_coloring_error 
    : public std::runtime_error
  {
  public:
    graph_coloring_error(std::string const& what)
      : std::runtime_error(what)
    { };
  };
} // namespace origin

#endif

