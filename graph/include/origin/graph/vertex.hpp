// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_VERTEX_HPP
#define ORIGIN_GRAPH_VERTEX_HPP

#include <origin/iterator.hpp>
#include <origin/functional.hpp>
#include <origin/graph/handle.hpp>

namespace origin 
{
  // Vertex handle
  // The vertex handle is an opaque reference to a vertex object in a graph. 
  //
  // Note that we explicitly subclass the handle<T> so that we don't confuse
  // vertex and edge handles. They are incompatible types, even if they
  // represent the same set of values.
  template<typename T>
    class vertex_handle
    {
    public:
      using value_type = Value_type<handle<T>>;
      
      vertex_handle() : impl{} { }
      vertex_handle(value_type n) : impl{n} { }

      // Equality_comparable
      bool operator==(vertex_handle v) const { return impl == v.impl; }
      bool operator!=(vertex_handle v) const { return impl != v.impl; }
      
      // Totally_ordered
      bool operator<(vertex_handle v) const  { return impl < v.impl; }
      bool operator>(vertex_handle v) const  { return impl > v.impl; }
      bool operator<=(vertex_handle v) const { return impl <= v.impl; }
      bool operator>=(vertex_handle v) const { return impl >= v.impl; }

      // Boolean
      explicit operator bool() const { return (bool)impl; }
      
      // Ordinal
      // Only if Ordinal<T>
      std::size_t ord() const { return impl.ord(); }
      
      // Hashable
      std::size_t hash() const { return impl.hash(); }
      
      // Returns the underlying value.
      value_type  value() const { return impl.value; }

      // The handle implementation.
      handle<T> impl;
    };
    
  
    
  // Vertex iterator
  // The vertex iterator provides an iterator over successive vertices.
  template<typename T>
    using vertex_iterator = handle_iterator<vertex_handle<T>>;

} // namespace origin

namespace std
{
  // Make vertex handles hashable by the standard library.
  template<typename T>
    struct hash<origin::vertex_handle<T>>
    {
      std::size_t operator()(const origin::vertex_handle<T>& v) const
      { 
        return v.hash();
      }
    };
   
} // namespace std

#endif
