// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_ZIP_HPP
#define ORIGIN_ITERATOR_ZIP_HPP

#include <origin/iterator/facade.hpp>

namespace origin
{
  // Increment each sub-iterator in a zip iterator.
  //
  // TODO: This is a polymorphic version of increment_action. Should we make
  // all actions, functions polymorphic in this way. We can easily enforce
  // constraints using concepts.
  struct zip_increment
  {
    template<typename I>
      void operator()(I& x) const { ++x; }
  };

  // Increment each sub-iterator in a zip iterator.
  struct zip_decrement
  {
    template<typename Iter>
      void operator()(Iter& x) const { --x; }
  };

  // Dereference iterators iter a zip iterator.
  template<typename Result>
    struct zip_dereference
    {
      using result_type = Result;

      template<typename... Args>
        Result operator()(Args&&... args) const { return Result{*args...}; }
    };

  // FIXME: Actually determine the minimum supportable iterator type. These
  // could, in theory, be random access iterators.

  // FIXME: What happens if the ranges are not of equal length? It might be
  // sufficient to say that dereferencing a zip iterator where one is PTE
  // will result in undefined behavior.

  // The zipped iterator wraps a tuple of individual iterators. Operations
  // (e.g,. increment, decrement, dereference, etc.) are applied to all
  // iterators at the same time, causing them to iterate in lock-step.
  // The zipped iterator models the least model of all it's consitituent
  // iterator's models. For example, zipping over a list and a vector will
  // produce a Forward_Iterator.
  //
  // Note the zip iterator references are temporaries so pointer syntax is
  // not supported.
  template <typename... Is>
    class zip_iterator
    {
      static_assert(All(Iterator<Is>()...), "");
      using Ref = std::tuple<Iterator_reference<Is>...>;
    public:
      zip_iterator()
        : iters{}
      { }

      zip_iterator(Is... iters)
        : iters{iters...}
      { }

      // Return the underlying iterator tuple.
      std::tuple<Is...> iterators() const { return iters; }


      // Readable
      Ref operator*() const 
      { 
        return tuple_invoke(zip_dereference<Ref>{}, iters); 
      }

      // Equality comparable
      bool operator==(const zip_iterator& x) const { return iters == x.iters; }
      bool operator!=(const zip_iterator& x) const { return iters != x.iters; }
      
      // Increment
      zip_iterator& operator++()
      { 
        tuple_for_each(iters, zip_increment{}); 
        return *this;
      }
      
      zip_iterator operator++(int)
      {
        zip_iterator tmp{*this};
        operator++();
        return *this;
      }

    private:
      std::tuple<Is...> iters;
    };

    
  // Make zip iterator
  //
  // Returns an iterator that zips over the sequence of iters... in lock step.
  template <typename... Is>
    zip_iterator<Is...> make_zip_iterator(Is... iters)
    {
      return zip_iterator<Is...>(iters...);
    }


} // namespace origin

#endif
