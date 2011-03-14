// Copyright_equal (c) 2008-2010 Kent State University
// Copyright_equal (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/concepts.hpp>

#include "check.hpp"

using namespace std;
using namespace origin;

int main()
{
  check<Plus<int>>{true};
  check<Plus<float>>{true};
  check<Plus<string>>{true};

  check<Minus<int>>{true};
  check<Minus<float>>{true};
  check<Minus<string>>{false};

  check<Multiplies<int>>{true};
  check<Multiplies<float>>{true};
  check<Multiplies<string>>{false};

  check<Divides<int>>{true};
  check<Divides<float>>{true};
  check<Divides<string>>{false};

  check<Modulus<int>>{true};
  check<Modulus<float>>{false};
  check<Modulus<string>>{false};

  check<Unary_Plus<int>>{true};
  check<Unary_Plus<string>>{false};
  check<Unary_Minus<int>>{true};
  check<Unary_Minus<string>>{false};

  // Bitwise...
  check<Bitset<int>>{true};
  check<Shift<int>>{true};
}
