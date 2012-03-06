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
  template <typename T, typename U>
    struct equality_comparable_semantics
    {
      static_assert(Equality_comparable<T, U>(), "");
      using C = Common_type<T, U>;

      // Randomly test the specification using values of type T generated from
      // gen1 and values of type U generated from gen2.
      template <typename Env, typename Eng, typename Gen1, typename Gen2>
        void operator()(Env& env, Eng&& eng, Gen1&& gen1, Gen2&& gen2) const
        {
          static_assert(Same<T, Result_type<Forwarded<Gen1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Gen2>>>(), "");
          (*this)(env, eng, gen1, gen2, default_distribution<C>());
        }

        
      // Randomly test the specification using values of type T generated
      // from gen1, values of type U generated from gen2, and values of type
      // C (the common type) generated from gen3.
      template <typename Env, typename Eng, typename Gen1, typename Gen2, typename Gen3>
        void operator()(Env& env, Eng&& eng, Gen1&& gen1, Gen2&& gen2, Gen3&& gen3) const
        {
          static_assert(Same<T, Result_type<Forwarded<Gen1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Gen2>>>(), "");
          static_assert(Same<C, Result_type<Forwarded<Gen3>>>(), "");

          check(env, equality_comparable_semantics<T>{}, eng, gen1);
          check(env, equality_comparable_semantics<U>{}, eng, gen2);
          check(env, equality_comparable_semantics<C>{}, eng, gen3);

          check(env, eq, eng, gen1, gen2);  // a == b <=> C(a) == C(b)
          check(env, neq, eng, gen1, gen2); // a != b <=> C(a) != C(b)
        }

      common_type_equivalence<origin::eq> eq;
      common_type_equivalence<origin::neq> neq;
    };
    
  // Specialization when testing for a single type.
  template <typename T>
    struct equality_comparable_semantics<T, T>
    {
      static_assert(Equality_comparable<T>(), "");
      
      template <typename Env, typename Eng, typename Gen>
        void operator()(Env& env, Eng&& eng, Gen&& gen) const
        {
          static_assert(Same<T, Result_type<Forwarded<Gen>>>(), "");
          check(env, equivalence, eng, gen);
          check(env, not_equal, eng, gen, gen);
        }

      equivalence_relation_spec<eq> equivalence;
      logical_equivalence<neq, complement<eq>> not_equal;
    };


    
  // Totally ordered (specification)
  // The total ordering specification defines the semantics for totally order
  // types. When used with heterogeneous types, the semantics are defined in
  // terms of the common type.
  template <typename T, typename U>
    struct totally_ordered_semantics
    {
      static_assert(Totally_ordered<T, U>(), "");
      using C = Common_type<T, U>;

      template <typename Env, typename Eng, typename Gen1, typename Gen2>
        void operator()(Env& env, Eng&& eng, Gen1&& gen1, Gen2&& gen2) const
        {
          static_assert(Same<T, Result_type<Forwarded<Gen1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Gen2>>>(), "");
          (*this)(env, eng, gen1, gen2, default_distribution<C>());
        }
      
      template <typename Env, typename Eng, typename Gen1, typename Gen2, typename Gen3>
        void operator()(Env& env, Eng&& eng, Gen1&& gen1, Gen2&& gen2, Gen3&& gen3) const
        {
          static_assert(Same<T, Result_type<Forwarded<Gen1>>>(), "");
          static_assert(Same<U, Result_type<Forwarded<Gen2>>>(), "");
          static_assert(Same<C, Result_type<Forwarded<Gen3>>>(), "");
          
          check(env, totally_ordered_semantics<T>{}, eng, gen1);
          check(env, totally_ordered_semantics<U>{}, eng, gen2);
          check(env, totally_ordered_semantics<C>{}, eng, gen3);
          
          check(env, lt, eng, gen1, gen2);  // a < b <=> C(a) < C(b)
          check(env, gt, eng, gen1, gen2);  // a > b <=> C(a) > C(b)
          check(env, leq, eng, gen1, gen2); // a <= b <=> C(a) <= C(b)
          check(env, geq, eng, gen1, gen2); // b >= a <=> C(a) >= C(b)
        }
        
      common_type_equivalence<origin::lt> lt;
      common_type_equivalence<origin::gt> gt;
      common_type_equivalence<origin::leq> leq;
      common_type_equivalence<origin::geq> geq;
    };
    
  // Specialization for the unary type.
  template <typename T>
    struct totally_ordered_semantics<T, T>
    {
      static_assert(Totally_ordered<T>(), "");

      template <typename Env, typename Eng, typename Gen>
        void operator()(Env& env, Eng&& eng, Gen&& gen) const
        {
          static_assert(Same<T, Result_type<Forwarded<Gen>>>(), "");
          check(env, lt, eng, gen);
          check(env, gt, eng, gen, gen);
          check(env, leq, eng, gen, gen);
          check(env, geq, eng, gen, gen);
        }
        
      strict_total_order_spec<origin::lt> lt;
      logical_equivalence<origin::gt, converse<origin::lt>> gt;
      logical_equivalence<origin::leq, complement_of_converse<origin::lt>> leq;
      logical_equivalence<origin::geq, complement<origin::lt>> geq;
    };
    
    

  // Move semantics (specification)
  //
  // TODO: Implement tests for move semantics.
  template <typename T>
    struct move_semantics
    {
      template <typename Eng, typename Gen>
        void operator()(Eng&&, Gen&&) { }
    };
    

    
  // Copy construction preservation (property)
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
      template <typename Eng, typename Gen>
        void operator()(Eng&& eng, Gen& gen)
        {
          check(construct, eng, gen);
          check(assign, eng, gen);
        }

      copy_construction_preservation<T> construct;
      copy_assignment_preservation<T> assign;
    };


  
  // Default value property
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
  
  // Default initialization semantics (specification)
  // The default initialization semantics require a default initialized type
  // to have a unique default value.
  template <typename T>
    struct default_semantics
    {
      template <typename Eng>
        void operator()(Eng&)
        {
          check(init);
        }
        
      default_value_property<T> init;
    };
    
    

  template <typename T>
    struct regular_semantics
    {
      template <typename Eng, typename Gen>
        void operator()(Eng&& eng, Gen&& gen)
        {
          check(def, eng);
          check(copy, eng, gen);
        }
      
      default_semantics<T> def;
      copy_semantics<T> copy;
    };
    
    
    
  // Check regular
  // Check the type of the randomly generated value to determine if it 
  // satisfies the semantics of the Regular concept.
  template <typename Eng, typename Gen>
    void check_regular(Eng&& eng, Gen&& gen)
    {
      using T = Result_type<Forwarded<Gen>>;
      check(regular_semantics<T>{}, eng, gen);
    }

    
} // namespace origin
