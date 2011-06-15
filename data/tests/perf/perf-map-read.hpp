// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef PERF_MAP_READ_HPP
#define PERF_MAP_READ_HPP

#include <functional>
#include <random>
#include <chrono>

typedef std::chrono::system_clock default_clock;

/**
 * Simulate reading and writing of reps number of keys in a map of type 
 * Map<size_t, int>.
 */
template<template<typename...> class Map, typename Engine>
  std::chrono::microseconds 
  read_map(Engine& eng, std::size_t sz, std::size_t reps)
  {
    // Construct an initial map with keys in the range [0, sz) with all
    // mapped values initially 0.
    Map<std::size_t, int> m;
    for(std::size_t i = 0; i < 10000; ++i) {
      m[i] = 0;
    }
    
    // Define a uniform random variable for selecting indexes
    std::uniform_int_distribution<int> index_dist{0, 1000};
    auto index = bind(index_dist, eng);
    
    // Randomly lookup values in the map.
    auto start = default_clock::now();
    for(std::size_t i = 0; i < reps; ++i) {
      ++m[index()];
    }
    auto stop = default_clock::now();
    return stop - start;
  }

#endif
