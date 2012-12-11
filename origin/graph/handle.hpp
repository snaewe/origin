// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_HANDLE_HPP
#define ORIGIN_GRAPH_HANDLE_HPP

#include <cstdint>
#include <functional>

namespace origin 
{
  // ------------------------------------------------------------------------ //
  //                              Handle
  //
  // A handle is an ordinal value type used to represent an object in a data
  // structure. Handles are implicitly interoperable with unsigned intger
  // (size_t) values, and have the special property that unsigned -1 indicates
  // an invalid object.
  //
  // TODO: Disable arithmetic operations?
  class handle
  {
  public:
    static constexpr std::size_t npos = -1;

    handle(std::size_t n = npos);

    // Boolean
    explicit operator bool() const;

    // Integral
    operator std::size_t() const { return value; }
    
    // Hashable
    std::size_t hash() const;
    
    std::size_t value;
  };
  
  inline
  handle::handle(std::size_t n) : value(n) { }

  inline 
  handle::operator bool() const { return value != npos; }
  
  inline std::size_t 
  handle::hash() const { return std::hash<std::size_t>{}(value); }

  // Equality
  inline bool 
  operator==(handle a, handle b) { return a.value == b.value; }
  
  inline bool 
  operator!=(handle a, handle b) { return !(a == b); }

  // Ordering
  inline bool 
  operator<(handle a, handle b)
  { 
    if (!a)
      return bool(b);
    else 
      return b ? a.value < b.value : false; 
  }
    
  inline bool 
  operator>(handle a, handle b) { return b < a; }
  
  inline bool 
  operator<=(handle a, handle b) { return !(b < a); }
  
  inline bool 
  operator>=(handle a, handle b) { return !(a < b); }


  // ------------------------------------------------------------------------ //
  //                            Vertex Handle
  //
  // A vertex handle is a handle specifically for graph vertices. It is
  // the same as a normal handle in every way except its type.
  struct vertex_handle : handle
  {
    vertex_handle(std::size_t n = npos) : handle(n) { }
  };

  // ------------------------------------------------------------------------ //
  //                             Edge Handle
  //
  // An edge handle is a handle specifically for graph vertices. It is
  // the same as a normal handle in every way except its type.
  struct edge_handle : handle
  {
    edge_handle(std::size_t n = npos) : handle(n) { }
  };

} // namespace origin


// Natively support the standard hashing protocol for vertex handles.
namespace std 
{
  template<>
    struct hash<origin::handle>
    {
      std::size_t operator()(origin::handle x) const { return x.hash(); }
    };
} // namespace std


#if 0
namespace origin
{
  // Handle iterator
  // The handle iterator implements an iterator over handles. The properties
  // of the iterator depend on the handle's value type. If the handle's value
  // type is std::size_t, then the iterator is a random access iterator. If
  // not, it may be a forward or bidirectional iterator.
  //
  // The handle iterator takes two parameters: the underlying handle type and
  // the value type of that handle (which should never be explicitly given).
  // The intent is to partially specialize handle iterators based on the
  // underlying value type. This allows specializations to define value-specific
  // iterators for different models. For example:
  //
  //    handle_iterator<H, size_t>        For default vertex and edge handles
  //    handle_iterator<H, vertex_node*>  Iterator over vertex nodes
  //
  // Note that only the size_t specialization is given here. Other graph
  // data structures must explicitly specialize this for their node types.
  template<typename H, typename T = Value_type<H>>
    class handle_iterator;

  // Handle iterator (integer)
  // The default specialization for handle iterators.
  template<typename H>
    class handle_iterator<H, std::size_t>
    {
    public:
      using value_type        = H;
      using reference         = const H&;
      using pointer           = const H*;
      using difference_type   = std::ptrdiff_t;
      using iterator_category = std::random_access_iterator_tag;

      handle_iterator() : handle{} { }
      handle_iterator(std::size_t x) : handle{x} { }

      reference operator*() const  { return handle; }
      pointer   operator->() const { return &handle; }

      // Equatable
      bool operator==(handle_iterator x) const { return handle == x.handle; }
      bool operator!=(handle_iterator x) const { return handle != x.handle; }

      // TotallyOrdered
      bool operator<(handle_iterator x) const  { return handle < x.handle; }
      bool operator>(handle_iterator x) const  { return handle > x.handle; }
      bool operator<=(handle_iterator x) const { return handle <= x.handle; }
      bool operator>=(handle_iterator x) const { return handle >= x.handle; }

      // Increment
      handle_iterator& operator++() { ++handle.impl.value; return *this; }
      handle_iterator operator++(int) { handle_iterator tmp{*this}; ++*this; return tmp; }

      // Decrement
      handle_iterator& operator--() { --handle.impl.value; return *this; }
      handle_iterator operator--(int) { handle_iterator tmp{*this}; --*this; return tmp; }

      // Advance
      handle_iterator& operator+=(difference_type n) { handle.impl.value += n; return *this; }
      handle_iterator& operator-=(difference_type n) { handle.impl.value -= n; return *this; }

      // 
      friend handle_iterator operator+(handle_iterator i, difference_type n) 
      { 
        i += n; 
        return i; 
      }

      friend handle_iterator operator+(difference_type n, handle_iterator i) 
      { 
        i += n; 
        return i; 
      }
      
      friend handle_iterator operator-(handle_iterator i, difference_type n) 
      { 
        i -= n; 
        return i; 
      }
      
      // Distance
      friend difference_type operator-(handle_iterator i, handle_iterator j)
      {
        return i->value() - j->value();
      }

    private:
      H handle;
    };

} // namespace origin

#endif

#endif
