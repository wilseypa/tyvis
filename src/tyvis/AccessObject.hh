#ifndef ACCESS_OBJECT_HH
#define ACCESS_OBJECT_HH

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
#include "tyvis/vhdl.hh"
#include "tyvis/Value.hh"
#include "tyvis/UniversalInteger.hh"
#include "tyvis/ArrayInfo.hh"
#include "tyvis/Types.hh"
#include "tyvis/TyvisVarArgs.hh"
#include <string.h>
#include <iostream>

using std::cout;
class AccessTypeInfo;

class AccessObject : public LValue {
public:
  AccessObject( const AccessTypeInfo &initTypeInfo );
  AccessObject( const AccessTypeInfo &initTypeInfo , const RValue &);
  AccessObject( const AccessObject & );
  AccessObject( const AccessTypeInfo &initTypeInfo, TyvisVarArgs &initValue );
  ~AccessObject();

  static const string &getAccessObjectType(){
    static const string accessObjectType = "AccessObject";
    return accessObjectType;
  }

  const string &getDataType() const {
    return getAccessObjectType();
  }

  void serialize( SerializedInstance * ) const {
    abort();
  }

  AccessObject &operator=(const AccessObject & );

  void print( ostream &os ) const;
  void vcdPrint( ostream &os ) const {os << "INVALID";}
  virtual Value length( const RValue & = UniversalInteger(1) ) const {
	  return UniversalInteger(static_cast<int>(val->toString().size())); }
  void advance( int amount ){ curPos += amount; }
  
  int getCurrentPos( ){ return curPos; }
  int getIntValue() const { abort(); return 0; }
  LONG getInt64Value() const { abort(); return 0; }
  double getDoubleValue() const { abort(); return 0; }

  void setVal( const string & ){ abort(); }
  
  const string &getVal(){
    abort();
  }

  bool operator==( const RValue &compareTo ) const {
    if ( dynamic_cast<const AccessObject *>(&compareTo) ){
      return ( dynamic_cast<const AccessObject &>( compareTo ).curPos == curPos &&
	     dynamic_cast<const AccessObject &>( compareTo ).val == val );
    }
    return (compareTo == *val);
  }

  bool operator!=( const RValue &compareTo ) const {
    return !( dynamic_cast<const AccessObject &>( compareTo ).curPos == curPos &&
	      dynamic_cast<const AccessObject &>( compareTo ).val == val );
  }

  bool operator>( const RValue &compareTo ) const{
    return val > dynamic_cast<const AccessObject &>( compareTo ).val;
  }

  bool operator>=( const RValue &compareTo ) const {
    return val >= dynamic_cast<const AccessObject &>( compareTo ).val;
  }

  bool operator<( const RValue &compareTo ) const {
    return val < dynamic_cast<const AccessObject &>( compareTo ).val;
  }

  bool operator<=( const RValue &compareTo ) const {
    return val <= dynamic_cast<const AccessObject &>( compareTo ).val;
  }

  inline void printstr(ostream& os) {
    os << val;
  }
  
  /**
     Eats whitespace starting at currentPos.  Returns true if we're still
     inside the string at the end, false otherwhise.
  */
  bool eatwhite(){
    abort();
//     for( int i = curPos; i < length()(); i++ ){
//       while( val[i] == ' ' || val[i] == '\t' ){
// 	curPos++;
//       }
//     }
//     return curPos < length()();
  }

  void append( const string & ){
    // val += toAppend;
    abort();
  }
  
  void reset();

  const RValue &readVal() const;
  RValue &readVal();
  void updateVal( const RValue &newValue );
  RValue &operator=(const RValue &);
  RValue *clone() const;

  string toString() const;
  bool isCompositeType() const;
  bool isScalarType() const;
  bool isPhysicalType() const;
  const RValue &operator[](int) const;
  RValue &operator[](int);
  const RValue &operator[](const RValue&) const;
  RValue &operator[](const RValue&);
  RValue &getField(int) const;
  RValue &getField(int);
  const RValue& getField(const RValue&) const;
  RValue &getField(const RValue&);
  const string &getUnit() const;
  LONG getValue() const;
  int getUnits() const;
  const TypeInfo &getTypeInfo() const;
  const EnumerationLiteral &vhdlEqual(const RValue&) const;
  const EnumerationLiteral &vhdlNotEqual(const RValue&) const;
  const EnumerationLiteral &vhdlGreaterThan(const RValue&) const;
  const EnumerationLiteral &vhdlGreaterThanEqual(const RValue&) const;
  const EnumerationLiteral &vhdlLessThan(const RValue&) const;
  const EnumerationLiteral &vhdlLessThanEqual(const RValue&) const;
  Value vhdlConcatenate( const RValue & ) const;

  RValue &all();
  const RValue &all() const;

  Value vhdlUnaryPlus( ) const;
  Value vhdlUnaryMinus( ) const;
  
  Value vhdlPlus( const RValue & ) const;
  Value vhdlMinus( const RValue & ) const;
  Value vhdlMultiply( const RValue & ) const;
  Value vhdlDivide( const RValue & ) const;
  Value vhdlMod( const RValue & ) const;
  Value vhdlRem( const RValue & ) const;
  Value vhdlPow( const RValue & ) const;

  SignalNetinfo &addToFanOut( VHDLKernel * );
  //DRH
  SignalNetinfo &addChild( SignalNetinfo &, VHDLKernel * );
  void deallocate( );
  
protected:

  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  const AccessTypeInfo *myTypeInfo;
  int curPos; // pointer into val indicating, e.g., the current read position
  RValue *val;
  void die() const;
};
  
#endif
