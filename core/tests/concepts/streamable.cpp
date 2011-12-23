// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <origin/concepts.hpp>

using namespace std;
using namespace origin;

struct fail { };

struct pass { };

template<typename C, typename T>
  basic_istream<C, T>& operator>>(basic_istream<C, T>& is, pass&)
  {
    return is;
  }

template<typename C, typename T>
  basic_istream<C, T>& operator<<(basic_ostream<C, T>& os, pass const&)
  {
    return os;
  }

int main()
{
  // 1-argument concept
  static_assert(Input_streamable<int>(), "");
  static_assert(Output_streamable<int>(), "");
  static_assert(Streamable<int>(), "");

  static_assert(Input_streamable<pass>(), "");
  static_assert(Output_streamable<pass>(), "");
  static_assert(Streamable<pass>(), "");

  static_assert(!Input_streamable<fail>(), "");
  static_assert(!Output_streamable<fail>(), "");
  static_assert(!Streamable<fail>(), "");
  
  // 2-argument concept
  static_assert(Input_streamable<ifstream, pass>(), "");
  static_assert(Output_streamable<ofstream, pass>(), "");
  static_assert(Streamable<fstream, pass>(), "");

  static_assert(Input_streamable<ifstream, string>(), "");
  static_assert(Output_streamable<ofstream, string>(), "");
  static_assert(Streamable<fstream, int>(), "");


}

