// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/testing.hpp>


using namespace std;
using namespace origin;


// All of (property)
struct all_of_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return all_of(range, pred) == (find_if_not(range, pred) == end(range));
    }
};



// All of/count if (property)
struct all_of_count_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return all_of(range, pred) == (count_if(range, pred) == distance(range));
    }
};




// Some of (property)
struct some_of_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return some_of(range, pred) == (find_if(range, pred) != end(range));
    }
};



// Some of (property)
struct some_of_count_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return some_of(range, pred) == (count_if(range, pred) != 0);
    }
};



// Not all of (property)
struct not_all_of_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return not_all_of(range, pred) == !all_of(range, pred);
    }
};



// Not all of/count if (property)
struct not_all_of_count_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return not_all_of(range, pred) == (count_if(range, pred) != distance(range));
    }
};




// None of (property)
struct none_of_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return none_of(range, pred) == !some_of(range, pred);
    }
};



// None of/count if (property)
struct none_of_count_check
{
  template <typename R, typename P>
    bool operator()(const R& range, P pred) const
    {
      return none_of(range, pred) == (count_if(range, pred) == 0);
    }
};



// All equal (property)
struct all_equal_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return all_equal(range, value) 
          == (find_not_equal(range, value) == end(range));
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      return all_equal(range, value, comp) 
          == (find_not_equal(range, value, comp) == end(range));
    }
};



// Some equal (property)
struct some_equal_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return some_equal(range, value) == (find(range, value) != end(range));
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      return some_equal(range, value, comp) 
         == (find(range, value, comp) != end(range));
    }
};



// Not all equal (property)
struct not_all_equal_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return not_all_equal(range, value) == !all_equal(range, value);
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, C>(), bool>
    {
      return not_all_equal(range, value, comp) == !all_equal(range, value, comp);
    }
};



// Some equal (property)
struct none_equal_check
{
  template <typename R, typename T>
    auto operator()(const R& range, const T& value) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return none_equal(range, value) == !some_equal(range, value);
    }

  template <typename R, typename T, typename C>
    auto operator()(const R& range, const T& value, C comp) const
      -> Requires<Range_search<R, T, Equal_to>(), bool>
    {
      return none_equal(range, value, comp) == !some_equal(range, value, comp);
    }
};


int main()
{
  assert_checker<> env;

  // Test predicate sequences
  {
    using V = vector<bool>;
    using P = to_bool_function;
    using Bool_dist = single_value_distribution<P>;
    
    auto range = checkable_var<V>(env);
    auto pred = checkable_var(env, Bool_dist {});

    quick_check(env, all_of_check {}, range, pred);
    quick_check(env, all_of_count_check {}, range, pred);
    quick_check(env, some_of_check {}, range, pred);
    quick_check(env, some_of_count_check {}, range, pred);
    quick_check(env, not_all_of_check {}, range, pred);
    quick_check(env, not_all_of_count_check {}, range, pred);
    quick_check(env, none_of_check {}, range, pred);
    quick_check(env, none_of_count_check {}, range, pred);
  }

  // Test value sequences
  {
    using V = vector<int>;
    using Less_dist = single_value_distribution<less_relation>;

    auto range = checkable_var<V>(env);
    auto value = checkable_var<int>(env);
    auto less = checkable_var(env, Less_dist {});

    quick_check(env, all_equal_check {}, range, value);
    quick_check(env, all_equal_check {}, range, value, less);
    
    quick_check(env, some_equal_check {}, range, value);
    quick_check(env, some_equal_check {}, range, value, less);

    quick_check(env, not_all_equal_check {}, range, value);
    quick_check(env, not_all_equal_check {}, range, value, less);

    quick_check(env, none_equal_check {}, range, value);
    quick_check(env, none_equal_check {}, range, value, less);
  }
}
