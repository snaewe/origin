// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <list>
#include <forward_list>
#include <iterator>
#include <iostream>

#include <origin/iterator/core.hpp>

using namespace std;
using namespace origin;

template<typename I>
  void not_an_iterator()
  {
    static_assert(!Readable<I>(), "");
    static_assert(!Input_iterator<I>(), "");
    static_assert(!Forward_iterator<I>(), "");
    static_assert(!Bidirectional_iterator<I>(), "");
    static_assert(!Random_access_iterator<I>(), "");
  }

template<typename I>
  void input()
  {
    static_assert(Input_iterator<I>(), "");
    static_assert(!Forward_iterator<I>(), "");
  }


// Forward iterators
template<typename I>
  void forward()
  {
    static_assert(Input_iterator<I>(), "");
    static_assert(Forward_iterator<I>(), "");
    static_assert(!Bidirectional_iterator<I>(), "");
  }

template<typename I>
  void mutable_forward()
  {
    forward<I>();
    static_assert(Mutable<I>(), "");
  }

template<typename I>
  void permutable_forward()
  {
    forward<I>();
    static_assert(Permutable<I>(), "");
  }


// Bidirectional iterators
template<typename I>
  void bidirectional()
  {
    static_assert(Input_iterator<I>(), "");
    static_assert(Forward_iterator<I>(), "");
    static_assert(Bidirectional_iterator<I>(), "");
    static_assert(!Random_access_iterator<I>(), "");
  }

template<typename I>
  void mutable_bidirectional()
  {
    bidirectional<I>();
    static_assert(Mutable<I>(), "");
  }

template<typename I>
  void permutable_bidirectional()
  {
    bidirectional<I>();
    static_assert(Permutable<I>(), "");
  }

  
// Random access iterators
template<typename I>
  void random_access()
  {
    static_assert(Input_iterator<I>(), "");
    static_assert(Forward_iterator<I>(), "");
    static_assert(Bidirectional_iterator<I>(), "");
    static_assert(Random_access_iterator<I>(), "");
  }
  
template<typename I>
  void mutable_random_access()
  {
    random_access<I>();
    static_assert(Mutable<I>(), "");
  }

template<typename I>
  void permutable_random_access()
  {
    random_access<I>();
    static_assert(Permutable<I>(), "");
  }
  
  
int main()
{
  not_an_iterator<int>();
  
  using Sll_int = forward_list<int>::iterator;
  using Sll_cint = forward_list<int>::iterator;
  mutable_forward<Sll_int>();
  forward<Sll_cint>();

  using Sll_unique_int = forward_list<unique_ptr<int>>::iterator;
  using Sll_unique_cint = forward_list<unique_ptr<int>>::const_iterator;
  permutable_forward<Sll_unique_int>();
  forward<Sll_unique_cint>();

  using Dll_int = list<int>::iterator;
  using Dll_cint = list<int>::const_iterator;
  mutable_bidirectional<Dll_int>();
  bidirectional<Dll_cint>();

  using Dll_unique_int = list<unique_ptr<int>>::iterator;
  using Dll_unique_cint = list<unique_ptr<int>>::const_iterator;
  permutable_bidirectional<Dll_unique_int>();
  bidirectional<Dll_unique_cint>();

  using Vec_int = std::vector<int>::iterator;
  using Vec_cint = std::vector<int>::const_iterator;
  mutable_random_access<Vec_int>();
  random_access<Vec_cint>();

  using Vec_unique_int = vector<unique_ptr<int>>::iterator;
  using Vec_unique_cint = vector<unique_ptr<int>>::const_iterator;
  permutable_random_access<Vec_unique_int>();
  random_access<Vec_unique_cint>();

  random_access<int*>();
  mutable_random_access<int*>();
}
