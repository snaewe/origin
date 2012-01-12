// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_FILTER_HPP
#define ORIGIN_RANGE_FILTER_HPP

#include <origin/algorithm.hpp>
#include <origin/iterator.hpp>
#include <origin/iterator/filter.hpp>
#include <origin/range/traits.hpp>

namespace origin
{

  // A filter range...
  //
  // Note that R may be const. 
  template<typename R, typename Pred>
    class filter_range
    {
    public:
      using value_type = Value_type<R>;
      using iterator = filter_iterator<Iterator_type<R>, Pred>;
      
      // Construct a filter range over the underlying range. The predicate
      // may be omitted if Pred is Default_constructible.
      filter_range(R& r, Pred pred = Pred{})
        : range(r), pred(pred)
      { }


      // Returns the underlying range.
      R const& base() const { return range; }
      
      // Returns the predicate function.
      Pred predicate() const { return pred; }
      
      // Begin and end.
      iterator begin() const { return {std::begin(range), std::end(range), pred}; }
      iterator end() const   { return {std::end(range), pred}; }
      
    private:
      R& range;
      Pred pred;
    };
    
  // Return an adapted filter over the given range.
  template<typename R, typename Pred>
    inline auto filter(R& range, Pred pred)
      -> Requires<Range<R>(), filter_range<R, Pred>>
    {
      return {range, pred};
    }

  // A constant version of the function above.
  template<typename R, typename Pred>
    inline auto filter(R const& range, Pred pred)
      -> Requires<Range<R>(), filter_range<R const, Pred>>
    {
      return {range, pred};
    }

} // namespace origin

#endif
