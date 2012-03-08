// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TESTING_HPP
#define ORIGIN_TESTING_HPP

#include <origin/concepts.hpp>
#include <origin/random.hpp>

namespace origin
{
  // The testing library is a collection of algorithms that can be used to
  // construct a specification-based test suite.
  //
  // Concepts:
  //    Property_check
  //    Specification
  //    Randomized_check
  //    Randomized_property_check
  //    Randomized_specification_check
  //
  // TODO: Finish writing documentation.
  //
  // TODO: Refactor the environment so that it has a random number engine
  // by default?
  


  namespace traits 
  {
    // Infrastructure for determining if the expression
    //
    //    p.accept(args...) 
    //
    // is valid, and safely deducing its result type.

    template <typename P, typename... Args>
      struct get_property_accept
      {
      private:
        template <typename X, typename... Ys>
          static auto check(X x, Ys&&... ys) 
            -> decltype(x.accept(std::forward<Ys>(ys)...));
        static subst_failure check(...);
      public:
        using type = decltype(check(std::declval<P>(), std::declval<Args>()...));
      };
      
    template <typename P, typename... Args>
      using Property_accept_result = typename get_property_accept<P, Args...>::type;
      
    template <typename P, typename... Args>
      constexpr bool Has_property_accept()
      {
        return Subst_succeeded<Property_accept_result<P, Args...>>();
      }
  } // namespace traits
  
  
  
  // Conditional property
  // A conditional property is one that has the mathematical form:
  //
  //    P => Q
  //
  // Here, P is the antecedent of the conditionally specified property, and
  // Q is the consequent. Conditional properties are meaningfully verified
  // if both P and Q are true. If P is false, the entire property is vacuously
  // true. In the testing sense, P is a guard for Q.
  //
  // Conditional properties have an additional member, p.accept(args...) that 
  // determines if the precedent P is true.
  //
  // Note that the the predicate should still incorporate P into its actual
  // definition. Not all checking environments, algorithms, or uses will
  // use the conditional property concept.
  //
  // TODO: I'm not really using this right now. It's not bad (it works), but
  // it's not terribly important.
  template <typename P, typename... Args>
    constexpr bool Conditional_property()
    {
      return traits::Has_property_accept<P, Args...>();
    }

  
  
  // Property check (concept)
  template <typename Env, typename P, typename... Args>
    constexpr bool Property_check()
    {
      return Predicate<P, Args...>() && Function<Env, P, Args...>();
    }
  
    
  
  // Specification (concept)
  // A specification is a function object that can be checked using a
  // checking environment (Env) with values provided by some random value 
  // generators (Gens...) and a random number engine (Eng).
  template <typename Spec, typename Env, typename Eng, typename... Gens>
    constexpr bool Specification()
    {
      return Function<Spec, Env&, Eng&, Forwarded<Gens>&...>();
    }

  

  // Randomized check (concept)
  // A randomized check is a property or specification check whose values are
  // generated using a random engine (Eng) and some number of random value
  // generators (Gens...)
  template <typename Eng, typename... Gens>
    constexpr bool Randomized_check()
    {
      return Random_bit_generator<Forwarded<Eng>>()
          && All(Random_value_generator<Forwarded<Gens>>()...);
    }
    

  // A randomized property check is an algorithm that invokes some predicate
  // over randomly generated data. A pseudorandom bit generator (Eng) is 
  // required along with random value generators (Gens...) for each predicate 
  // argument.
  template <typename Env, typename P, typename Eng, typename... Gens>
    constexpr bool Randomized_property_check()
    {
      return Randomized_check<Eng, Gens...>()
          && Property_check<Env, P, Result_type<Forwarded<Gens>>...>();
    }

  
  // Randomized specification check (algorithm concept)
  // A randomized specification check is an algorithm that invokes a 
  // specification testing function using a psuedorandom value generator and a
  // random value geenrator for each specification argument (Gens...)
  template <typename Env, typename Spec, typename Eng, typename... Gens>
    constexpr bool Randomized_specification_check()
    {
      return Randomized_check<Eng, Gens...>()
          && Specification<Spec, Env, Eng, Gens...>();
    }



  // Property checking algorithms
  // The following algorithms evaluate a property over a set of values. The
  // following syntax is supported:
  //
  //    check(pred, a)
  //    check(pred, a, b)
  //    check(pred, a, b, c)
  //
  // Where pred is a predicate that can be invoked over arguments a, b, and c.


  
  // Check the nullary (constant) predicate functin.
  template <typename Env, typename P>
    auto check(Env& env, P pred) -> Requires<Property_check<Env, P>()>
    {
      env(pred);
    }

  // Check the given unary property for the specified value.
  template <typename Env, typename P, typename T>
    auto check(Env& env, P pred, const T& a) 
      -> Requires<Property_check<Env, P, T>()>
    {
      env(pred, a);
    }

  // Check the given binary property using the given arguments.
  template <typename Env, typename P, typename T1, typename T2>
    auto check(Env& env, P pred, const T1& a, const T2& b) 
      -> Requires<Property_check<Env, P, T1, T2>()>
    {
      env(pred, a, b);
    }

  // Check the given ternary property using the given arguments.
  template <typename Env, typename P, typename T1, typename T2, typename T3>
    auto check(Env& env, P pred, const T1& a, const T2& b, const T3& c)
      -> Requires<Property_check<Env, P, T1, T2, T3>()>
    {
      env(pred, a, b, c);
    }



  // Randomized property checking
  // The following algoirthms evaluate a property over a set of randomly
  // generated values. The following syntax is supported:
  //
  //    check(env, pred, eng, gen)
  //    check(env, pred, eng, gen1, gen2)
  //    check(env, pred, eng, gen1, gen2, gen3)
  //
  // Here, eng is a random number engine, and gen* are random value generators.
  
  
  // Check the given unary property for a randomly generated value from the
  // given distribution.
  template <typename Env, typename P, typename Eng, typename Gen>
    auto check(Env& env, P pred, Eng&& eng, Gen&& gen)
      -> Requires<Randomized_property_check<Env, P, Eng, Gen>()>
    {
      check(env, pred, gen(eng));
    }


  
  // Check the given binary property for randomly generated values from the 
  // given distributions.
  template <typename Env, typename P, typename Eng, typename Gen1, typename Gen2>
    auto check(Env& env, P pred, Eng&& eng, Gen1&& gen1, Gen2&& gen2)
      -> Requires<Randomized_property_check<Env, P, Eng, Gen1, Gen2>()>
    {
      check(env, pred, gen1(eng), gen2(eng));
    }
    
  
  
  // Check the given ternary property for randomly generated values from the
  // given distributions.
  template <typename Env, typename P, typename Eng, typename Gen1, typename Gen2, typename Gen3>
    auto check(Env& env, P pred, Eng&& eng, Gen1&& gen1, Gen2&& gen2, Gen3&& gen3)
      -> Requires<Randomized_property_check<Env, P, Eng, Gen1, Gen2, Gen3>()>
    {
      check(env, pred, gen1(eng), gen2(eng), gen3(eng));
    }



  // Randomized specification checking
  // The following algorithms evaluate a specification over a sequence of 
  // randomly generated values. The following syntax is supported.
  //
  //    check(env, spec, eng, gen)
  //    check(env, spec, eng, gen1, gen2)
  //    check(env, spec, eng, gen1, gen2, gen3)
  //
  // Here, env is the checking environment, eng is a random number engine and 
  // gen* are random value generators.
  
  
  
  // Check the given specification using values generated by the given
  // specification.
  template <typename Env, typename Spec, typename Eng, typename... Gens>
    auto check(Env& env, Spec spec, Eng&& eng, Gens&&... gens)
      -> Requires<Randomized_specification_check<Env, Spec, Eng, Gens...>()>
    {
      spec(env, std::forward<Eng>(eng), std::forward<Gens>(gens)...);
    }


    
  // Quick check
  // Repeatedly check a given specification by continuously generating random
  // inputs. The value of n is given as the last argument and is taken as 100
  // by default.
  
  // Evaluate the checkable type Check over the given arguments n times.
  template <typename Env, typename Check, typename... Args>
    void quick_check_impl(int n, Env& env, Check c, Args&&... args)
    {
      while(n != 0) {
        check(c, std::forward<Args>(args)...);
        --n;
      }
    }
    
  // Check the unary property specification 100 times.
  template <typename Env, typename Check, typename Eng, typename Gen>
    void quick_check(Env& env, Check c, Eng&& eng, Gen&& gen, int n = 100)
    {
      check_n_impl(n, c, eng, gen);
    }
    
  
  
  // Check the binary property specification 100 times.
  template <typename Env, typename Check, typename Eng, typename Gen1, typename Gen2>
    void quick_check(Env& env, Check c, Eng&& eng, Gen1&& gen1, Gen2&& gen2, int n = 100)
    {
      check_n_impl(n, c, eng, gen1, gen2);
    }

  
  
  // Check the ternary property specification 100 times.
  template <typename Env, typename Check, typename Eng, typename Gen1, typename Gen2, typename Gen3>
    void quick_check(Env& env, Check c, Eng&& eng, Gen1&& gen1, Gen2&& gen2, Gen3&& gen3, int n = 100)
    {
      check_n_impl(n, c, eng, gen1, gen2, gen3);
    }
    


  // The basic check environment evaluates predicates and records the results.
  //
  // TODO: There is a ton of stuff we can do with this, but I'd need to develop
  // more fully thought out approaches to input classification, conditional
  // properties, and property/input association. There's a lot of work that
  // could go into this.
  struct basic_checker
  {
    template <typename P>
      void operator()(P pred)
      {
        if(pred())
          ++succ;
        else
          ++fail;
      }
    
    template <typename P, typename T>
      void operator()(P pred, const T& a)
      {
        if(pred(a)) 
          ++succ;
        else
          ++fail;
      }
    
    template <typename P, typename T1, typename T2>
      void operator()(P pred, const T1& a, const T2& b)
      {
        if(pred(a, b)) 
          ++succ;
        else
          ++fail;
      }

    template <typename P, typename T1, typename T2, typename T3>
      void operator()(P pred, const T1& a, const T2& b, const T3& c)
      {
        if(pred(a, b, c))
          ++succ;
        else
          ++fail;
      }
      
    int succ;   // Number of tests succeeded
    int fail;   // Number of tests failed.
  };
    
  
  
  // Assert checker (checking environment)
  // The assert checker simply asserts the validity of a specification. This
  // is useful for debugging.
  struct assert_checker
  {
    template <typename P>
      void operator()(P pred) const
      {
        assert(pred());
      }

    template <typename P, typename T>
      void operator()(P pred, const T& a) const
      {
        assert(pred(a));
      }
    
    template <typename P, typename T1, typename T2>
      void operator()(P pred, const T1& a, const T2& b) const
      {
        assert(pred(a, b));
      }

    template <typename P, typename T1, typename T2, typename T3>
      void operator()(P pred, const T1& a, const T2& b, const T3& c)
      {
        assert(pred(a, b, c));
      }
  };
    
} // namespace origin

#include <origin/testing/relations.hpp>
#include <origin/testing/concepts.hpp>
#include <origin/testing/iterators.hpp>

#endif
