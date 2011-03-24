// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/range/count_range.hpp>

using namespace std;
using namespace origin;

template<typename Range>
vector<typename iterator_traits<typename range_traits<Range>::iterator>::value_type>
gen(Range&& r)
{
  typedef typename range_traits<Range>::iterator Iter;
  typedef typename iterator_traits<Iter>::value_type Value;

  vector<Value> ret;
  for(auto x : r) {
//     cout << x << " ";
    ret.push_back(x);
  }
//   cout << "\n";
  return ret;
}

template<typename Range, typename T>
bool eq(Range&& r, initializer_list<T> l)
{
  auto&& v = gen(std::move(r));
  if(v.size() != l.size())
    return false;
  return std::equal(v.begin(), v.end(), l.begin());
}

template<typename Range>
ptrdiff_t distance(Range&& r)
{ return std::distance(r.begin(), r.end()); }

int main()
{
  // Positive ranges
  assert(( eq(range(3), {0, 1, 2}) ));
  assert(( eq(range(0, 3), {0, 1, 2}) ));
  assert(( eq(range<3>(0, 9), {0, 3, 6}) ));
  assert(( eq(range<3>(1, 9), {1, 4, 7 }) ));
  assert(( eq(range<3>(2, 9), {2, 5, 8 }) ));
  assert(( eq(range<3>(3, 9), {3, 6 }) ));

  // Negative ranges
  assert(( eq(range<-1>(3), {3, 2, 1}) ));
  assert(( eq(range<-1>(3, 0), {3, 2, 1}) ));
  assert(( eq(range<-3>(9, 0), {9, 6, 3}) ));
  assert(( eq(range<-3>(8, 0), {8, 5, 2}) ));
  assert(( eq(range<-3>(7, 0), {7, 4, 1}) ));
  assert(( eq(range<-3>(6, 0), {6, 3}) ));

  // Empty ranges
  assert(( distance(range(1, 0)) == 0 ));
  assert(( distance(range<-1>(0, 1)) == 0 ));
}

