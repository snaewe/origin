// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_UNTIL_HPP
#define ORIGIN_RANGE_UNTIL_HPP

#include <origin/range/core.hpp>
#include <origin/iterator/until.hpp>

namespace origin
{
  /*
  // Terminating range
  // A terminate stops whenever a specified predicate is true.
  //
  // FIXME: Does the terminating function actually need to be a regular
  // function? Does it even need to take an iterator as an argument? It could
  // simply be called to observe some non-local state.
  template <typename I, typename P>
    class terminating_range
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Predicate<P, Value_type<I>>(), "");
    public:
      using value_type = Value_type<I>;
      using iterator = until_iterator<I, P>;
      
      terminating_range(I first, I last, P pred)
        : data {first, last, pred}
      { }
      
      // Returns the transformation function
      P terminating_pred() const { return pred(); }
      
      // Range operations
      iterator begin() const { return {first(), last(), pred()}; }
      iterator end() const   { return {last(), last(), pred()}; }
      
    private:
      const I& first() const { return std::get<0>(data); }
      const I& last() const  { return std::get<1>(data); }
      const P& pred() const  { return std::get<2>(data); }

    private:
      std::tuple<I, I, P> data;
    };
  */
    

  // Until
  //
  // The until function constructs a range...

  template <typename I, typename P>
    inline bounded_range<until_iterator<I, P>> until(I first, I last, P pred)
    {
      return {
        make_until_iterator(first, last, pred),
        make_until_iterator(last, pred)
      };
    }
    
  template <typename R, typename P>
    inline auto until(R&& range, P pred)
      -> decltype(until(begin(range), end(range), pred))
    {
      return until(begin(range), end(range), pred);
    }


} // namespace origin

#endif
