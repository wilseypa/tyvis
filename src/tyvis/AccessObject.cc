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

#include "AccessObject.hh"
#include "AccessTypeInfo.hh"
#include "Aggregate.hh"
#include "SignalNetinfo.hh"
#include "std_standardPkg.hh"
#include "Types.hh"

static StringLiteral nullStr(VHDL_STRING(""));
void 
AccessObject::print(ostream& os) const {
  os << toString();
}

AccessObject::AccessObject( const AccessTypeInfo &initTypeInfo ) :
  myTypeInfo( dynamic_cast<AccessTypeInfo *>(initTypeInfo.clone()) ),
  curPos(0),
  val(&nullStr){}

AccessObject::AccessObject( const AccessTypeInfo &initTypeInfo , const RValue &initVal) :
  myTypeInfo( dynamic_cast<AccessTypeInfo *>(initTypeInfo.clone()) ),
  curPos(0),
  val(initVal.clone()){
}

AccessObject::AccessObject( const AccessObject &that ) : 
  LValue( that ),
  myTypeInfo( that.myTypeInfo ? dynamic_cast<AccessTypeInfo *>(that.myTypeInfo->clone()) : 0 ),
  curPos( that.curPos ){
  if( that.val != 0 ){
    val = that.val->clone();
  }
  else{
    val = &nullStr;
  }
}

AccessObject::AccessObject( const AccessTypeInfo &initTypeInfo, TyvisVarArgs &initValue ) :
  myTypeInfo( dynamic_cast<AccessTypeInfo *>(initTypeInfo.clone()) ),
  curPos(0),
  val(new Aggregate(dynamic_cast<const ArrayTypeInfo &>(myTypeInfo->getDesignatedType()), initValue)) {
}

AccessObject::~AccessObject(){
  if (val != &nullStr)
    delete val;
  val = &nullStr;
  delete myTypeInfo;
  myTypeInfo = 0;
}

AccessObject &
AccessObject::operator=( const AccessObject &that ) {
  myTypeInfo = that.myTypeInfo ? dynamic_cast<AccessTypeInfo *>(that.myTypeInfo->clone()) : 0;
  curPos = that.curPos;
  val = that.val;
  return *this;
}

void 
AccessObject::reset(){
  curPos = 0;
}

const RValue &
AccessObject::readVal() const{
  ASSERT( val != 0 );
  return *this;
}

RValue &
AccessObject::readVal(){
  ASSERT( val != 0 );
  return *const_cast<AccessObject *>(this);
}

RValue &
AccessObject::all() {
  ASSERT( val != 0 );
  return *const_cast<RValue *>(val);
}
 
const RValue &
AccessObject::all() const {
  ASSERT( val != 0 );
  return *val;
}
 
void
AccessObject::updateVal( const RValue &newVal ){
  delete val;
  val = newVal.clone();
}

RValue &
AccessObject::operator=(const RValue &newVal){
  val = newVal.clone();
  return *this;
}

RValue *
AccessObject::clone() const {
  return new AccessObject( *this );
}

string
AccessObject::toString() const { 
  ASSERT( val != 0 );
  return val->toString().substr(curPos);
}

bool
AccessObject::isCompositeType() const { 
  ASSERT( val != 0 );
  return val->isCompositeType();
}

bool
AccessObject::isScalarType() const { 
  ASSERT( val != 0 );
  return val->isScalarType();
}

bool
AccessObject::isPhysicalType() const { 
  ASSERT( val != 0 );
  return val->isPhysicalType();
}

const RValue &
AccessObject::operator[](int ind) const { 
  ASSERT( val != 0 );
  return (*val)[ind];
}

RValue &
AccessObject::operator[](int ind) { 
  ASSERT( val != 0 );
  return (*val)[ind];
}

const RValue &
AccessObject::operator[](const RValue& ind) const {
  ASSERT( val != 0 );
  return (*val)[ind];
}

RValue &
AccessObject::operator[](const RValue& ind) { 
  ASSERT( val != 0 );
  return (*val)[ind];
}

RValue &
AccessObject::getField(int ind) const { 
  ASSERT( val != 0 );
  return val->getField(ind);
}

RValue &
AccessObject::getField(int ind)  {
  ASSERT( val != 0 );
  return val->getField(ind);
}

const RValue & 
AccessObject::getField(const RValue& ind) const  {
  ASSERT( val != 0 );
  return val->getField(ind);
}

RValue &
AccessObject::getField(const RValue& ind) {
  ASSERT( val != 0 );
  return val->getField(ind);
}

const string &
AccessObject::getUnit() const {
  die(); 
}

LONG
AccessObject::getValue() const {
  die(); 
}

int
AccessObject::getUnits() const {
  die(); 
}

const TypeInfo &
AccessObject::getTypeInfo() const {
  ASSERT( myTypeInfo != 0 );
  return *myTypeInfo;
}

Value
AccessObject::vhdlUnaryPlus( ) const {
  die();
}

Value
AccessObject::vhdlUnaryMinus( ) const {
  die();
}

Value
AccessObject::vhdlPlus( const RValue & ) const {
  die();
}


Value
AccessObject::vhdlMinus( const RValue & ) const {
  die();
}

Value
AccessObject::vhdlMultiply( const RValue & ) const {
  die();
}

Value
AccessObject::vhdlDivide( const RValue & ) const {
  die();
}

Value
AccessObject::vhdlMod( const RValue & ) const {
  die();
}

Value
AccessObject::vhdlRem( const RValue & ) const {
  die();
}

Value
AccessObject::vhdlPow( const RValue & ) const {
  die();
}

const EnumerationLiteral &
AccessObject::vhdlEqual( const RValue &that ) const {
  ASSERT( val != 0 );
  return val->vhdlEqual( that );
}

const EnumerationLiteral &
AccessObject::vhdlNotEqual(const RValue &that) const {
  ASSERT( val != 0 );
  return val->vhdlNotEqual( that );
}

const EnumerationLiteral &
AccessObject::vhdlGreaterThan(const RValue &that) const {
  ASSERT( val != 0 );
  return val->vhdlGreaterThan( that );
}

const EnumerationLiteral &
AccessObject::vhdlGreaterThanEqual(const RValue &that) const {
  ASSERT( val != 0 );
  return val->vhdlGreaterThanEqual( that );
}

const EnumerationLiteral &
AccessObject::vhdlLessThan(const RValue &that) const {
  ASSERT( val != 0 );
  return val->vhdlLessThan( that );
}

const EnumerationLiteral &
AccessObject::vhdlLessThanEqual(const RValue &that) const {
  ASSERT( val != 0 );
  return val->vhdlLessThanEqual( that );
}

Value
AccessObject::vhdlConcatenate( const RValue &that ) const {
  ASSERT( val != 0 );
  AccessObject retval( getTypeInfo() );
  retval.updateVal( val->vhdlConcatenate( that ) );
  return retval;
}

SignalNetinfo &
AccessObject::addToFanOut( VHDLKernel * ){
  die();
  return *new SignalNetinfo;
}

SignalNetinfo &
AccessObject::addChild( SignalNetinfo &, VHDLKernel * ){
  die();
  return *new SignalNetinfo;
}

void 
AccessObject::deallocate( ){
  delete this;
}

void
AccessObject::debugPrintDetails( ostream &os, int indent ) const {
  os << NL(indent) << "curPos = " << curPos;
  os << NL(indent) << "AccessTypeInfo *myTypeInfo: ";
  if (myTypeInfo)
    myTypeInfo->debugPrint(os, indent+2);
  else
    os << " is null";

  os << NL(indent) << "RValue *val: ";
  if (val)
    val->debugPrint(os, indent+5);
  else
    os << " is null";

  return;
}

void 
AccessObject::die() const {
  cerr << "Illegal operation called on AccessObject - aborting()" << endl;
  abort();
}
