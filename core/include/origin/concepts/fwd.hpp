// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_FWD_HPP
#define ORIGIN_CONCEPTS_FWD_HPP

namespace origin
{
  // Queries
  template<typename... Args> struct tSame;
  template<typename T, typename U> struct tDifferent;
  template<typename T, typename U> struct tConvertible;

  // Classifiers
  template<typename T> struct tInt;
  template<typename T> struct tSigned_Int;
  template<typename T> struct tUnsigned_Int;

  // Interfaces
  template<typename T> struct tDestructible;
  template<typename T, typename... Args> struct tConstructible;
  template<typename T> struct tCopyable;
  template<typename T> struct tMoveable;

  // Operators
  template<typename T, typename U = T> struct tLogical_And;
  template<typename T, typename U = T> struct tLogical_Or;
  template<typename T> struct tLogical_Not;
  template<typename T, typename U = T> struct tEqual;
  template<typename T, typename U = T> struct tLess;

  // Function Traits
  template<typename F, typename... Args> struct tCallable;
  template<typename F, typename... Args> struct tProcecure;
  template<typename F, typename... Args> struct tOracle;


  // Concepts
  template<typename T> struct cComparable;
  template<typename T> struct cCopyable;
  template<typename T> struct cRegular;
  template<typename T> struct cOrdered;

  // Functions
  template<typename F, typename... Args> struct cFunction;
  template<typename F, typename... Args> struct cOperation;
  template<typename F, typename... Args> struct cPredicate;
  template<typename F, typename T, typename U> struct cRelation;
  template<typename F, typename T, typename U> struct cEquivalence_Relation;
  template<typename F, typename T, typename U> struct cStrict_Weak_Order;

  // Types
  template<typename T> struct cBoolean;


} // namespace origin

#endif
