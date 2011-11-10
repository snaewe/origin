// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_HPP
#define ORIGIN_ITERATOR_HPP

#include <iterator>

#include <origin/concepts.hpp>

namespace origin
{
  // Separate the checking of static requirements out of the Readable concept.
  // This prevents compiler errors when associated types aren't found. If
  // associated types can't be found, then the requirements will obviously
  // fail to pass.
  template<typename Iter, bool Types>
    struct Readable_requirements
    {
      static constexpr bool check()
      {
        return false;
      }
    };

  template<typename Iter> 
    struct Readable_requirements<Iter, true>
    {
      static constexpr bool check()
      {
        return Has_dereference<Iter>()
            && Convertible<Dereference_result<Iter>, Value_type<Iter> const&>();
      }
    };
  
  // Allows the following expression: Value_type<Iter> const& x = *i.
  template<typename Iter>
    struct Readable_concept
    {
      static constexpr bool check()
      {
        return Readable_requirements<Iter, Has_value_type<Iter>()>::check();
      }
      
      static bool test(Iter i)
      {
        Value_type<Iter> const& x = *i; // NOTE: Not an axiom.
        return true;
      }
    };
    
  template<typename Iter>
    constexpr bool Readable()
    {
      return Readable_concept<Iter>::check();
    }

    
  
  // An iterator is Move_writable if the expression '*i = move(x)' is valid.
  template<typename Iter, typename T>
    struct Move_writable_concept
    {
      static constexpr bool check()
      {
        return Movable<T>()
            && Has_dereference<Iter>()
            && Assignable<Dereference_result<Iter>, T&&>();
      }
      
      static bool test(Iter i, T x)
      {
        *i = std::move(x);  // NOTE: Not an axiom.
        return true;
      }
    };
    
  template<typename Iter, typename T>
    constexpr bool Move_writable()
    {
      return Move_writable_concept<Iter, T>::check();
    }

  
  
  // An iterator is writable if the expression '*i = x' is valid.
  template<typename Iter, typename T>
    struct Writable_concept
    {
      static constexpr bool check()
      {
        return Copyable<T>()
            && Move_writable<Iter, T>()
            && Assignable<Dereference_result<Iter>, T const&>();
      }
      
      static bool test()
      {
        // FIXME: Write semantics
        return true;
      }
    };
    
  template<typename Iter, typename T>
    constexpr bool Writable()
    {
      return Writable_concept<Iter, T>::check();
    }
    
    
    
  // Iterator categories
  // This implementation is slightly different than the specialization
  // mechanism used for Value_type and Difference_type.
  //
  // TODO: This may be a better solution for managing specializations than
  // the traits for Value_type and Difference_type. One reason why we might
  // not is the fact that the we couldn't write a universally useful function
  // for returning an object of the appropriate type. Functions for those
  // types would end up taking dummy arguments.
  
  // Safely deduce a custom iterator category as a nested type.
  template<typename Iter>
    struct get_iterator_category
    {
    private:
      template<typename X>
        static typename X::iterator_category check(X&&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<Iter>()));
    };
  
    
  // The iterator_cateogry function returns an object of the associated 
  // iterator category type. Specializations can be created for user-defined
  // types, if needed.
  template<typename Iter>
    typename get_iterator_category<Iter>::type iter_category(Iter) { return {}; }

  template<typename T>
    typename std::iterator_traits<T*>::iterator_category iter_category(T*) { return {}; }
    
  // An alias for the category of Iter.
  template<typename Iter>
    using Iterator_category = decltype(iter_category(std::declval<Iter>()));
    
  // Return true if the iterator category is valid.
  template<typename Iter>
    constexpr bool Has_iterator_category()
    {
      return Subst_succeeded<Iterator_category<Iter>>();
    }

  // Return true if T is an iterator. All iterators have an iterator category.
  template<typename T>
    constexpr bool Iterator()
    {
      return Has_iterator_category<T>();
    }
  
  
    
  // Input iterators
  // Unlike the TR, we start with Input_iterators, not Incrementable types.
  // Also, there is no such thing as a weak input iterator. We always assume
  // equality exists.
  //
  // FIXME: How closely should this follow the TR?
  
  template<typename Iter, bool Types>
    struct Input_iterator_requirements
    {
      static constexpr bool check()
      {
        return false;
      }
    };
    
  template<typename Iter>
    struct Input_iterator_requirements<Iter, true>
    {
      static constexpr bool check()
      {
        return Regular<Iter>()
            && Derived<Iterator_category<Iter>, std::input_iterator_tag>()
            && Readable<Iter>()
            && Has_pre_increment<Iter>()
            && Same<Pre_increment_result<Iter>, Iter&>()
            && Has_post_increment<Iter>()
            && Readable<Post_increment_result<Iter>>();
      }
    };
    
  // An input iterator...
  template<typename Iter>
    struct Input_iterator_concept
    {
      static constexpr bool check()
      {
        return Input_iterator_requirements<
          Iter, Has_iterator_category<Iter>() && Has_difference_type<Iter>()
        >::check();
      }
      
      static bool test()
      {
        return true;
      }
    };
    
  template<typename Iter>
    constexpr bool Input_iterator()
    {
      return Input_iterator_concept<Iter>::check();
    }
    
  
  
  // A forward iterator
  template<typename Iter>
    struct Forward_iterator_concept
    {
      static constexpr bool check()
      {
        return Input_iterator<Iter>()
            && Derived<Iterator_category<Iter>, std::forward_iterator_tag>()
            && Same<Post_increment_result<Iter>, Iter>();
      }
      
      static bool test()
      {
        return true;
      }
    };
    
  template<typename Iter>
    constexpr bool Forward_iterator()
    {
      return Forward_iterator_concept<Iter>::check();
    }
    
    
    
  // A bidirectional iterators...
  template<typename Iter>
    struct Bidirectional_iterator_concept
    {
      static constexpr bool check()
      {
        return Forward_iterator<Iter>()
            && Derived<Iterator_category<Iter>, std::bidirectional_iterator_tag>()
            && Has_pre_decrement<Iter>()
            && Same<Pre_decrement_result<Iter>, Iter&>()
            && Has_post_decrement<Iter>()
            && Same<Post_decrement_result<Iter>, Iter>();
      }
    };

  template<typename Iter>
    constexpr bool Bidirectional_iterator()
    {
      return Bidirectional_iterator_concept<Iter>::check();
    };
    
    
    
  // A bidirectional iterators...
  template<typename Iter>
    struct Random_access_iterator_concept
    {
      static constexpr bool check()
      {
        // FIXME: Not done.
        return Bidirectional_iterator<Iter>()
            && Derived<Iterator_category<Iter>, std::random_access_iterator_tag>()
            && Has_plus_assign<Iter, Difference_type<Iter>>()
            && Same<Plus_assign_result<Iter, Difference_type<Iter>>, Iter&>()
            && Has_minus_assign<Iter, Difference_type<Iter>>()
            && Same<Minus_assign_result<Iter, Difference_type<Iter>>, Iter&>();
      }
    };

  template<typename Iter>
    constexpr bool Random_access_iterator()
    {
      return Random_access_iterator_concept<Iter>::check();
    };
    
/*
  // A helper trait for deciding is_iterator.
  template<typename T>
    struct is_iterator__
    {
    private:
      template<typename X>
        static typename std::iterator_traits<X>::iterator_category check(X const&);
      static subst_failure check(...);
    public:
      typedef decltype(check(std::declval<T>())) type;
    };
  
  // Return true if T is an iterator.
  template<typename T>
    struct is_iterator
      : substitution_succeeded<typename is_iterator__<T>::type>
    { };


  // Return an iterator category that is not more derived than the given limit. 
  // For example, if Tag is "random access" and Limit is "forward", this will
  // return limit.
  template<typename Tag, typename Limit>
  struct clamp_iterator_category
    : std::conditional<std::is_base_of<Limit, Tag>::value, Limit, Tag>
  { };
*/

} // namespace origin

// Iterator adaptors
// #include <origin/iterator/filter.hpp>


#endif
