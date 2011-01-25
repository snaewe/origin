// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TUPLE_HPP
#define ORIGIN_TUPLE_HPP

#include <cstddef>
#include <type_traits>
#include <tuple>

namespace origin
{
  /**
   * @defgroup tuple Tuples
   * This module defines additional features that support generic programming
   * with tuples. This includes various tuple/sequence algorithms and expanding
   * tuples as function arguments.
   *
   * @todo Look at the Boost.Fusion library. What other algorithms might be
   * useful for working with tuples and sequences of types.
   */

  /**
   * @internal
   * This class applies the a function to each element of the tuple, from left
   * to right.
   *
   * @tparam I The current tuple index
   * @tparam N The number of elements in the tuple.
   * @tparam Func A polymorphic, unary function object.
   */
  template<std::size_t I, std::size_t N, typename Func>
  struct tuple_for_each_
  {
    template<typename Tuple>
    static void apply(Func f, Tuple& x)
    {
      f(std::get<I>(x));
      tuple_for_each_<I + 1, N, Func>::apply(f, x);
    }
  };

  // Base case for the for_each.
  template<std::size_t N, typename Func>
  struct tuple_for_each_<N, N, Func>
  {
    template<typename Tuple>
    static void apply(Func f, Tuple& x)
    { }
  };

  /**
   * Apply the function f to each element of the given tuple. The order of
   * applicaiton is from left to right (i.e,. [0, sizeof...(ArgS]) rather
   * than in reverse.
   *
   * @tparam Func A polymorphic Function Object
   * @tparam Args... Tuple arguments.
   */
  template<typename Func, typename... Args>
  void tuple_for_each(std::tuple<Args...>& x, Func f)
  { tuple_for_each_<0, sizeof...(Args), Func>::apply(f, x); }

  template<typename Func, typename... Args>
  void tuple_for_each(std::tuple<Args...> const& x, Func f)
  { tuple_for_each_<0, sizeof...(Args), Func>::apply(f, x); }

  /**
   * @internal
   * The expander template is responsible for recursively constructing the
   * sequence of arguments that will ultimately be passed to the function
   * call.
   *
   * @tparam Result The type resulting from the call. This would typically be
   *                the same as the result type of the called function or the
   *                constructed type.
   * @tparam I      The current tuple index.
   * @tparam N      The number of arguments in the tuple.
   */
  template<size_t I, size_t N, typename Result>
  struct tuple_expand_
  {
    template<typename Func, typename Tuple, typename... Args>
    static Result apply(Func f, Tuple& x, Args&&... args)
    {
      return tuple_expand_<I + 1, N, Result>::apply(
        f, x, std::forward<Args>(args)..., std::get<I>(x)
      );
    }

    template<typename Func, typename Tuple, typename... Args>
    static Result apply(Func f, Tuple const& x, Args&&... args)
    {
      return tuple_expand_<I + 1, N, Result>::apply(
        f, x, std::forward<Args>(args)..., std::get<I>(x)
      );
    }
  };

  // Base case. This ultimately calls the target function.
  template<size_t N, typename Result>
  struct tuple_expand_<N, N, Result>
  {
    template<typename Func, typename Tuple, typename... Args>
    static Result apply(Func f, Tuple& x, Args&&... args)
    { return f(std::forward<Args>(args)...); };

    template<typename Func, typename Tuple, typename... Args>
    static Result apply(Func f, Tuple const& x, Args&&... args)
    { return f(std::forward<Args>(args)...); };
  };

  // Recursive case when the result is void.
  template<size_t I, size_t N>
  struct tuple_expand_<I, N, void>
  {
    template<typename Func, typename Tuple, typename... Args>
    static void apply(Func f, Tuple& x, Args&&... args)
    {
      tuple_expand_<I + 1, N, void>::apply(
        f, x, std::forward<Args>(args)..., std::get<I>(x)
      );
    }

    template<typename Func, typename Tuple, typename... Args>
    static void apply(Func f, Tuple const& x, Args&&... args)
    {
      tuple_expand_<I + 1, N, void>::apply(
        f, x, std::forward<Args>(args)..., std::get<I>(x)
      );
    }
  };

  // Base case when the result type is void,
  template<size_t N>
  struct tuple_expand_<N, N, void>
  {
    template<typename Func, typename Tuple, typename... Args>
    static void apply(Func f, Tuple& x, Args&&... args)
    { f(std::forward<Args>(args)...); };

    template<typename Func, typename Tuple, typename... Args>
    static void apply(Func f, Tuple const& x, Args&&... args)
    { f(std::forward<Args>(args)...); };
  };

  /**
   * The tuple invoke function invokes the polymorphic forwarding function f
   * over the arguments expanded from the given tuple. A "polymorphic
   * forwarding function object" is a function object that defines a variadic
   * template function call operator that forwards the expanded arguments to
   * the target function call.
   *
   * @note If the target function call is NOT itself a variadic function, then
   * the function object should NOT be a variadic function, but overload for
   * the precise number and type of arguments expected. I think.
   */
  // non-void result, non-const tuple
  template<typename Func, typename... Args>
  inline typename std::enable_if<
    !std::is_same<typename std::result_of<Func(Args...)>::type, void>::value,
    typename std::result_of<Func(Args...)>::type
  >::type
  tuple_invoke(Func f, std::tuple<Args...>& x)
  {
    typedef typename std::result_of<Func(Args...)>::type Result;
    return tuple_expand_<0, sizeof...(Args), Result>::apply(f, x);
  }

  // non-void result, const tuple
  template<typename Func, typename... Args>
  inline typename std::enable_if<
    !std::is_same<typename std::result_of<Func(Args...)>::type, void>::value,
    typename std::result_of<Func(Args...)>::type
  >::type
  tuple_invoke(Func f, std::tuple<Args...> const& x)
  {
    typedef typename std::result_of<Func(Args...)>::type Result;
    return tuple_expand_<0, sizeof...(Args), Result>::apply(f, x);
  }

  // void result, non-const tuple
  template<typename Func, typename... Args>
  inline typename std::enable_if<
    std::is_same<typename std::result_of<Func(Args...)>::type, void>::value, void
  >::type
  tuple_invoke(Func f, std::tuple<Args...>& x)
  { tuple_expand_<0, sizeof...(Args), void>::apply(f, x); }

  // void result, non-const tuple
  template<typename Func, typename... Args>
  inline typename std::enable_if<
    std::is_same<typename std::result_of<Func(Args...)>::type, void>::value, void
  >::type
  tuple_invoke(Func f, std::tuple<Args...> const& x)
  { tuple_expand_<0, sizeof...(Args), void>::apply(f, x); }
} // namespace origin

#endif
