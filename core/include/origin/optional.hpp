// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_OPTIONAL_HPP
#define ORIGIN_OPTIONAL_HPP

#include <origin/utility/facades.hpp>

namespace origin
{
  /**
   * The optional class...
   */
  template<typename T>
  class optional
    : implicit_bool_facade<optional<T>>
  {
  public:
    // Default constructor
    optional()
      : init_(false)
    { }
    
    // FIXME: Are copies and moves any more complex than the trivial versions?
    
    // Copy value initialization
    optional(T const& x)
      : init_(true)
    { new (ptr()) T{x}; }
    
    optional& operator=(T const& x)
    { optional tmp{x}; swap(tmp); return *this; }
    
    // Move value initialization
    // FIXME: This is probably realy broken.
    optional(T&& x)
      : init_(false)
    { swap(x); }
    
    optional& operator=(T&& x)
    { swap(x); return *this; }
  
    // Facade functions
    bool valid() const
    { return init_; }
    
    bool equal(optional const& x) const
    { return *ptr() == *x.ptr(); }
    
    bool less(optional const& x) const
    { return *ptr() < *x.ptr(); }
    
    void swap(optional& x)
    {
      std::swap(init_, x.init_);
      std::swap(obj_, x.obj_);
    }
  
  private:
    T* ptr()
    { return reinterpret_cast<T*>(obj_); }
    
    T const* ptr() const
    { return reinterpret_cast<T*>(obj_); }
  
  private:
    bool init_;
    char obj_[sizeof(T)];
  };

} // namespace origin


#endif

