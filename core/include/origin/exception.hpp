// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_EXCEPTION_HPP
#define ORIGIN_EXCEPTION_HPP

#include <stdexcept>

namespace origin
{
  // Not implemented (exception)
  // The 'not implemented' exception is a runtime error that is thrown to
  // indicate that a function has been called that has not been implemented.
  //
  // FIXME: This is currently classified as a runtime error, but it really
  // isn't. It's not really a logic error either. It's more of a program
  // limitation than anything else. It might be nice to reclassify it as such.
  class not_implemented : public std::runtime_error
  {
  public:
    not_implemented(std::string const& what)
      : std::runtime_error(what + " is not implemented")
    { }
  };




}

#endif
