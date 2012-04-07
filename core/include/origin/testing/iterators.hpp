// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{

  // Compare readable
  // This compares two read values for equality. The primary use of this
  // function is to evaluate the semantics of Readable and Writable types, 
  // which require that we can form a constant reference to an iterator, and
  // possibly compare values for equality.
  template <typename I>
    inline auto compare_readable(const Value_type<I>& a, const Value_type<I>& b)
      -> Requires<Readable<I>() && Equality_comparable<Value_type<I>>(), bool>
    {
      return a == b;
    }

  template <typename I>
    inline auto compare_readable(const Value_type<I>& a, const Value_type<I>& b)
     -> Requires<Readable<I>() && !Equality_comparable<Value_type<I>>(), bool>
    {
      return true;
    }

  template <typename I, typename T>
    inline auto compare_readable(const T& a, const T& b)
      -> Requires<!Readable<I>(), bool>
    {
      return true;
    }



  // Value readable (property)
  struct value_readable_property
  {
    template <typename I>
      auto operator()(I first, I last) const
        -> Requires<Readable<I>(), bool>
      {
        if (first == last)
          return true;

        return compare_readable<I>(*first, *first);
      }

    template <typename R>
      auto operator()(const R& range) const
        -> Requires<Input_range<R>(), bool>
      {
        return (*this)(begin(range), end(range));
      }
  };



  // Value writable (property)
  struct value_writable_property
  {
    template <typename I, typename T>
      auto operator()(I first, I last, const T& value) const
        -> Requires<Writable<I, T>(), bool>
      {
        if (first == last)
          return true;

        *first = value;
        return compare_readable<I>(*first, value);
      }

    template <typename R, typename T>
      auto operator()(R&& range, const T& value) const
        -> Requires<Output_range<Forwarded<R>, T>(), bool>
      {
        return (*this)(begin(range), end(range), value);
      }
  };



  // FIXME: Implement testing for value-moving semantics.



  // Pre-increment result (property)
  // The pre-increment operator returns a reference to its operand.
  struct pre_increment_identity
  {
    template <typename I>
      auto operator()(I first, I last) const
        -> Requires<Weakly_incrementable<I>(), bool>
      {
        if (first == last)
          return true;

        return &++first == &first;
      }

    template <typename R>
      auto operator()(const R& range) const
        -> Requires<Range<R>(), bool>
      {
        return (*this)(begin(range), end(range));
      }
  };



  // Pre-increment result (property)
  // The post-increment operator returns an iterator pointing to the previous
  // element.
  //
  // Note that this is only testable for incrementable types. Weakly 
  // incrementable types cannot support this property since only one value at
  // iterator at a time can be in play.
  struct post_increment_result
  {
    template <typename I>
      auto operator()(I first, I last) const
        -> Requires<Incrementable<I>(), bool>
      {
        if (first == last)
          return true;

        I i = first, j = first;
        return i++ == j;
      }

    template <typename R>
      auto operator()(const R& range) const
        -> Requires<Forward_range<R>(), bool>
      {
        return (*this)(begin(range), end(range));
      }
  };



  // Pre- and post-increment (property)
  // The pre- and post-increment operations are related.
  struct post_increment_effect
  {
    template <typename I>
      auto operator()(I first, I last) const
        -> Requires<Forward_iterator<I>(), bool>
      {
        if (first == last)
          return true;

        I i = first, j = first;
        return (i++, i) == ++j;
      }

    template <typename R>
      auto operator()(const R& range) const
        -> Requires<Forward_range<R>(), bool>
      {
        return (*this)(begin(range), end(range));
      }
  };




  // Pre-decrement result (property)
  // The pre-decrement operator returns a reference to its operand.
  struct pre_decrement_identity
  {
    template <typename I>
      auto operator()(I first, I last) const
        -> Requires<Bidirectional_iterator<I>(), bool>
      {
        if (first == last)
          return true;

        return &--first == &first;
      }

    template <typename R>
      auto operator()(const R& range) const
        -> Requires<Bidirectional_range<R>(), bool>
      {
        return (*this)(begin(range), end(range));
      }
    };


  
  // Post-decrement result (property)
  // The post-decrement operator returns an iterator pointing to the previous
  // element.
  struct post_decrement_result
  {
    template <typename I>
      auto operator()(I first, I last) const
        -> Requires<Bidirectional_iterator<I>(), bool>
      {
        if (first == last)
          return true;

        I i = first, j = first;
        return i-- == j;
      }

    template <typename R>
      auto operator()(const R& range) const
        -> Requires<Bidirectional_range<R>(), bool>
      {
        return (*this)(begin(range), end(range));
      }
  };


  
  // Pre- and post-decrement (property)
  // The pre- and post-decrement operations are related.
  struct post_decrement_effect
  {
    template <typename I>
      auto operator()(I first, I last) const
        -> Requires<Bidirectional_iterator<I>(), bool>
      {
        if (first == last)
          return true;

        I i = first, j = first;
        return (i--, i) == --j;
      }

    template <typename R>
      auto operator()(const R& range) const
        -> Requires<Bidirectional_range<R>(), bool>
      {
        return (*this)(begin(range), end(range));
      }
  };



  // Increment/decrement identity
  // The increment and decrement operations are opposite operations.
  struct increment_decrement_equivalence
  {
    template <typename I>
      auto operator()(I first, I last) const
        -> Requires<Bidirectional_iterator<I>(), bool>
      {
        if (first == last)
          return true;

        I i = first, j = first;
        return (--(++i) == j) && (++(--i) == j);
      }

    template <typename R>
      auto operator()(const R& range) const
        -> Requires<Bidirectional_range<R>(), bool>
      {
        return (*this)(begin(range), end(range));
      }
  };



  // Random access assign check (property)
  struct random_access_advance_identity
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, n, last))
          return true;

        return &(first += n) == &first;
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  struct random_access_advance_result
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, n, last))
          return true;

        I i = first, j = first;
        return (i += n) == (advance(j, n), j);
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  struct random_access_next_result
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, n, last))
          return true;

        return first + n == next(first, n);
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  struct random_access_retreat_identity
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, -n, last))
          return true;

        return &(last -= n) == &last;
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  struct random_access_retreat_result
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, -n, last))
          return true;

        I i = first, j = first;
        return (i += n) == (advance(j, n), j);
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  struct random_access_prev_result
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, -n, last))
          return true;

        return last - n == prev(last, n);
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  struct random_access_commutativity
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, n, last))
          return true;

        return first + n == n + first;
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  struct random_access_associativity
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, 2 * n, last))
          return true;

        return first + (n + n) == (first + n) + n;
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  struct random_access_peano
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (exceeds_limits(first, n, last))
          return true;

        return (first + 0 == first)
            && (first + n == ++(first + (n - 1)));
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  // Subscript equivalence (property)
  // The subsecript operation is canonically definined in terms of iterator
  // arithmetic.
  struct subscript_equivalence
  {
    template <typename I>
      auto operator()(I first, I last, Distance_type<I> n) const
        -> Requires<Random_access_iterator<I>(), bool>
      {
        if (iterative_bounded_next(first, n, last) == last)
          return true;

        return &first[n] == &*(first + n);
      }

    template <typename R>
      auto operator()(const R& range, Distance_type<R> n) const
        -> Requires<Random_access_range<R>(), bool>
      {
        return (*this)(begin(range), end(range), n);
      }
  };



  // Specifications
  // The following specifications develop structured tests for the properties
  // above. Iterator concepts have to be tested using ranges, to ensure that
  // we (mostly) get valid input ranges.
  //
  // FIXME: It would probably be a good idea to check that the generators
  // generated values of the correct type.


  // Input range (specification)
  template <typename I>
    struct input_range_spec
    {
      template <typename Env, typename Range_gen>
        void operator()(Env& env, Range_gen&& range)
        {
          check(env, pre_increment_identity {}, range);
          check(env, value_readable_property {}, range);
        }
    };

  template <typename R, typename Env>
    void check_input_range(Env& env)
    {
      auto range = checkable_var<R>(env);
      check(env, input_range_spec<R> {}, range);
    };




  // Output range (specification)
  //
  // FIXME: An output range is also a move output range.
  template <typename O, typename T>
    struct output_range_spec
    {
      template <typename Env, typename Range_gen, typename Value_gen>
        void operator()(Env& env, Range_gen&& range, Value_gen&& value)
        {
          check(env, pre_increment_identity {}, range);
          check(env, value_writable_property {}, range, value);
        }
    };

  template <typename R, typename T, typename Env>
    void check_output_range(Env& env)
    {
      auto range = checkable_var<R>(env);
      auto value = checkable_var<T>(env);
      check(env, output_range_spec<R, T> {}, range, value);
    };



  // Forward range (specification)
  template <typename I>
    struct forward_range_spec
    {
      template <typename Env, typename Range_gen>
        void operator()(Env& env, Range_gen&& range)
        {
          check(env, input_range_spec<I> {}, range);
          check(env, post_increment_result {}, range);
          check(env, post_increment_effect {}, range);
        }
    };

  template <typename R, typename Env>
    void check_forward_range(Env& env)
    {
      auto range = checkable_var<R>(env);
      check(env, forward_range_spec<R> {}, range);
    };



  // Bidirectional ragne (specification)
  template <typename I>
    struct bidirectional_range_spec
    {
      template <typename Env, typename Range_gen>
        void operator()(Env& env, Range_gen&& range)
        {
          check(env, forward_range_spec<I> {}, range);
          check(env, post_decrement_result {}, range);
          check(env, post_decrement_effect {}, range);
          check(env, increment_decrement_equivalence {}, range);
        }
    };

  template <typename R, typename Env>
    void check_bidirectional_range(Env& env)
    {
      auto range = checkable_var<R>(env);
      check(env, bidirectional_range_spec<R> {}, range);
    };



  template <typename I>
    struct random_access_range_spec
    {
      template <typename Env, typename Range_gen>
        void operator()(Env& env, Range_gen&& range)
        {
          using R = Result_type<Forwarded<Range_gen>>;

          // By default, evaluate distances over small values.
          std::uniform_int_distribution<Distance_type<R>> dist {0, 5};
          auto num = checkable_var(env, dist);
          
          check(env, bidirectional_range_spec<I> {}, range);
          
          check(env, random_access_advance_identity {}, range, num);
          check(env, random_access_advance_result {}, range, num);
          check(env, random_access_next_result {}, range, num);
          
          check(env, random_access_retreat_identity {}, range, num);
          check(env, random_access_retreat_result {}, range, num);
          check(env, random_access_prev_result {}, range, num);

          check(env, random_access_commutativity {}, range, num);
          check(env, random_access_associativity {}, range, num);
          check(env, random_access_peano {}, range, num);

          check(env, subscript_equivalence {}, range, num);
        }
    };

  template <typename R, typename Env>
    void check_random_access_range(Env& env)
    {
      auto range = checkable_var<R>(env);
      check(env, random_access_range_spec<R> {}, range);
    };

} // namespace origin
