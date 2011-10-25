// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_BOUNDED_HPP
#define ORIGIN_RANGE_BOUNDED_HPP

#include <origin/iterator/range.hpp>

namespace origin
{
  // A bounded range wraps a pair of iterators.
  //
  // requires: Incrementable<Iter>
  template<typename Iter>
    class bounded_range
    {
    public:
      typedef Iter iterator;

      // Initialize the bounded range so that both values are the same. The
      // range is initially empty.
      bounded_range()
        : first(), last(first)
      { }
    
      // Initialize the bounded range.
      //
      // precondition: bounded_range(first, last)
      // postcondition: this->begin() == first && this->end() == last
      bounded_range(iterator first, iterator last)
        : first(first), last(last)
      { }
      
      iterator begin() const { return first; }
      iterator end() const { return last; }
      
    private:
      iterator first;
      iterator last;
    };

  // The adapted range class defines a (right) half-open range over some
  // incrementable type, Iter. Each element in the range is an an object in that
  // underlying sequence.
  template<typename Iter>
    class wrapped_bounded_range
    {
    public:
      typedef range_iterator<Iter> iterator;

      wrapped_bounded_range(Iter first, Iter last)
        : first{first}, last{last}
      { }
    
      iterator begin() const { return first; }
      iterator end()   const { return last; }

    private:
      Iter first;
      Iter last;
    };


  // Return a (right) half-open range [first, last) over the elements in that
  // range. For example:
  //
  //    for(auto i : range(0, 5)) count << *i << ' ';
  //
  // prints "0 1 2 3 4". Similarly:
  //
  //    vector<int> v = {1, 2, 3}
  //    for(auto i : range(v.begin(), v.end())) cout << **i << ' ';
  //
  // prints "1 2 3". Because the arguments to range are iterators, each value
  // of i is also an iterator (hence the need to write **i).
  //
  // requires: Incrementable<Iter>
  // precondition: bounded_range(first, last);
  template<typename Iter>
    wrapped_bounded_range<Iter> range(Iter first, Iter last)
    {
      return {first, last};
    }
    
  // Return a closed range [first, last] over the incrementable values first
  // and last.
  //
  // requires: Incrementable<Iter>
  // precondition: bounded_range(first, last + 1);
  template<typename Iter>
    wrapped_bounded_range<Iter> closed_range(Iter first, Iter last)
    {
      return {first, std::next(last)};
    }

} // namespace origin

#endif
