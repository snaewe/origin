// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_UTILITY_FACADES_HPP
#define ORIGIN_UTILITY_FACADES_HPP

#include <type_traits>

namespace origin
{
  /**
   * The implicit_bool_facade implements the safe bool idiom for derived
   * classes, which allows objects of the derived type to be implicitly cast
   * to boolean values. The deriving class must define a member function named
   * 'valid' that takes no arguments and returns true or false.
   *
   * @todo Is this really a good name for this class. Historically, this was
   * called safe_bool_facade. I renamed it to implicit_bool because it
   * implements an implicit cast to something that can be used in a boolean
   * context.
   */
  template<typename Derived>
  struct implicit_bool_facade
  {
  private:
    typedef bool (Derived::*unspecified_bool_type)() const;
  public:
    operator unspecified_bool_type() const
    {
      return static_cast<Derived const*>(this)->valid()
        ? &Derived::valid
        : nullptr;
    }
  };

  /**
   * The dereference facade provides an implementation of the dereference
   * operator for the given type. Note that the reference type must be
   * given explicitly since there is no simple way to deduce it without
   * requiring the derived type to be complete (which is somewhat paradoxical
   * given the use of the curiously recurring template pattern).
   *
   * @tparam Derived The derived type inheriting the facade.
   * @tparam Reference The type returned by the dereference operator.
   */
  template<typename Derived, typename Reference>
  class dereference_facade
  {
  public:
    Reference operator*() const
    { return static_cast<Derived const*>(this)->dereference(); }
  };

  /**
   * The equality comparable facade provides an implementation of the equality
   * comparable operations for the derived class.
   */
  template<typename Derived>
  struct equality_facade
  {
    friend bool operator==(Derived const& x, Derived const& y)
    { return x.equal(y); }

    friend bool operator!=(Derived const& x, Derived const& y)
    { return !x.equal(y); }
  };

  /**
   * The increment facade provides implementations of then pre- and pos-
   * decrement operators for the Derived type.
   *
   * @tparam Derived The derived type inheriting the facade. This type must be
   * Copyable.
   */
  template<typename Derived>
  class increment_facade
  {
  public:
    Derived& operator++()
    {
      static_cast<Derived*>(this)->increment();
      return *static_cast<Derived*>(this);
    }

    Derived operator++(int)
    {
      Derived tmp(*static_cast<Derived*>(this));
      static_cast<Derived*>(this)->increment();
      return tmp;
    }
  };

  /**
   * The decrement facade provides implements of then pre- and post-
   * decrement operators for the Derived type.
   *
   * @tparam Derived The derived type inheriting the facade. This type must be
   * Copyable.
   */
  template<typename Derived>
  class decrement_facade
  {
  public:
    Derived& operator--()
    {
      static_cast<Derived*>(this)->increment();
      return *static_cast<Derived*>(this);
    }

    Derived operator--(int)
    {
      Derived tmp(*static_cast<Derived*>(this));
      static_cast<Derived*>(this)->decrement();
      return tmp;
    }
  };

} // namespace origin


#endif

