// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{
  // Logical equivalence (property)
  // The logical equivalence property is true when two relations are true
  // or false for the same inputs.
  struct logical_equivalence
  {
    template <typename R1, typename R2, typename T, typename U>
      auto operator()(R1 r1, R2 r2, const T& a, const U& b) const
        -> Requires<Relation<R1, T>() && Relation<R2, U>(), bool>
      {
        return r1(a, b) == r2(a, b);
      }
  };

    
  
  // Common type equivalence (property)
  // The common type equivalence property says that any expression involving
  // arguments of different types T and U that share a common type C is 
  // equivalent to the same expression with all arguments converted to C.
  //
  // Note that the expressions are checked symmetrically so that, for some
  // expression expr:
  //
  //    expr(a, b) <=> expr(C(a), C(b)) && expr(b, a) <=> expr(C(b), C(a))
  //
  // For all valid arguments a and b, whose common type is C.
    struct common_type_equivalence
    {
      template <typename Expr, 
                typename T, 
                typename U, 
                typename C = Common_type<T, U>>
        auto operator()(Expr expr, const T& a, const U& b) const
          -> decltype(expr(a, b) == expr(C(a), C(b)
                   && expr(b, a) == expr(C(b), C(a))))
        {
          return expr(a, b) == expr(C(a), C(b)) 
              && expr(b, a) == expr(C(b), C(a));
        }
    };

    

  // Reflexive property (property)
  // A relation is reflexive if, for all a, r(a, a) is true.
  struct reflexive_property
  {
   template <typename R, typename T>
      auto operator()(R r, const T& a) const -> Requires<Relation<R, T>(), bool>
      {
        return r(a, a); 
      }
  };



  // Irreflexive property (property)
  // A relation is irreflexive if, for all a, r(a, a) is false.
  struct irreflexive_property
  {
    template <typename R, typename T>
      auto operator()(R r, const T& a) const -> Requires<Relation<R, T>(), bool>
      {
        return !r(a, a); 
      }
  };
 


  // Symmetric property (property)
  // A relation is symmetric if, for all a and b, r(a, b) => r(b, a).
  struct symmetric_property
  {
    template <typename R, typename T>
      auto operator()(R r, const T& a, const T& b) const 
        -> Requires<Relation<R, T>(), bool>
      { 
        return r(a, b) ? r(b, a) : true; 
      }
  };


  // Asymmetric relation (property)
  // A relation is asymmetric if, for all a and b, r(a, b) => !r(b, a)
  struct asymmetric_property
  {
    template <typename R, typename T>
      auto operator()(R r, const T& a, const T& b) const
        -> Requires<Relation<R, T>(), bool>
      {
        return r(a, b) ? !r(b, a) : true;
      }
  };


  
  // Antisymmetric relation (property)
  // A relation is antisymmetric if, for all a and b, 
  // r(a, b) && r(b, a) => a == b.
  struct antisymmetric_property
  {
    template <typename R, typename T>
      auto operator()(R r, const T& a, const T& b) const
        -> Requires<Relation<R, T>() && Equality_comparable<T>(), bool>
      {
        return r(a, b) && r(b, a) ? a == b : true;
      }
  };



  // Transitive property (property)
  // A relation is transitive if, for all a, b, and c, 
  // r(a, b) && r(b, c) => r(a, c).
  struct transitive_property
  {
    template <typename R, typename T>
      auto operator()(R r, const T& a, const T& b, const T& c) const
        -> Requires<Relation<R, T>(), bool>
      {
        return r(a, b) && r(b, c) ? r(a, c) : true;
      }
  };



  // Trichotomous property (property)
  // A relation is trichotomous if, for all a and b, exactly one of the 
  // following are true: a < b, b < a, or a == b.
  struct trichotomous_property
  {
    template <typename R, typename T>
      auto operator()(R r, const T& a, const T& b) const
        -> Requires<Relation<R, T>() && Equality_comparable<T>(), bool>
      {
        if(r(a, b))
          return !(r(b, a) || a == b);
        else if(r(b, a))
          return a != b;
        else
          return a == b;
      }
  };
    
    
    
  // Equivalence relation (specification)
  // An equivalence relation is a relation that is reflexive, symmetric, and 
  // transitive.
  struct equivalence_relation_spec
  {
    template <typename Env, typename R, typename T>
      void operator()(Env& env, R&& r, T&& value) const
      {
        check(env, reflexive_property {}, r, value);
        check(env, symmetric_property {}, r, value, value);
        check(env, transitive_property {}, r, value, value, value);
      }
  };

  // Check if comp satisfies the semantics of an equivalence relation.
  template <typename T, typename Env, typename R>
    void check_equivalence_relation(Env& env, R comp)
    {
      auto rel = checkable_func(env, comp);
      auto value = checkable_var<T>(env);
      check(env, equivalence_relation_spec {}, rel, value);
      
    }



  // Strict partial order (specification)
  // A strict partial order is irreflexive, asymmetric, and transitive.
  struct strict_partial_order_spec
  {
    template <typename Env, typename R, typename T>
      void operator()(Env& env, R&& r, T&& value) const
      {
        check(env, irreflexive_property {}, r, value);
        check(env, asymmetric_property {}, r, value, value);
        check(env, transitive_property {}, r, value, value, value);
      }
  };
    
  // Check if comp satisfies the semantics of a strict partial order.
  template <typename T, typename Env, typename R>
    void check_strict_partial_order(Env& env, R comp)
    {
      auto rel = checkable_func(env, comp);
      auto value = checkable_var<T>(env);
      check(env, strict_partial_order_spec {}, rel, value);
    }



  // Strict weak order (specification)
  // A strict weak order is a strict weak order whose symmetric complement 
  // (a.k.a., incomparability) is transitive. Note that incomparability can 
  // also be  shown to be an equivalence relation.
  struct strict_weak_order_spec
  {
    template <typename Env, typename R, typename T>
      void operator()(Env& env, R&& r, T&& value) const
      {
        using Ord = Result_type<Forwarded<R>>;
        using Eq = Symmetric_complement_of<Ord>;

        auto dist = adapt_distribution<Eq>(r.distribution());
        auto equiv = checkable_var(env, dist);
        
        check(env, strict_partial_order_spec {}, r, value);
        check(env, equivalence_relation_spec {}, equiv, value);
      }
  };

  // Check if comp satisfies the semantics of a strict weak order.
  template <typename T, typename Env, typename R>
    void check_strict_weak_order(Env& env, R comp)
    {
      auto rel = checkable_func(env, comp);
      auto value = checkable_var<T>(env);
      check(env, strict_weak_order_spec {}, rel, value);
    }
 


  // Strict total order (specification)
  // A strict total ordering is antisymmetric, transitive, and trichotomous.
  // Said otherwise, it is a weak order where the equivalence relation defined
  // by incomparability is the same as equality.
  //
  // Note that the value type over which the specification is tested is
  // required to be equality comparable.
    struct strict_total_order_spec
    {
      template <typename Env, typename R, typename T>
        void operator()(Env& env, R&& r, T&& value) const
        {
          check(env, antisymmetric_property {}, r, value, value);
          check(env, transitive_property {}, r, value, value, value);
          check(env, trichotomous_property {}, r, value, value);
        }
    };

  // Check if comp satisfies the requirements of a strict total order.
  template <typename T, typename Env, typename R>
    void check_strict_total_order(Env& env, R comp)
    {
      auto rel = checkable_func(env, comp);
      auto value = checkable_var<T>(env);
      check(env, strict_total_order_spec {}, rel, value);
    }

} // namespace origin
