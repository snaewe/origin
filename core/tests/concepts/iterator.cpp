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
#include <iterator>
#include <iostream>

#include <origin/iterator/concepts.hpp>

#include "check.hpp"

using namespace std;
using namespace origin;

int main()
{
  typedef int* Ptr;
  static_assert(cIterator<Ptr>::value, "");
  static_assert(cForward_Iterator<Ptr>::value, "");
  static_assert(cBidirectional_Iterator<Ptr>::value, "");
  static_assert(cRandom_Access_Iterator<Ptr>::value, "");
  static_assert(tInput_Iterator<Ptr>::value, "");
  static_assert(tOutput_Iterator<Ptr>::value, "");

  typedef forward_list<int>::iterator Fwd_Iter;
  static_assert(cIterator<Fwd_Iter>::value, "");
  static_assert(cForward_Iterator<Fwd_Iter>::value, "");
  static_assert(!cBidirectional_Iterator<Fwd_Iter>::value, "");
  static_assert(tInput_Iterator<Fwd_Iter>::value, "");
  static_assert(tOutput_Iterator<Fwd_Iter>::value, "");

  typedef forward_list<int>::const_iterator Const_Fwd_Iter;
  static_assert(cIterator<Const_Fwd_Iter>::value, "");
  static_assert(cForward_Iterator<Const_Fwd_Iter>::value, "");
  static_assert(!cBidirectional_Iterator<Const_Fwd_Iter>::value, "");
  static_assert(tInput_Iterator<Const_Fwd_Iter>::value, "");
  static_assert(!tOutput_Iterator<Const_Fwd_Iter>::value, "");
  
  typedef list<int>::iterator Bidi_Iter;
  static_assert(cIterator<Bidi_Iter>::value, "");
  static_assert(cForward_Iterator<Bidi_Iter>::value, "");
  static_assert(cBidirectional_Iterator<Bidi_Iter>::value, "");
  static_assert(!cRandom_Access_Iterator<Bidi_Iter>::value, "");
  static_assert(tInput_Iterator<Bidi_Iter>::value, "");
  static_assert(tOutput_Iterator<Bidi_Iter>::value, "");
  
  typedef list<int>::const_iterator Const_Bidi_Iter;
  static_assert(cIterator<Const_Bidi_Iter>::value, "");
  static_assert(cForward_Iterator<Const_Bidi_Iter>::value, "");
  static_assert(cBidirectional_Iterator<Const_Bidi_Iter>::value, "");
  static_assert(!cRandom_Access_Iterator<Const_Bidi_Iter>::value, "");
  static_assert(tInput_Iterator<Const_Bidi_Iter>::value, "");
  static_assert(!tOutput_Iterator<Const_Bidi_Iter>::value, "");

  typedef vector<int>::iterator Vec_Iter;
  static_assert(cIterator<Vec_Iter>::value, "");
  static_assert(cForward_Iterator<Vec_Iter>::value, "");
  static_assert(cBidirectional_Iterator<Vec_Iter>::value, "");
  static_assert(cRandom_Access_Iterator<Vec_Iter>::value, "");
  static_assert(tInput_Iterator<Vec_Iter>::value, "");
  static_assert(tOutput_Iterator<Vec_Iter>::value, "");

  typedef vector<int>::const_iterator Const_Vec_Iter;
  static_assert(cIterator<Const_Vec_Iter>::value, "");
  static_assert(cForward_Iterator<Const_Vec_Iter>::value, "");
  static_assert(cBidirectional_Iterator<Const_Vec_Iter>::value, "");
  static_assert(cRandom_Access_Iterator<Const_Vec_Iter>::value, "");
  static_assert(tInput_Iterator<Const_Vec_Iter>::value, "");
  static_assert(!tOutput_Iterator<Const_Vec_Iter>::value, "");

  
  typedef istream_iterator<int> In_Iter;
  static_assert(cIterator<In_Iter>::value, "");
  static_assert(!cForward_Iterator<In_Iter>::value, "");
  static_assert(tInput_Iterator<In_Iter>::value, "");
  static_assert(!tOutput_Iterator<In_Iter>::value, "");
  
  // FIXME: GCC's ostream_iterator isn't actually a valid iterator because
  // it's reference type is void. Actually, all of its associated types are
  // void. There's absolutely nothing I can do about it except re-define the
  // class.
  typedef ostream_iterator<int> Out_Iter;
//   static_assert(cIterator<Out_Iter>::value, "");
//   static_assert(!cForward_Iterator<Out_Iter>::value, "");
//   static_assert(tInput_Iterator<In_Iter>::value, "");
//   static_assert(tOutput_Iterator<Out_Iter>::value, "");
  
  
}
