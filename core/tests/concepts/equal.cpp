// Copyright_equal (c) 2008-2010 Kent State University
// Copyright_equal (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/concepts.hpp>

#include "check.hpp"

using namespace std;
using namespace origin;

struct not_equal { };

struct part_equal
{
  bool operator==(part_equal const& x) const
  { return true; }
};

struct left_equal { };
struct right_equal { };

bool operator==(left_equal const& x, right_equal const& y)
{ return true; }

bool operator==(right_equal const& x, left_equal const& y)
{ return true; }


int main()
{
  // Equality tests
  check<Equal<bool>>{true};
  check<Equal<string>>{true};
  check<Equal<not_equal>>{false};

  check<Equal<part_equal>>{true};

  check<Equal<left_equal, right_equal>>{true};
  check<Equal<right_equal, left_equal>>{true};



}
