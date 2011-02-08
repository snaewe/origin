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
  /**
   * @ingroup types
   * The optional template implements an "uninitialized" state for types that
   * do not normally include one in their set of values. An optional type
   * behaves similarly to pointer types. The underlying value can be accessed
   * by dereferencing the optional object and members may be accessed using
   * the arrow operator.
   *
   * Optional types can also be compared for equality and ordered. When both
   * objects are initialized, these comparisons are the same as the underlying
   * type. Otherwise, the unitialized state acts like nullptr.
   *
   * An optional type has two states, initialized or uninitialized. A default
   * initialized optional type is uninitialized. Note that the the underlying
   * type is _not_ initialized until needed. Accessing the value of an optional
   * object in an uninitialized state results in undefined behavior.
   */
  template<typename T>
  class optional
    : public implicit_bool_facade<optional<T>>
    , public equality_facade<optional<T>>
    , public ordered_facade<optional<T>>
  {
  public:
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef T* pointer;
    typedef T const* const_pointer;

    // Default constructor
    optional()
      : init_{false}
    { }

    ~optional()
    { if(init_) ptr()->~T(); }

    // FIXME: Are copies and moves any more complex than the trivial versions?

    // FIXME: Make sure that we have as strong exception guarantees as possible.

    // Copy value initialization
    optional(T const& x)
      : init_{true}
    { new (ptr()) T{x}; }

    optional& operator=(T const& x)
    { optional tmp{x}; swap(tmp); return *this; }

    // Nullptr initialization
    optional(std::nullptr_t)
      : init_{false}
    { }

    optional& operator=(std::nullptr_t)
    { clear(); return *this; }

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
    { return init_; }

    bool equal(optional const& x) const
    {
      if(init_ == x.init_)
        return init_ ? *ptr() == *x.ptr() : true;
      else
        return false;
    }

    bool less(optional const& x) const
    {
      if(!x.init_)
        return false;
      else if(!init_)
        return true;
      else
        return *ptr() < *x.ptr();
    }

    void swap(optional& x)
    {
      std::swap(init_, x.init_);
      std::swap(obj_, x.obj_);
    }

    void clear()
    {
      if(init_) {
        ptr()->~T();
        init_ = false;
      }
    }

  private:
    pointer ptr()
    {
      assert(( init_ ));
      return reinterpret_cast<T*>(obj_);
    }

    const_pointer ptr() const
    {
      assert(( init_ ));
      return reinterpret_cast<T const*>(obj_);
    }

  private:
    bool init_;
    char obj_[sizeof(T)];
  };

} // namespace origin


#endif

