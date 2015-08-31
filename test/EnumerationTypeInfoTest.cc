#include "EnumerationTypeInfoTest.hh"
#include "tyvis/ArrayInfo.hh"
#include "tyvis/EnumerationTypeInfo.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/std_standardPkg.hh"

//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------


const vector<string> &
EnumerationTypeInfoTest::getColorLiterals(){
  static vector<string> *literals = new vector<string>;
  if( literals->size() == 0 ){
    literals->push_back( "red" );
    literals->push_back( "green" );
    literals->push_back( "blue" );
  }
  return *literals;
}

const vector<string> &
EnumerationTypeInfoTest::getBitLiterals(){
  static vector<string> bitLiterals;
  if( bitLiterals.size() == 0 ){
    bitLiterals.push_back("'0'");
    bitLiterals.push_back("'1'");
  }
  return bitLiterals;
}

const EnumerationTypeInfo &
EnumerationTypeInfoTest::getBitInfo(){
  static EnumerationTypeInfo bitInfo( getBitLiterals() );
  return bitInfo;
}

const EnumerationTypeInfo &
EnumerationTypeInfoTest::getColorInfo(){
  static EnumerationTypeInfo colorInfo( getColorLiterals() );
  return colorInfo;
}

void
EnumerationTypeInfoTest::setUp(){
  toTest = new EnumerationTypeInfo( getColorLiterals() );
}

void
EnumerationTypeInfoTest::tearDown(){
  delete toTest;
}

void
EnumerationTypeInfoTest::testCopyConstructor(){
  EnumerationTypeInfo t1( *toTest );

  CPPUNIT_ASSERT( t1.left() == toTest->left() );
  CPPUNIT_ASSERT( t1.getDirection() == toTest->getDirection() );
  CPPUNIT_ASSERT( t1.right() == toTest->right() );
  CPPUNIT_ASSERT( t1.getImageMap() == toTest->getImageMap() );
}

void
EnumerationTypeInfoTest::testCreateObject(){
  RValue *object = toTest->create(0);
  EnumerationLiteral *asEnum = dynamic_cast<EnumerationLiteral *>(object);
  CPPUNIT_ASSERT( asEnum != 0 );
  delete object;
}

void
EnumerationTypeInfoTest::testIsCompositeType(){
 CPPUNIT_ASSERT( !toTest->isCompositeType() );
}

void
EnumerationTypeInfoTest::testIsPhysicalType(){
  CPPUNIT_ASSERT( !toTest->isPhysicalType() );
}

void
EnumerationTypeInfoTest::testIsScalarType(){
  CPPUNIT_ASSERT( toTest->isScalarType() );
}

void
EnumerationTypeInfoTest::testOperatorEqual(){
  CPPUNIT_ASSERT( toTest->left().getIntValue() == 0 );
  CPPUNIT_ASSERT( toTest->getDirection() == ArrayInfo::to );
  CPPUNIT_ASSERT( toTest->right().getIntValue() == 2 );
}

void
EnumerationTypeInfoTest::testValue1(){
  CPPUNIT_ASSERT( getColorInfo().value( VHDL_STRING("red" ) ) == 
		  EnumerationLiteral( getColorInfo(), 0 ) );
}

void
EnumerationTypeInfoTest::testValue2(){
  CPPUNIT_ASSERT( getColorInfo().value( "red" ) ==  EnumerationLiteral( getColorInfo(), 0 ) );
}
