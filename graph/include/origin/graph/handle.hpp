// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_HANDLE_HPP
#define ORIGIN_GRAPH_HANDLE_HPP

#include <iterator>
#include <functional>

namespace origin 
{
  // Handle
  // A Handle is an opaque reference to a vertex or edge within a graph. 
  // Handles are Regular, Totally_ordered and Hashable. If the value type is 
  // Ordinal, then the handle is also Ordinal.
  //
  // Although the class is parameterized, there are few valid specializations.
  // The default specializations provided by this library are:
  //
  //    handle<size_t>
  //    handle<void*>
  //
  // If you think you need to specialize this template, you are probably wrong.
  template<typename T>
    class handle;
    
  // Vertex handle (integer)
  template<>
    class handle<std::size_t>
    {
      static constexpr size_t null = -1ul;
    public:
      typedef std::size_t value_type;
      
      // Default constructor
      // The default constructor initializes the object to the null vertex
      // handle. It does not refer to any vertex.
      constexpr handle() : value{null} { }
      
      // Value constructor
      explicit handle(value_type n) : value{n} { }

      // Equality_comparable
      bool operator==(handle x) const { return value == x.value; }
      bool operator!=(handle x) const { return value != x.value; }

      // Totally ordered
      // Note that the null vertex handle is less than all other vertex handles.
      bool operator<(handle x) const 
      { 
        if(value == null)
          return x.value != null;
        else if(x.value == null)
          return false;
        else
          return value < x.value; 
      }
      
      bool operator>(handle x) const { return value > x.value; }
      bool operator<=(handle x) const { return value <= x.value; }
      bool operator>=(handle x) const { return value <= x.value; }

      // Boolean
      explicit operator bool() const { return value != -1ul; }
      
      // Ordinal
      std::size_t ord() const { return value; }
      
      // Hashable
      std::size_t hash() const { return hash_value(value); }
      
      value_type value;
    };
    
    

  // Vertex handle (pointer)
  // The pointer handle is partially specialized over a pointer to referenced
  // object.
  template<typename T>
    class handle<T*>
    {
    public:
      using value_type = T*;
      
      // Default constructor
      // The default constructor initializes the object to the null vertex
      // handle. It does not refer to any vertex.
      constexpr handle() : value{nullptr} { }
      
      // Value constructor
      handle(T* p) : value{p} { }

      // Equality_comparable
      bool operator==(handle x) const { return value == x.value; }
      bool operator!=(handle x) const { return value != x.value; }

      // Totally ordered.
      bool operator<(handle x) const { return value < x.value; }
      bool operator>(handle x) const { return value > x.value; }
      bool operator<=(handle x) const { return value <= x.value; }
      bool operator>=(handle x) const { return value <= x.value; }

      // Boolean
      explicit operator bool() const { return value != nullptr; }
      
      // Hashable
      std::size_t hash() const { return hash_value(value); }

      value_type value;
    };

} // namespace origin



// Natively support the standard hashing protocol for vertex handles.
namespace std 
{
  template<typename T>
    struct hash<origin::handle<T>>
    {
      std::size_t operator()(origin::handle<T> x) const { return x.hash(); }
    };
} // namespace std



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
