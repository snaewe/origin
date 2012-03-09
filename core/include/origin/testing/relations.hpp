// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{
  // Logical equivalence (property)
  //
  // The logical equivalence property is true when two relations are true
  // or false for the same inputs.
  template <typename R1, typename R2>
    struct logical_equivalence
    {
      logical_equivalence(R1 r1 = {}, R2 r2 = {}) 
        : r1(r1), r2(r2) 
      { }

      template <typename T, typename U>
        bool operator()(const T& a, const U& b) const
        {
          static_assert(Relation<R1, T>(), "");
          static_assert(Relation<R2, U>(), "");
          return r1(a, b) == r2(a, b);
        }
        
      R1 r1;
      R2 r2;
    };

    
  
  // Common type equivalence (property)
  // The common type equivalence property says that any expression involving
  // arguments of different types T and U that share a common type C is 
  // equivalent to the same expression with all arguments converted to C.
  //
  // Note that the expressions are checked symmetrically so that, for some
  // expression expr,
  //
  //    expr(a, b) <=> expr(C(a), C(b)) && expr(b, a) <=> expr(C(b), C(a))
  //
  // For all valid arguments a and b, whose common type is C.
  //
  // FIXME: Do I need to store Expr as an object?
  template <typename Expr>
    struct common_type_equivalence
    {
      common_type_equivalence(Expr e = {}) : expr(e) { }

      template <typename T, typename U>
        bool operator()(const T& a, const U& b) const
        {
          using C = Common_type<T, U>;
          return expr(a, b) == expr(C(a), C(b)) 
              && expr(b, a) == expr(C(b), C(a));
        }
        
      Expr expr;
    };

    

  // Reflexive property (property)
  // A relation is reflexive if, for all a, r(a, a) is true.
  template <typename R>
    struct reflexive_property
    {
      reflexive_property(R r = {}) : r(r) { }
      
      template <typename T>
        bool operator()(const T& a) const 
        { 
          static_assert(Relation<R, T>(), "");
          return r(a, a); 
        }
      
      R r;
    };



  // Irreflexive property (property)
  // A relation is irreflexive if, for all a, r(a, a) is false.
  template <typename R>
    struct irreflexive_property
    {
      irreflexive_property(R r = {}) : r(r) { }
      
      template <typename T>
        bool operator()(const T& a) const 
        {
          static_assert(Relation<R, T>(), "");
          return !r(a, a); 
        }
      
      R r;
    };
 


  // Symmetric property (property)
  // A relation is symmetric if, for all a and b, r(a, b) => r(b, a).
  template <typename R>
    struct symmetric_property
    {
      symmetric_property(R r = {}) : r(r) { }
    
      template <typename T>
        bool operator()(const T& a, const T& b) const 
        { 
          static_assert(Relation<R, T>(), "");
          return r(a, b) ? r(b, a) : true; 
        }
      
      R r;
    };


  // Asymmetric relation (property)
  // A relation is asymmetric if, for all a and b, r(a, b) => !r(b, a)
  template <typename R>
    struct asymmetric_property
    {
      asymmetric_property(R r = {}) : r(r) { }
    
      template <typename T>
        bool operator()(const T& a, const T& b) const
        {
          static_assert(Relation<R, T>(), "");
          return r(a, b) ? !r(b, a) : true;
        }
      
      R r;
    };


  
  // Antisymmetric relation (property)
  // A relation is antisymmetric if, for all a and b, 
  // r(a, b) && r(b, a) => a == b.
  template <typename R>
    struct antisymmetric_property
    {
      antisymmetric_property(R r = {}) : r(r) { }
      
      template <typename T>
        bool operator()(const T& a, const T& b) const
        {
          static_assert(Relation<R, T>(), "");
          static_assert(Equality_comparable<T>(), "");
          return r(a, b) && r(b, a) ? a == b : true;
        }

      R r;
    };



  // Transitive property (property)
  // A relation is transitive if, for all a, b, and c, 
  // r(a, b) && r(b, c) => r(a, c).
  template <typename R>
    struct transitive_property
    {
      transitive_property(R r = {}) : r(r) { }
      
      template <typename T>
        bool operator()(const T& a, const T& b, const T& c) const
        {
          static_assert(Relation<R, T>(), "");
          return r(a, b) && r(b, c) ? r(a, c) : true;
        }

      R r;
    };
    
  
  
  // Trichotomous property (property)
  // A relation is trichotomous if, for all a and b, exactly one of the 
  // following are true: a < b, b < a, or a == b.
  template <typename R>
    struct trichotomous_property
    {
      trichotomous_property(R r = {}) : r(r) { }
      
      template <typename T>
        bool operator()(const T& a, const T& b) const
        {
          static_assert(Relation<R, T>(), "");
          static_assert(Equality_comparable<T>(), "");

          if(r(a, b))
            return !(r(b, a) || a == b);
          else if(r(b, a))
            return a != b;
          else
            return a == b;
        }
      
      R r;
    };
    
    
    
  // Equivalence relation (specification)
  // An equivalence relation is a relation that is reflexive, symmetric, and 
  // transitive.
  template <typename R>
    struct equivalence_relation_spec
    {
      equivalence_relation_spec(R r = {})
        : reflexive(r), symmetric(r), transitive(r)
      { }

      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          check(env, reflexive, var);
          check(env, symmetric, var, var);
          check(env, transitive, var, var, var);
        }

      reflexive_property<R> reflexive;
      symmetric_property<R> symmetric;
      transitive_property<R> transitive;
    };



  // Strict partial order (specification)
  // A strict partial order is irreflexive, asymmetric, and transitive.
  template <typename R>
    struct strict_partial_order_spec
    {
      strict_partial_order_spec(R r = {})
        : irreflexive(r), asymmetric(r), transitive(r)
      { }
      
      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          check(env, irreflexive, var);
          check(env, asymmetric, var, var);
          check(env, transitive, var, var, var);
        }
    
      irreflexive_property<R> irreflexive;
      asymmetric_property<R> asymmetric;
      transitive_property<R> transitive;
    };
    


  // Strict weak order (specification)
  // A strict weak order is a strict weak order whose symmetric complement 
  // (a.k.a., incomparability) is transitive. Note that incomparability can 
  // also be  shown to be an equivalence relation.
  template <typename R>
    struct strict_weak_order_spec
    {
      strict_weak_order_spec(R r = {})
        : partial(r), equivalence(r)
      { }
      
      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          check(env, partial, var);
          check(env, equivalence, var, var, var);
        }
    
      strict_partial_order_spec<R> partial;
      transitive_property<symmetric_complement<R>> equivalence;
    };
    


  // Strict total order (specification)
  // A strict total ordering is antisymmetric, transitive, and trichotomous.
  // Said otherwise, it is a weak order where the equivalence relation defined
  // by incomparability is the same as equality.
  //
  // Note that the value type over which the specification is tested is
  // required to be equality comparable.
  template <typename R>
    struct strict_total_order_spec
    {
      strict_total_order_spec(R r = {})
        : antisymmetric{r}, transitive{r}, trichotomous{r}
      { }
    
      template <typename Env, typename Var>
        void operator()(Env& env, Var&& var) const
        {
          static_assert(Equality_comparable<Result_type<Forwarded<Var>>>(), "");
          check(env, antisymmetric, var, var);
          check(env, transitive, var, var, var);
          check(env, trichotomous, var, var);
        }

      antisymmetric_property<R> antisymmetric;
      transitive_property<R> transitive;
      trichotomous_property<R> trichotomous;
    };

} // namespace origin
