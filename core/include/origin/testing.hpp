// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TESTING_HPP
#define ORIGIN_TESTING_HPP

#include <origin/concepts.hpp>
#include <origin/exception.hpp>
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



  // Check (algorithm)
  // The check algorithm evaluates a property or specification (a testable
  // type) over some arguments. The syntax is:
  //
  //    check(env, test, args...)
  //
  // The env is a a checking environment that is ultimately responsible for 
  // test evaluation. It should be one of the checker objects defined below
  // or a custom checker.
  //
  // In essence, test is a function object that can be alled with args... as
  // arguments. There are several kinds of testable types:
  //
  // A property is a predicate that can be called with some specific inputs.
  // For example, a property that evaluates the semnatics of copy construction 
  // may take a specific kind of value to be copied. A property can also be
  // randomly tested by providing random variables that generate arguments of
  // the required types.
  //
  // A specification is also a function object that takes a sequence of random
  // variables as input, which are then used to generate inputs to a sequence
  // of properties.
  //
  // NOTE: if a property's functional call operator(s) are templates, then the
  // template must be constrained using Requires (or enable_if). The reason for
  // this is that the different constraints on the overloads of check  evaluate
  // whether or not the testable type can be called with different input kinds.
  // If the template is unconstrained, then the compiler will find ambiguous
  // overloads for check().

  


  // Basic property check
  // Evaluate pred against the given args.
  template <typename Env, typename P, typename... Args>
    auto check(Env& env, P pred, Args&&... args) 
      -> Requires<Property_check<Env, P, Args...>()>
    {
      env(pred, std::forward<Args>(args)...);
    }


  // Randomized property check
  // Evalutae pred by checking it against a smple of the random variables.
  template <typename Env, typename P, typename... Vars>
    auto check(Env& env, P pred, Vars&&... vars)
      -> Requires<Randomized_property_check<Env, P, Vars...>()>
    {
      check(env, pred, vars()...);
    }



  // Randomized specification check
  // Evaluate spec using the given random vars.
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
  


  // Evaluate the checkable type over the given arguments n times.
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
  //
  // FIXME: Obviously this doesn't assert any more. This is going to replace
  // the basic checker because it's going to do a lot of interesting reporting.
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
          if (!pred(std::forward<Args>(args)...)) {
            // FIXME: Build a string describing the failure. Be sure to
            // include information about the arguments. It might be useful
            // to wrap the actual evaluation of the arguments in a separate
            // function, in order to wrap it with a try/catch block.
            //
            // NOTE: If we can't Stream an argument, then we can at least
            // stream out its type name.
            std::string msg = "test failed: " + typestr<P>();
            throw std::logic_error(msg);
          }
        }


      // Return the psuedo random number engine.
      Eng& random_engine()       { return eng; }
      Eng  random_engine() const { return eng; }

    private:


      Eng eng; // Random number engine
    };



  // FIXME: These have the wrong names. The objects being created aren't
  // checkable, their used for checking. Should I call them testing_var and
  // testing_func?



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
      return make_random(env.random_engine(), dist);
    }

  // Return a random variable that always returns the given function object.
  // This is an alias for creating a random variable that returns a function
  // from a single-value distribution.
  template <typename Env, typename F>
    auto checkable_func(Env& env, F f)
      -> decltype(checkable_var(env, single_value_distribution<F> {f}))
    {
      return checkable_var(env, single_value_distribution<F> {f});
    }



  // Fake value
  // The fake value facility is used to create fake values. It is similar to
  // except that it can be evaluated -- sort of. A program that actually reads
  // from a fake object invokes undefined behavior.
  //
  // Fake objects are created through the fake() facility.


  // Fake type
  // The fake type class is specialized to return appropriately cv-qualified
  // fake objects.
  //
  // FIXME: In order to be complete, I probably need specializations for
  // volatile types.
  template <typename T>
    struct fake_type
    {
      static T& get() 
      { 
        static char object[sizeof(T)];
        return *reinterpret_cast<T*>(object);
      }
    };

  template <typename T>
    struct fake_type<const T>
    {
      const T& get() 
      { 
        static char object[sizeof(T)];
        return *reinterpret_cast<T*>(object);
      }
    };



  // Fake (constructor)
  // Return a fake object of the specified type.
  template <typename T>
    auto fake() -> decltype(fake_type<T>::get())
    {
      return fake_type<T>::get();
    }

} // namespace origin

#include <origin/testing/relations.hpp>
#include <origin/testing/concepts.hpp>
#include <origin/testing/iterators.hpp>
#include <origin/testing/archetypes.hpp>

#endif
