// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_ZIPPED_RANGE_HPP
#define ORIGIN_RANGE_ZIPPED_RANGE_HPP

#include <origin/tuple.hpp>
#include <origin/iterator/zip_iterator.hpp>
#include <origin/range/support.hpp>

namespace origin
{
  /**
   * @internal
   * Function to be applied to compute the begin iterators of a zipped range.
   */
  template<typename Result>
  struct zip_begin_
  {
    typedef Result result_type;

    template<typename... Args>
    result_type operator()(Args&&... args) const
    {
      using std::begin;
      return Result{begin(args)...};
    }
  };

  /**
   * @internal
   * Function to be applied to compute the end iterators of a zipped range.
   */
  template<typename Result>
  struct zip_end_
  {
    typedef Result result_type;

    template<typename... Args>
    result_type operator()(Args&&... args) const
    {
      using std::end;
      return Result{end(args)...};
    }
  };

  /**
   * The zipped range wraps a tuple of other ranges, exposing zip iterators.
   */
  template<typename... Ranges>
  class zip_range
  {
    typedef std::tuple<Ranges...> range_tuple;
  public:
    typedef zip_iterator<
      typename range_traits<typename std::remove_reference<Ranges>::type>::iterator...
    > iterator;

    zip_range(Ranges&... rngs)
      : ranges_(rngs...)
    { }

    iterator begin() const
    { return tuple_invoke(zip_begin_<iterator>{}, ranges_); }

    iterator end() const
    { return tuple_invoke(zip_end_<iterator>{}, ranges_); }


  private:
    range_tuple ranges_;
  };

  template<typename... Ranges>
  inline zip_range<Ranges&...>
  zip(Ranges&... rngs)
  { return {rngs...}; }

  template<typename... Ranges>
  inline zip_range<Ranges const...>
  zip(Ranges const&... rngs)
  { return {rngs...}; }

} // namespace origin

#endif
