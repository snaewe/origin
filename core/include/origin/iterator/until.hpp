// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_UNTIL_HPP
#define ORIGIN_ITERATOR_UNTIL_HPP

#include <origin/iterator/facade.hpp>

namespace origin
{
  template <typename I, typename P> constexpr bool Query();


  template <typename I, typename P> 
    class until_iterator;

  // An alias for the facade used by 
  template <typename I, typename P>
    using Until_iterator_facade = iterator_facade<until_iterator<I, P>, I>;


  // Until iterator
  //
  // The until iterator is an iterator that will increment until is has reached
  // its final end point or som predicate returns true.
  template <typename I, typename P>
    class until_iterator : public Until_iterator_facade<I, P>
    {
      static_assert(Query<I, P>(), "");

      using Base = Until_iterator_facade<I, P>;
    public:
      until_iterator(I first, I last, P pred)
        : data{init(first, last, pred), last, pred}
      { }

      // Returns the underlying iterator.
      I base() const { return first(); }

      // Readable
      Iterator_reference<I> operator*();

      // Incrementable
      until_iterator& operator++();
      using Base::operator++;

    private:
      static I init(I first, I last, P pred);

      // Returns the first (current) iterator
      I&       first()       { return std::get<0>(data); }
      const I& first() const { return std::get<0>(data); }

      // Returns the last iterator
      const I& last() const  { return std::get<1>(data); }
      
      // Returns the predicate function
      const P& pred() const  { return std::get<2>(data); }

    private:
      std::tuple<I, I, P> data;
    };


  template <typename I, typename P>
    inline Iterator_reference<I> until_iterator<I, P>::operator*()
    {
      assert(first() != last());
      return *first();
    }

  template <typename I, typename P>
    inline until_iterator<I, P>& until_iterator<I, P>::operator++()
    {
      assert(first() != last());
      first() = pred()(*first()) ? last() : next(first());
      return *this;
    }

  // Returns last if pred(*first) is true or first if it is not. If
  // first == last, then return last, obviously.
  template <typename I, typename P>
    I until_iterator<I, P>::init(I first, I last, P pred) 
    {
      if (first != last)
        return pred(*first) ? last : first;
      else
        return last;
    }


  // Equality comparable
  // Two terminating iterators are equal when their first iterators are
  // equal.
  template <typename I, typename P>
    inline bool 
    operator==(const until_iterator<I, P>& a, const until_iterator<I, P>& b)
    { 
      return a.base() == b.base(); 
    }
    
  template <typename I, typename P>
    inline bool
    operator!=(const until_iterator<I, P>& a, const until_iterator<I, P>& b)
    { 
      return a.base() != b.base();
    }


  // Make until iterator
  // Returns an iterator that will increment until pred returns true.
  template <typename I, typename P>
    inline until_iterator<I, P> make_until_iterator(I first, I last, P pred)
    {
      return {first, last, pred};
    }

  template <typename I, typename P>
    inline until_iterator<I, P> make_until_iterator(I last, P pred)
    {
      return {last, last, pred};
    }

} // namespace origin

#endif
