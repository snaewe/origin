// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_CONTAINER_RANGE_HPP
#define ORIGIN_RANGE_CONTAINER_RANGE_HPP

#include <origin/range/support.hpp>

namespace origin
{
  /**
   * @ingroup range
   * The container abstracts a range on a container, allowing it to be passed
   * by value. If the container type parameter is const-qualified, then the
   * iterator type will be a constant iterator.
   *
   * @tparam Cont A Container type, possibly const.
   */
  template<typename Cont>
  class container_range
  {
    typedef typename range_traits<Cont>::iterator base_iterator;
  public:
    typedef base_iterator iterator;

    container_range() = delete;

    container_range(Cont& c)
      : cont_(c)
    { }

    iterator begin() const
    { return cont_.begin(); }

    iterator end() const
    { return cont_.end(); }

  private:
    Cont& cont_;
  };

  /** Return a range for the given container. */
  template<typename Cont>
  inline container_range<Cont> get_range(Cont& c)
  { return c; }

  template<typename Cont>
  inline container_range<Cont> get_range(Cont const& c)
  { return c; }

} // namespace origin

#endif
