// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_OPTIONAL_HPP
#define ORIGIN_OPTIONAL_HPP

#include <cassert>
#include <origin/concepts.hpp>

namespace origin
{
  // Optional
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
  //
  // TODO: Analyze this class for exception safety.
  template<typename T>
    class optional
    {
    public:
      // Default constructor
      optional() : init {false} { }
      
      // Move semantics
      optional(optional&& x)
        : init {x.init}
      {
        if (x.init)
          new (ptr()) T {std::move(x.get())};
        x.init = false;
      }
      
      optional& operator=(optional&& x)
      {
        optional tmp {std::move(x)};
        swap(tmp);
        return *this;
      }
      
      // Copyable
      // FIXME: Allow conversion from optionals of different types?
      optional(optional const& x)
        : init {x.init}
      {
        if (x.init)
          new (ptr()) T(x.get());
      }
      
      optional& operator=(optional const& x)
      {
        optional tmp(x);
        swap(tmp);
        return *this;
      }
      
      
      // Value move initialization
      explicit optional(T&& x)
        : init {true}
      {
        new (ptr()) T {std::move(x)};
      }
      
      optional& operator=(T&& x)
      {
        optional tmp {std::move(x)};
        swap(tmp); 
        return *this; 
      }

      
      // Value copy initialization
      // FIXME: Allow initialization from convertible types?
      explicit optional(const T& x)
        : init {true}
      { 
        new (ptr()) T(x); 
      }

      optional& operator=(const T& x)
      { 
        optional tmp {x}; 
        swap(tmp); 
        return *this; 
      }

      
      // Nullptr initialization
      optional(std::nullptr_t)
        : init {false}
      { }

      optional& operator=(std::nullptr_t)
      { 
        clear(); 
        return *this; 
      }

      
      ~optional()
      {
        if (init)
          ptr()->~T(); 
      }


      // Returns true if the value is initialized.
      bool initialized() const { return init; }
      
      // Dereference
      // Return a reference to the underlying object.
      T&       operator*()       { return get(); }
      const T& operator*() const { return get(); }

      // Arrow
      // Return a pointer to the underlying object.
      T*       operator->()       { return &get(); }
      const T* operator->() const { return &get(); }

      // Boolean
      explicit operator bool() const { return init; }
      
      // Mutators
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
      T*       ptr()       { return reinterpret_cast<T*>(&mem); }
      const T* ptr() const { return reinterpret_cast<T const*>(&mem); }
      
      // Checked pointer access
      T&       get()       { assert(init); return *ptr(); }
      const T& get() const { assert(init); return *ptr(); }

    private:
      bool init;
      Aligned_storage<sizeof(T), alignof(T)> mem;
    };


  // Equality_comparable<optional<T>>
  // Two optional objects compare equal when they have the same initialization
  // and the initializedd values compare true.
  template <typename T>
    inline bool operator==(const optional<T>& a, const optional<T>& b)
    {
      if (a.initialized() == b.initialized())
        return a ? *a == *b : true;
      else
        return false;
    }
    
  template <typename T>
    inline bool operator!=(const optional<T>& a, const optional<T>& b)
    {
      return !(a == b);
    }


  // Equality_comparable<optional<T>, T>
  // An optional object, a, compares equal to an object b of type T when a
  // is initialized and has the same value as b.
  template <typename T>
    inline bool operator==(const optional<T>& a, const T& b) 
    { 
      return a && *a == b; 
    }
  
  template <typename T>
    inline bool operator==(const T& a, const optional<T>& b) 
    { 
      return b && *b == a; 
    }

  template <typename T>
    inline bool operator!=(const T& a, const optional<T>& b) 
    { 
      return !(a == b); 
    }

  template <typename T>
    inline bool operator!=(const optional<T>& a, const T& b) 
    { 
      return !(a == b); 
    }


  // Equality_comparable<Optional<T>, nullptr_t>
  // An optional object compares equal to nullptr when it is uninitialized.
  template <typename T>
    inline bool operator==(const optional<T>& a, std::nullptr_t) 
    { 
      return !a; 
    }

  template <typename T>
    inline bool operator==(std::nullptr_t, const optional<T>& a) 
    { 
      return !a; 
    }

  template <typename T>
    inline bool operator!=(const optional<T>& a, std::nullptr_t) 
    { 
      return (bool)a; 
    }
  
  template <typename T>
    inline bool operator!=(std::nullptr_t, const optional<T>& a) 
    { 
      return (bool)a; 
    }


  // Totally ordered
  // When considering total ordereings, the "uninitialized state" is considered
  // to be less than all other values.
  template <typename T>
    inline bool operator<(const optional<T>& a, const optional<T>& b)
    {
      if(!b)
        return false;
      else if(!a)
        return true;
      else
        return *a < *b;
    }
    
  template <typename T>
    inline bool operator>(const optional<T>& a, const optional<T>& b) 
    { 
      return b < a;
    }
    
  template <typename T>
    inline bool operator<=(const optional<T>& a, const optional<T>& b) 
    { 
      return !(b < a); 
    }

  template <typename T>
    inline bool operator>=(const optional<T>& a, const optional<T>& b) 
    { 
      return !(a < b);
    }

    // Totally_ordered<optional<T>, T>
  template <typename T>
    inline bool operator<(const optional<T>& a, const T& b) 
    { 
      return a ? *a < b : true; 
    }
    
  template <typename T>
    inline bool operator<(const T& a, const optional<T>& b) 
    { 
      return b ? a < *b : true; 
    }
    
  template <typename T>
    inline bool operator>(const optional<T>& a, const T& b)  
    { 
      return b < a; 
    }
    
  template <typename T>
    inline bool operator>(const T& b, const optional<T>& a)  
    { 
      return b < a; 
    }
    
  template <typename T>
    inline bool operator<=(const optional<T>& a, const T& b) 
    { 
      return !(b < a); 
    }
    
  template <typename T>
    inline bool operator<=(const T& a, const optional<T>& b) 
    { 
      return !(b < a); 
    }
    
  template <typename T>
    inline bool operator>=(const optional<T>& a, const T& b) 
    { 
      return !(a < b); 
    }
    
  template <typename T>
    inline bool operator>=(const T& a, const optional<T>& b) 
    { 
      return !(a < b); 
    }

  
  // Totally_ordered<optional<T>, nullptr_t>
  // In these comparisons, nullptr is used to represent the uninitialized state.
  template <typename T>
    inline bool operator<(const optional<T>& a, std::nullptr_t)  
    { 
      return false; 
    }
    
  template <typename T>
    inline bool operator<(std::nullptr_t, const optional<T>& b)  
    { 
      return false; 
    }
    
  template <typename T>
    inline bool operator>(const optional<T>& a, std::nullptr_t)  
    { 
      return a; 
    }
    
  template <typename T>
    inline bool operator>(std::nullptr_t, const optional<T>& a)  
    { 
      return a; 
    }
    
  template <typename T>
    inline bool operator<=(const optional<T>& a, std::nullptr_t) 
    { 
      return !a; 
    }
    
  template <typename T>
    inline bool operator<=(std::nullptr_t, const optional<T>& b) 
    { 
      return !b; 
    }
    
  template <typename T>
    inline bool operator>=(const optional<T>& a, std::nullptr_t) 
    { 
      return true; 
    }
    
  template <typename T>
    inline bool operator>=(std::nullptr_t, const optional<T>& b) 
    { 
      return true; 
    }

} // namespace origin


#endif

