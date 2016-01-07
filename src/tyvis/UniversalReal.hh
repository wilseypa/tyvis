#ifndef UNIVERSAL_REAL_HH
#define UNIVERSAL_REAL_HH

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
#include <SerializedInstance.h>
#include <DeserializerManager.h>
#include "tyvis/VHDLData.hh"
using std::cout;

class AccessObject;
class UniversalInteger;

class UniversalReal : public VHDLData {
public:
  UniversalReal();
  UniversalReal( double );
  UniversalReal( int );
  UniversalReal( const RValue & );
  UniversalReal( const TypeInfo &,
		 const RValue & );

  static const UniversalReal &getMax();
  static const UniversalReal &getMin();

  bool isScalarType() const { return false; }

  RValue &operator=( const RValue &that ) {
    myValue = that.getDoubleValue();
    return *this;
  }

  UniversalReal &operator=( const UniversalReal &that ) {
    myValue = that.getDoubleValue();
    return *this;
  }

  UniversalReal &operator=( double that ){
    myValue = that;
    return *this;
  }

  bool operator==( const RValue &compareTo ) const {
    return myValue == compareTo.getDoubleValue();
  }

  bool operator!=( const RValue &compareTo ) const {
    return myValue != compareTo.getDoubleValue();
  }

  bool operator>( const RValue &compareTo ) const{
    return myValue > compareTo.getDoubleValue();
  }

  bool operator>=( const RValue &compareTo ) const {
    return myValue >= compareTo.getDoubleValue();
  }

  bool operator<( const RValue &compareTo ) const {
    return myValue < compareTo.getDoubleValue();
  }

  bool operator<=( const RValue &compareTo ) const {
    return myValue <= compareTo.getDoubleValue();
  }
  
  operator double() const { return myValue; }
  
  UniversalReal assign(char ch) { 
    myValue = UniversalReal(ch - '0');
    return myValue;
  }

  int getIntValue() const { return (int)myValue; }
  LONG getInt64Value() const { return (LONG)myValue; }
  double getDoubleValue() const { return myValue; }
  
  RValue *clone() const {
    return new UniversalReal(*this);
  }
  
  int getSize() const { return sizeof(*this); }

  static UniversalReal typeCast(VHDLData& toCast);

  static const string &getUniversalUniversalReal(){
    static const string universalUniversalReal = "UniversalReal";
    return universalUniversalReal;
  }

  const string &getDataType() const {
    return getUniversalUniversalReal();
  }

  void serialize( SerializedInstance *addTo ) const {
    addTo->addDouble( myValue );
  }

  static Serializable *deserialize( SerializedInstance *si ){
    return new UniversalReal( si->getDouble() );
  }

  static void registerDeserializer(){
    DeserializerManager::instance()->registerDeserializer( getUniversalUniversalReal(),
							   &UniversalReal::deserialize );
  }

  Value vhdlUnaryPlus() const;
  Value vhdlUnaryMinus() const;
  UniversalReal vhdlAbs() const;
  Value vhdlPlus( const RValue & ) const;
  Value vhdlMinus( const RValue & ) const;
  Value vhdlMultiply( const RValue & ) const;
  Value vhdlDivide( const RValue & ) const;
  Value vhdlMod( const RValue & ) const;
  Value vhdlRem( const RValue & ) const;
  Value vhdlPow( const RValue & ) const;

  const TypeInfo &getTypeInfo() const;

  string toString() const;
  void print( ostream & ) const;
  void vcdPrint( ostream &os ) const;

protected:
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  double myValue;
};

#endif
