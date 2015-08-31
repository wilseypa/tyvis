#ifndef PHYSICAL_LITERAL_HH
#define PHYSICAL_LITERAL_HH

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
#include "tyvis/VHDLVTime.hh"
#include "tyvis/PhysicalLiteral.hh"
#include "tyvis/UniversalInteger.hh"

class UniversalInteger;
class PhysicalLiteral;
class PhysicalTypeInfo;
class TypeInfo;

/**
   This class represents a physical literal in tyvis.  The representation
   is a long int (64 bits if possible - determined at configure time) and a
   string representation of the units.

   A literal like "1 ps" is equivalent to "1000 fs".  "1 ps" would return
   "1" for getValue(), "ps" for getUnit(), and "1000" for getInt64Value().
*/
class PhysicalLiteral: public VHDLData {
public:
  PhysicalLiteral();


  PhysicalLiteral( LONG initValue,
		   const string &unitName );

  PhysicalLiteral( const PhysicalTypeInfo &initTypeInfo,
		   double initValue,
		   const string &unitName );
  
  PhysicalLiteral( const PhysicalTypeInfo & );

  PhysicalLiteral( const PhysicalTypeInfo &,
		   const RValue &initValue );

  PhysicalLiteral( const PhysicalTypeInfo &,
		   LONG initValue );

  PhysicalLiteral( const PhysicalLiteral &that );
  
  virtual ~PhysicalLiteral();
  
  RValue &operator=( const RValue & );
  PhysicalLiteral &operator=( const PhysicalLiteral & );

  bool isScalarType() const { return true; }
  
  virtual const RValue &leftValue();
  const TypeInfo &getTypeInfo() const;

  Value vhdlPlus( const RValue & ) const;
  Value vhdlMinus( const RValue & ) const;
  Value vhdlMultiply( const RValue & ) const;
  Value vhdlDivide( const RValue & ) const;
  Value vhdlMod( const RValue & ) const;
  Value vhdlRem( const RValue & ) const;
  Value vhdlPow( const RValue & ) const;
  UniversalInteger  vhdlDivide( const PhysicalLiteral & ) const;
  Value vhdlAbs() const;
  Value vhdlUnaryPlus() const;
  Value vhdlUnaryMinus() const; 
   
  RValue *clone() const;

  void print(ostream& os) const;
  string toString() const;

  void vcdPrint( ostream &os ) const {os << "INVALID";}

  /**
     Returns (int)getInt64Value().
  */
  int getIntValue() const;

  /**
     Returns the value of this literal in terms of the primary unit.  For
     instance, if getValue() == 1 and getUnits == "ps", then
     "getInt64Value()" will return "1000".
  */
  LONG getInt64Value() const;

  /**
     Returns (double)getInt64Value().
  */
  double getDoubleValue() const;

  /**
     Returns whatever value this literal was created with.  For example, if
     it was created with "1 ps", would return 1.
  */
  LONG getValue() const;

  const string &getUnit() const;

  bool operator==( const RValue & ) const;
  bool operator!= (const RValue & ) const;
  bool operator> (const RValue &) const;
  bool operator>= (const RValue &) const;
  bool operator< (const RValue &) const;
  bool operator<= (const RValue &) const;

  const string &getDataType() const;
  static const string &getPhysicalLiteralDataType();
  void serialize( SerializedInstance * ) const;

  static void registerDeserializer(){
    DeserializerManager::instance()->registerDeserializer( getPhysicalLiteralDataType(),
							   &UniversalInteger::deserialize );
  }

  VHDLVTime toVTime() const;

  /**
     Calculate and return the value of this literal if it was expressed in
     primary units.  For example, 1 ps would return 1000 since the primary
     unit is fs.
  */
  LONG getValuePrimaryUnit() const;

protected:
  virtual void debugPrintDetails( ostream &os, int indent ) const;

public:
  const PhysicalTypeInfo *myTypeInfo;
  LONG myValue;
  string myUnit;
};

#endif
