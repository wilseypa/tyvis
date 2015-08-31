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

#include "PhysicalLiteral.hh"
#include "EnumerationLiteral.hh"
#include "std_standardPkg.hh"
#include "VHDLVTime.hh"
#include <cmath>
#include <sstream>
using std::ostringstream;

PhysicalLiteral::PhysicalLiteral() :
  VHDLData(),
  myTypeInfo(0),
  myValue(0){}

PhysicalLiteral::PhysicalLiteral( LONG initMultiplier,
				  const string &initUnit ) :
  VHDLData(),
  myTypeInfo(0),
  myValue( initMultiplier ),
  myUnit( initUnit ){}

PhysicalLiteral::PhysicalLiteral( const PhysicalTypeInfo &initTypeInfo,
				  double initMultiplier,
				  const string &initUnit ) :
  VHDLData(),
  myTypeInfo( dynamic_cast<PhysicalTypeInfo *>(initTypeInfo.clone()) ),
  myUnit( initUnit ){
  myValue = static_cast<LONG>(initMultiplier);
  if (static_cast<double>(myValue) != initMultiplier) {
    double t;
    if (myUnit == "hr")
      t = 3600e+15 * initMultiplier;
    else if (myUnit == "min")
      t = 60e+15 * initMultiplier;
    else if (myUnit == "sec")
      t = 1e+15 * initMultiplier;
    else if (myUnit == "ms")
      t = 1e+12 * initMultiplier;
    else if (myUnit == "us")
      t = 1e+9 * initMultiplier;
    else if (myUnit == "ns")
      t = 1e+6 * initMultiplier;
    else if (myUnit == "ps")
      t = 1e+3 * initMultiplier;
    else if (myUnit == "fs")
      t = initMultiplier;
    else
      t = 0;
   
    if (t != 0) {
      if (fabs(t) > 1e+15) {
        if (fabs(t) > 1e+18) {
          myValue = static_cast<LONG>(t/1000000);
          myUnit = "ns";
        }
        else {
          myValue = static_cast<LONG>(t/1000);
          myUnit = "ps";
        }
      }
      else {
        myValue = static_cast<LONG>(t);
        myUnit = "fs";
      }
    }
  }
}

PhysicalLiteral::PhysicalLiteral(const PhysicalLiteral &that ): 
  VHDLData(that),
  myTypeInfo( that.myTypeInfo ? dynamic_cast<PhysicalTypeInfo *>(that.getTypeInfo().clone()) : 0 ),
  myValue( that.myValue ),
  myUnit( that.myUnit ){}

PhysicalLiteral::PhysicalLiteral( const PhysicalTypeInfo &initTypeInfo ) :
  myTypeInfo( dynamic_cast<PhysicalTypeInfo *>(initTypeInfo.clone()) ),
  myValue( initTypeInfo.left().getInt64Value() ){}

PhysicalLiteral::PhysicalLiteral( const PhysicalTypeInfo &initTypeInfo,
				  const RValue &initValue ) :
  myTypeInfo( dynamic_cast<PhysicalTypeInfo *>(initTypeInfo.clone()) ),
  myValue( initValue.getInt64Value() ),
  myUnit( initTypeInfo.getPrimaryUnit() ){}

PhysicalLiteral::PhysicalLiteral( const PhysicalTypeInfo &initTypeInfo,
				  const LONG initValue ) :
  myTypeInfo( dynamic_cast<PhysicalTypeInfo *>(initTypeInfo.clone()) ),
  myValue( initValue ),
  myUnit( initTypeInfo.getPrimaryUnit() ){}

PhysicalLiteral::~PhysicalLiteral(){
  delete myTypeInfo;
  myTypeInfo = 0;
}

PhysicalLiteral &
PhysicalLiteral::operator=( const PhysicalLiteral &that ) {
  myTypeInfo = that.myTypeInfo ? dynamic_cast<PhysicalTypeInfo *>(that.myTypeInfo->clone()) : 0;
  myValue = that.getInt64Value();
  myUnit = dynamic_cast<const PhysicalLiteral &>(that.readVal()).getUnit();
  return *this;
}

RValue &
PhysicalLiteral::operator=( const RValue &that ) {
  if( that.isPhysicalType() ){
    *this = dynamic_cast<const PhysicalLiteral &>(that.readVal());
  }
  else{
    ASSERT( myTypeInfo != 0);
    myValue = that.getInt64Value();
    myUnit = myTypeInfo->getPrimaryUnit();
  }
  return *this;
}

Value
PhysicalLiteral::vhdlPlus( const RValue &rhs ) const {
  ASSERT( myTypeInfo != 0 );
  return PhysicalLiteral( *myTypeInfo,
			  getInt64Value() + rhs.getInt64Value() );
}

Value 
PhysicalLiteral::vhdlMinus( const RValue &rhs ) const {
  ASSERT( myTypeInfo != 0 );  
  return PhysicalLiteral( *myTypeInfo, 
			  getInt64Value() - rhs.getInt64Value() );
}

Value
PhysicalLiteral::vhdlUnaryPlus() const {
  return *this;
}

Value
PhysicalLiteral::vhdlUnaryMinus() const {
  ASSERT( myTypeInfo != 0 );    
  return PhysicalLiteral( *myTypeInfo, 
			  (LONG)-1 * getInt64Value() );
}

Value
PhysicalLiteral::vhdlMultiply( const RValue &rhs ) const {
  ASSERT( myTypeInfo != 0 );    
  if (dynamic_cast<const UniversalReal *>(&rhs)) {
    return PhysicalLiteral( *myTypeInfo,
                            static_cast<LONG>(getInt64Value() * rhs.getDoubleValue()) );
  }
  return PhysicalLiteral( *myTypeInfo, 
			  getInt64Value() * rhs.getInt64Value() );
}

UniversalInteger
PhysicalLiteral::vhdlDivide( const PhysicalLiteral &rhs ) const {
  return UniversalInteger( getInt64Value() / rhs.getInt64Value() );
}

Value
PhysicalLiteral::vhdlDivide( const RValue &rhs ) const {
  ASSERT( myTypeInfo != 0 );
  return PhysicalLiteral( *myTypeInfo,
			  getInt64Value() / rhs.getInt64Value() );
}

Value
PhysicalLiteral::vhdlMod( const RValue &rhs ) const {
  int lval = getIntValue();
  int rval = rhs.getIntValue();
  return PhysicalLiteral( *myTypeInfo,
			  (lval*rval < 0) ? (rval + lval%rval) : (lval%rval));
}

Value
PhysicalLiteral::vhdlRem( const RValue &rhs ) const {
  return PhysicalLiteral( *myTypeInfo, getIntValue() % rhs.getIntValue() );
}

Value
PhysicalLiteral::vhdlPow( const RValue &rhs ) const {
  return PhysicalLiteral( *myTypeInfo,
			  (LONG)(::pow(getDoubleValue(), rhs.getDoubleValue())));
}

Value
PhysicalLiteral::vhdlAbs() const {
  ASSERT( myTypeInfo != 0 );
  LONG retval = getInt64Value();
  if( myValue < 0 ){
    retval = retval * (LONG)-1;
  }
  return PhysicalLiteral( *myTypeInfo, retval );
}

const RValue &
PhysicalLiteral::leftValue() {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->left();
}

const TypeInfo &
PhysicalLiteral::getTypeInfo() const {
  ASSERT( myTypeInfo != 0 );
  return *myTypeInfo;
}

void
PhysicalLiteral::print( ostream &os ) const {
  os << getValue() << " " << getUnit();
}

string
PhysicalLiteral::toString() const {
  ostringstream os;
  print(os);
  return os.str();
}

RValue *
PhysicalLiteral::clone() const {
  PhysicalLiteral *retval = new PhysicalLiteral( *this );
  return retval;
}

int
PhysicalLiteral::getIntValue() const { 
  return (int)getInt64Value();
}

LONG
PhysicalLiteral::getInt64Value() const { 
  LONG retval = myValue;
  if( getUnit() != "" && myTypeInfo != 0 ){
    retval = myValue * myTypeInfo->getBaseMultiplier( getUnit() );
  }
  return retval;
}

double
PhysicalLiteral::getDoubleValue() const { 
  return (double)getInt64Value();
}

LONG
PhysicalLiteral::getValue() const {
  return myValue;
}

const string &
PhysicalLiteral::getUnit() const {
  return myUnit;
}

bool
PhysicalLiteral::operator==( const RValue &that ) const {
  return (getInt64Value() == that.getInt64Value());
}

bool
PhysicalLiteral::operator!= (const RValue &that ) const {
  return !operator==( that );
}

bool 
PhysicalLiteral::operator> (const RValue &that ) const {
  return (getInt64Value() > that.getInt64Value());
}

bool
PhysicalLiteral::operator>= (const RValue &that ) const {
  return (getInt64Value() >= that.getInt64Value());
}

bool
PhysicalLiteral::operator< (const RValue &that ) const {
  return (getInt64Value() < that.getInt64Value());
}

bool
PhysicalLiteral::operator<= (const RValue &that ) const {
  return (getInt64Value() <= that.getInt64Value());
}

const string &
PhysicalLiteral::getDataType() const {
  return getPhysicalLiteralDataType();
}

const string &
PhysicalLiteral::getPhysicalLiteralDataType(){
  static const string physicalLiteralDataType("PhysicalLiteral");
  return physicalLiteralDataType;
}

void
PhysicalLiteral::serialize( SerializedInstance * ) const {
  cerr << "PhysicalLiteral::serialize not yet implemented!" << endl;
  abort();
}

VHDLVTime 
PhysicalLiteral::toVTime() const {
  return VHDLVTime( getInt64Value(), 0 );
}

LONG 
PhysicalLiteral::getValuePrimaryUnit() const {
  ASSERT(myTypeInfo != 0);
  return  getInt64Value() * (myTypeInfo->getBaseMultiplier(myTypeInfo->getPrimaryUnit())) / (myTypeInfo->getBaseMultiplier(getUnit()));
  
}

void
PhysicalLiteral::debugPrintDetails( ostream &os, int indent ) const {
  os << "Value = " << myValue << "  unit = " << myUnit;
  os << NL(indent) << "PhysicalLiteralTypeInfo *myTypeInfo: ";
  if (myTypeInfo)
    myTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";
 
  return;
}
