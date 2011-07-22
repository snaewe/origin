// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_FUNCTIONAL_HPP
#define ORIGIN_FUNCTIONAL_HPP

#include <functional>
#include <limits>

#include <origin/concepts.hpp>

namespace origin
{
  template<typename T, typename... Args> struct tConstructible;
  template<typename R, typename T, typename U> struct cRelation;
  
  /**
   * @ingroup logic
   *
   * The truth function object is a constant Function that returns a value
   * that represents the notion of "true" for the given type.
   *
   * @tparam T  A Boolean type
   *
   * @note T is generally required to be Boolean, but we relax the requirement
   * to simply Constructible<T, bool> to avoid recursive definitions.
   */
  template<typename T>
    struct truth
      : tConstructible<T, bool>
    {
      T operator()() const
      {
        return T{true};
      }
    };

  /**
   * @ingroup logic
   *
   * The falsity function object is a constant Function that returns a value
   * that represents the notion of "false" for the given type.
   *
   * @tparam T  A Boolean type
   * 
   * @note T is generally required to be Boolean, but we relax the requirement
   * to simply Constructible<T, bool> to avoid recursive definitions.
   */
  template<typename T>
    struct falsity
      : tConstructible<T, bool>
    {
      T operator()() const
      {
        return T{false};
      }
    };

  // FIXME: Optimize with EBO?
  /**
   * The incomparable_to operation is defined in terms of a strict ordering
   * comparison on T, less by default. The operation returns true if, for
   * objects x and y, it is neither the case that x < y, nor y < x. 
   * 
   * Note that if the  template parameter T is a total order, then this is 
   * equivalent to the equal_to function.
   *
   * @tparam T      A type
   * @tparam Comp   A Strict Partial Order on T
   */
  template<typename T, typename Comp>
    struct incomparable_to
      : private cRelation<Comp, T, T>
    {
      // aStrict_Partial_Order<Comp, T>
      incomparable_to(Comp c)
        : comp{c}
      { }
      
      bool operator()(T const& x, T const& y) const
      {
        return !comp(x, y) && !comp(y, x);
      }
      
      Comp comp;
    };
    
  /**
   * The reciprocal operation computes the multiplicative inverse of its
   * argument (i.e,. x^-1 or equivalently 1/x).
   */
  template<typename T>
    struct reciprocal
    {
      T operator()(T const& x) const
      {
        return T{1} / x;
      }
    };
  
  /**
   * The operation traits class abstracts static properties about an operation.
   * Specifically, this provides associated types for defining the inverse
   * operations (both binary and unary), and the operations identity element.
   * Note that inverse operations may only be defined for operations satisfying
   * the requirements of the Group axiom.
   */
  template<typename Op> struct operation_traits;
  
  // Specialization for plus<T> where T is Arithmetic.
  // FIXME: T must be arithmetic in order for this template to be valid. The
  // same goes for the traits below.
  template<typename T>
    struct operation_traits<std::plus<T>>
    {
      typedef std::plus<T> operation;
      typedef std::minus<T> binary_inverse;
      typedef std::negate<T> unary_inverse;

      static constexpr T identity()
      {
        return T{0};
      }
    };

  // Specialization for multiplies<T> where T is Arithmetic.
  template<typename T>
    struct operation_traits<std::multiplies<T>>
    {
      typedef std::plus<T> operation;
      typedef std::divides<T> binary_inverse;
      typedef reciprocal<T> unary_inverse;

      static constexpr T identity()
      {
        return T{1};
      }
    };

  /**
   * @ingroup functional
   * The identity element function associates an identify value with a function. 
   * The function must be specialized for an Operation. For example, the 
   * additive identity, associated with the template plus<T> is T{0} for any 
   * Arithmetic type T.
   * 
   * Note that there is not general notion of identity. It is specific to the
   * type of an Operation.
   */
  template<typename Op>
    auto identity_element(Op const&)
      -> decltype(operation_traits<Op>::identity())
    {
      return operation_traits<Op>::identity();
    }
  
  // FIXME: Should I also provide properties that return unary operations?
  // I can use std::negate for addition, but I'd need to provide reciprocal for 
  // multiplication. Maybe unary_inverse?
  /**
   * @ingroup functional
   * 
   * The inverse operation returns an operation that can be used to compute
   * the inverse eleemnt of any value in the domain of the operation. 
   * Specifcally, for an operator Op1, this operation returns Op2 such that,
   * for any x in the domain of Op1 and Op2 Op2(x, x) == inv where inv is
   * the inverse element.
   */
  template<typename Op>
    typename operation_traits<Op>::inverse
      inverse_operation(Op const&)
      {
        return typename operation_traits<Op>::inverse{};
      }
  
  /**
   * The inverse comparison function object changs the position of arguments
   * in a comparison function in order to define an inverse order. For example,
   * if Comp is less<T>, the inverse_compare<less<T>> is equivalent to
   * greater<T>.
   */
  template<typename Comp>
    struct inverse_compare : Comp
    {
      inverse_compare(Comp c)
        : Comp(c)
      { }
      
      template<typename T>
        bool operator()(T const& x, T const& y) const
        {
          return Comp::operator()(y, x);
        }
    };
    
  /**
   * The invert order function returns the inverse of the supplied comparison.
   * 
   * @param Comp    An Order
   */
  template<typename Comp>
    inverse_compare<Comp> invert_order(Comp comp)
    {
      return inverse_compare<Comp>{comp};
    }
  
  // FIXME: Semantically, I think this should be:
  //  !(x < L) => x == L.
  // Note that this isn't strictly true for floating point types since NaN
  // compares false, but inf isn't NaN. That's exceptional, however.

  /**
   * The order traits abstracts information about the properties of an order
   * of an element. In particular, the order traits associoate an inverse
   * comparison operation with the comparison parameter and its extreme
   * value. The extreme value is a value V for which any other value in the 
   * domain of Comp, comp(x, V) == true and comp(V, x) == false. For example,
   * the extreme value of less<int> is numeric_limits<int>::max.
   */
  template<typename Comp> struct order_traits;
  
  // Specialization of order traits for less<T> where T is numeric.
  template<typename T>
    struct order_traits<std::less<T>>
    {
      typedef inverse_compare<std::less<T>> inverse;

      static constexpr T extreme()
      {
        return std::numeric_limits<T>::max();
      }
    };
  
  // Specialization of order traits for greater<T> where T is numeric.
  template<typename T>
    struct order_traits<std::greater<T>>
    {
      typedef inverse_compare<std::greater<T>> inverse;

      static constexpr T extreme()
      {
        return std::numeric_limits<T>::min();
      }
    };
    
  /**
   * Return the extreme value associated with the domain of the given
   * comparison operator.
   */
  template<typename Op>
    auto extreme_element(Op const&)
      -> decltype(order_traits<Op>::extreme())
    {
      return order_traits<Op>::extreme();
    }
    
} // namespace origin

#endif
