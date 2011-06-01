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
  // Type Traits
  template<typename... Args> struct tSame;
  template<typename T, typename U> struct tDifferent;
  template<typename T, typename U> struct tConvertible;
  
  // Type Classification
  template<typename T> struct tInt;
  template<typename T> struct tSigned_Int;
  template<typename T> struct tUnsigned_Int;

  // Object Properties
  template<typename T> struct tDestructible;
  template<typename T, typename... Args> struct tConstructible;
  template<typename T, typename U> struct tAssignable;

  // Operator Traits
  // FIXME: Traits for !=, >, <= and >=
  template<typename T, typename U = T> struct tEqual;
  template<typename T, typename U = T> struct tLess;

  template<typename T, typename U = T> struct tPlus;
  template<typename T, typename U = T> struct tMinus;
  template<typename T, typename U = T> struct tMultiply;
  template<typename T, typename U = T> struct tDivide;
  template<typename T, typename U = T> struct tModulus;
  template<typename T> struct tNegate;

  template<typename T, typename U = T> struct tBit_And;
  template<typename T, typename U = T> struct tBit_Or;
  template<typename T, typename U = T> struct tBit_Xor;
  template<typename T> struct tComplement;
  
  // FIXME: Traits for << and >>
  
  template<typename T, typename U = T> struct tLogical_And;
  template<typename T, typename U = T> struct tLogical_Or;
  template<typename T> struct tLogical_Not;
  
  // Function Traits
  template<typename F, typename... Args> struct tCallable;
  template<typename F, typename... Args> struct tProcecure;


  // Concepts
  template<typename T> struct cComparable;
  template<typename T> struct cCopyable;
  template<typename T> struct cMoveable;
  template<typename T> struct cRegular;
  template<typename T> struct cOrdered;

  // Functions
  template<typename F, typename... Args> struct cFunction;
  template<typename F, typename... Args> struct cOperation;
  template<typename F, typename... Args> struct cPredicate;
  template<typename F, typename T, typename U> struct cRelation;
  
  // Types
  template<typename T> struct cBoolean;
  template<typename T> struct Integral;


} // namespace origin

#endif
