// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/iterator/facade.hpp>

using namespace std;
using namespace origin;


template <typename T>
  struct input_iter
  {
    using This = input_iter<T>;

    T* ptr;

    input_iter(T* p) : ptr{p} { }

    T& operator*() const { return *ptr; }

    This& operator++() { ++ptr; return *this; }

    friend bool operator==(const This& a, const This& b) { return a.ptr == b.ptr; }
    friend bool operator!=(const This& a, const This& b) { return a.ptr != b.ptr; }
  };


template <typename T>
  struct forward_iter
  {
    using This = forward_iter<T>;

    T* ptr;

    forward_iter(T* p) : ptr{p} { }

    T& operator*() const { return *ptr; }

    This& operator++() { ++ptr; return *this; }
    This operator++(int) { This tmp{*this}; ++ ptr; return tmp; }

    friend bool operator==(const This& a, const This& b) { return a.ptr == b.ptr; }
    friend bool operator!=(const This& a, const This& b) { return a.ptr != b.ptr; }
  };



template <typename T>
  struct bidirectional_iter
  {
    using This = bidirectional_iter<T>;

    T* ptr;

    bidirectional_iter(T* p) : ptr{p} { }

    T& operator*() const { return *ptr; }

    This& operator++() { ++ptr; return *this; }
    This operator++(int) { This tmp{*this}; ++ ptr; return tmp; }

    This& operator--() { --ptr; return *this; }
    This operator--(int) { This tmp{*this}; -- ptr; return tmp; }

    friend bool operator==(const This& a, const This& b) { return a.ptr == b.ptr; }
    friend bool operator!=(const This& a, const This& b) { return a.ptr != b.ptr; }
  };



template <typename T>
  struct random_access_iter
  {
    using This = random_access_iter<T>;
    using Diff = std::ptrdiff_t;

    T* ptr;

    random_access_iter(T* p) : ptr{p} { }

    T& operator*() const { return *ptr; }
    T& operator[](Diff n) const { return ptr[n]; }

    This& operator++() { ++ptr; return *this; }
    This operator++(int) { This tmp{*this}; ++ ptr; return tmp; }

    This& operator--() { --ptr; return *this; }
    This operator--(int) { This tmp{*this}; -- ptr; return tmp; }

    This& operator+=(Diff n) { ptr += n; return *this; }
    This& operator-=(Diff n) { ptr -= n; return *this; }

    friend This operator+(This a, Diff n) { return a += n; }
    friend This operator+(Diff n, This a) { return a += n; }
    friend This operator-(This a, Diff n) { return a -= n; }

    friend Diff operator-(This a, This b) { return a.ptr - b.ptr; }

    friend bool operator==(const This& a, const This& b) { return a.ptr == b.ptr; }
    friend bool operator!=(const This& a, const This& b) { return a.ptr != b.ptr; }

    friend bool operator<(const This& a, const This& b) { return a.ptr < b.ptr; }
    friend bool operator>(const This& a, const This& b) { return a.ptr > b.ptr; }
    friend bool operator<=(const This& a, const This& b) { return a.ptr <= b.ptr; }
    friend bool operator>=(const This& a, const This& b) { return a.ptr >= b.ptr; }
  };



template <typename I>
  struct wrapped_iter : iterator_facade<wrapped_iter<I>, I>
  {
    using Base = iterator_facade<wrapped_iter<I>, I>;
    using This = wrapped_iter<I>;
    using Diff = Difference_type<I>;

    I iter;

    wrapped_iter() : I{} { }
    wrapped_iter(I i) : I{i} { }

    auto operator*() const -> decltype(*this->iter) { return *iter; }
    // auto operator[](Diff n) const -> decltype(this->iter[n]) { return iter[n]; }

    This& operator++() { ++iter; return *this; }
    using Base::operator++;

    This& operator--() { --iter; return *this; }
    using Base::operator--;

    This& operator+=(Diff n) { iter += n; return *this; }
    This& operator-=(Diff n) { iter -= n; return *this; }

    friend Diff operator-(const This& a, const This& b) { return a.ptr - b.ptr; }

    friend bool operator==(const This& a, const This& b) { return a.ptr == b.ptr; }
    friend bool operator!=(const This& a, const This& b) { return a.ptr != b.ptr; }

    friend bool operator<(const This& a, const This& b) { return a.ptr < b.ptr; }
    friend bool operator>(const This& a, const This& b) { return a.ptr > b.ptr; }
    friend bool operator<=(const This& a, const This& b) { return a.ptr <= b.ptr; }
    friend bool operator>=(const This& a, const This& b) { return a.ptr >= b.ptr; }
  };



int main()
{
  {
    using I = wrapped_iter<input_iter<int>>;
    static_assert(Input_iterator<I>(), "");
    static_assert(!Forward_iterator<I>(), "");
  }

  {
    using I = wrapped_iter<forward_iter<int>>;
    static_assert(Forward_iterator<I>(), "");
    static_assert(!Bidirectional_iterator<I>(), "");
  }

  {
    using I = wrapped_iter<bidirectional_iter<int>>;
    static_assert(Bidirectional_iterator<I>(), "");
    static_assert(!Random_access_iterator<I>(), "");
  }

  {
    using I = wrapped_iter<random_access_iter<int>>;
    using D = Difference_type<I>;
    static_assert(Random_access_iterator<I>(), "");
  }
}
