// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MEMORY_HPP
#define ORIGIN_MEMORY_HPP

#include <memory>

namespace origin
{
  /**
   * @fn destroy(f, l, alloc)
   *
   * Destroy the objects in the given iterator range.
   *
   * @tparam Iter
   * @tparam Alloc
   *
   * @param f       The initial iterator
   * @param l       The final iterator
   * @param alloc   A reference to an allocator
   */
  //@{
  // FIXME: Specialize this for trivially destructible types.
  template<typename Iter, typename Alloc>
  void destroy(Iter f, Iter l, Alloc& alloc)
  {
    for( ; f != l; ++f)
      alloc.destroy(std::addressof(*f));
  }
  //@}

} // namespace origin

#endif
