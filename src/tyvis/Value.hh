#ifndef VALUE_HH
#define VALUE_HH

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
#include <iostream>
using std::ostream;
#include <string>
using std::string;
#include "tyvis/LValue.hh"
#include "tyvis/UniversalInteger.hh"

class TypeInfo;
class EnumerationLiteral;
class Aggregate;
class StringLiteral;

/**
   Generally we want to pass RValues around by reference.  But in some
   cases, we have to pass them by value.  Specifically, in cases like the
   return types of the operators of ObjectBase, we need a generic value
   object that implements RValue but can hold any type of literal.  This
   class implements that generic value object.
*/

class Value : public LValue {
public:

  /** Constructor.  Clones the data passed in; does not assume ownership. */
  Value( const RValue &initData );

  /** Constructor.  Assumes ownership of the data passed in. */
  Value( RValue *initData );

  /** Copy constructor. */
  Value( const Value &that );

  /** Slice constructors. */
  Value( RValue &, const ScalarTypeInfo & );
  Value( Value, const ScalarTypeInfo & );

  virtual ~Value();

  const RValue &readVal() const;
  RValue &readVal();
  void updateVal( const RValue & );
  int getIntValue() const;
  LONG getInt64Value() const;
  double getDoubleValue() const;
  
  RValue &operator=( const RValue &that ) ;
  Value &operator=( const Value &that ) ;

  bool operator==( const RValue &compareTo ) const;
  bool operator!=( const RValue &compareTo ) const;
  bool operator>( const RValue &compareTo ) const;
  bool operator>=( const RValue &compareTo ) const;
  bool operator<( const RValue &compareTo ) const;
  bool operator<=( const RValue &compareTo ) const;

  /** Virtualized printing. */
  void print( ostream &os ) const;

  /** Get a string representation of the value. */
  string toString() const;

  void vcdPrint( ostream &os ) const;

  /** Does this value have a composite type? */
  bool isCompositeType() const;

  /** Does this value have a scalar type? */
  bool isScalarType() const;

  /** Does this value have a physical type? */
  bool isPhysicalType() const;

  //@{
  /** Array access operators, should only be called on array typed Values. */
  const RValue &operator[]( int index ) const;
  RValue &operator[]( int index ) ;
  const RValue &operator[]( const RValue &index ) const;
  RValue &operator[]( const RValue &index ) ;
  //@}

  //@{

  /**The following methods allow access to composite types for the
     implementation, not for published VHDL code.  */

  /** Get the field at the index.  Only for composite types unless the
      index is 0. */
  const RValue &getField( int index ) const;

  /** Get the field at the index.  Only for composite types unless the
      index is 0. */
  RValue &getField( int index ) ;

  /** Get the field found at the index passed in.  Generates a runtime
      error if the index does not exist. */
  const RValue &getField( const RValue &index ) const;

  /** Get the field found at the index passed in.  Generates a runtime
     error if the index does not exist. */
  RValue &getField( const RValue &index ) ;
  //@}

  /** Get the unit string for a PhysicalLiteral.  If it's not a
     PhysicalLiteral, will return ""; */
  const string &getUnit() const;

  /** Clone this object. */
  RValue *clone() const;

  /** Access for physical types.  Returns the value portion of a physical
      type. */
  LONG getValue() const;

  /** Access for physical types.  Returns the units portion of a physical
      type.  Has to be applied to the typeInfo to get full unit
      information. */
  int getUnits() const;

  /** Get the TypeInfo for this Value. */
  const TypeInfo &getTypeInfo() const;

  const EnumerationLiteral &vhdlEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlNotEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlGreaterThan( const RValue & ) const;
  const EnumerationLiteral &vhdlGreaterThanEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlLessThan( const RValue & ) const;
  const EnumerationLiteral &vhdlLessThanEqual( const RValue & ) const;
  Value vhdlConcatenate( const RValue & ) const;
	
  Value vhdlUnaryPlus( ) const;
  Value vhdlUnaryMinus( ) const;
  Value vhdlPlus( const RValue & ) const;
  Value vhdlMinus( const RValue & ) const;
  Value vhdlMultiply( const RValue & ) const;
  Value vhdlDivide( const RValue & ) const;
  Value vhdlMod( const RValue & ) const;
  Value vhdlRem( const RValue & ) const;
  Value vhdlPow( const RValue & ) const;

  Value vhdlNot() const;
  Value vhdlAnd( const RValue & ) const;
  Value vhdlOr( const RValue & ) const;
  Value vhdlNand( const RValue & ) const;
  Value vhdlNor( const RValue & ) const;
  Value vhdlXor( const RValue & ) const;
  Value vhdlXnor( const RValue & ) const;

  Value vhdlSll( const RValue &rhs ) const;
  Value vhdlSrl( const RValue &rhs ) const;
  Value vhdlSla( const RValue &rhs ) const;
  Value vhdlSra( const RValue &rhs ) const;
  Value vhdlRol( const RValue &rhs ) const;
  Value vhdlRor( const RValue &rhs ) const;

  RValue &all();
  const RValue &all() const;

  Value left( const RValue &param = defaultIndex() ) const;
  Value right( const RValue &param = defaultIndex() ) const;
  Value high( const RValue &param = defaultIndex() ) const;
  Value low( const RValue &param = defaultIndex() ) const;
  const ScalarTypeInfo &range( const RValue &index = defaultIndex() ) const;
  const ScalarTypeInfo &reverse_range( const RValue &index = defaultIndex() ) const;
  Value ascending( const RValue &index = defaultIndex() ) const;

  /** Get the total number of fields in this object.  Non-composites will
      return 1.  Composites, it depends. */
  virtual Value length( const RValue &index = UniversalInteger(1) ) const;

  SignalNetinfo &addToFanOut( VHDLKernel * );
  //DRH
  SignalNetinfo &addChild( SignalNetinfo &, VHDLKernel * );

  void saveMyData() {saveData = true;}
  Value();

protected:

  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  RValue *myData;
  bool saveData;
};

#endif
