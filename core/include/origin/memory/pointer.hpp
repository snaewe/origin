// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MEMORY_POINTER_HPP
#define ORIGIN_MEMORY_POINTER_HPP

#include <origin/traits.hpp>

namespace origin
{
  // Pointer (concept)
  // A pointer encapsulates the location of an object in memory. Note that
  // memory does not need to be in-process; it could easily be in secondary
  // storage, or it could be remote.
  //
  // A void pointer is a kind of generic pointer to which all types of pointers
  // can be implicitly converted. In main memory, this is called void*. A
  // garbage-collected pointer might be called gc_ptr<void>.
  // 
  // FIXME: What are the actual requirements of a pointer? They vary based on
  // the type: object pointers and array pointers may be different types.
  template <typename Ptr>
    constexpr bool Pointer()
    {
      return true;
    }



  // Element type (trait)
  // Infrastructure for accessing the element type of a pointer.
  namespace traits
  {
    template <typename Ptr>
      struct element_type
      {
        using type = typename Ptr::element_type;
      };

    template <typename T>
      struct element_type<T*>
      {
        using type = T;
      };
  } // namespace traits



  // Element type (alias)
  // The element type of a pointer is the type of object being pointed at.
  // If Ptr is a C++ object or function pointer, then the element type is 
  // simply the type being pointed at. Otherwise, the element type must be
  // explicitly named by the Ptr class as Ptr::element_type.
  //
  // Note that the element is distinct from a pointer's value type, which does
  // not include cv-qualifiers.
  template <typename Ptr>
    using Element_type = typename traits::element_type<Ptr>::type;



  // Rebind pointer (trait)
  // The rebind pointer trait yields a pointer, in the same family as Ptr, to
  // an object of type T. If Ptr is not of type X*, then it must have an
  // associated rebind template to support rebinding.
  namespace traits
  {
    template <typename Ptr, typename T>
      struct rebind_pointer
      {
        using type = typename Ptr::template rebind<T>;
      };

    template <typename T, typename U>
      struct rebind_pointer<T*, U>
      {
        using type = U*;
      };
  } // namespace traits

  // Rebind pointer (alias)
  // The rebidin pointer
  template <typename Ptr, typename T>
    using Rebind_pointer = typename traits::rebind_pointer<Ptr, T>::type;



  // Same pointer family (constraint)
  // Returns true if P and Q are in the same family of pointers. P and Q are
  // in the same family if rebinding P to the element type of Q is the same
  // as Q.
  template <typename P, typename Q>
    constexpr bool Same_pointer_family()
    {
      return Same<Rebind_pointer<P, Element_type<Q>>, Q>();
    }



  // Pointer cast (utility)
  // The pointer casting utility abstracts the standard cast operators. In
  // particular, these classes work with the general Pointer abstraction in
  // addition to low-level C++ pointers.
  template <typename Result, typename Ptr>
    struct pointer_cast_util
    {
      static Result do_static_cast(Ptr p)
      {
        return p.template get_as<Result>();
      }
    };

  template <typename T, typename U>
    struct pointer_cast_util<T*, U*>
    {
      static T* do_static_cast(U* p)
      {
        return static_cast<T*>(p);
      }
    };


  // Static ptr cast
  // An abstraction of static_cast for any Pointer model.
  //
  // NOTE: We need this because the language does not understand generalized
  // Pointer concept. If it did, we should just be able to use static_cast on
  // any kind of pointer.
  template <typename Result, typename Ptr>
    inline Result static_ptr_cast(Ptr p)
    {
      return pointer_cast_util<Result, Ptr>::do_static_cast(p);
    }

} // namespace origin


#endif
