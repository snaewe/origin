// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <iostream>

#include <origin/concepts.hpp>
#include <origin/iterator/concepts.hpp>

using namespace std;
using namespace origin;

template<typename T>
void f() {
  cExplicit<T>{};
}

// Create a model of the concept.
namespace origin
{
  template<>
    struct cExplicit<char> : cModel
  { };
}

int main()
{
  // This will always generate an error.
  // f<int>();
  f<char>();
}
