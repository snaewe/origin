// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>
#include <chrono>

#include <origin/iterator.hpp>

namespace origin
{
  // FIXME: Make something more formal and coherent.

  template <typename C, typename Traits, typename T>
    void log_args(std::basic_ostream<C, Traits>& os, const T& value)
    {
      os << value;
    }

  template <typename C, typename Traits, typename T, typename... Args>
    void log_args(std::basic_ostream<C, Traits>& os, 
                  const T& value, 
                  const Args&... args)
    {
      os << value << ',' << ' ';
      log_args(os, args...);
    }


  // Timing environment (tester)
  // The timing environment executes programs and records the results of
  // each run.
  //
  // FIXME: What's the best way to log stuff?
  template <typename Eng = std::minstd_rand,
            typename Clock = std::chrono::system_clock>
    class timing_environment
    {
      Eng eng;

    public:
      using seed_type = std::time_t;

      timing_environment() : eng() { }
      
      // Initialize the environment with the given pseduo-random number
      // generator.
      explicit timing_environment(const Eng& eng) : eng(eng) { }

      // Initialize the environment by seeding the engine with given value.
      explicit timing_environment(seed_type n) : eng(n) { }


      template <typename F, typename... Args>
        void operator()(F func, Args&&... args) const
        {
          auto start = Clock::now();
          func(std::forward<Args>(args)...);
          auto stop = Clock::now();

          std::cout << typestr<F>() << '\t';
          log_args(std::cout, args...);
          std::cout << '\t';
          std::cout << (stop - start).count() << '\n';
        }


      // Return the psuedo random number engine.
      Eng& random_engine()       { return eng; }
      Eng  random_engine() const { return eng; }
    };

} // namespace origin
