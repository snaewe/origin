// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONTAINER_FWD_HPP
#define ORIGIN_CONTAINER_FWD_HPP

namespace std
{
  // Sequences
  template <typename T, std::size_t N> class array;
  template <typename T, typename A> class vector;
  template <typename T, typename A> class forward_list;
  template <typename T, typename A> class list;
  template <typename T, typename A> class deque;
  
  // Associative containers
  
  // Search trees
  template <typename T, typename C, typename A> class set;
  template <typename T, typename C, typename A> class multiset;
  template <typename K, typename V, typename C, typename A> class map;
  template <typename K, typename V, typename C, typename A> class multimap;

  // Hash tables
  template <typename T, typename H, typename C, typename A> class unorderd_set;
  template <typename T, typename H, typename C, typename A> class unorderd_multiset;
  template <typename K, typename V, typename H, typename C, typename A> class unorderd_map;
  template <typename K, typename V, typename H, typename C, typename A> class unorderd_multimap;

} // namespace std;

namespace origin
{
  // Concepts
  template <typename T> constexpr bool Sequence();
  template <typename T> constexpr bool Associative_container();
} // namespace origin

#endif
