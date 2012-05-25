// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_FILTER_HPP
#define ORIGIN_ITERATOR_FILTER_HPP

#include <origin/iterator/facade.hpp>

namespace origin
{
  template <typename I, typename P> constexpr bool Query();

  template <typename I, typename P> I find_if(I, I, P);
  template <typename I, typename P> I find_next_if(I, I, P);


  template <typename I, typename F> class filter_iterator;


  // An alias to the facade used by the filter iterator.
  template <typename I, typename P>
    using Filter_iterator_facade
      = iterator_facade<filter_iterator<I, P>, I>;


  // Filter iterator
  //
  // A filter iterator is an Iator adaptor that, when incremented, will
  // move to the next element satisfying the given predicate. Filter iterators
  // are maximally forward iterators (i.e., they do not support decrement or
  // random access).
  //
  // Invariants: 
  //    this->first == this->last || pred(*this->first) == true
  //
  // A comparison (e.g., == or <) or operation (e.g., -) on filter iterators is
  // valid only when those iterators have the same predicate. If the iterators
  // have different actions, the computation results in undefined behavior.
  template <typename I, typename P>
    class filter_iterator
      : public Filter_iterator_facade<I, P>
    {
      static_assert(Query<I, P>(), "");

      using Base = Filter_iterator_facade<I, P>;
    public:
      // Initialize the filter Iator as its limit. The predicate may be 
      // omitted if Pred is Default_constructible.
      filter_iterator(I last, P pred = {})
        : data{last, last, pred}
      { }
    
      // Initialize the Iator. Note that this->base() may not be equal to
      // first after initialization.
      filter_iterator(I first, I last, P pred)
        : data{find_if(first, last, pred), last, pred}
      { }
      
      // Readable
      Iterator_reference<I> operator*() const;
      
      // Incrementable
      // Advance to the next the next iterator whose referenced referenced
      // element satisfies the predicate.
      filter_iterator& operator++();
      using Base::operator++;
      
      // Iterator properties
      I base() const { return first(); }
      P predicate() const { return pred(); }


    private:
      I&       first()       { return std::get<0>(data); }
      const I& first() const { return std::get<0>(data); }
      
      const I& last() const { return std::get<1>(data); }
      
      const P& pred() const { return std::get<2>(data); }

    private:
      std::tuple<I, I, P> data;
    };

  // NOTE: If first == last, reading and incrementing are undefined behavior.
  // Asserting this will help programmers from falling into the trap of
  // accidentally constructing empty ranges of filter iterators by forgetting
  // that a non-empty range requires 3 arguments: first, last, and pred.

  template <typename I, typename P>
    inline Iterator_reference<I> filter_iterator<I, P>::operator*() const 
    { 
      assert(first() != last());
      return *first(); 
    }

  template <typename I, typename P>  
    inline auto filter_iterator<I, P>::operator++() -> filter_iterator&
    { 
      assert(first() != last());
      first() = find_next_if(first(), last(), pred()); 
      return *this;
    }


  // Equality_comparable<fitler_iterator<I, P>>
  // Two filter iterators compare equal if they refer to the same element.
  // The limit and predicate are not compared.
  template <typename I, typename P>
    inline bool operator==(const filter_iterator<I, P>& a, const filter_iterator<I, P>& b)
    {
      return a.base() == b.base();
    }

  template <typename I, typename P>
    inline bool operator!=(const filter_iterator<I, P>& a, const filter_iterator<I, P>& b)
    {
      return !(a == b);
    }



  // Make filtered iterator

  // Returns an adapted filter iterator over [first, last) that includes all
  // iterators i where pred(*i) is true.
  template <typename I, typename P>
    inline filter_iterator<I, P> make_filter_iterator(I first, I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      assert(is_bounded_range(first, last));
      return {first, last, pred};
    }
    
  // Returns an adapted filter iterator over the empty range [last, last). The
  // predicate is a required argument, but not used.
  template <typename I, typename P>
    inline filter_iterator<I, P> make_filter_iterator(I last, P pred)
    {
      static_assert(Query<I, P>(), "");
      return {last, pred};
    }

} // namespace origin

#include <origin/algorithm.hpp>

#endif
