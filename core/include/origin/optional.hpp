// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_OPTIONAL_HPP
#define ORIGIN_OPTIONAL_HPP

#include <cassert>
#include <algorithm>

#include <origin/utility/facades.hpp>

namespace origin
{
  // The optional class template implements an optional "qualifier". The 
  // optional class extends the set of values of an underlying value type
  // with an "invalid" value, much like nullptr for pointer types.
  //
  // Optional types can also be compared for equality and ordered. When both
  // objects are initialized, these comparisons are the same as the underlying
  // type. Otherwise, the unitialized state acts like nullptr.
  //
  // An optional type has two states, initialized or uninitialized. A default
  // initialized optional type is uninitialized. Note that the the underlying
  // type is _not_ initialized until needed. Accessing the value of an optional
  // object in an uninitialized state results in undefined behavior.
  template<typename T>
    class optional
    {
    public:
      typedef T value_type;
      typedef T& reference;
      typedef T const& const_reference;
      typedef T* pointer;
      typedef T const* const_pointer;

      // Default constructor
      optional() : init(false) { }
      
      // Movable
      // FIXME: Generalize over convertible types?
      optional(optional&& x)
        : init(x.init)
      {
        if(init)
          new(ptr()) T(std::move(x));
        x.init = false;
      }
      
      optional& operator=(optional&& x)
      {
        optional tmp(std::move(x));
        swap(tmp);
        return *this;
      }
      
      // Copyable
      // FIXME: Generalize over convertible types?
      optional(optional const& x)
        : init(x.init)
      {
        if(init)
          new(ptr()) T(x);
      }
      
      optional& operator=(optional const& x)
      {
        optional tmp(x);
        swap(tmp);
        return *this;
      }
      
      optional(value_type&& x)
        : init(true)
      {
        new(ptr()) T(std::move(x));
      }

      // Value copy initialization
      // FIXME: Should this be explicit?
      optional(value_type const& x)
        : init(true)
      { 
        new (ptr()) T(x); 
      }

      optional& operator=(T const& x)
      { optional tmp{x}; swap(tmp); return *this; }

      // Nullptr initialization
      optional(std::nullptr_t)
        : init{false}
      { }

      optional& operator=(std::nullptr_t)
      { clear(); return *this; }

      
      
      ~optional()
      {
        if(init) ptr()->~T(); 
      }

      
      // Return a reference to the underlying object.
      reference operator*()
      { return *ptr(); }

      const_reference operator*() const
      { return *ptr(); }

      // Return a pointer to the underlying object.
      pointer operator->()
      { return ptr(); }

      const_pointer operator->() const
      { return ptr(); }

      // Facade functions
      bool valid() const
      { return init; }

      bool equal(optional const& x) const
      {
        if(init == x.init)
          return init ? *ptr() == *x.ptr() : true;
        else
          return false;
      }

      static bool equal(optional const& x, T const& y)
      { return x.init && *x.ptr() == y; }

      static bool equal(optional const& x, std::nullptr_t)
      { return !x.init; }


      bool less(optional const& x) const
      {
        if(!x.init)
          return false;
        else if(!init)
          return true;
        else
          return *ptr() < *x.ptr();
      }

      // FIXME: It would probably be more efficient to implement comparisons
      // without explicitly constructing new optional values.
      static bool less(optional const& x, T const& y)
      { return x < optional{y}; }

      static bool less(T const& x, optional const& y)
      { return optional{x} < y; }

      static bool less(optional const& x, std::nullptr_t)
      { return x < optional{nullptr}; }

      static bool less(std::nullptr_t, optional const& x)
      { return optional{nullptr} < x; }

      void swap(optional& x)
      {
        std::swap(init, x.init);
        std::swap(mem, x.mem);
      }

      void clear()
      {
        if(init) {
          ptr()->~T();
          init = false;
        }
      }

    private:
      // Return a pointer to the underlying memory.
      pointer       ptr()       { return reinterpret_cast<T*>(mem); }
      const_pointer ptr() const { return reinterpret_cast<T const*>(mem); }

    private:
      bool init;
      char mem[sizeof(T)];
    };

} // namespace origin


#endif

