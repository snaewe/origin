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



template <typename R>
  void print(const R& range)
  {
    for(const auto& x : range)
      cout << x;
  }


int main()
{
  // Randomly generated names from http://www.kleimo.com/random/name.cfm
  // Randomly generated numbers from http://www.random.org/
  vector<person> v = {
    {"Christian", "Ratti", 58},
    {"Clinton", "Welden", 23},
    {"Lonnie", "Rochford", 40},
    {"Javier", "Fragale", 26},
    {"Jessie", "Altom", 46},
    {"Clayton", "Wensel", 38},
    {"Fernando", "Maffett", 48},
    {"Mathew", "Pecor", 45},
    {"Allan", "Parten", 59},
    {"Hugh", "Lobue", 28},
  };


  project(v[0], &person::first, get_first {});

  /*
  multi_sort(v, &person::first, get_last {});
  print(v);
  cout << '\n';

  random_shuffle(v);
  multi_sort(v, &person::first_reverse, &person::first);
  print(v);
  cout << '\n';
  // multi_sort(v, &)
  */
}

