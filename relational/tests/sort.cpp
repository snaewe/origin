// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <string>
#include <vector>
#include <iostream>

#include <origin/tuple.hpp>
#include <origin/sort.hpp>

using namespace std;
using namespace origin;



template <typename R>
  void print(const R& range)
  {
    for(const auto& x : range)
      cout << x;
  }

// FIXME: Implement this generically for tuples.

template <typename C, typename CT, typename T>
  inline basic_ostream<C, CT>& 
  operator<<(basic_ostream<C, CT>& os, const tuple<T>& t)
  {
    return os << '{' << get<0>(t) << '}';
  }

template <typename C, typename CT, typename T1, typename T2>
  inline basic_ostream<C, CT>& 
  operator<<(basic_ostream<C, CT>& os, const tuple<T1, T2>& t)
  {
    return os << '{' << get<0>(t) << ", " << get<1>(t) << '}';
  }

template <typename C, typename CT, typename T1, typename T2, typename T3>
  inline basic_ostream<C, CT>& 
  operator<<(basic_ostream<C, CT>& os, const tuple<T1, T2, T3>& t)
  {
    return os << '{' << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << '}';
  }


// Represents a person. There are some data fields.
struct person 
{
  string first;
  string last;
  int num;

  string first_reverse() const 
  {
    string result = first;
    reverse(result);
    return result;
  }

  string& mutable_last() { return last; }
};

// Output_streamable.
// Print people by their first name and last name.
template <typename C, typename T>
  basic_ostream<C, T>& operator<<(basic_ostream<C, T>& os, const person& p)
  {
    return os << p.first << ' ' << p.last << '\n';
  }


// Key functions
struct get_first
{
  const string& operator()(const person& p) const { return p.first; }
};

struct get_last
{
  const string& operator()(const person& p) const { return p.last; }
};

struct get_num
{
  int operator()(const person& p) const { return p.num; }
};




int main()
{
  // Randomly generated names from http://www.kleimo.com/random/name.cfm
  // Randomly generated numbers from http://www.random.org/
  vector<person> v = {
    {"Christian", "Ratti", 58},
    {"Clinton", "Welden", 23},
    {"Lonnie", "Rochford", 40},
    {"Javier", "Fragale", 26},
    {"Lonnie", "Altom", 46},
    {"Clayton", "Wensel", 38},
    {"Fernando", "Maffett", 48},
    {"Mathew", "Pecor", 45},
    {"Allan", "Parten", 59},
    {"Hugh", "Lobue", 28},
  };

  auto a1 = &person::first;
  auto a2 = get_last {};
  auto a3 = &person::first_reverse;
  auto a4 = &person::mutable_last;

  // NOTE: Can't use a4 because its a non-const operation, and comparators
  // bind their arguments by const reference. Should they?

  // Yes, I'm sorting by the last letter in the first name.
  sort_ascending(v, a3, a2, a1);


  print(v);
  cout << '\n';
  sort_descending(v, a1, a2);
  print(v);
}

