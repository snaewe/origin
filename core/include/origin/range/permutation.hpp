// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_PERMUTATION_HPP
#define ORIGIN_RANGE_PERMUTATION_HPP

#include <origin/range.hpp>

namespace origin
{
  // FIXME: This is going change.
  template <typename R, typename Comp>
    bool next_partial_permutation(R& range, Iterator_type<R> mid, Comp comp);

  template <typename R, typename Comp>
    bool next_combination(R& range, Iterator_type<R> mid, Comp comp);


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



  template <typename R, typename P, typename C>
    class permutation_range;


  // Permutation iterator
  //
  // The permtuation iterator is an input iterator that, when incremented,
  // permutes the elements of an underlying range. The value returned by the
  // iterator is a bounded range that refers to the elements of the current
  // permutation.
  //
  // TODO: Can we make the value type a const range? That would ideally prevent
  // the sequence from being modified during traversal (which could invalidate)
  // the algorithm.
  //
  // TODO: It isn't actually necessary to cache the current range. We easily
  // reconstruct it from the source range's [first, middle) configuration.
  template <typename R, typename P, typename C>
    class permutation_iterator
    {
    public:
      // The source (creator) type of this iterator.
      using Source = permutation_range<R, P, C>;

      permutation_iterator(Source* src = {}) : src{src} { }

      // Returns the underlying permtuation range.
      const Source* source() const { return src; }

      // Readable
      // Returns a bounded range delimiting the current permtuation.
      bounded_range<Iterator_type<R>> operator*() const;

      // Incrementable
      // Moves to the next lexicographical permutation.
      permutation_iterator& operator++();
      
    private:
      Source* src;
    };


  template <typename R, typename P, typename C>
    inline bounded_range<Iterator_type<R>> permutation_iterator<R, P, C>::operator*() const 
    { 
      return range(src->first(), src->middle());
    }

  template <typename R, typename P, typename C>
    inline auto permutation_iterator<R, P, C>::operator++() -> permutation_iterator&
    {
      auto perm = src->permutation_func();
      auto comp = src->value_comp();
      if (!perm(src->base(), src->middle(), comp))
        src = nullptr;
      return *this;
    }


  // Equality_comparable
  // Two permutation range interators are equal when they point to the same
  // underlying range being permuted. 
  template <typename R, typename P, typename C>
    inline bool operator==(const permutation_iterator<R, P, C>& a, 
                           const permutation_iterator<R, P, C>& b) 
    { 
      return a.source() == b.source();
    }

  template <typename R, typename P, typename C>
    inline bool operator!=(const permutation_iterator<R, P, C>& a, 
                           const permutation_iterator<R, P, C>& b) 
    { 
      return a.source() != b.source(); 
    }



  // Permutation range
  //
  // The permutation range represents a traversal over the lexicographical
  // sequence of permutations of an underlying range object (typically a
  // container of some kind). Each element in the permutation range is a 
  // range over the elements in that permtutation.
  //
  // The permutation of the underlying range is controlled by the Perm 
  // parameter. It is generally assumed to be either next_permutation_func
  // or next_combination_func.
  //
  // TODO: How do we make this work in reverse? Is it as easy as writing
  // prev_permutation_func? Clearly, this is not a bidirectional iterator,
  // so writing reversed(permutations(v)) would not normally be correct. 
  // However, it is possible to adapt that syntax to its correct behavior. This
  // is worth considering.
  template <typename R, typename Perm, typename Comp>
    class permutation_range
    {
      using This = permutation_range<R, Perm, Comp>;
    public:
      using Iter = permutation_iterator<R, Perm, Comp>;

      // Initialize an end iterator.
      permutation_range(R& range, Perm perm, Comp comp = {})
        : data{range, origin::end(range), perm, comp}
      { }

      // Initialize the iterator.
      permutation_range(R& range, Iterator_type<R> mid, Perm perm, Comp comp = {})
        : data{range, mid, perm, comp}
      { }


      // Observers

      // Returns the range's permutation function.
      Perm permutation_func() const { return std::get<2>(data); }

      // Returns the range's value comparison relation.
      Comp value_comp() const { return std::get<3>(data); }
      
      // Returns a reference to the underlying range.
      R& base() const { return std::get<0>(data); }

      // Returns the first iterator of the underlying range.
      Iterator_type<R> first() const  { return origin::begin(base()); }

      // Returns the current midpoint iterator of this range
      Iterator_type<R> middle() const { return std::get<1>(data); }

      // Returns an iterator past the end of the underlying range.
      Iterator_type<R> last() const   { return origin::end(base()); }


      // Range
      Iter begin() const { return Iter{const_cast<This*>(this)}; }
      Iter end() const   { return {}; }

    private:
      std::tuple<R&, Iterator_type<R>, Perm, Comp> data;
    };




  // Permutations
  //
  // Return a range over the lexicographically sorted permutations of a range
  // of n elements.
  //
  // The size of the returned range is given by factorial(n). This operation
  // is found in the math library.
  template <typename R, typename Comp = less_relation>
    inline auto permutations(R& range, Comp comp = {})
      -> permutation_range<R, next_permutation_func, Comp>
    {
      return {range, next_permutation_func{}, comp};
    }


  // Partial permutations
  // Return a range over the lexicographically sorted partial-permutations of a 
  // range of n elements.
  //
  // The size of the returned range is given by falling_factorial(n, k). This 
  // operation is in the math library.
  template <typename R, typename Comp = less_relation>
    inline auto partial_permutations(R& range, Size_type<R> k, Comp comp = {})
      -> permutation_range<R, next_permutation_func, Comp> 
    {
      assert(k < size(range));
      return {range, next(begin(range), k), next_permutation_func{}, comp};
    }


  // Combinations
  //
  // Return a range over the k first lexicographically sorted combinations of a 
  // range of n elements.
  //
  // If the elements in r are unique (i.e., r is a set), the size of the 
  // returned range is given by binomial_coefficient(n, k). This operation is 
  // found in the math library. 
  //
  // If the elements are not unique (i.e., r is a multiset), the size of the 
  // returned range is given by multinomial_coefficient(n, k).
  template <typename R, typename Comp = less_relation>
    inline auto combinations(R& range, Size_type<R> k, Comp comp = {})
      -> permutation_range<R, next_combination_func, Comp>
    {
      assert(k <= size(range));
      return {range, next(std::begin(range), k), next_combination_func{}, comp};
    }

} // namespace origin

#endif
