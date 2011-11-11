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
#include <origin/range.hpp>

namespace origin
{
  // The range proxy class is the result type of the post-increment operator.
  // for some range types. It can be dereferenced, but that's about it.
  //
  // Because it's Readable, it has to have a value type.
  template<typename R>
    class range_proxy
    {
      using value_type = R;

      range_proxy(R const* r)
        : range(r)
      { }
      
      R const& operator*() const { return *range; }
      
      R const* range;
    };


  // A function object that computes the next partial permutation on some
  // given range.
  struct next_permutation_func
  {
    template<typename R, typename I, typename C>
      bool operator()(R& r, I mid, C comp) const
      {
        return next_partial_permutation(r, mid, comp);
      }
  };
  
  
  // A helper function that computes the next combionation on some given range.
  struct next_combination_func
  {
    template<typename R, typename I, typename C>
      bool operator()(R& r, I mid, C comp) const
      {
        return next_combination(r, mid, comp);
      }
  };



  // A helper iterator for the permutation_range. Dereferncing this iterator
  // yields a bounded range that some subset of the original range. The sub-
  // range depends on the number of elements in the set of permutations or
  // combinations.
  // 
  // Note that, despite the fact, that we could use prev_permutation to 
  // implement bidirectional features, the iterator is only actually an input
  // iterator. The iterator has a shared state, meaning its increment operation
  // is not regular.
  template<typename R>
    class permutation_range_iterator
    {
    public:
      using value_type = bounded_range<typename R::base_iterator>;
      using reference = value_type const&;
      using pointer = value_type const*;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::input_iterator_tag;
    
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
      
      // FIXME: I think this is totally broken, and I don't think I can get
      // it to return the previous value. This may be a good argument against
      // input iterators requiring post-increment.
      range_proxy<R> operator++(int)
      {
        range_proxy<R> ret{range};
        operator++();
        return ret;
      }
      
      private:
        R* range;
        value_type value;   // The current selection.
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
      using value_type = Value_type<R>;
      using base_range = R
      using base_iterator = Iterator_type<R>;
      using value_compare = Comp;
      using range_permute = Perm;
      using iterator = permutation_range_iterator<this_type>;

      permutation_range(base_range& range, Perm perm, Comp comp = {})
        : range(range), mid(std::end(range)), perm(perm), comp(comp)
      { }

      permutation_range(base_range& r, base_iterator mid, Perm perm, Comp comp = {})
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
      -> permutation_range<R, next_permutation_func, std::less<Value_type<R>>>
    {
      return {range, next_permutation_func{}, std::less<Value_type<R>>{}};
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
  template<typename R>
    inline auto permutations(R& range, Size_type<R> k)
      -> permutation_range<R, next_permutation_func, std::less<Value_type<R>>> 
    {
      assert(( std::size_t{k} < size(range) ));
      return {range, std::next(std::begin(range), k), next_permutation_func{}};
    }

  template<typename R, typename Comp>
    inline auto permutations(R& range, Size_type<R> k, Comp comp)
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
  template<typename R>
    inline auto combinations(R& range, Size_type<R> k)
      -> permutation_range<R, next_combination_func, std::less<Value_type<R>>>
    {
      assert(( k <= size(range) ));
      return {range, std::next(std::begin(range), k), next_combination_func{}};
    }

  template<typename R, typename Comp>
    inline auto combinations(R& range, Size_type<R> k, Comp comp)
      -> permutation_range<R, next_combination_func, Comp> 
    {
      assert(( k <= size(range) ));
      return {range, std::next(std::begin(range), k), next_combination_func{}, comp};
    }



} // namespace origin

#endif
