// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{
  // TODO: Better names would be nice.



  // Pre-increment result (property)
  // The pre-increment operator returns a reference to its operand.
  template <typename I>
    struct test_pre_increment_result
    {
      static_assert(Weakly_incrementable<I>(), "");

      bool operator()(I first, I last) const
      {
        return first != last ? &++first == &first : true;
      }
    };


  
  // Post-increment result (property)
  // The post-increment operator returns an iterator pointing to the previous
  // element.
  template <typename I>
    struct test_post_increment_result
    {
      static_assert(Incrementable<I>(), "");

      bool operator()(I first, I last) const
      {
        I i = first, j = first;
        return i++ == j;
      }
    };


  
  // Pre- and post-increment (property)
  // The pre- and post-increment operations are related.
  template <typename I>
    struct test_pre_post_increment_equivalence
    {
      static_assert(Forward_iterator<I>(), "");

      bool operator()(I first, I last) const
      {
        I i = first, j = first;
        return (++i, i) == j++;
      }
    };



  // Pre-decrement result (property)
  // The pre-decrement operator returns a reference to its operand.
  template <typename I>
    struct test_pre_decrement_result
    {
      static_assert(Decrementable<I>(), "");

      bool operator()(I first, I last) const
      {
        return first != last ? &--first == &first : true;
      }
    };


  
  // Post-decrement result (property)
  // The post-decrement operator returns an iterator pointing to the previous
  // element.
  template <typename I>
    struct test_post_decrement_result
    {
      static_assert(Decrementable<I>(), "");

      bool operator()(I first, I last) const
      {
        I i = first, j = first;
        return i-- == j;
      }
    };


  
  // Pre- and post-decrement (property)
  // The pre- and post-decrement operations are related.
  template <typename I>
    struct test_pre_post_decrement_equivalence
    {
      static_assert(Decrementable<I>(), "");

      bool operator()(I first, I last) const
      {
        I i = first, j = first;
        return (--i, i) == j--;
      }
    };



  // Increment/decrement identity
  // The increment and decrement operations are opposite operations.
  template <typename I>
      struct increment_decrement_identity
      {
        static_assert(Decrementable<I>(), "");

        bool operator()(I first, I last) const
        {
          I i = first, j = first;
          return first != last ? (--(++i) == j) && (++(--i) == j) : true;
        }
      };



  // Subscript equivalence (property)
  // The subsecript operation is canonically definined in terms of iterator
  // arithmetic.
  template <typename I>
    struct subscript_equivalence
    {
      static_assert(Random_access_iterator<I>(), "");

      bool operator()(I first, Distance_type<I> n) const
      {
        return is_readable_range(first, n) 
            ? (&first[n] == *(first + n)) : true;
      }
    };

} // namespace origin
