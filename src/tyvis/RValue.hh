#ifndef RVALUE_HH
#define RVALUE_HH

// Copyright (c) Clifton Labs, Inc.  All Rights Reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "tyvis/tyvis-config.h"
#include "tyvis/Types.hh"
#include <iostream>
using std::ostream;
#include <string>
using std::string;

class Value;
class TypeInfo;
class EnumerationLiteral;
class ScalarTypeInfo;
class UniversalInteger;

class RValue {
public:
  void * operator new (size_t);
  void operator delete (void *, size_t);

  virtual const RValue &readVal() const = 0;
  virtual int getIntValue() const = 0;
  virtual LONG getInt64Value() const = 0;
  virtual double getDoubleValue() const = 0;
  
  virtual RValue &operator=( const RValue &that ) = 0;
  virtual bool operator==( const RValue &compareTo ) const = 0;
  virtual bool operator!=( const RValue &compareTo ) const = 0;
  virtual bool operator>( const RValue &compareTo ) const = 0;
  virtual bool operator>=( const RValue &compareTo ) const = 0;
  virtual bool operator<( const RValue &compareTo ) const = 0;
  virtual bool operator<=( const RValue &compareTo ) const = 0;

  /**
     Virtualized printing.
  */
  virtual void print( ostream &os ) const = 0;
  /**
     Get a string representation of the value.
  */
  virtual string toString() const = 0;
  /**
     Virtualized Value Change Dump format printing.
  */
  virtual void vcdPrint( ostream &os ) const = 0;

  virtual void debugPrint( ostream &, int = 0 ) const;
  virtual void debugPrintDetails( ostream &, int ) const {};
  virtual const string &getName() const;

  /**
     Does this value have a composite type?
  */
  virtual bool isCompositeType() const {return false;}
  /**
     Does this value have a scalar type?
  */
  virtual bool isScalarType() const {return false;}
  /**
     Does this value have a physical type?
  */
  virtual bool isPhysicalType() const {return false;}

  /**
     Default object is not a signal
   */
  virtual bool is_signal() const { return false; }

  //@{
  /**
     Array access operators, should only be called on array typed RValues.
  */
  virtual const RValue &operator[]( int index ) const = 0;
  virtual RValue &operator[]( int index ) = 0;
  virtual const RValue &operator[]( const RValue &index ) const = 0;
  virtual RValue &operator[]( const RValue &index ) = 0;
  //@}

  //@{
  /**
     The following methods allow access to composite types for the
     implementation, not for published VHDL code.
  */

  /**
     Get the field at the index.  Only for composite types unless the index
     is 0.
  */
  virtual const RValue &getField( int index ) const = 0;
  /**
     Get the field at the index.  Only for composite types unless the index
     is 0.
  */
  virtual RValue &getField( int index ) = 0;

  /**
     Get the field found at the index passed in.  Generates a runtime error
     if the index does not exist.
  */
  virtual const RValue &getField( const RValue &index ) const = 0;

  /**
     Get the field found at the index passed in.  Generates a runtime error
     if the index does not exist.
  */
  virtual RValue &getField( const RValue &index ) = 0;
  //@}

  /**
     Get the unit string for a PhysicalLiteral.  If it's not a PhysicalLiteral,
     will return "";
  */
  virtual const string &getUnit() const = 0;

  /**
     Clone this object.
  */
  virtual RValue *clone() const = 0;

  //@{ Physical Types
  /**
     Returns the value portion of a physical type.
  */
  virtual LONG getValue() const = 0;
  /**
     Returns the units portion of a physical type.  Has to be applied to
     the typeInfo to get full unit information.
  */
  virtual int getUnits() const = 0;
  //@}
  
  /**
     Get the TypeInfo for this RValue.
  */
  virtual const TypeInfo &getTypeInfo() const = 0;

  //@{
  /** Relational Operators */
  virtual const EnumerationLiteral &vhdlEqual( const RValue & ) const = 0;
  virtual const EnumerationLiteral &vhdlNotEqual( const RValue & ) const = 0;
  virtual const EnumerationLiteral &vhdlGreaterThan( const RValue & ) const = 0;
  virtual const EnumerationLiteral &vhdlGreaterThanEqual( const RValue & ) const = 0;
  virtual const EnumerationLiteral &vhdlLessThan( const RValue & ) const = 0;
  virtual const EnumerationLiteral &vhdlLessThanEqual( const RValue & ) const = 0;
  //@}
  //@{
  /** Arithmetic Operators */
  virtual Value vhdlUnaryPlus( ) const = 0;
  virtual Value vhdlUnaryMinus( ) const = 0;
  virtual Value vhdlPlus( const RValue & ) const = 0;
  virtual Value vhdlMinus( const RValue & ) const = 0;
  virtual Value vhdlMultiply( const RValue & ) const = 0;
  virtual Value vhdlDivide( const RValue & ) const = 0;
  virtual Value vhdlMod( const RValue & ) const = 0;
  virtual Value vhdlRem( const RValue & ) const = 0;
  virtual Value vhdlPow( const RValue & ) const = 0;
  //@}
  //@{
  /** Logical Operators */
  virtual Value vhdlNot() const;
  virtual Value vhdlAnd( const RValue & ) const;
  virtual Value vhdlOr( const RValue & ) const;
  virtual Value vhdlNand( const RValue & ) const;
  virtual Value vhdlNor( const RValue & ) const;
  virtual Value vhdlXor( const RValue & ) const;
  virtual Value vhdlXnor( const RValue & ) const;
  //@}
  //@{
  /** Shift Operators */
  virtual Value vhdlSll( const RValue &rhs ) const;
  virtual Value vhdlSrl( const RValue &rhs ) const;
  virtual Value vhdlSla( const RValue &rhs ) const;
  virtual Value vhdlSra( const RValue &rhs ) const;
  virtual Value vhdlRol( const RValue &rhs ) const;
  virtual Value vhdlRor( const RValue &rhs ) const;
  //@}

  /**
     Concatenate the argument with "this" and return the result.
  */
  virtual Value vhdlConcatenate( const RValue & ) const = 0;

  //@{
  /**
     Methods support access types.
  */
  virtual RValue &all() = 0;
  virtual const RValue &all() const = 0;
  //@}

  //@{
  /** Object attributes */
  /**
     Get the total number of fields in this object.  Non-composites will
     return 1.  Composites, it depends.
  */
  virtual Value left( const RValue &param = defaultIndex() ) const;
  virtual Value right( const RValue &param = defaultIndex() ) const;
  virtual Value high( const RValue &param = defaultIndex() ) const;
  virtual Value low( const RValue &param = defaultIndex() ) const;
  virtual const ScalarTypeInfo &range( const RValue &index = defaultIndex() ) const;
  virtual const ScalarTypeInfo &reverse_range( const RValue &index = defaultIndex() ) const;
  virtual Value length( const RValue &index = defaultIndex() ) const = 0;
  virtual Value ascending( const RValue &index = defaultIndex() ) const;
  //@}

  virtual ~RValue();

  const string getKindText() const;

  static const RValue &defaultIndex();

protected:
  RValue(){}
  const string & NL(const int ) const;
};

#include <vector>
using std::vector;

inline ostream &
operator<<( ostream &os, const RValue &rvalue ){
  rvalue.print(os);
  return os;
}

#endif

