// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef PERF_MAP_BUILD_HPP
#define PERF_MAP_BUILD_HPP

#include <algorithm>
#include <functional>
#include <random>
#include <chrono>

using namespace std;

typedef std::chrono::system_clock default_clock;

/**
 * Build a map of type Map<size_t, int> up to the given size. The order of
 * insertions is determined randomly.
 */
template<template<typename...> class Map, typename Engine>
  chrono::microseconds build_map(Engine& eng, std::size_t sz)
  {
    // Construct a vector of indexes and shuffle them. This will be the
    // insertion order of objects.
    std::vector<std::size_t> v(sz);
    for(std::size_t i = 0; i < sz; ++i) {
      v[i] = i;
    }
    std::random_shuffle(v.begin(), v.end());
    
    auto start = default_clock::now();
    Map<std::size_t, int> m;
    for(std::size_t i = 0; i < sz; ++i) {
      m[v[i]] = 0;
    }
    auto stop = default_clock::now();
    return stop - start;
  }

#endif