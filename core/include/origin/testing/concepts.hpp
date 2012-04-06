// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{
  // Equality comparable (specification)
  // The equality comparable test can be used to evaluate the semantics of the
  // equality comparison relation. That is, the == operator must satisfy
  // the requirements of an equivalence relation, and the != operator must
  // be the negation of the equality operator.
  //
  // In actuality, the == operator must compare for value equality, but we have
  // no way to actually test that.
  template <typename T1, typename T2 = T1>
    struct equality_comparable_spec
    {
      static_assert(Equality_comparable<T1, T2>(), "");
      using C = Common_type<T1, T2>;

      template <typename Env, typename T1gen, typename T2gen>
        void operator()(Env& env, T1gen&& a, T2gen&& b) const
        {
          auto c = checkable_var<C>(env);
          (*this)(env, a, b, c);
        }
        
      template <typename Env, typename T1gen, typename T2gen, typename Cgen>
        void operator()(Env& env, T1gen&& a, T2gen&& b, Cgen&& c) const
        {
          auto equal = checkable_func(env, eq());
          auto not_equal = checkable_func(env, neq());

          // T1, T2, and C must be euqality comparable.
          check(env, equality_comparable_spec<T1>{}, a);
          check(env, equality_comparable_spec<T2>{}, b);
          check(env, equality_comparable_spec<C>{}, c);

          // Expressions involving both types are equivalent to expressions
          // involving the common type.
          check(env, common_type_equivalence {}, equal, a, b);
          check(env, common_type_equivalence {}, not_equal, a, b);
        }
    };
    
  // Specialization when testing for a single type.
  template <typename T>
    struct equality_comparable_spec<T, T>
    {
      static_assert(Equality_comparable<T>(), "");
      
      template <typename Env, typename Tgen>
        void operator()(Env& env, Tgen&& value) const
        {
          auto equal = checkable_func(env, eq());
          auto not_equal = checkable_func(env, neq());
          auto comp_equal = checkable_func(env, complement(eq()));

          check(env, equivalence_relation_spec {}, equal, value);
          check(env, logical_equivalence {}, not_equal, comp_equal, value, value);
        }
    };

  // Check if T is equality comparable.
  template <typename T, typename Env>
    void check_equality_comparable(Env& env)
    {
      auto value = checkable_var<T>(env);
      check(env, equality_comparable_spec<T> {}, value);
    }

  // Check if T and U are cross-type equality comparable.
  template <typename T1, typename T2, typename Env>
    void check_equality_comparable(Env& env)
    {
      auto a = checkable_var<T1>(env);
      auto b = checkable_var<T2>(env);
      check(env, equality_comparable_spec<T1, T2> {}, a, b);
    }



  // Weakly ordered semantics (specification)
  // The weak ordering specification defines the semantics for weakly ordered
  // types.
  template <typename T1, typename T2 = T1>
    struct weakly_ordered_spec
    {
      static_assert(Weakly_ordered<T1, T2>(), "");
      using C = Common_type<T1, T2>;

      template <typename Env, typename T1gen, typename T2gen>
        void operator()(Env& env, T1gen&& a, T2gen&& b) const
        {
          auto c = checkable_var<C>(env);
          (*this)(env, a, b, c);
        }
      
      template <typename Env, typename T1gen, typename T2gen, typename Cgen>
        void operator()(Env& env, T1gen&& a, T2gen&& b, Cgen&& c) const
        {
          // Each of T1, T2, and C must be weakly ordered.
          check(env, weakly_ordered_spec<T1> {}, a);
          check(env, weakly_ordered_spec<T2> {}, b);
          check(env, weakly_ordered_spec<C> {}, c);
          

          auto less = checkable_func(env, lt());
          auto greater = checkable_func(env, gt());
          auto less_equal = checkable_func(env, lte());
          auto greater_equal = checkable_func(env, gte());

          // Expressions in both types are equivalent to expressions in the
          // common type.
          check(env, common_type_equivalence {}, less, a, b);
          check(env, common_type_equivalence {}, greater, a, b);
          check(env, common_type_equivalence {}, less_equal, a, b);
          check(env, common_type_equivalence {}, greater_equal, a, b);
        }
    };
    
  // Specialization for the unary type.
  template <typename T>
    struct weakly_ordered_spec<T, T>
    {
      static_assert(Weakly_ordered<T>(), "");

      template <typename Env, typename Tgen>
        void operator()(Env& env, Tgen&& value) const
        {
          auto less = checkable_func(env, lt());
          auto greater = checkable_func(env, gt());
          auto less_eq = checkable_func(env, lte());
          auto greater_eq = checkable_func(env, gte());

          auto conv_less = checkable_func(env, converse(lt()));
          auto comp_conv_less = checkable_func(env, complement_of_converse(lt()));
          auto comp_less = checkable_func(env, complement(lt()));

          check(env, strict_weak_order_spec {}, less, value);
          check(env, logical_equivalence {}, greater, conv_less, value, value);
          check(env, logical_equivalence {}, less_eq, comp_conv_less, value, value);
          check(env, logical_equivalence {}, greater_eq, comp_less, value, value);
        }
    };


  // Check if T is weakly ordered.
  template <typename T, typename Env>
    void check_weakly_ordered(Env& env)
    {
      auto value = checkable_var<T>(env);
      check(env, weakly_ordered_spec<T> {}, value);
    }

  // Check if T and U satisfy the requirements of cross-type weak ordering.
  template <typename T1, typename T2, typename Env>
    void check_weakly_ordered(Env& env)
    {
      auto a = checkable_var<T1>(env);
      auto b = checkable_var<T2>(env);
      check(env, weakly_ordered_spec<T1, T2>{}, a, b);
    }


  // Totally ordered (specification)
  // The total ordering specification defines the semantics for totally ordered
  // types. When used with heterovareous types, the semantics are defined in
  // terms of the common type.
  template <typename T1, typename T2 = T1>
    struct totally_ordered_spec
    {
      static_assert(Totally_ordered<T1, T2>(), "");
      using C = Common_type<T1, T2>;

      template <typename Env, typename T1gen, typename T2gen>
        void operator()(Env& env, T1gen&& a, T2gen&& b) const
        {
          auto c = checkable_var<C>(env);
          (*this)(env, a, b, c);
        }
      
      template <typename Env, typename T1gen, typename T2gen, typename T3gen>
        void operator()(Env& env, T1gen&& a, T2gen&& b, T3gen&& c) const
        {
          // Each of T1, T2, and C must be totally ordered.
          check(env, totally_ordered_spec<T1> {}, a);
          check(env, totally_ordered_spec<T2> {}, b);
          check(env, totally_ordered_spec<C> {}, c);

          // Note that the semantics of weak ordering cover the common type
          // equivalences required by this cross-type concept.
        }
    };
    
  // Specialization for the unary type.
  template <typename T>
    struct totally_ordered_spec<T, T>
    {
      static_assert(Totally_ordered<T>(), "");

      template <typename Env, typename Tgen>
        void operator()(Env& env, Tgen&& value) const
        {
          check(env, weakly_ordered_spec<T> {}, value);
          
          // Incomparability is the same as equality.
          auto equal = checkable_func(env, eq());
          auto incomp = checkable_func(env, symmetric_complement(lt()));
          check(env, logical_equivalence {}, equal, incomp, value, value);
        }
    };
    
  // Check if T is totally ordered.
  template <typename T, typename Env>
    void check_totally_ordered(Env& env)
    {
      auto value = checkable_var<T>(env);
      check(env, totally_ordered_spec<T> {}, value);
    }

  // Check if T and U satisfy the requirements for cross-type ordering.
  template <typename T1, typename T2, typename Env>
    void check_totally_ordered(Env& env)
    {
      auto a = checkable_var<T1>(env);
      auto b = checkable_var<T2>(env);
      check(env, totally_ordered_spec<T1, T2> {}, a, b);
    }



  // Move semantics (specification)
  //
  // TODO: Implement tests for move semantics.
  template <typename T>
    struct move_semantics
    {
      template <typename Env, typename Var>
        void operator()(Env&&, Var&&) { }
    };


    
  // Copy construction preservation (property)
  // The result of a copy construction is a new value that is equal to the
  // unmodified original.
    struct copy_construction_preservation
    {
      template <typename T>
        auto operator()(const T& a) const 
          -> Requires<Copy_constructible<T>() && Equality_comparable<T>(), bool>
        {
          return T{a} == a;
        }
    };
    
    
  
  // Copy assignment preservation (property)
  // The result of copy-assigning a value is that the assigned variable is
  // equal to the unmodified original.
  struct copy_assignment_preservation
  {
    template <typename T>
      auto operator()(const T& a)
        -> Requires<Copy_constructible<T>() && Equality_comparable<T>(), bool>
      {
        T b = a;
        return b == a;
      }
  };

    
    
  // Copy semantics (specification)
  // A type T implements copy semantics, if for all values of T a copy
  // compares equal to its original.
  template <typename T>
    struct copy_semantics
    {
      template <typename Env, typename Tgen>
        void operator()(Env& env, Tgen&& value)
        {
          check(env, move_semantics<T> {}, value);
          check(env, copy_construction_preservation {}, value);
          check(env, copy_assignment_preservation {}, value);
        }
    };

  // Check if the quantified variable is copyable.
  template <typename T, typename Env>
    void check_copyable(Env& env)
    {
      auto value = checkable_var<T>(env);
      check(env, copy_semantics<T> {}, value);
    }


  
  // Default value (property)
  // The default value property is true when T has a unique default value, that
  // is initialized by expression T{}.
  //
  // NOTE: Unfortunately, we have to parameterize the algorithm over a dummy
  // argument so we can constrain the overload, and avoid ambiguous lookups.
  // If only I had real constraints...
  struct default_value_property
  {
    template <typename T>
      auto operator()(const T&) const 
        -> Requires<Default_constructible<T>() && Equality_comparable<T>(), bool>
      { 
        return T{} == T{}; 
      }
  };



  // Default semantics (specification)
  // The default initialization semantics require a default initialized type
  // to have a unique default value.
  template <typename T>
    struct default_semantics
    {
      template <typename Env>
        void operator()(Env&& env) const
        {
          check(env, default_value_property {}, fake<T>());
        }
    };

    

  // Check if T is default constructible.
  template <typename Env, typename T>
    void check_default_constructible(Env& env)
    {
      check(env, default_semantics<T> {});
    }



  // Regular semantics (specification)
  // A regular type is required to implement default initializtaion, copy,
  // and 
  template <typename T>
    struct regular_spec
    {
      template <typename Env, typename Tgen>
        void operator()(Env&& env, Tgen&& value) const
        {
          check(env, default_semantics<T> {});
          check(env, copy_semantics<T> {}, value);
          check(env, equality_comparable_spec<T> {}, value);
        }
    };
    
  // Check if T is regular.
  template <typename T, typename Env>
    void check_regular(Env& env)
    {
      auto value = checkable_var<T>(env);
      check(env, regular_spec<T> {}, value);
    }



  // FIXME: We do not have semantics for functions because most functions are
  // not equality comparable -- and they're kind of weird besides. I can't
  // really generate random function objects. It would be nice to assert
  // these properties, but I'm not entirely sure how yet.



  // Equality preserving (property)
  // A function is equality preserving if equal inputs yield equal outputs,
  // for all posssible inputs to the function, including the implicit program
  // state. In other words, given the same arguments, the function always
  // gives the same results.
  //
  // Note that arguments to this test must be given as tuples.
  struct equality_preserving
  {
    template <typename F, typename... Args>
      auto operator()(F f, 
                      const std::tuple<Args...>& args1, 
                      const std::tuple<Args...>& args2) const
        -> Requires<Regular_function<F, Args...>(), bool>
      {
        return tuple_invoke(f, args1) == tuple_invoke(f, args2);
      }
  };



  // Regular function semantics (specification)
  // A regular function is equality preserving: equal inputs yield equal
  // outputs. 
  //
  // Note that the random variable over which the test is quantified must be a 
  // tuple generator of the specified argument types.
  //
  // This class does not evaluate the copy semantics of functions since 
  // functions are not generally equality comparable (i.e., we can't check
  // the property.
  template <typename F>
    struct regular_function_spec
    {
      // Note that Args_gen must generate a tuple of function arguments.
      template <typename Env, typename Func_gen, typename Arg_gen>
        void operator()(Env& env, Func_gen&& f, Arg_gen&& args) const
        {
          check(env, equality_preserving {}, f, args, args);
        }
    };


    /*
  // The signature for checking the semantics of regular functions is a bit 
  // different than the usual approach.
  //
  // check_regular_function<F(Args...)>(enf, f)
  //
  // where f is the function object or pointer under test and F(Args...) is
  // its function type. Note that if f is a function pointer, then decltype(f)
  // will yield a type expression like F(Args...).
  //
  // TODO: I could extend this for check(env, f, vars...) where vars are
  // independent distributions.
  template <typename Sig, typename Env, typename F>
    inline void check_regular_function(Env& env, F f)
    {
      static_assert(Tuple_callable<F, Argument_types<Sig>>(), "");

      auto var = checkable_var<Argument_types<Sig>>(env);
      regular_function_semantics<F> spec{f};
      check(env, spec, var);
    }



  template <typename Sig, typename Env, typename P>
    inline void check_predicate(Env& env, P pred)
    {
      static_assert(Tuple_callable<P, Argument_types<Sig>>(), "");
      check_regular_function<Sig>(env, pred);
    }



  template <typename Sig, typename Env, typename R>
    inline void check_relation(Env& env, R comp)
    {
      static_assert(Tuple_callable<R, Argument_types<Sig>>(), "");
      check_regular_function<Sig>(env, comp);
    }
  */
} // namespace origin

