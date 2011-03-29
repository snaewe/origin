// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <tuple>
#include <string>

#include <origin/utility/typestr.hpp>
#include <origin/iterator/enumerator.hpp>

using namespace std;
using namespace origin;

int main()
{
  char const* cstr[] = {"a", "b", "c"};
  {
    auto e = make_enumerator(cstr);

    typedef decltype(e) Iter;
    typedef iterator_traits<Iter>::value_type Value_Type;
    static_assert(std::is_same<Value_Type::first_type, std::ptrdiff_t>::value,"");
    static_assert(std::is_same<Value_Type::second_type, char const*>::value, "");
    typedef iterator_traits<Iter>::reference Reference;
    static_assert(std::is_same<Reference::second_type, char const*&>::value, "");
    typedef iterator_traits<Iter>::pointer Pointer;
    static_assert(std::is_same<Pointer::second_type, char const*&>::value, "");

    assert(( get<0>(*e) == 0 ));
    assert(( string(get<1>(*e)) == "a" ));
    ++e;
    assert(( get<0>(*e) == 1 ));
    assert(( string(get<1>(*e)) == "b" ));
    ++e;
    assert(( get<0>(*e) == 2 ));
    assert(( string(get<1>(*e)) == "c" ));

  }

  string str[] = {"abc", "def", "hij"};
  {
    auto f = make_enumerator(str), l = make_enumerator(str + 3);

    typedef decltype(f) Iter;
    typedef iterator_traits<Iter>::value_type Value_Type;
    static_assert(std::is_same<Value_Type::first_type, std::ptrdiff_t>::value,"");
    static_assert(std::is_same<Value_Type::second_type, string>::value, "");
    typedef iterator_traits<Iter>::reference Reference;
    static_assert(std::is_same<Reference::second_type, string&>::value, "");
    typedef iterator_traits<Iter>::pointer Pointer;
    static_assert(std::is_same<Pointer::second_type, string&>::value, "");

    assert(( distance(f, l) == 3 ));
    assert(( (*f).second == "abc" ));
    (*f).second[1] = 'x';
    assert(( (*f).second == "axc" ));
  }
}
