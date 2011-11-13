// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_VERTEX_HPP
#define ORIGIN_GRAPH_VERTEX_HPP

#include <iterator>
#include <functional>

namespace origin 
{
  /**
   * The vertex_t type represents an ordinal reference to a vertex in a Graph.
   * The integral value -1u corresponds to a null vertex.
   */
  class vertex_t
  {
  public:
    typedef std::size_t value_type;
    
    vertex_t()
      : value{-1ul}
    { }

    explicit vertex_t(value_type n)
      : value{n}
    { }

    // Equatable
    bool operator==(vertex_t x) const { return value == x.value; }
    bool operator!=(vertex_t x) const { return value != x.value; }

    bool operator<(vertex_t x) const { return value < x.value; }
    bool operator>(vertex_t x) const { return value > x.value; }
    bool operator<=(vertex_t x) const { return value <= x.value; }
    bool operator>=(vertex_t x) const { return value <= x.value; }

    // Boolean
    explicit operator bool() const { return value != -1ul; }

    value_type value;
  };

  /**
   * Return the ordinal value of the given vertex index.
   */
  vertex_t::value_type ord(vertex_t v)
  {
    return v.value;
  }
} // namespace origin

// FIXME: Is there anything we can do about this short of rewriting our own
// hash table. Probably not. This is pretty bad, though.
// Support std::hash<> interoperability.
namespace std 
{
  template<>
    struct hash<origin::vertex_t>
    {
      std::size_t operator()(origin::vertex_t const& v) const
      { 
        return std::hash<std::size_t>{}(v.value); 
      }
  };
} // namespace std

namespace origin
{
  /**
   * The vertex iterator implements a random access iterator over a vertex 
   * index. Dereferencing a vertex iterator yields a vertex_t object.
   */
  class vertex_iterator
  {
  public:
    typedef vertex_t value_type;
    typedef vertex_t const& reference;
    typedef vertex_t const* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;

    vertex_iterator(vertex_t e)
      : vert{e}
    { }

    vertex_t const& operator*() const { return vert; }
    vertex_t const* operator->() const { return &vert; }

    // Equatable
    bool operator==(vertex_iterator x) const { return vert == x.vert; }
    bool operator!=(vertex_iterator x) const { return vert != x.vert; }

    // TotallyOrdered
    bool operator<(vertex_iterator x) const { return vert < x.vert; }
    bool operator>(vertex_iterator x) const { return vert > x.vert; }
    bool operator<=(vertex_iterator x) const { return vert <= x.vert; }
    bool operator>=(vertex_iterator x) const { return vert >= x.vert; }

    // Increment
    vertex_iterator& operator++() { ++vert.value; return *this; }
    vertex_iterator operator++(int) { vertex_iterator tmp{*this}; ++*this; return tmp; }

    // Decrement
    vertex_iterator& operator--() { --vert.value; return *this; }
    vertex_iterator operator--(int) { vertex_iterator tmp{*this}; --*this; return tmp; }

    // Advance
    vertex_iterator& operator+=(difference_type n) { vert.value += n; return *this; }
    vertex_iterator& operator-=(difference_type n) { vert.value -= n; return *this; }

    // Next/Prev
    friend vertex_iterator operator+(vertex_iterator i, difference_type n) { i += n; return i; }
    friend vertex_iterator operator+(difference_type n, vertex_iterator i) { i += n; return i; }
    friend vertex_iterator operator-(vertex_iterator i, difference_type n) { i -= n; return i; }
    
    // Distance
    friend difference_type operator-(vertex_iterator i, vertex_iterator j) 
    {
      return i->value - j->value; 
    }

  private:
    vertex_t vert;
  };

} // namespace origin

#endif
