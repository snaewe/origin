// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>
#include <iostream>
#include <functional>

#include <origin/utility/typestr.hpp>
#include <origin/range/transform_range.hpp>
#include <origin/range/zip_range.hpp>

using namespace std;
using namespace origin;

int foo(int) { return 0; }

template<typename Func>
void func(Func f)
{
  cout << typestr<typename result_of<Func(int)>::type>() << "\n";
};

int main()
{
  func(foo);

//   func(std::negate<int>{});
//   func([](int) { return 0.0; });
//   func(std::less<int>{});

//   vector<int> v = {1, 2, 3, 4, 5};
//   auto f = [](int& x) { return x * x; };
//   for(auto x : zip(v, xform(v, f))) {
//     cout << get<0>(x) << " " << get<1>(x) << "\n";
//   }
}

