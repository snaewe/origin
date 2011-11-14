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
        if(x.init)
          new(ptr()) T(std::move(x.get()));
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
        if(x.init)
          new(ptr()) T(x.get());
      }
      
      optional& operator=(optional const& x)
      {
        optional tmp(x);
        swap(tmp);
        return *this;
      }
      
      
      // Value move initialization
      explicit optional(value_type&& x)
        : init(true)
      {
        new(ptr()) T(std::move(x));
      }
      
      optional& operator=(value_type&& x)
      {
        optional tmp(std::move(x));
        swap(tmp); 
        return *this; 
      }

      
      // Value copy initialization
      explicit optional(value_type const& x)
        : init(true)
      { 
        new(ptr()) T(x); 
      }

      optional& operator=(T const& x)
      { 
        optional tmp{x}; 
        swap(tmp); 
        return *this; 
      }

      
      // Nullptr initialization
      optional(std::nullptr_t)
        : init(false)
      { }

      optional& operator=(std::nullptr_t)
      { 
        clear(); 
        return *this; 
      }

      
      ~optional()
      {
        if(init) 
          ptr()->~T(); 
      }

      
      // Dereferenceable
      // Return a reference to the underlying object.
      reference       operator*()       { return get(); }
      const_reference operator*() const { return get(); }

      // Return a pointer to the underlying object.
      pointer       operator->()       { return &get(); }
      const_pointer operator->() const { return &get(); }

      // Boolean
      explicit operator bool() const { return init; }
      
      // Equality_comparable
      bool operator==(optional const& a) const
      {
        if(init == a.init)
          return init ? get() == a.get() : true;
        else
          return false;
      }
      
      bool operator!=(optional const& a) const { return !(*this == a); }

      // Equality_comparable<optional<T>, T>
      friend bool operator==(optional const& a, value_type const& b) { return a && *a == b; }
      friend bool operator==(value_type const& a, optional const& b) { return b && *b == a; }
      friend bool operator!=(value_type const& a, optional const& b) { return !(a == b); }
      friend bool operator!=(optional const& a, value_type const& b) { return !(a == b); }

      // Equality_comparable<Optional<T>, nullptr_t>
      friend bool operator==(optional const& a, std::nullptr_t) { return !a; }
      friend bool operator==(std::nullptr_t, optional const& a) { return !a; }
      friend bool operator!=(optional const& a, std::nullptr_t) { return (bool)a; }
      friend bool operator!=(std::nullptr_t, optional const& a) { return (bool)a; }

      // Totally_ordered
      bool operator<(optional const& a) const
      {
        if(!a.init)
          return false;
        else if(!init)
          return true;
        else
          return get() < a.get();
      }
      
      bool operator>(optional const& a) const  { return a < *this; }
      bool operator<=(optional const& a) const { return !(a < *this); }
      bool operator>=(optional const& a) const { return !(*this < a); }

      // Totally_ordered<optional<T>, T>
      friend bool operator<(optional const& a, value_type const& b) { return a ? *a < b : true; }
      friend bool operator<(value_type const& a, optional const& b) { return b ? a < *b : true; }
      friend bool operator>(optional const& a, value_type const& b)  { return b < a; }
      friend bool operator>(value_type const& b, optional const& a)  { return b < a; }
      friend bool operator<=(optional const& a, value_type const& b) { return !(b < a); }
      friend bool operator<=(value_type const& a, optional const& b) { return !(b < a); }
      friend bool operator>=(optional const& a, value_type const& b) { return !(a < b); }
      friend bool operator>=(value_type const& a, optional const& b) { return !(a < b); }

      // Totally_ordered<optional<T>, nullptr_t>
      friend bool operator<(optional const& a, std::nullptr_t)  { return false; }
      friend bool operator<(std::nullptr_t, optional const& b)  { return false; }
      friend bool operator>(optional const& a, std::nullptr_t)  { return a; }
      friend bool operator>(std::nullptr_t, optional const& a)  { return a; }
      friend bool operator<=(optional const& a, std::nullptr_t) { return !a; }
      friend bool operator<=(std::nullptr_t, optional const& b) { return !b; }
      friend bool operator>=(optional const& a, std::nullptr_t) { return true; }
      friend bool operator>=(std::nullptr_t, optional const& b) { return true; }
      
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
      
      // Checked pointer access
      reference       get()       { assert(( init )); return *ptr(); }
      const_reference get() const { assert(( init )); return *ptr(); }

    private:
      bool init;
      char mem[sizeof(T)];
    };

  // Swap specialization.
  template<typename T>
    void swap(optional<T>& a, optional<T>& b)
    {
      a.swap(b);
    }
    
    
} // namespace origin


#endif

