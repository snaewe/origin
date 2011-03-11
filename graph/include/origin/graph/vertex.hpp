// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_VERTEX_HPP
#define ORIGIN_GRAPH_VERTEX_HPP

#include <functional>

#include <origin/iterator/facades.hpp>

namespace origin 
{
  /**
   * The vertex_t type represents in index into indexable graph.
   */
  class vertex_t
    : public implicit_bool_facade<vertex_t>
  {
  public:
    typedef std::size_t value_type;

    vertex_t()
      : value(-1)
    { }
    
    vertex_t(std::size_t n)
      : value(n)
    { }

    bool equal(vertex_t x) const
    { return value == x.value; }

    bool less(vertex_t x) const
    { return value < x.value; }

    bool valid() const
    { return value != value_type(-1); }

    value_type value;
  };
} // namespace origin

// Support std::hash<> interoperability.
namespace std 
{
  template<>
  struct hash<origin::vertex_t>
    : public hash<std::size_t>
  {
    typedef std::size_t size_type;

    size_type operator()(origin::vertex_t const& v) const
    { return hash<size_type>::operator()(v.value); }
  };
} // namespace std

namespace origin
{
  /**
   * The vertex iterator implements a random access iterator over a vertex 
   * index. Dereferencing a vertex iterator yields a vertex_t object.
   */
  class vertex_iterator
    : public random_access_iterator_facade<vertex_iterator, vertex_t, std::size_t>
  {
    typedef random_access_iterator_facade<
      vertex_iterator, vertex_t, std::size_t
    > base_type;
  public:
    typedef typename base_type::reference reference;
    typedef typename base_type::difference_type difference_type;

    vertex_iterator(vertex_t v)
      : vertex_(v)
    { }
    
    reference dereference() const
    { return vertex_; }
    
    bool equal(vertex_iterator iter) const
    { return vertex_.value == iter.vertex_.value; }
    
    bool less(vertex_iterator iter) const
    { return vertex_.value < iter.vertex_.value; }

    void increment()
    { ++vertex_.value; }
    
    void decrement()
    { --vertex_.value; }
    
    void advance(difference_type n)
    { vertex_.value += n; }
    
    difference_type distance(vertex_iterator iter)
    { return iter.vertex_.value - vertex_.value; }
    
  private:
    vertex_t vertex_;
  };

} // namespace origin

#endif
