// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef HEAP_SORT_HPP
#define HEAP_SORT_HPP

#include <cassert>
#include <algorithm>
#include <functional>
#include <vector>
#include <random>

#include <origin/ordinal_map.hpp>

/**
 * Indirectly compare two pointers using the given comparison operator.
 */
template<typename T, typename Comp = std::less<T>>
  struct indirect_compare
  {
    indirect_compare(Comp c = Comp{})
      : comp{c}
    { }
    
    bool operator()(T const* p, T const* q) const
    {
      return comp(*p, *q);
    }
    
    Comp comp;
  };

// FIXME: T should really be a random access iterator in order to be fully
// general.
/**
 * Indirectly compare two objects based on their indexes into an underlying
 * random access sequence.
 */
template<typename T, typename Comp = std::less<T>>
  struct index_compare
  {
    index_compare(T const* p, Comp c = Comp{})
      : ptr{p}, comp{c}
    { }
    
    // Don"t allow signed arguments because ptr is intended to be the start of
    // an array.
    bool operator()(std::size_t x, std::size_t y) const
    {
      return comp(*(ptr + x), *(ptr + y));
    }
    
    T const* ptr;
    Comp comp;
  };


/**
 * Check the heap order on a copy of the given heap. This is done by comparing
 * each popped element to the previous using the heaps value comparison function
 */
template<typename Heap>
  void check_heap_order(Heap h)
  {
    // Empty heaps are valid.
    if(h.empty())
      return;
    

    // Compare each subsequent pair of elements popped from the heap. They
    // must not violate the order property of the heap.
    auto comp = h.value_comp();
    auto x = h.top();
    h.pop();
    while(!h.empty()) {
      auto y = h.top();
      h.pop();
      assert(( !comp(x, y) ));
      x = y;
    }
  }

/**
 * Check the validity of the (non-mutable) Priority_Queue. This just pushes a
 * number of objects into the heap and verifies that the result will be sorted
 * each time.
 */
template<template<typename...> class Heap, typename Eng>
  void check_heap(Eng& eng)
  {
    int const N = 100;

    std::uniform_int_distribution<> dist{0, 1000};
    auto var = bind(dist, eng);

    Heap<int> h;
    for(int i = 0; i < N; ++i)
      h.push(var());
    check_heap_order(h);
  }



/**
 * Check the validity of the mutable Heap template. This constructs a heap
 * over a domain of objects and then modifies the objects in the domain,
 * updating the heap. The heap order is verified after each update.
 */
template<template<typename...> class Heap, typename Engine>
  void check_mutable_heap(Engine& eng)
  {
    typedef indirect_compare<int> Comp;
    
    int const N = 100;
    
    // Random variable picking indexes
    std::uniform_int_distribution<size_t> index_dist{0, N - 1};
    auto index = bind(index_dist, eng);
    
    // Random variable for generating values.
    std::uniform_int_distribution<int> value_dist{0, 1000};
    auto value = bind(value_dist, eng);

    // Build a small domain of randomly generated objects.
    std::vector<int> v(N);
    std::generate(v.begin(), v.end(), value);
    
    // Construct a mutable heap to order that domain by pointers into it.
    Heap<int*, Comp> h;
    for(auto& p : v)
      h.push(&p);
    check_heap_order(h);
    
    // Change some values and validate the heap order.
    for(int i = 0; i < 10; ++i) {
      size_t n = index();
      int* p = &v[n];
      *p = value();
      
      h.update(p);
      check_heap_order(h);
    }
  }

/**
 * Check the validity of the mutable Heap template using a dense index mapping
 * over the domain of objects. The testing strategy is practically identical
 * to that of check_mutable_heap.
 */
template<template<typename...> class Heap, typename Engine>
  void check_dense_mutable_heap(Engine& eng)
  {
    int const N = 100;
    
    // Random variable picking indexes
    std::uniform_int_distribution<size_t> index_dist{0, N - 1};
    auto index = bind(index_dist, eng);
    
    // Random variable for generating values.
    std::uniform_int_distribution<int> value_dist{0, 1000};
    auto value = bind(value_dist, eng);

    // Build a small domain of randomly generated objects.
    std::vector<int> v(N);
    std::generate(v.begin(), v.end(), value);
    
    // Construct a mutable heap to order that domain by offsets into the
    // vector v, initialized above.
    typedef index_compare<int> Comp;
    typedef std::vector<size_t> Cont;
    typedef origin::ordinal_map<std::size_t, int> Map;

    // Initialize the heap
    Comp comp{v.data()};
    Heap<std::size_t, Comp, Cont, Map> h{comp};
    for(size_t i = 0; i < v.size(); ++i)
      h.push(i);
    check_heap_order(h);
    
    // Change some values and validate the heap order.
    for(int i = 0; i < 10; ++i) {
      size_t n = index();
      v[n] = value();
      
      h.update(n);
      check_heap_order(h);
    }
  }


#endif