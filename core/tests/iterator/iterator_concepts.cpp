// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <forward_list>
#include <list>
#include <vector>
#include <iostream>

#include <origin/iterator/concepts.hpp>

using namespace std;
using namespace origin;

int main()
{
  static_assert(cIterator<int*>::value, "");
  static_assert(cForward_Iterator<int*>::value, "");
  static_assert(cBidirectional_Iterator<int*>::value, "");
  static_assert(cRandom_Access_Iterator<int*>::value, "");

  typedef forward_list<int>::iterator Fwd_Iter;
  static_assert(cIterator<Fwd_Iter>::value, "");
  static_assert(cForward_Iterator<Fwd_Iter>::value, "");

  typedef list<int>::iterator Bidi_Iter;
  static_assert(cIterator<Bidi_Iter>::value, "");
  static_assert(cForward_Iterator<Bidi_Iter>::value, "");
  static_assert(cBidirectional_Iterator<Bidi_Iter>::value, "");

  typedef vector<int>::iterator Vec_Iter;
  static_assert(cIterator<Vec_Iter>::value, "");
  static_assert(cForward_Iterator<Vec_Iter>::value, "");
  static_assert(cBidirectional_Iterator<Vec_Iter>::value, "");
  static_assert(cRandom_Access_Iterator<Vec_Iter>::value, "");
}
