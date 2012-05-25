// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/iterator/filter.hpp>

using namespace std;
using namespace origin;

// copy(filtered(first), filtered(last), result, pred) 
//   <=> copy_if(first, last, result, pred)
template<typename I, typename P>
  bool check_filter_iterator(I first, I last, P pred)
  {
    using V = vector<Value_type<I>>;
    
    auto n = count_if(first, last, pred);
    
    V a(n);
    copy_if(first, last, a.begin(), pred);
    
    V b(n);
    auto first2 = filter_iter(first, last, pred);
    auto last2 = filter_iter(last, pred);
    copy(first2, last2, b.begin());
    
    return equal(a, b);
  }

  

// Returns true if n is non-zero.
struct non_zero
{
  bool operator()(int n) const { return n != 0; }
};

struct even
{
  bool operator()(int n) const { return n % 2 == 0; }
};

template <typename T>
  struct input_iter
  {
    T* ptr;

    input_iter(T* p) : ptr{p} { }

    T& operator*() const { return *ptr; }

    input_iter& operator++()
    {
      ++ptr;
      return *this;
    }
  };

template <typename T>
  bool operator==(const input_iter<T>& a, const input_iter<T>& b)
  {
    return a.ptr == b.ptr;
  }

template <typename T>
  bool operator!=(const input_iter<T>& a, const input_iter<T>& b)
  {
    return a.ptr != b.ptr;
  }


template <typename I>
  void print(I first, I last) 
  {
    while (first != last) {
      std::cout << *first << ' ';
      ++first;
    }
    std::cout << '\n';
  }


int main()
{
  int a[] {1, 2, 3, 4, 5, 6};
  input_iter<int> i = a;
  input_iter<int> j = a + 6;
  {
    auto f = make_filter_iterator(i, j, even{});
    auto l = make_filter_iterator(j, even{});
    print(f, l);
    static_assert(Strict_input_iterator<decltype(f)>(), "");
  }

  {
    int* p = a;
    int* q = a + 6;
    auto f = make_filter_iterator(p, q, even{});
    auto l = make_filter_iterator(q, even{});
    print(f, l);
    static_assert(Forward_iterator<decltype(f)>(), "");
  }
}
