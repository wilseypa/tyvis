#ifndef OBJECTBASE_HH
#define OBJECTBASE_HH

// Copyright (c) Ohio Board of Regents and the University of Cincinnati.  
// All Rights Reserved.

// UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
// LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR
// DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "tyvis/tyvis-config.h"
#include "tyvis/VHDLData.hh"
#include "tyvis/LValue.hh"
#include "tyvis/Value.hh"
#include "tyvis/Types.hh"
#include "tyvis/TyvisVarArgs.hh"
#include "tyvis/StringLiteral.hh"
#include "tyvis/IntegerTypeInfo.hh"
#include <iostream>
#include <fstream>

using std::ostream;
using std::ofstream;
using std::cerr;
using std::endl;

class TypeInfo;
class VHDLKernel;
class RValue;
class SubElementFactory;

/** This class is the base class for the Signal and Variables.  Any Type is
    going to have a pointer to this, so that a Type can have any object. */
class ObjectBase : public LValue {
public:
  virtual ~ObjectBase();

  virtual ObjectBase &operator=( const ObjectBase &that );
  virtual RValue &operator=( const RValue &that );

  RValue *clone() const;

  const RValue &operator[]( int index ) const;
  RValue &operator[]( int index );
  const RValue &operator[]( const RValue &index ) const;
  RValue &operator[]( const RValue &index );
 
  /**
     Print the value of this object to the stream passed in.
  */
  virtual void print( ostream & ) const;

  /**
     Get the string representation of the value of this object.
  */
  virtual string toString() const;
  /**
     Init assignment from the string.  The int return is how many
     characters were consumed.
  */

  virtual void vcdPrint( ostream &os ) const;
		  
  const RValue &readVal() const;
  RValue &readVal();
  void updateVal( const RValue & );

  bool operator==( const RValue &compareTo ) const;
  bool operator!=( const RValue &compareTo ) const;
  bool operator>( const RValue &compareTo ) const;
  bool operator>=( const RValue &compareTo ) const;
  bool operator<( const RValue &compareTo ) const;
  bool operator<=( const RValue &compareTo ) const;

  const EnumerationLiteral &vhdlEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlNotEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlGreaterThan( const RValue & ) const;
  const EnumerationLiteral &vhdlGreaterThanEqual( const RValue & ) const;
  const EnumerationLiteral &vhdlLessThan( const RValue & ) const;
  const EnumerationLiteral &vhdlLessThanEqual( const RValue & ) const;
  Value vhdlConcatenate( const RValue & ) const;

  Value vhdlUnaryPlus( ) const;
  Value vhdlUnaryMinus( ) const;
  Value vhdlPlus    ( const RValue &that ) const;
  Value vhdlMinus   ( const RValue &that ) const;
  Value vhdlMultiply( const RValue &that ) const;
  Value vhdlDivide  ( const RValue &that ) const;
  Value vhdlMod     ( const RValue &that ) const;
  Value vhdlRem     ( const RValue &that ) const;
  Value vhdlPow     ( const RValue &that ) const;

  //@{
  /**
     Logical operators, minus the short circuit operators.  Those get
     covered by the EnumerationLiteral operators due to how they are code
     generated.
  */
  Value vhdlNot() const;
  Value vhdlXor( const RValue & ) const;
  Value vhdlXnor( const RValue & ) const;
  //@}

  virtual void setSourceBaseDeleteFlag( bool flag ) const {
    (void)flag; // quiet the compiler.
    cerr << " ERROR  ObjectBase:: setSourceBaseDeleteFlag( bool flag ) called" << endl;
    abort();
  }

  virtual void setResolutionFunctionId(int) {
    cerr << " ERROR  ObjectBase::setResolutionFunctionId(int) called" << endl;
    abort();
  }

  virtual void setTypeConversionFunctionId(int) {
    cerr << " ERROR  ObjectBase::setTypeConversionFunctionId(int) called" << endl;
    abort();
  }

  virtual void setElaborationInfo(ObjectBase&) {
    cerr << "ERROR ObjectBase::setElaborationInfo(ObjectBase&) called" << endl;
    abort();
  }

  virtual void setNumAttributes(int ) {
    cerr << "ERROR ObjectBase::setNumAttributes(int) called" << endl;
    abort();
  }

  virtual void setAttrib( AttribType ,VHDLData & ) {
    cerr << "ERROR ObjectBase::setAttrib(AttribType, VHDLData &) called" << endl;
    abort();
  }

  virtual void setBusKind() { 
    cerr << "ERROR ObjectBase::setBusKind() called" << endl;
  }

  virtual Value resolve( VHDLKernel * ) { abort(); }

  int getIntValue() const { return readVal().getIntValue(); }
  LONG getInt64Value() const { return readVal().getInt64Value(); }
  double getDoubleValue() const { return readVal().getDoubleValue(); }

  bool isScalarType() const;
  virtual bool isResolvedSignal() const { return false; }

  bool isPhysicalType() const;
  LONG getValue() const;
  int getUnits() const;

  bool isCompositeType() const;
  bool isCompositeResolvedObject() const { return false; }

  const RValue &getField( int index ) const;
  RValue &getField( int index );
  const RValue &getField( const RValue &index ) const;
  RValue &getField( const RValue &index );
  const string &getUnit() const;

  const TypeInfo &getTypeInfo() const;

  const string &getName() const;

  //@{
  /** Attributes */
  virtual Value left( const RValue &param = defaultIndex() ) const;
  virtual Value right( const RValue &param = defaultIndex() ) const;
  virtual Value high( const RValue &param = defaultIndex() ) const;
  virtual Value low( const RValue &param = defaultIndex() ) const;
  virtual const ScalarTypeInfo &range( const RValue &index = defaultIndex() ) const;
  virtual const ScalarTypeInfo &reverse_range( const RValue &index = defaultIndex() ) const;
  virtual Value length( const RValue &index = defaultIndex() ) const;
  virtual Value ascending( const RValue &index = defaultIndex() ) const;
  //@}

  RValue &all();
  const RValue &all() const;

  SignalNetinfo &addToFanOut( VHDLKernel * );
  SignalNetinfo &addChild( SignalNetinfo &, VHDLKernel * ); 
protected:
  ObjectBase( const TypeInfo &initTypeInfo, const string &initName );
  ObjectBase( SignalNetinfo &parent, const IntegerTypeInfo &initTypeInfo );
  ObjectBase( const ArrayTypeInfo &initTypeInfo, TyvisVarArgs &siIn );
  ObjectBase( const TypeInfo &initTypeInfo, const string &initName, const RValue &initValue );
  ObjectBase( const SubElementFactory &subElementFactory );
  ObjectBase( const ObjectBase &that );
  ObjectBase();
  void voidElements();

  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  const TypeInfo *myTypeInfo;
  string myName;
  RValue *myData;
};
#endif
