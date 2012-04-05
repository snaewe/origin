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
  


  // Property check (concept)
  // A property check algorithm is an algorithm that takes a predicate P, and
  // a checking environment Env, which is a function that evalutes the 
  // predicate type P over its arguments.
  template <typename Env, typename P, typename... Args>
    constexpr bool Property_check()
    {
      return Predicate<P, Args...>() && Function<Env, P, Args...>();
    }
  
    
  
  // Specification (concept)
  // A specification is a function object that can be checked using a
  // checking environment (Env) over the given argument types.
  template <typename Spec, typename Env, typename... Args>
    constexpr bool Specification()
    {
      return Function<Spec, Env&, Args...>();
    }

  

  // Randomized property check (concept)
  // A randomized property check is an algorithm that invokes some predicate
  // over randomly generated data. A pseudorandom bit generator (Eng) is 
  // required along with random value generators (Gens...) for each predicate 
  // argument.
  template <typename Env, typename P, typename... Vars>
    constexpr bool Randomized_property_check()
    {
      return All(Random_variable<Forwarded<Vars>>()...)
          && Property_check<Env, P, Result_type<Forwarded<Vars>>...>();
    }

  

  // Randomized specification check (algorithm concept)
  // A randomized specification check is an algorithm that invokes a 
  // specification testing function using a psuedorandom value generator and a
  // random value geenrator for each specification argument (Gens...)
  template <typename Env, typename Spec, typename... Vars>
    constexpr bool Randomized_specification_check()
    {
      return All(Random_variable<Forwarded<Vars>>()...)
          && Specification<Spec, Env, Forwarded<Vars>&...>();
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
  //
  // FIXME: Make this variadic.

  
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

  // Check the given quternary property using the given arguments.
  template <typename Env, typename P, 
            typename T1, typename T2, typename T3, typename T4>
    auto check(Env& env, P pred, 
               const T1& a, const T2& b, const T3& c, const T4& d)
      -> Requires<Property_check<Env, P, T1, T2, T3>()>
    {
      env(pred, a, b, c, d);
    }


  // Randomized property checking
  // The following algoirthms evaluate a property over a set of randomly
  // generated values. It supports the following syntax:
  //
  //    check(env, pred, vars...)
  //
  // Vars is a sequence of random variables. There must be the same number of
  // random variables as the arity of the pred function.
  template <typename Env, typename P, typename... Vars>
    auto check(Env& env, P pred, Vars&&... vars)
      -> Requires<Randomized_property_check<Env, P, Vars...>()>
    {
      check(env, pred, vars()...);
    }



  // Randomized specification checking
  // The following algorithms evaluate a specification over a sequence of 
  // randomly generated values. The following syntax is supported.
  //
  //    check(env, spec, vars...)
  //
  // Vars is a sequence of random variables. There must be the same number of
  // random variables as the arity of the spcification function.
  template <typename Env, typename Spec, typename... Vars>
    auto check(Env& env, Spec spec, Vars&&... vars)
      -> Requires<Randomized_specification_check<Env, Spec, Vars...>()>
    {
      spec(env, std::forward<Vars>(vars)...);
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
        check(env, c, std::forward<Args>(args)...);
        --n;
      }
    }


  // FIXME: Rewrite this so it's variadic, and then pull the optionally
  // specified count off the end of the argument list. Actually, is that
  // possible? Probably if I force the random variables into a tuple, and
  // then expand then during invocation.


  // Check the nullary specification.
  template <typename Env, typename Check>
    void quick_check(Env& env, Check c, int n = 100)
    {
      quick_check_impl(n, env, c);
    }



  // Check the unary property specification 100 times.
  template <typename Env, typename Check, typename Var>
    void quick_check(Env& env, Check c, Var&& var, int n = 100)
    {
      quick_check_impl(n, env, c, var);
    }
    
  
  
  // Check the binary property specification 100 times.
  template <typename Env, typename Check, typename Var1, typename Var2>
    void quick_check(Env& env, Check c, Var1&& var1, Var2&& var2, int n = 100)
    {
      quick_check_impl(n, env, c, var1, var2);
    }

  
  
  // Check the ternary property specification 100 times.
  template <typename Env, typename Check, typename Var1, typename Var2, typename Var3>
    void quick_check(Env& env, Check c, Var1&& var1, Var2&& var2, Var3&& var3, int n = 100)
    {
      quick_check_impl(n, env, c, var1, var2, var3);
    }

  // Check the quaternary property specification 100 times.
  template <typename Env, typename Check, 
            typename Var1, typename Var2, typename Var3, typename Var4>
    void quick_check(Env& env, Check c, 
                     Var1&& var1, Var2&& var2, Var3&& var3, Var4&& var4,
                     int n = 100)
    {
      quick_check_impl(n, env, c, var1, var2, var3, var4);
    }


  // The basic check environment evaluates predicates and records the results.
  //
  // TODO: There is a ton of stuff we can do with this, but I'd need to develop
  // more fully thought out approaches to input classification, conditional
  // properties, and property/input association. There's a lot of work that
  // could go into this.
  //
  // TODO: Make sure that this wraps a random number generator.
  struct basic_checker
  {
    template <typename P, typename... Args>
      void operator()(P pred, Args&&... args)
      {
        if(pred(std::forward<Args>(args)...))
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
  template <typename Eng = std::minstd_rand>
    class assert_checker
    {
    public:
      assert_checker() : eng() { }
      explicit assert_checker(Eng eng) : eng(eng) { }


      // Evaluate the given predicate by asserting it.
      template <typename P, typename... Args>
        void operator()(P pred, Args&&... args) const
        {
          assert(pred(std::forward<Args>(args)...));
        }


      // Return the psuedo random number engine.
      Eng& random_engine()       { return eng; }
      Eng  random_engine() const { return eng; }

    private:
      Eng eng; // Random number engine
    };



  // Return a random variable for the given type T that will be used for
  // checking in the given environment.
  template <typename T, typename Env>
    auto checkable_var(Env& env) 
      -> decltype(make_random<T>(env.random_engine()))
    {
      return make_random<T>(env.random_engine());
    }

  // Return a random variable for the given type T that will be used for
  // checking in the given environment.
  template <typename Env, typename Dist>
    auto checkable_var(Env& env, Dist&& dist) 
      -> decltype(make_random(env.random_engine(), dist))
    {
      return make_random(env.random_engine(), dist);;
    }

} // namespace origin

#include <origin/testing/relations.hpp>
#include <origin/testing/concepts.hpp>
#include <origin/testing/iterators.hpp>
#include <origin/testing/archetypes.hpp>

#endif
