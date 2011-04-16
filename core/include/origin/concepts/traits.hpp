// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_HPP
#define ORIGIN_CONCEPTS_HPP

#include <origin/concepts/impl.hpp>

namespace origin
{
  /**
   * @ingroup concepts
   * This concept implements a same-type constraint over a set of types.
   */
  template<typename... Args>
  struct tSame
  {
    tSame()
    { 
      static_assert(are_same<Args...>::value, "Types are not the same"); 
    }

    typedef std::tuple<are_same<Args...>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };
  
  /**
   * @ingroup concepts
   *
   * The Common trait is valid if the type arguments share a common type. A
   * single type is its own common type. Two types have a common type, they can 
   * be used as result values of a conditional expression (?:). An arbitrary
   * sequence of types share a common type if they can all be used as result
   * types of a recursivrely constructed conditional expression.
   */
  template<typename... Args>
  struct tCommon
  {
    tCommon()
    { 
      static_assert(
        has_common_type<Args...>::value, 
        "Arguments do not share a common type"
      );
    }

    typedef std::tuple<has_common_type<Args...>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   *
   * The Convertible trait if a type T can be converted to a type U.
   */
  template<typename T, typename U>
  struct tConvertible
  {
    tConvertible()
    { 
      static_assert(std::is_convertible<T, U>::value, "Cannot convert types");
    }

    typedef std::tuple<std::is_convertible<T, U>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };


  /**
   * @ingroup concepts
   *
   * The Constructible trait is valid for a type T that can be constructed over 
   * the given sequence of arguments.
   */
  template<typename T, typename... Args>
  struct tConstructible
  {
    tConstructible()
    { 
      auto p = constraints; 
    }

    static void constraints(Args&&... args)
    {
      T{std::forward<Args>(args)...};
    }

    typedef std::tuple<is_constructible<T, Args...>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   *
   * The Assignable trait is valid for a T that supports assignment to a type 
   * U.
   */
  template<typename T, typename U>
  struct tAssignable
  {
    tAssignable()
    { 
      auto p = constraints; 
    }

    static void constraints(T& x, U y)
    { x = y; }

    typedef std::tuple<has_assign<T, U>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   *
   * The Default trait is valid for types that can be default constructed.
   */
  template<typename T>
  struct tDefault
  {
    tDefault()
    { 
      static_assert(
        std::is_constructible<T>::value, 
        "Type is not Default constructible"
      );
    }

    // FIXME: Use std::has_default_constructor when it becomes available.
    typedef std::tuple<std::is_constructible<T>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   *
   * The Copyable trait is valid for a type T that supports copy construction. 
   * Note that the trait not associate semantics with copyability.
   *
   * See the constraint class cCopyable for a semantic version of Copyable.
   */
  template<typename T>
  struct tCopyable
  {
    tMoveable()
    { 
      static_assert(
        is_constructible<T, T const&>::value,
        "Type is not Copy constructible"
      );
    }

    // FIXME: Use std::has_copy_constructor when it becomes available.
    typedef std::tuple<std::is_constructible<T, T const&>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   *
   * The Moveable trait is valid for a type T that supports move construction.
   */
  template<typename T>
  struct tMoveable
  {
    tMoveable()
    { 
      static_assert(
        is_constructible<T, T&&>::value,
        "Type is not Move constructible"
      );
    }

    // FIXME: Use std::has_move_constructor when it becomes available.
    typedef std::tuple<std::is_constructible<T, T&&>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

} // namespace origin

#endif
