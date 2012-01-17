// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_FILTER_RANGE_HPP
#define ORIGIN_RANGE_FILTER_RANGE_HPP

#include <origin/iterator/filter_iterator.hpp>

namespace origin
{
  // A filter range is a bounded range adaptor that describes a subset of
  // elements satisfying a given predicate. In other words, iterating over a
  // filter range visits all elements x in r such that r.predicate()(x) is
  // true.
  template<typename I, typename P>
    class filter_range
    {
      static_assert(Searchable<I, P>(), "");
    public:
      using value_type = Value_type<I>;
      using difference_type = Distance_type<I>;
      using iterator = filter_iterator<I, P>;
      
      // Construct a filter range over the underlying range. The predicate
      // may be omitted if Pred is Default_constructible.
      filter_range(I first, I last, P pred = {})
        : data{first, last, pred}
      { }

      // Returns the predicate function.
      P predicate() const { return pred(); }
      
      // Begin and end.
      iterator begin() const { return {first(), last(), pred()}; }
      iterator end() const   { return {last(), pred()}; }
      
    private:
      const I& first() const { return std::get<0>(data); }
      const I& last() const  { return std::get<1>(data); }
      const P& pred() const  { return std::get<2>(data); }
    
    private:
      std::tuple<I, I, P> data;
    };
    
    
    
  // Filtered
  // The filtered function constructs a bounded range from its range arguments
  // and a unary prediate such that, pred(x) is true for all x in the 
  // constructed range. The constructor supports the following syntax:
  //
  //    filtered(first, last, func)
  //    filtered(range, func)
   
  // Filtered (iterator range)
  // Returns a bounded range [first', last') where pred(x) is true for all 
  // elements in that range.
  template<typename I, typename P>
    inline filter_range<I, P> filtered(I first, I last, P pred)
    {
      return {first, last, pred};
    }
    
    
  
  // Filter (range)
  // Returns a bounded range r' where pred(x) is true for all elements in that
  // range.
  template<typename R, typename P>
    inline filter_range<Iterator_type<R>, P> filtered(R& range, P pred)
    {
      return {std::begin(range), std::end(range), pred};
    }

    
    
  // Filter (const range)
  // Return a bounded range r' where pred(x) is true for all elemnts in that
  // range.
  template<typename R, typename P>
    inline filter_range<Iterator_type<const R>, P> filtered(const R& range, P pred)
    {
      return {std::begin(range), std::end(range), pred};
    }

} // namespace origin

#endif
