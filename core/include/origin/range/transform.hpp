// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_TRANSFORM_HPP
#define ORIGIN_RANGE_TRANSFORM_HPP

#include <origin/range/core.hpp>
#include <origin/iterator/transform.hpp>

namespace origin
{
  /*
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
  */
    

  // Transformed
  // The transformed function constructs a range [first', last') where each
  // element x' == f(x) for each element x' in [first', last') and 
  // corresponding element x in the original range. The function supports
  // the following syntax:
  //
  //    transformed(first, last, func)
  //    transformed(range, func)

  // Returns a transformed range over the elements in [first, last).
  template <typename I, typename F>
    inline bounded_range<transform_iterator<I, F>> 
    transformed(I first, I last, F func)
    {
      return {
        make_transform_iterator(first, func),
        make_transform_iterator(last, func)
      };
    }

  // Returns a transform range over r.
  template<typename R, typename F>
    inline auto transformed(R&& range, F func)
      -> decltype(transformed(begin(range), end(range), func))
    {
      return transformed(begin(range), end(range), func);
    }

} // namespace origin

#endif
