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

#include <origin/functional.hpp>
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
// general, but that's not a huge concern right now... Maybe this should be
// called "offset_compare" since the elements compared are given as offsets
// from the underlying values.
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

// Generate some data for testing.
template<typename Eng>
  std::vector<int> make_data(Eng& eng)
  {
    // Used for generating random data.
    std::uniform_int_distribution<int> value_dist{0, 1000};
    auto value = bind(value_dist, eng);

    // Generate some random data
    std::vector<int> v(100);
    std::generate(v.begin(), v.end(), value);
    return std::move(v);
  }
  
template<typename T, typename A>
  void print(std::vector<T, A> const& v)
  {
    for(auto const& x : v)
      std::cout << v << ' ';
    std::cout << '\n';
  }

template<typename T, typename A, typename Comp>
  bool sorted(std::vector<T, A> const& v, Comp comp)
  {
    return is_sorted(v.begin(), v.end(), comp);
  }

/**
 * Check the heap order on a copy of the given heap. This is done by comparing
 * each popped element to the previous using the heaps value comparison function
 */
template<typename Heap>
  void check_heap_order(Heap h)
  {
    typedef typename Heap::value_type Value;
    std::vector<Value> v;
    while(!h.empty()) {
      v.push_back(h.top());
      h.pop();
      // print(v);
      assert(( sorted(v, origin::invert_order(h.value_comp())) ));
    }
  }

/**
 * Check the validity of the (non-mutable) Priority_Queue. This just pushes a
 * number of objects into the heap and verifies that the result will be sorted
 * each time.
 */
template<typename Heap, typename Eng>
  void check_heap(Eng& eng)
  {
    int const N = 100;

    std::uniform_int_distribution<> dist{0, 1000};
    auto var = bind(dist, eng);

    Heap h;
    for(int i = 0; i < N; ++i)
      h.push(var());
    check_heap_order(h);
  }


/**
 * Check the validity of the mutable heap. This constructs a heap over the
 * given data set (a vector or array of integral values), and then randomly 
 * modifies the values, updating the heap.
 */
template<typename Heap, typename Vec, typename Engine>
  void check_indirect_mutable_heap(Vec& v, Engine& eng)
  {
    typedef typename Vec::value_type Value;
    typedef typename Vec::size_type Size;
    typedef indirect_compare<Value> Comp;
    
    Size N = 100;
    
    // Random variable picking indexes
    std::uniform_int_distribution<Size> index_dist{0, N - 1};
    auto index = bind(index_dist, eng);
    
    // Random variable for generating values.
    std::uniform_int_distribution<Value> value_dist{0, 1000};
    auto value = bind(value_dist, eng);

    // Initialize the heap.
    Heap h;
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
 * Check the validity of the mutable heap. This constructs a heap over the
 * given data set (a vector or array of integral values), and then randomly 
 * modifies the values, updating the heap.
 */
template<typename Heap, typename Vec, typename Engine>
  void check_ordinal_mutable_heap(Vec& v, Engine& eng)
  {
    typedef typename Vec::value_type Value;
    typedef typename Vec::size_type Size;
    typedef index_compare<Value> Comp;  // Same as Heap::value_compare
    
    Size N = 100;
    
    // Random variable picking indexes
    std::uniform_int_distribution<Size> index_dist{0, N - 1};
    auto index = bind(index_dist, eng);
    
    // Random variable for generating values.
    std::uniform_int_distribution<Value> value_dist{0, 1000};
    auto value = bind(value_dist, eng);

    // Initialize the heap.
    Comp comp{&v.front()};
    Heap h{comp};
    for(Size i = 0; i < v.size(); ++i)
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