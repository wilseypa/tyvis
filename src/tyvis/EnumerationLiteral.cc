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

#include "EnumerationLiteral.hh"
#include "EnumerationTypeInfo.hh"
#include "std_standardPkg.hh"
#include "UniversalInteger.hh"
#include <cmath>
#include "TyvisVarArgs.hh"
#include "Aggregate.hh"

using std::ostringstream;

const EnumerationLiteral &
EnumerationLiteral::getEnumerationZero(){
  static const EnumerationLiteral ENUMERATION_ZERO(std_standard_bitTypeInfo(), 0);
  return ENUMERATION_ZERO;
}

const EnumerationLiteral &
EnumerationLiteral::getEnumerationOne(){
  static const EnumerationLiteral ENUMERATION_ONE(std_standard_bitTypeInfo(), 1);
  return ENUMERATION_ONE;
}

const EnumerationLiteral &
EnumerationLiteral::getEnumerationFalse(){
  static const EnumerationLiteral ENUMERATION_FALSE(std_standard_booleanTypeInfo(), 0);
  return ENUMERATION_FALSE;
}

const EnumerationLiteral &
EnumerationLiteral::getEnumerationTrue(){
  static const EnumerationLiteral ENUMERATION_TRUE(std_standard_booleanTypeInfo(), 1 );
  return ENUMERATION_TRUE;
}

const EnumerationLiteral &
EnumerationLiteral::getEnumerationReadMode(){
  static const EnumerationLiteral ENUMERATION_READ_MODE(std_standard_file_open_kindTypeInfo(), 0 );
  return ENUMERATION_READ_MODE;
}

const EnumerationLiteral &
EnumerationLiteral::getEnumerationWriteMode(){
  static const EnumerationLiteral ENUMERATION_WRITE_MODE(std_standard_file_open_kindTypeInfo(), 1 );
  return ENUMERATION_WRITE_MODE;
}

const EnumerationLiteral &
EnumerationLiteral::toBoolean( const RValue &toConvert ){
  return toBoolean( toConvert.getIntValue() );
}

const EnumerationLiteral &
EnumerationLiteral::toBoolean( bool toConvert ){
  if( toConvert ){
    return getEnumerationTrue();
  }
  else{
    return getEnumerationFalse();
  }
}

EnumerationLiteral::EnumerationLiteral() : IntegerLiteral(), myTypeInfo(0){}

EnumerationLiteral::EnumerationLiteral( const EnumerationTypeInfo &initTypeInfo) :
  IntegerLiteral(initTypeInfo),
  myTypeInfo( dynamic_cast<EnumerationTypeInfo *>(initTypeInfo.clone())){}

EnumerationLiteral::EnumerationLiteral( const EnumerationTypeInfo &initTypeInfo,
					const string &initValue ) :
  IntegerLiteral( initTypeInfo.getIndex( initValue ) ),
  myTypeInfo( dynamic_cast<EnumerationTypeInfo *>(initTypeInfo.clone()) ){}

EnumerationLiteral::EnumerationLiteral( const EnumerationTypeInfo &initTypeInfo,
					int initValue ) :
  IntegerLiteral( initValue ),
  myTypeInfo( dynamic_cast<EnumerationTypeInfo *>(initTypeInfo.clone()) ){
  ASSERT( initValue >= 0 );
}

EnumerationLiteral::EnumerationLiteral( const EnumerationTypeInfo &initTypeInfo,
					const RValue &initValue ) :
  IntegerLiteral( initValue ),
  myTypeInfo( dynamic_cast<EnumerationTypeInfo *>(initTypeInfo.clone()) ){}

EnumerationLiteral::EnumerationLiteral( const EnumerationLiteral &that ):
  IntegerLiteral(that),
  myTypeInfo( that.myTypeInfo ? dynamic_cast<EnumerationTypeInfo *>(that.myTypeInfo->clone()) : 0 ){}

EnumerationLiteral::~EnumerationLiteral(){
  delete myTypeInfo;
  myTypeInfo = 0;
}

RValue &
EnumerationLiteral::operator=( const RValue &that ){
  val = that.getIntValue();
  return *this;
}

EnumerationLiteral &
EnumerationLiteral::operator=( const EnumerationLiteral &that ){
  myTypeInfo = dynamic_cast<const EnumerationTypeInfo *>(that.getTypeInfo().clone());
  val = that.getIntValue();
  return *this;
}

bool
operator==(const EnumerationLiteral &lhs, const EnumerationLiteral &rhs) {
  return (lhs.getIntValue() == rhs.getIntValue());
}

Value
EnumerationLiteral::vhdlAnd( const RValue &rhs ) const {
  return toBoolean( getIntValue() && rhs.getIntValue());
}

Value
EnumerationLiteral::vhdlOr( const RValue &rhs ) const {
  return toBoolean( getIntValue() || rhs.getIntValue() );
}

Value
EnumerationLiteral::vhdlNot() const {
  return toBoolean( !getIntValue() );
}

Value
EnumerationLiteral::vhdlNand( const RValue &rhs ) const {
  return toBoolean( vhdlAnd(rhs).vhdlNot() );
}

Value
EnumerationLiteral::vhdlNor(const RValue &rhs ) const {
  return toBoolean( !(getIntValue() || rhs.getIntValue()) );
}

Value
EnumerationLiteral::vhdlXor( const RValue &rhs ) const {
  bool retval = true;
  if( getIntValue() && rhs.getIntValue() ){
    retval = false;
  }
  else if ( (!getIntValue() && !rhs.getIntValue()) ){
    retval = false;
  }
  
  return toBoolean( retval );
}

Value
EnumerationLiteral::vhdlXnor( const RValue &rhs ) const {
  return toBoolean( !vhdlXor(rhs).getIntValue() );
}

Value
EnumerationLiteral::vhdlUnaryPlus( ) const {
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()), 
			     getIntValue() );
}

Value
EnumerationLiteral::vhdlUnaryMinus( ) const {
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()), 
			     getIntValue() * -1 );
}

Value
EnumerationLiteral::vhdlPlus( const RValue &rhs ) const {
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()), 
			     getIntValue() + rhs.getIntValue() );
}

Value
EnumerationLiteral::vhdlMinus( const RValue &rhs ) const {
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()),
			     getIntValue() - rhs.getIntValue() );
}

Value
EnumerationLiteral::vhdlMultiply( const RValue &rhs ) const {
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()),
			     getIntValue() * rhs.getIntValue() );
}

Value
EnumerationLiteral::vhdlDivide( const RValue &rhs ) const {
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()),
			     getIntValue() / rhs.getIntValue() );
}

Value
EnumerationLiteral::vhdlMod( const RValue &rhs ) const {
  int lval = getIntValue();
  int rval = rhs.getIntValue();
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()),
			     (lval*rval < 0) ? (rval + lval%rval) : (lval%rval));
}

Value
EnumerationLiteral::vhdlRem( const RValue &rhs ) const {
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()), 
			     getIntValue() % rhs.getIntValue() );
}

Value
EnumerationLiteral::vhdlPow( const RValue &rhs ) const {
  return EnumerationLiteral( dynamic_cast<const EnumerationTypeInfo&>(getTypeInfo()),
			     (int)::pow(getDoubleValue(), rhs.getDoubleValue() ));
}

Value
EnumerationLiteral::vhdlConcatenate( const RValue &rhs ) const {
  TyvisVarArgs args(RVALUE);
  args << *this << rhs;
  return Aggregate( args );
}

const EnumerationLiteral &
EnumerationLiteral::leftValue() {
  ASSERT( myTypeInfo != 0 );
  return dynamic_cast<const EnumerationLiteral &>(myTypeInfo->left());
}

ostream&
operator<<(ostream &os, const EnumerationLiteral &var) {
  if (var.getTypeInfo().getImageMap() == std_standard_characterTypeInfo().getImageMap()) {
    // Okay. This is a character type so use the default stuff, please!!
    os << (char)var.getIntValue();
  }
  else {
    os << var.getTypeInfo().getImageMap()[var.getIntValue()];
  }

  return os;
}

const TypeInfo &
EnumerationLiteral::getTypeInfo() const {
  ASSERT( myTypeInfo != 0 );
  return *myTypeInfo;
}

RValue *
EnumerationLiteral::clone() const {
  EnumerationLiteral *retval = new EnumerationLiteral( *this );
  return retval;
}

const string &
EnumerationLiteral::getDataType() const {
  return getEnumerationLiteralDataType();
}

const string &
EnumerationLiteral::getEnumerationLiteralDataType(){
  static const string retval("EnumerationLiteral");
  return retval;
}

void
EnumerationLiteral::serialize( SerializedInstance * ) const{
  cerr << "EnumerationLiteral::serialize not yet implemented!" << endl;
  abort();
}

void
EnumerationLiteral::print( ostream &os ) const {
  os << toString();
}

void
EnumerationLiteral::vcdPrint( ostream &os ) const {
  string lit = toString();
  if (lit[0] == '\'')
    os << lit.substr(1,lit.size()-2);
  else if (lit == "FALSE")
    os << "0";
  else if (lit == "TRUE")
    os << "1";
  else {
    os << "'" << lit << "'";
  }
}

string 
EnumerationLiteral::toString() const {
  ASSERT( myTypeInfo != 0 );
  return myTypeInfo->getLiteral( getIntValue() );
}

void
EnumerationLiteral::debugPrintDetails( ostream &os, int indent ) const {
  IntegerLiteral::debugPrintDetails( os, indent);
  os << NL(indent) << "EnumerationTypeInfo *myTypeInfo: ";
  if (myTypeInfo)
    myTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";

  return;
}
