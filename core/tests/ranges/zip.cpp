// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/range/zip.hpp>

using namespace origin;

std::ostream& operator<<(std::ostream& os, std::tuple<int, char> t)
{
  return os << '{' << std::get<0>(t) << ',' << std::get<1>(t) << '}';
}

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      std::cout << x << ' ';
    std::cout << '\n';
  }


int main()
{
  int a[] {1, 2, 3};
  char c[] {'a', 'b', 'c'};
  print(zip(a, c));

  /*
  auto z = zip(a1, v2);
  auto i = v3.begin();
  for(const auto& x : z) {
    // cout << get<0>(x) << ' ' << get<1>(x) << '\n';
    assert(x == *i);
    ++i;
  }
  
  {
    const vector<int>& cv1 = a1;
    const vector<char>& cv2 = v2;
    
    auto r1 = zip(cv1, v2);
    auto r2 = zip(a1, cv2);
    
    cout << typestr(r1) << "\n";
    cout << typestr(r2) << "\n";
  }
  */
}
