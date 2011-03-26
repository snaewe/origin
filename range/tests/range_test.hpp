// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef RANGE_TEST_HPP
#define RANGE_TEST_HPP

#include <iosfwd>
#include <algorithm>
#include <vector>

// Print the elements of a range
template<typename Char, typename Traits, typename Range>
void print(std::basic_ostream<Char, Traits>& os, Range&& r)
{
  for(auto x : r)
    os << x << " ";
  os << "\n";
}

// A helper function that copies values of a range into a vector.
// NOTE: This would be easier if range_traits had a value type.
template<typename Range>
std::vector<
  typename std::iterator_traits<
    typename origin::range_traits<Range>::iterator
  >::value_type
>
gen(Range&& r)
{
  typedef typename origin::range_traits<Range>::iterator Iter;
  typedef typename std::iterator_traits<Iter>::value_type Value;
  std::vector<Value> v;
  std::copy(begin(r), end(r), back_inserter(v));
  return v;
}

// Check that a range produces the values in the given initializer list.
template<typename Range, typename T>
bool check(Range&& r, std::initializer_list<T> l)
{
  auto&& v = gen(std::move(r));
  if(v.size() != l.size())
    return false;
  return std::equal(v.begin(), v.end(), l.begin());
}

// Return the length of a range
template<typename Range>
std::ptrdiff_t distance(Range&& r)
{ return std::distance(r.begin(), r.end()); }

#endif
