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
  template <typename T, typename U = T>
    struct equality_comparable_semantics
    {
      static_assert(Equality_comparable<T, U>(), "");
      using C = Common_type<T, U>;

      // Randomly test the specification using values of type T varerated from
      // var1 and values of type U varerated from var2.
      template <typename Env, typename Var1, typename Var2>
        void operator()(Env& env, Var1&& var1, Var2&& var2) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          auto var3 = make_random<C>(var1.engine());
          (*this)(env, var1, var2, var3);
        }

        
      // Randomly test the specification using values of type T varerated
      // from var1, values of type U varerated from var2, and values of type
      // C (the common type) varerated from var3.
      template <typename Env, typename Var1, typename Var2, typename Var3>
        void operator()(Env& env, Var1&& var1, Var2&& var2, Var3&& var3) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          static_assert(Same<C, Result_type<Forwarded<Var3>>>(), "");

          check(env, equality_comparable_semantics<T>{}, var1);
          check(env, equality_comparable_semantics<U>{}, var2);
          check(env, equality_comparable_semantics<C>{}, var3);

          check(env, eq, var1, var2);  // a == b <=> C(a) == C(b)
          check(env, neq, var1, var2); // a != b <=> C(a) != C(b)
        }

      common_type_equivalence<origin::eq> eq;
      common_type_equivalence<origin::neq> neq;
    };
    
  // Specialization when testing for a single type.
  template <typename T>
    struct equality_comparable_semantics<T, T>
    {
      static_assert(Equality_comparable<T>(), "");
      
      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var>>>(), "");
          check(env, equivalence, var);
          check(env, not_equal, var, var);
        }

      equivalence_relation_spec<eq> equivalence;
      logical_equivalence<neq, complement<eq>> not_equal;
    };



  // Check if the quantified variable is equality comparable.
  template <typename Env, typename Var>
    void check_equality_comparable(Env& env, Var&& var)
    {
      using T = Result_type<Forwarded<Var>>;
      equality_comparable_semantics<T> spec;
      check(env, spec, var);
    }



  // Check if quantified variables satisfy the requirements of cross-type 
  // equality comparability.
  template <typename Env, typename Var1, typename Var2>
    void check_equality_comparable(Env& env, Var1&& var1, Var2&& var2)
    {
      using T = Result_type<Forwarded<Var1>>;
      using U = Result_type<Forwarded<Var2>>;
      equality_comparable_semantics<T, U> spec;
      check(env, spec, var1, var2);
    }



  // Check if T is equality comparable.
  template <typename T, typename Env>
    void check_equality_comparable(Env& env)
    {
      auto var = checkable_var<T>(env);
      check_equality_comparable(env, var);
    }



  // Check if T and U are cross-type equality comparable.
  template <typename T, typename U, typename Env>
    void check_equality_comparable(Env& env)
    {
      auto var1 = checkable_var<T>(env);
      auto var2 = checkable_var<U>(env);
      check_equality_comparable(env, var1, var2);
    }




  // Weakly ordered semantics (specification)
  // The weak ordering specification defines the semantics for weakly ordered
  // types.
  template <typename T, typename U = T>
    struct weakly_ordered_semantics
    {
      static_assert(Weakly_ordered<T, U>(), "");
      using C = Common_type<T, U>;

      template <typename Env, typename Var1, typename Var2>
        void operator()(Env& env, Var1&& var1, Var2&& var2) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          auto var3 = make_random<C>(var1.engine());
          (*this)(env, var1, var2, var3);
        }
      
      template <typename Env, typename Var1, typename Var2, typename Var3>
        void operator()(Env& env, Var1&& var1, Var2&& var2, Var3&& var3) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          static_assert(Same<C, Result_type<Forwarded<Var3>>>(), "");
          
          check(env, weakly_ordered_semantics<T>{}, var1);
          check(env, weakly_ordered_semantics<U>{}, var2);
          check(env, weakly_ordered_semantics<C>{}, var3);
          
          check(env, lt, var1, var2);  // a < b <=> C(a) < C(b)
          check(env, gt, var1, var2);  // a > b <=> C(a) > C(b)
          check(env, leq, var1, var2); // a <= b <=> C(a) <= C(b)
          check(env, geq, var1, var2); // b >= a <=> C(a) >= C(b)
        }
        
      common_type_equivalence<origin::lt> lt;
      common_type_equivalence<origin::gt> gt;
      common_type_equivalence<origin::leq> leq;
      common_type_equivalence<origin::geq> geq;
    };
    
  // Specialization for the unary type.
  template <typename T>
    struct weakly_ordered_semantics<T, T>
    {
      static_assert(Weakly_ordered<T>(), "");

      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var>>>(), "");
          check(env, lt, var);
          check(env, gt, var, var);
          check(env, leq, var, var);
          check(env, geq, var, var);
        }
        
      strict_weak_order_spec<origin::lt> lt;
      logical_equivalence<origin::gt, converse<origin::lt>> gt;
      logical_equivalence<origin::leq, complement_of_converse<origin::lt>> leq;
      logical_equivalence<origin::geq, complement<origin::lt>> geq;
    };



  // Check if the quantified variable is weakly ordered.
  template <typename Env, typename Var>
    void check_weakly_ordered(Env& env, Var&& var)
    {
      using T = Result_type<Forwarded<Var>>;
      weakly_ordered_semantics<T> spec;
      check(env, spec, var);
    }



  // Check if the quantified variables satisfy the requirements of cross-type
  // weak ordering.
  template <typename Env, typename Var1, typename Var2>
    void check_weakly_ordered(Env& env, Var1&& var1, Var2&& var2)
    {
      using T = Result_type<Forwarded<Var1>>;
      using U = Result_type<Forwarded<Var2>>;
      weakly_ordered_semantics<T, U> spec;
      check(env, spec, var1, var2);
    }



  // Check if T is weakly ordered.
  template <typename T, typename Env>
    void check_weakly_ordered(Env& env)
    {
      auto var = checkable_var<T>(env);
      check_weakly_ordered(env, var);
    }



  // Check if T and U satisfy the requirements of cross-type weak ordering.
  template <typename T, typename U, typename Env>
    void check_weakly_ordered(Env& env)
    {
      auto var1 = checkable_var<T>(env);
      auto var2 = checkable_var<U>(env);
      check_weakly_ordered(env, var1, var2);
    }



  // Totally ordered (specification)
  // The total ordering specification defines the semantics for totally ordered
  // types. When used with heterovareous types, the semantics are defined in
  // terms of the common type.
  template <typename T, typename U = T>
    struct totally_ordered_semantics
    {
      static_assert(Totally_ordered<T, U>(), "");
      using C = Common_type<T, U>;

      template <typename Env, typename Var1, typename Var2>
        void operator()(Env& env, Var1&& var1, Var2&& var2) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          auto var3 = make_random<C>(var1.engine());
          (*this)(env, var1, var2, var3);
        }
      
      template <typename Env, typename Var1, typename Var2, typename Var3>
        void operator()(Env& env, Var1&& var1, Var2&& var2, Var3&& var3) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Var2>>>(), "");
          static_assert(Same<C, Result_type<Forwarded<Var3>>>(), "");
          
          check(env, totally_ordered_semantics<T>{}, var1);
          check(env, totally_ordered_semantics<U>{}, var2);
          check(env, totally_ordered_semantics<C>{}, var3);

          check(env, weak, var1, var2, var3);
          check(env, equal, var1, var2);
        }
        
      weakly_ordered_semantics<T, U> weak;
      logical_equivalence<eq, symmetric_complement<lt>> equal;
    };
    
  // Specialization for the unary type.
  template <typename T>
    struct totally_ordered_semantics<T, T>
    {
      static_assert(Totally_ordered<T>(), "");

      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          static_assert(Same<T, Result_type<Forwarded<Var>>>(), "");
          check(env, lt, var);
          check(env, gt, var, var);
          check(env, leq, var, var);
          check(env, geq, var, var);
        }
        
      strict_total_order_spec<origin::lt> lt;
      logical_equivalence<origin::gt, converse<origin::lt>> gt;
      logical_equivalence<origin::leq, complement_of_converse<origin::lt>> leq;
      logical_equivalence<origin::geq, complement<origin::lt>> geq;
    };
    


  // Check if the quantified variable is totally ordered.
  template <typename Env, typename Var>
    void check_totally_ordered(Env& env, Var&& var)
    {
      using T = Result_type<Forwarded<Var>>;
      totally_ordered_semantics<T> spec;
      check(env, spec, var);
    }

    

  // Check if the quantified variables satisfy the requirements of cross-type
  // total ordering.
  template <typename Env, typename Var1, typename Var2>
    void check_totally_ordered(Env& env, Var1&& var1, Var2&& var2)
    {
      using T = Result_type<Forwarded<Var1>>;
      using U = Result_type<Forwarded<Var2>>;
      totally_ordered_semantics<T, U> spec;
      check(env, spec, var1, var2);
    }



  // Check if T is totally ordered.
  template <typename T, typename Env>
    void check_totally_ordered(Env& env)
    {
      auto var = checkable_var<T>(env);
      check_totally_ordered(env, var);
    }



  // Check if T and U satisfy the requirements for cross-type ordering.
  template <typename T, typename U, typename Env>
    void check_totally_ordered(Env& env)
    {
      auto var1 = checkable_var<T>(env);
      auto var2 = checkable_var<U>(env);
      check_totally_ordered(env, var1, var2);
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
  template <typename T>
    struct copy_construction_preservation
    {
      static_assert(Equality_comparable<T>(), "");

      bool operator()(const T& a) const
      {
        static_assert(Copy_constructible<T>(), "");
        return T{a} == a;
      }
    };
    
    
  
  // Copy assignment preservation (property)
  // The result of copy-assigning a value is that the assigned variable is
  // equal to the unmodified original.
  template <typename T>
    struct copy_assignment_preservation
    {
      static_assert(Equality_comparable<T>(), "");
      
      bool operator()(const T& a)
      {
        static_assert(Copy_assignable<T>(), "");
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
      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var)
        {
          check(env, move, var);
          check(env, construct, var);
          check(env, assign, var);
        }

      move_semantics<T> move;
      copy_construction_preservation<T> construct;
      copy_assignment_preservation<T> assign;
    };



  // Check if the quantified variable is copyable.
  template <typename Env, typename Var>
    void check_copyable(Env& env, Var&& var)
    {
      using T = Result_type<Forwarded<Var>>;
      copy_semantics<T> spec;
      check(env, spec, var);
    }



  // Check if T is regular.
  template <typename T, typename Env>
    void check_copyable(Env& env)
    {
      auto var = checkable_var<T>(env);
      check_copyable(env, var);
    }


  
  // Default value (property)
  // The default value property is true when T has a unique default value, that
  // is initialized by expression T{}.
  template <typename T>
    struct default_value_property
    {
      bool operator()() const
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
          check(env, init);
        }
        
      default_value_property<T> init;
    };
    
    

  // Check if T is default constructible.
  template <typename Env, typename T>
    void check_default_constructible(Env& env)
    {
      check(env, default_semantics<T>{});
    }



  // Regular semantics (specification)
  // A regular type is required to implement default initializtaion, copy,
  // and 
  template <typename T>
    struct regular_semantics
    {
      template <typename Env, typename Var>
        void operator()(Env&& env, Var&& var) const
        {
          check(env, def);
          check(env, copy, var);
          check(env, equal, var);
        }
      
      default_semantics<T> def;
      copy_semantics<T> copy;
      equality_comparable_semantics<T> equal;
    };
    


  // Check if the quantified variable is regular.
  template <typename Env, typename Var>
    void check_regular(Env& env, Var&& var)
    {
      using T = Result_type<Forwarded<Var>>;
      regular_semantics<T> spec;
      check(env, spec, var);
    }


  // Check if T is regular.
  template <typename T, typename Env>
    void check_regular(Env& env)
    {
      auto var = checkable_var<T>(env);
      check_regular(env, var);
    }



  // TODO: We do not have semantics for functions because most functions are
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
  template <typename F>
    struct equality_preserving
    {
      equality_preserving(F f = {}) : fn(f) { }

      template <typename... Args>
        auto operator()(const std::tuple<Args...>& args1, 
                        const std::tuple<Args...>& args2) const
          -> Requires<Regular_function<F, Args...>(), bool>
        {
          return tuple_invoke(fn, args1) == tuple_invoke(fn, args2);
        }

      F fn;
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
    struct regular_function_semantics
    {
      regular_function_semantics(F f = {}) : regular(f) { }

      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          check(env, regular, var, var);
        }

      equality_preserving<F> regular;
    };



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


} // namespace origin
