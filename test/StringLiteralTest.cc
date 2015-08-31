//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "StringLiteralTest.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/StringLiteral.hh"
#include "tyvis/ArrayTypeInfo.hh"
#include "tyvis/std_standardPkg.hh"
#include <warped/SerializedInstance.h>
#include <clutils/StringUtilities.h>
#include <sstream>
using std::ostringstream;

void 
StringLiteralTest::setUp(){}

void 
StringLiteralTest::tearDown(){}

const ArrayTypeInfo &
StringLiteralTest::getStringInfo(){
  return std_standard_stringTypeInfo();
}

void
StringLiteralTest::testCopyConstructor(){
  StringLiteral l1( getStringInfo(), "foo" );
  StringLiteral l2( l1 );
  CPPUNIT_ASSERT( l1 == l2 );
}

void
StringLiteralTest::testConcatenate(){
  StringLiteral l1( getStringInfo(), "foo" );
  StringLiteral l2( getStringInfo(), "bar" );
  CPPUNIT_ASSERT( l1.vhdlConcatenate( l2 ) == VHDL_STRING("foobar") );
  EnumerationLiteral el( std_standard_characterTypeInfo(), 
			 std_standard_characterTypeInfo().getIndex("'b'") );

  CPPUNIT_ASSERT( l1.vhdlConcatenate( el ) == VHDL_STRING("foob") );
  
}

void
StringLiteralTest::testOperatorEqualRValue(){
  StringLiteral l1( getStringInfo(), "foo" );
  StringLiteral l2( getStringInfo(), "xxx" );
  l2 = l1;
  CPPUNIT_ASSERT( l2.toString() == "foo" );
}

void
StringLiteralTest::testPrint(){
  StringLiteral a( getStringInfo(), "foo" );

  ostringstream printStream;
  a.print( printStream );
  CPPUNIT_ASSERT( printStream.str() == "foo" );
}

void
StringLiteralTest::testEqual(){
  StringLiteral a( getStringInfo(), "foo" );
  StringLiteral b( getStringInfo(), "foo" );
  
  CPPUNIT_ASSERT( a == b );
  CPPUNIT_ASSERT( a.vhdlEqual(b) == EnumerationLiteral::getEnumerationTrue() );
}

void
StringLiteralTest::testNotEqual(){
  StringLiteral a( getStringInfo(), "foo" );
  StringLiteral b( getStringInfo(), "bar" );
  StringLiteral c( getStringInfo(), "fox" );
  
  CPPUNIT_ASSERT( a != b );
  CPPUNIT_ASSERT( a != c );
}

void
StringLiteralTest::testGreater(){
  abort();
}

void
StringLiteralTest::testGreaterEqual(){
  abort();
}

void
StringLiteralTest::testLess(){
  abort();
}

void
StringLiteralTest::testLessEqual(){
  abort();
}

void
StringLiteralTest::testClone(){
  StringLiteral a( getStringInfo(), "foo" );

  StringLiteral *cloned = dynamic_cast<StringLiteral *>(a.clone());
  CPPUNIT_ASSERT( cloned != 0 );
  CPPUNIT_ASSERT( cloned != &a );
  CPPUNIT_ASSERT( *cloned == a );
  delete cloned;
}

void
StringLiteralTest::testSerialization(){
//   StringLiteral a( getStringInfo(), 0 );

//   SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
//   CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
//   StringLiteral *deserialized = dynamic_cast<StringLiteral *>( serialized->deserialize() );
//   CPPUNIT_ASSERT( deserialized != 0 );
//   CPPUNIT_ASSERT( deserialized != toTest );
//   CPPUNIT_ASSERT( *deserialized == *toTest );
//   delete deserialized;
}

void
StringLiteralTest::testToString(){
  StringLiteral a( getStringInfo(), "foo" );
  StringLiteral b( getStringInfo(), "bar" );
  
  CPPUNIT_ASSERT( a.toString() == "foo" );
  CPPUNIT_ASSERT( b.toString() == "bar" );
}

void
StringLiteralTest::testGetSlice(){
  StringLiteral l1( getStringInfo(), "foobar" );
  Value slice1 = l1.getSlice( IntegerTypeInfo( 1, ArrayInfo::to, 3 ) );
  Value slice2 = l1.getSlice( IntegerTypeInfo( 4, ArrayInfo::to, 6 ) );
  CPPUNIT_ASSERT( slice1 == VHDL_STRING("foo") );
  CPPUNIT_ASSERT( slice2 == VHDL_STRING("bar") );
}

void
StringLiteralTest::testLeft(){
  StringLiteral l1( getStringInfo(), "foobar" );
  CPPUNIT_ASSERT( l1.left() == UniversalInteger(1) );
}

void
StringLiteralTest::testRight(){
  StringLiteral l1( getStringInfo(), "foobar" );
  CPPUNIT_ASSERT( l1.right() == UniversalInteger(6) );
}

void
StringLiteralTest::testHigh(){
  StringLiteral l1( getStringInfo(), "foobar" );
  CPPUNIT_ASSERT( l1.high() == UniversalInteger(6) );
}

void
StringLiteralTest::testLow(){
  StringLiteral l1( getStringInfo(), "foobar" );
  CPPUNIT_ASSERT( l1.low() == UniversalInteger(1) );
}

void
StringLiteralTest::testRange(){
  StringLiteral l1( getStringInfo(), "foobar" );
  CPPUNIT_ASSERT( l1.range().left() == UniversalInteger(1) );
  CPPUNIT_ASSERT( l1.range().getDirection() == ArrayInfo::to );
  CPPUNIT_ASSERT( l1.range().right() == UniversalInteger(6) );
}


void
StringLiteralTest::testReverseRange(){
  CPPUNIT_ASSERT( false );
}

void
StringLiteralTest::testLength(){
  StringLiteral l1( getStringInfo(), "foo" );
  CPPUNIT_ASSERT( l1.length().getIntValue() == 3 );
}

void
StringLiteralTest::testAscending(){
  StringLiteral l1( getStringInfo(), "foo" );
  CPPUNIT_ASSERT( l1.ascending() == EnumerationLiteral::getEnumerationTrue() );
}
