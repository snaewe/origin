// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_TRANSFORM_RANGE_HPP
#define ORIGIN_RANGE_TRANSFORM_RANGE_HPP

#include <origin/range.hpp>
#include <origin/iterator/transform_iterator.hpp>

namespace origin
{
  // A reverse range is a bounded range adaptor that traverses the elements in
  // reverse order.
  template<typename I, typename F>
    class transform_range
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Function<F, Value_type<I>>(), "");
    public:
      using value_type = Value_type<I>;
      using difference_type = Distance_type<I>;
      using iterator = transform_iterator<I, F>;
      
      transform_range(I first, I last, F func)
        : data{first, last, func}
      { }
      
      // Returns the transformation function
      F transform_func() const { return func(); }
      
      // Range operations
      iterator begin() const { return {first(), func()}; }
      iterator end() const   { return {last(), func()}; }
      
    private:
      const I& first() const { return std::get<0>(data); }
      const I& last() const  { return std::get<1>(data); }
      const F& func() const  { return std::get<2>(data); }

    private:
      std::tuple<I, I, F> data;
    };
    
    
  // Transformed
  // The transformed function constructs a range [first', last') where each
  // element x' == f(x) for each element x' in [first', last') and 
  // corresponding element x in the original range. The function supports
  // the following syntax:
  //
  //    transformed(first, last, func)
  //    transformed(range, func)

    
    
  // Transformed (iterator range)
  template<typename I, typename F>
    inline transform_range<I, F> transformed(I first, I last, F func)
    {
      return {first, last, func};
    }
    
    
    
  // Transformed (range)
  template<typename R, typename F>
    inline transform_range<Iterator_type<R>, F> transforemd(R& range, F func)
    {
      return {std::begin(range), std::end(range), func};
    }
    
    
    
  // Transformed (const range)
  template<typename R, typename F>
    inline transform_range<Iterator_type<const R>, F> transformed(const R& range, F func)
    {
      return {std::begin(range), std::end(range), func};
    }

} // namespace origin

#endif
