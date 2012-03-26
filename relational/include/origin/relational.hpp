// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RELATIONAL_HPP
#define ORIGIN_RELATIONAL_HPP

#include <origin/traits.hpp>
#include <origin/tuple.hpp>

namespace origin
{
  // Member variable accessor
  // Wraps a member variable pointer of the form T(C::*) and returns a 
  // reference to the pointed-at member variable. 
  template <typename Ptr>
    struct memvar_accessor
    {
      Ptr ptr;

      memvar_accessor(Ptr p) : ptr(p) { }

      template <typename C>
        auto operator()(C&& x) const -> decltype(x.*ptr) 
        { 
          return x.*ptr; 
        }
    };


  // Member function accessor
  // Wraps a member function pointer of the form T(C::*)() and returns the 
  // result of that operation.
  template <typename Ptr>
    struct memfun_accessor
    {
      Ptr ptr;

      memfun_accessor(Ptr p) : ptr(p) { }

      template <typename C>
        auto operator()(C&& x) const ->decltype((x.*ptr)()) 
        { 
          return (x.*ptr)(); 
        }
    };

  /*
  // Const specialization
  template <typename T, typename C>
    struct memfun_accessor<T(C::*)() const>
    {
      using pointer_type = T(C::*)() const;
      using result_type = T;
      
      pointer_type ptr;

      memfun_accessor(pointer_type p) : ptr(p) { }

      T operator()(const C& x) const
      {
        return (x.*ptr)();
      }
    };
  */

  // Accessor traits
  // This traits class associates a concrete accessor function type with its
  // type argument. If A is already an accessor function, then the traits
  // class is simply an indentity operation.
  //
  // Specializations adapt member variable and member function pointers.
  template <typename A>
    struct accessor_traits
    {
      using function_type = A;
    };

  template <typename T, typename C>
    struct accessor_traits<T(C::*)>
    {
      using function_type = memvar_accessor<T(C::*)>;
    };

  template <typename T, typename C>
    struct accessor_traits<T(C::*)()>
    {
      using function_type = memfun_accessor<T(C::*)()>;
    };

  template <typename T, typename C>
    struct accessor_traits<T(C::*)() const>
    {
      using function_type = memfun_accessor<T(C::*)() const>;
    };

  // Return an accessor function for one of several kinds of types. 
  template <typename A>
    using Accessor_function = typename accessor_traits<A>::function_type;

  template <typename... Accs>
    using Projector_tuple = std::tuple<Accessor_function<Accs>...>;


  // Make accessor
  // Returns an accessor function or accessor function adpator for the unary
  // function, member variable pointer, or member function pointer.
  template <typename A>
    inline Accessor_function<A> make_accessor(A a) 
    { 
      return Accessor_function<A>(a); 
    }



  // Make projector
  // Returns a projector, a tuple comprising the given accessor functions.
  template <typename... As>
    inline Projector_tuple<As...> make_projector(As&&... as) 
    {
      return Projector_tuple<As...>(make_accessor(std::forward<As>(as))...);
    }



  // The project implementation actually constructs the projection by applying
  // a set of accessor functions to an object.
  //
  // NOTE: The actual implementation is written as a partial specialization in
  // order to get access to the tuple argument types. They're needed to generate
  // the result type.
  template <typename T, typename Proj>
    struct projection_impl;

  template <typename T, typename... Accs>
    struct projection_impl<T, std::tuple<Accs...>>
    {
      using result_type = std::tuple<Result_of<Accs(T)>...>;

      T& value;

      projection_impl(T& x) : value(x) { }

      result_type operator()(Accs... accs) const 
      {
        return result_type {accs(value)...};
      }
    };



  // NOTE: Proj == Projector, as in the tuple of accessor for functions that
  // can be used to project a tuple of attributes for some object.



  // Projection
  // The project function applies a sequence of key function to a given
  // object, returning a tuple of key values.
  template <typename T, typename Proj>
    struct projection
    {
      using Impl = projection_impl<T, Proj>;
      using Const_impl = projection_impl<const T, Proj>;

      Proj proj;

      projection(const Proj& p) : proj(p) { }

      auto operator()(T& x) const
        -> decltype(tuple_invoke(Impl {x}, proj))
      {
        return tuple_invoke(Impl {x}, proj);
      }

      auto operator()(const T& x) const
        -> decltype(tuple_invoke(Const_impl {x}, proj))
      {
        return tuple_invoke(Const_impl {x}, proj);
      }
    };

  template <typename T, typename... As>
    using Projection_result = Result_of<projection<T, Projector_tuple<As...>>(T)>;



  // Project
  // Return a tuple of elements projected by the given list of accessor
  // functions.
  template <typename T, typename... Accs>
    inline void project(T&& x, Accs&&... accs) 
    {
      std::cout << typestr<Projection_result<T, Accs...>>() << '\n';
    }



  // Projection equal
  // Compare the projections of two objects for equality.
  template <typename T, typename Proj>
    struct projection_equal
    {
      projection<T, Proj> proj;

      projection_equal(const Proj& p) : proj(p) { }

      bool operator()(const T& a, const T& b) const 
      { 
        return proj(a) == proj(b); 
      }
    };


  // Projection less
  // Order the projections of two objects.
  template <typename T, typename Proj>
    struct projection_less
    {
      projection<T, Proj> proj;

      projection_less(const Proj& p) : proj(p) { }

      bool operator()(const T& a, const T& b) const 
      {
        return proj(a) < proj(b);
      }
    };

} // namespace origin

#endif
