// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef PERF_MAP_READ_HPP
#define PERF_MAP_READ_HPP

#include <deque>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>

typedef std::chrono::system_clock default_clock;

/**
 * Simulate a number of operations (insert, find, erase) on a map.
 * 
 * @param pi  The probability of an insertation
 * @param pe  The probability of an erase
 */
template<template<typename...> class Map, typename Engine>
  std::chrono::microseconds 
  sim_map(Engine& eng, std::size_t reps, double pi, double pe)
  {
    // Define a uniform random variable for selecting events
    std::discrete_distribution<> event_dist{pi, pe, 1.0 - (pi + pe)};
    auto event = bind(event_dist, eng);

    // Allocate a bunch of free keys and shuffle them.
    // FIXME: How many keys should there be? For now, there are reps/10.
    std::size_t nkeys = reps / 10;
    std::deque<std::size_t> free(reps / 10);
    std::deque<std::size_t> used;
    for(std::size_t i = 0; i < reps / 10; ++i)
      free[i] = i;
    std::random_shuffle(free.begin(), free.end());

    // Create the initial map.
    Map<std::size_t, int> m;

    // Insert a free key into the map.
    auto insert = [&]() {
      if(!free.empty()) {
        m[free.front()] = 0;
        used.push_back(free.front());
        free.pop_front();
      }
    };

    // Erase a used key from the map.
    auto erase = [&] {
      if(!used.empty()) {
        m.erase(used.front());
        free.push_back(used.front());
        used.pop_front();
      }
    };

    // Read (modify) a key in the map.
    auto read = [&]() {
      if(!used.empty()) {
        // Define a uniform random variable for selecting a used key. 
        std::uniform_int_distribution<std::size_t> key_dist{0u, used.size() - 1};
        auto key = bind(key_dist, eng);
        m[used[key()]] += 1;
      }
    };

    // Seed the map with some 20% of the initial keys
    std::size_t init = free.size() / 5; // Also, reps / 50.
    for(std::size_t i = 0; i < init; ++i) {
      insert();
    }

    // Starting with an initial map, execute random events on the data 
    // structure.
    auto start = default_clock::now();
    for(std::size_t i = 0; i < reps; ++i) {
      int n = event();
      switch(event()) {
        case 0: insert(); break;
        case 1: erase(); break;
        case 2: read(); break;
      }
    }
    auto stop = default_clock::now();
    return stop - start;
  }

#endif
