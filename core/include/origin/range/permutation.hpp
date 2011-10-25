// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_PERMUTATION_HPP
#define ORIGIN_RANGE_PERMUTATION_HPP

#include <origin/functional.hpp>
#include <origin/algorithm.hpp>
#include <origin/range/traits.hpp>
#include <origin/range/utility.hpp>
#include <origin/range/bounded.hpp>

namespace origin
{
  struct next_permutation_func
  {
    template<typename R, typename I, typename C>
      bool operator()(R& r, I mid, C comp) const
      {
        return next_partial_permutation(r, mid, comp);
      }
  };
  
  struct next_combination_func
  {
    template<typename R, typename I, typename C>
      bool operator()(R& r, I mid, C comp) const
      {
        return next_combination(r, mid, comp);
      }
  };


  // A helper iterator for the permutation_range. Dereferncing this iterator
  // yields a constant reference to the underlying range. We return a constant
  // reference because modifying the dereferenced range could invalidate the
  // iterator.
  // 
  // Note that, despite the fact, that we could use prev_permutation to 
  // implement bidirectional features, the iterator is only actually an input
  // iterator. The iterator has a shared state, meaning its increment operation
  // is not regular.
  template<typename R>
    class permutation_range_iterator
    {
    public:
      typedef bounded_range<typename R::base_iterator> value_type;
      typedef value_type const& reference;
      typedef value_type const* pointer;
      typedef std::ptrdiff_t difference_type;
      typedef std::input_iterator_tag iterator_category;
    
      permutation_range_iterator()
        : range(nullptr)
      { }
    
      permutation_range_iterator(R const* r)
        : range(const_cast<R*>(r)), value(r->first(), r->middle())
      { }
      
      // Equality_comparable
      bool operator==(permutation_range_iterator const& x) const { return range == x.range; }
      bool operator!=(permutation_range_iterator const& x) const { return range != x.range; }

      // Readable
      reference operator*()  const { return value; }
      pointer   operator->() const { return &value; }

      // Incrementable
      permutation_range_iterator& operator++()
      {
        if(!range->permute())
          range = nullptr;
        return *this;
      }
      
      permutation_range_iterator operator++(int)
      {
        range_proxy<R> ret{range};
        operator++();
        return ret;
      }
      
      private:
        R* range;
        value_type value;   // The "current" range.
    };

  // A permutation range implements a traversal over a sequence of permutations
  // on the underlying range. Each iterator in the range represents one such
  // permutation. The actual permutation computed during traversal is determined
  // by the Perm template parameter. Common uses of the range are iterating
  // over lexicographical k-permuations and k-combinations.
  //
  // requires: BidirectionalIterator<IteratorType<R>>
  // requires: PermutationFunction<Perm, R>
  // requires: Relation<Comp, ValueType<R>>
  //
  // Note that R may be const.
  template<typename R, typename Perm, typename Comp>
    class permutation_range
    {
      typedef permutation_range<R, Perm, Comp> this_type;
    public:
      typedef R                                     base_range;
      typedef typename range_traits<R>::iterator    base_iterator;
      typedef Comp                                  value_compare;
      typedef Perm                                  range_permute;
      typedef permutation_range_iterator<this_type> iterator;

      permutation_range(base_range& range, Perm perm, Comp comp = Comp{})
        : range(range), mid(std::end(range)), perm(perm), comp(comp)
      { }

      permutation_range(base_range& r, base_iterator mid, Perm perm, Comp comp = Comp{})
        : range(r), mid(mid), perm(perm), comp(comp)
      { }

      // Range
      iterator begin() const { return iterator(this); }
      iterator end() const   { return {}; }

      // Range properties
      base_range&       base() const       { return range; }
      
      // Range iterators
      base_iterator     first() const      { return range.begin(); }
      base_iterator     middle() const     { return mid; }
      base_iterator     last() const       { return range.end(); }

      // Functions
      range_permute     range_perm() const { return perm; }
      value_compare     value_comp() const { return comp; }
      
      // Range modifiers
      bool permute() { return perm(range, mid, comp); }

    private:
      base_range& range;
      base_iterator mid;
      range_permute perm;
      value_compare comp;
    };

  // FIXME: For permutations on the entire range, should I have a different
  // range class and iterator that uses next_permutaiton and not 
  // next_partial_permutation?
  //
  // FIXME: Can this work in reverse using prev_permutation?

  // Return a range over the lexicographically sorted permutations of a range
  // of n elements.
  //
  // The size of the returned range is given by factorial(n). This operation
  // is found in the math library.
  template<typename R>
    inline auto all_permutations(R& range)
      -> permutation_range<
        R, next_permutation_func, std::less<typename range_traits<R>::value_type>
      >
    {
      typedef typename range_traits<R>::value_type Value;
      return {range, next_permutation_func{}, std::less<Value>{}};
    }

  template<typename R, typename Comp>
    inline auto all_permutations(R& range, Comp comp)
      -> permutation_range<R, next_permutation_func, Comp>
    {
      return {range, next_permutation_func{}, comp};
    }


  // Return a range over the lexicographically sorted k-permutations of a 
  // range of n elements.
  //
  // The size of the returned range is given by falling_factorial(n, k). This 
  // operation is in the math library.
  template<typename R, typename Size>
    inline auto permutations(R& range, Size k)
      -> permutation_range<
        R, next_permutation_func, std::less<typename range_traits<R>::value_type>
      > 
    {
      assert(( std::size_t{k} < size(range) ));
      return {range, std::next(std::begin(range), k), next_permutation_func{}};
    }

  template<typename R, typename Size, typename Comp>
    inline auto permutations(R& range, Size k, Comp comp)
      -> permutation_range<R, next_permutation_func, Comp>
    {
      assert(( std::size_t{k} < size(range) ));
      return {range, std::next(std::begin(range), k), next_permutation_func{}, comp};
    }


  // Return a range over the k first lexicographically sorted combinations of a 
  // range of n elements.
  //
  // If the elements in r are unique (i.e., r is a set), the size of the 
  // returned range is given by binomial_coefficient(n, k). This operation is 
  // found in the math library. 
  //
  // If the elements are not unique (i.e., r is a multiset), the size of the 
  // returned range is given by multinomial_coefficient(n, k).
  template<typename R, typename Size>
    inline auto combinations(R& range, Size k)
      -> permutation_range<
        R, next_combination_func, std::less<typename range_traits<R>::value_type>
      > 
    {
      assert(( std::size_t{k} < size(range) ));
      return {range, std::next(std::begin(range), k), next_combination_func{}};
    }

  template<typename R, typename Size, typename Comp>
    inline auto combinations(R& range, Size k, Comp comp)
      -> permutation_range<R, next_combination_func, Comp> 
    {
      assert(( std::size_t{k} < size(range) ));
      return {range, std::next(std::begin(range), k), next_combination_func{}, comp};
    }



} // namespace origin

#endif
