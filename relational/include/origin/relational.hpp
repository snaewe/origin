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
  // An attribute is an object that returns or computes a property associated 
  // with an object. It can be one of:
  //    - a unary function that map from an object to one of its a property.
  //    - a pointer to a member variable
  //    - a pointer to a unary member function
  //
  // An accessor is a unary function that maps from an object to a property.
  // Unary functions are natural accessors, but member pointers need to be 
  // adated.
  //
  // A projection is a tuple of property (references) specified by a sequence
  // of accessors. A projection of (adapted) attrs is also called a sort key,
  // when used in the context of sorting.
  //
  // A projector is a unary function (object) that returns a projection for
  // a specific object.



  // Member variable accessor
  // Wraps a member variable pointer of the form T(C::*) and returns a 
  // reference to the pointed-at member variable. 
  template <typename Memptr>
    struct memvar_accessor
    {
      Memptr ptr;

      memvar_accessor(Memptr p) : ptr(p) { }

      template <typename C>
        auto operator()(C&& x) const -> decltype(x.*ptr) { return x.*ptr; }
    };



  // Member function accessor
  // Wraps a member function pointer of the form T(C::*)() and returns the 
  // result of that operation.
  template <typename Memptr>
    struct memfun_accessor
    {
      Memptr ptr;

      memfun_accessor(Memptr p) : ptr(p) { }

      template <typename C>
        auto operator()(C&& x) const -> decltype((x.*ptr)()) { return (x.*ptr)(); }
    };



  // Accessor traits
  // This traits class associates a concrete accessor function type with its
  // type argument. If Attr is already an accessor function, then the traits
  // class is simply an indentity operation.
  namespace traits {
    template <typename Attr>
      struct accessor_traits
      {
        using type = Attr;
      };

    template <typename T, typename C>
      struct accessor_traits<T(C::*)>
      {
        using type = memvar_accessor<T(C::*)>;
      };

    template <typename T, typename C>
      struct accessor_traits<T(C::*)()>
      {
        using type = memfun_accessor<T(C::*)()>;
      };

    template <typename T, typename C>
      struct accessor_traits<T(C::*)() const>
      {
        using type = memfun_accessor<T(C::*)() const>;
      };
  } // namespace traits



  // Accessor function (alias)
  // Return an adapted accessor function for one of several kinds of types. 
  template <typename Attr>
    using Accessor_type = typename traits::accessor_traits<Attr>::type;



  // Make accessor
  // Returns an accessor function or accessor function adpator for the unary
  // function, member variable pointer, or member function pointer.
  template <typename Attr>
    inline Accessor_type<Attr> make_accessor(Attr attr) 
    { 
      return Accessor_type<Attr>(attr); 
    }



  // The project implementation actually constructs the projection by applying
  // a set of accessor functions to an object. Note that the object type T
  // can be const or non-const. The specific type matters.
  //
  // NOTE: The actual implementation is written as a partial specialization in
  // order to get access to the tuple argument types. They're needed to generate
  // the result type.
  template <typename T, typename Proj>
    struct projector_impl;

  template <typename T, typename... Accs>
    struct projector_impl<T, std::tuple<Accs...>>
    {
      using result_type = std::tuple<Result_of<Accs(T)>...>;

      T& value;

      projector_impl(T& x) : value(x) { }

      result_type operator()(Accs... accs) const 
      {
        return result_type(accs(value)...);
      }
    };



  // Projection (function)
  // The projection function applies a sequence of key function to a given
  // object, returning a tuple of key values.
  template <typename T, typename... Attrs>
    struct projector
    {
      using Proj = std::tuple<Accessor_type<Attrs>...>;

      Proj proj;

      projector(Attrs... attrs) : proj(make_accessor(attrs)...) { }

      template <typename C>
        auto operator()(C&& x) const 
          -> decltype(tuple_invoke(projector_impl<C, Proj> {x}, proj))
        {
          // NOTE: Same<Forwarded<C>, T> must be true.
          return tuple_invoke(projector_impl<C, Proj> {x}, proj);
        }
    };



  // Make projection
  // Returns a projector for the explicitly specified type T and the given
  // attrs.
  template <typename T, typename... Attrs>
    inline projector<T, Attrs...> projection(Attrs... attrs)
    {
      return projector<T, Attrs...> {attrs...};
    }


  
  // Projected type (alias)
  // An alias to the projected type of Attrs on T. This is a tuple referring to
  // elements specified by the sequence of expressions.
  template <typename T, typename... Attrs>
    using Projected_type = Result_of<projector<T, Attrs...>(T)>;



  // Project
  // Return a projection of the given attrs of x.
  template <typename T, typename... Attrs>
    inline auto project(T&& x, Attrs... attrs) 
      -> Projected_type<Forwarded<T>, Attrs...>
    {
      auto p = projection<T>(attrs...);
      return p(std::forward<T>(x));
    }



  // Projects equal (function)
  // Compare two objects, a and b, by their projected attributes. Returns true
  // if the projection of a is equal to the projection of b.
  template <typename T, typename... Attrs>
    struct projects_equal
    {
      projector<T, Attrs...> proj;

      projects_equal(Attrs... attrs) : proj(attrs...) { }

      bool operator()(const T& a, const T& b) const 
      { 
        return proj(a) == proj(b);
      }
    };



  // Projects distinct (function)
  // Compare two objects, a and b, by their projected attributes. Returns true
  // if the projection of a is distinct from (not equal to) the projection 
  // of b.
  template <typename T, typename... Attrs>
    struct projects_distinct
    {
      projector<T, Attrs...> proj;

      projects_distinct(Attrs... attrs) : proj(attrs...) { }

      bool operator()(const T& a, const T& b) const 
      { 
        return proj(a) != proj(b);
      }
    };



  // Projects less (function)
  // Compares two objects, a and b, by their projected attributes. Returns true 
  // if the projection of a is lexicographically less than the projection of b.
  template <typename T, typename... Attrs>
    struct projects_less
    {
      projector<T, Attrs...> proj;

      projects_less(Attrs... attrs) : proj(attrs...) { }

      bool operator()(const T& a, const T& b) const 
      { 
        return proj(a) < proj(b);
      }
    };



  // Projects less (function)
  // Compares two objects, a and b, by their projected attributes. Returns true 
  // if the projection of a is lexicographically greater than the projection 
  // of b.
  template <typename T, typename... Attrs>
    struct projects_greater
    {
      projector<T, Attrs...> proj;

      projects_greater(Attrs... attrs) : proj(attrs...) { }

      bool operator()(const T& a, const T& b) const 
      { 
        return proj(a) > proj(b);
      }
    };


  // TODO: Write more comparators.

  

  // Comparator constructors
  // Helper functions for creating comparators.



  // Make projection equal (constructor)
  // Returns a equality comparison function that compares the projected
  // accessor functions.
  template <typename T, typename... Attrs>
    inline projects_equal<T, Attrs...> make_projects_equal(Attrs... attrs)
    {
      return projects_equal<T, Attrs...> {attrs...};
    }



  // Make projection distinct (constructor)
  // Returns a distinction comparison function that compares the projected
  // accessor functions.
  template <typename T, typename... Attrs>
    inline projects_distinct<T, Attrs...> make_projects_distinct(Attrs... attrs)
    {
      return projects_distinct<T, Attrs...> {attrs...};
    }



  // Make projection less (constructor)
  // Returns a less-than comparison function that compares the projected
  // accessor functions.
  template <typename T, typename... Attrs>
    inline projects_less<T, Attrs...> make_projects_less(Attrs... attrs)
    {
      return projects_less<T, Attrs...> {attrs...};
    }



  // Make projection greater (constructor)
  // Returns a equality comparison operator that compares the projected
  // accessor functions.
  template <typename T, typename... Attrs>
    inline projects_greater<T, Attrs...> make_projects_greater(Attrs... attrs)
    {
      return projects_greater<T, Attrs...> {attrs...};
    }

} // namespace origin

#endif
