#ifndef UNIVERSAL_INTEGER_HH
#define UNIVERSAL_INTEGER_HH

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
#include "tyvis/IntegerLiteral.hh"

class UniversalReal;
class AccessObject;

class UniversalInteger : public IntegerLiteral {
public:
  UniversalInteger(){}
  UniversalInteger(int i) : IntegerLiteral( i ){}
  UniversalInteger(LONG i) : IntegerLiteral( i ){}
  UniversalInteger(char i) : IntegerLiteral( i ){}
  UniversalInteger(bool i) : IntegerLiteral( i ){}
  UniversalInteger(double i) : IntegerLiteral( i ){}
  UniversalInteger( const TypeInfo &, const RValue &i);
  UniversalInteger( const RValue &i );

  ~UniversalInteger(){}

  RValue *clone() const {
    return new UniversalInteger(*this);
  }
  
  void serialize( SerializedInstance *addTo ) const {
    addTo->addInt( getIntValue() );
  }

  static Serializable *deserialize( SerializedInstance *si ){
    return new UniversalInteger( si->getInt() );
  }

  static void registerDeserializer(){
    DeserializerManager::instance()->registerDeserializer( getUniversalUniversalInteger(),
							   &UniversalInteger::deserialize );
  }

  static const string &getUniversalUniversalInteger(){
    static const string universalUniversalInteger = "UniversalInteger";
    return universalUniversalInteger;
  }

  const string &getDataType() const {
    return getUniversalUniversalInteger();
  }

  Value vhdlUnaryPlus() const;
  Value vhdlUnaryMinus() const;
  UniversalInteger vhdlAbs() const;

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

  UniversalInteger &operator++(int);
  using IntegerLiteral::operator=;
  UniversalInteger &operator=( const UniversalInteger & );
  RValue &operator=( const RValue & );

  const TypeInfo &getTypeInfo() const;

  void print( ostream &os) const;
  string toString() const;
  void vcdPrint( ostream &os ) const;

private:
};

#endif
