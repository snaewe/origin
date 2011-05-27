// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_FUNCTION_HPP
#define ORIGIN_CONCEPTS_FUNCTION_HPP

#include <origin/tuple.hpp>
#include <origin/relation.hpp>
#include <origin/concepts/impl.hpp>

namespace origin
{
  /**
   * @defgroup concepts_function_traits
   *
   * Function traits are constraints that evaluate properties about functions,
   * function objects, and lambda expressions.
   */

  /**
   * @ingroup concepts_function_traits
   *
   * The Callable trait is valid for any type that can be called (as a
   * function) over the given sequence of argument types.
   */
    template<typename F, typename... Args>
    struct tCallable
    {
      tCallable()
      {
        auto p = constraints;
      }

      static void constraints(F f, Args&&... args)
      {
        f(std::forward<Args>(args)...);
      }

      typedef std::tuple<
        is_callable<F, Args...>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_function_traits
   *
   * The Procedure trait is valid for any Callable type that can be copy
   * constructed. The value-oriented progrmaming style of generic libraries
   * requires functions and function objects to be copy constructible.
   *
   * Note that the Copyable concept is not a requirement due to its additional
   * requirement on Comparable. Function objects and lambda expressions are
   * not equality comparable. Otherwise, this should be a concept.
   */
  template<typename Proc, typename... Args>
    struct tProcedure
    {
      tProcedure()
      {
        tConstructible<Proc, Proc const&>{};
        tCallable<Proc, Args...>{};
      }

      typedef std::tuple<
        tConstructible<Proc, Proc const&>,
        tCallable<Proc, Args...>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };


  /**
   * @defgroup concepts_function
   *
   * Function concepts are constraints that define semantics functions,
   * function objects, and lambda expressions.
   */

  /**
   * @ingroup concepts_axioms
   *
   * Application equivalence requires that a function applied to equivalent
   * arguments produce equivalent results.
   */
  template<typename F, typename... Args>
    bool aApplication_Equality(F f, std::tuple<Args...> x, std::tuple<Args...> y)
    {
      return implies(x == y, tuple_invoke(f, x) == tuple_invoke(f, y));
    }

  /**
   * @ingroup concepts_function
   *
   * The Function concept is valid for Procedures that return a value and
   * satisfy the Application_Equivalence requirements.
   */
  template<typename Func, typename... Args>
    struct cFunction
    {
      cFunction()
      {
        auto p = constriants;
      }

      static void constriants(Func func,
                              std::tuple<Args...> x,
                              std::tuple<Args...> y)
      {
        tProcedure<Func, Args...>{};
        tDifferent<typename std::result_of<Func(Args...)>::type, void>{};
        aApplication_Equality(func, x, y);
      }

      typedef std::tuple<
        tProcedure<Func, Args...>,
        tDifferent<typename std::result_of<Func(Args...)>::type, void>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_function
   *
   * The Operation concept is valid for Functions that are called over a
   * homogenous sequence of arguments; the sequence of arguments must share
   * a Common type.
   */
  template<typename Op, typename... Args>
    struct cOperation
      : cFunction<Op, Args...>
    {
      cOperation()
      {
        auto p = constraints;
      }
      
      static void constraints()
      {
        tCommon<Args...>{};
        tConvertible<
          typename get_conditional_result<Args...>::type,
          typename std::result_of<Op(Args...)>::type
        >{};
      }

      typedef std::tuple<
        cFunction<Op, Args...>,
        tCommon<Args...>,
        tConvertible<
          typename get_conditional_result<Args...>::type,
          typename std::result_of<Op(Args...)>::type
        >
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_function
   *
   * A Predicate is a Function whose result type is Boolean.
   */
  template<typename Pred, typename... Args>
    struct cPredicate
      :cFunction<Pred, Args...>
    {
      cPredicate()
      {
        auto p = constraints;
      }

      static void constraints(Pred p,
                              std::tuple<Args...> x,
                              std::tuple<Args...> y)
      {
        cBoolean<typename std::result_of<Pred(Args...)>::type>{};
      }

      typedef std::tuple<
        cFunction<Pred, Args...>,
        cBoolean<typename std::result_of<Pred(Args...)>::type>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

} // namespace origin

#endif
