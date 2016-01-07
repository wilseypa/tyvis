//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "EnumerationTypeInfoTest.hh"
#include "EnumerationLiteralTest.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/EnumerationTypeInfo.hh"
#include "tyvis/UniversalInteger.hh"
#include <SerializedInstance.h>
#include <StringUtilities.h>
#include <sstream>
using std::ostringstream;

void 
EnumerationLiteralTest::setUp(){}

void 
EnumerationLiteralTest::tearDown(){}

void
EnumerationLiteralTest::testCopyConstructor(){
  EnumerationLiteral l1( EnumerationTypeInfoTest::getBitInfo(), 0 );
  EnumerationLiteral l2( l1 );
  CPPUNIT_ASSERT( l1 == l2 );
}

void
EnumerationLiteralTest::testLength(){
  EnumerationLiteral l1( EnumerationTypeInfoTest::getBitInfo(), 0 );
  CPPUNIT_ASSERT( l1.length(UniversalInteger(1)).getIntValue() == 1 );
}

void
EnumerationLiteralTest::testPrint(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );

  ostringstream printStream;
  a.print( printStream );
  CPPUNIT_ASSERT( printStream.str() == "'0'" );
}

void
EnumerationLiteralTest::testEqual(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );
  EnumerationLiteral b( EnumerationTypeInfoTest::getBitInfo(), 0 );
  
  CPPUNIT_ASSERT( a == b );
  CPPUNIT_ASSERT( a.vhdlEqual(b) == EnumerationLiteral::getEnumerationTrue() );
}

void
EnumerationLiteralTest::testNotEqual(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );
  EnumerationLiteral b( EnumerationTypeInfoTest::getBitInfo(), 1 );
  
  CPPUNIT_ASSERT( a != b );
}

void
EnumerationLiteralTest::testGreater(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );
  EnumerationLiteral b( EnumerationTypeInfoTest::getBitInfo(), 1 );
    
  CPPUNIT_ASSERT( b > a );
}

void
EnumerationLiteralTest::testGreaterEqual(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );
  EnumerationLiteral b( EnumerationTypeInfoTest::getBitInfo(), 1 );
  EnumerationLiteral c( EnumerationTypeInfoTest::getBitInfo(), 1 );
  
  CPPUNIT_ASSERT( b >= a );
  CPPUNIT_ASSERT( b >= c );
}

void
EnumerationLiteralTest::testLess(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );
  EnumerationLiteral b( EnumerationTypeInfoTest::getBitInfo(), 1 );
  
  CPPUNIT_ASSERT( a < b );
}

void
EnumerationLiteralTest::testLessEqual(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );
  EnumerationLiteral b( EnumerationTypeInfoTest::getBitInfo(), 1 );
  EnumerationLiteral c( EnumerationTypeInfoTest::getBitInfo(), 1 );
  
  CPPUNIT_ASSERT( a <= c );
  CPPUNIT_ASSERT( b <= c );
}

void
EnumerationLiteralTest::testPlus(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );
  CPPUNIT_ASSERT( a + 1 == EnumerationLiteral( EnumerationTypeInfoTest::getBitInfo(), 1 ) );
}

void
EnumerationLiteralTest::testMinus(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 1 );
  CPPUNIT_ASSERT( a - 1 == EnumerationLiteral( EnumerationTypeInfoTest::getBitInfo(), 0 ) );
}

void
EnumerationLiteralTest::testMultiply(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 1 );
  CPPUNIT_ASSERT( a * a == EnumerationLiteral( EnumerationTypeInfoTest::getBitInfo(), 1 ) );
  CPPUNIT_ASSERT( a * 1 == EnumerationLiteral( EnumerationTypeInfoTest::getBitInfo(), 1 ) );
  CPPUNIT_ASSERT( a * 0 == EnumerationLiteral( EnumerationTypeInfoTest::getBitInfo(), 0 ) );
}

void
EnumerationLiteralTest::testOperatorEqualRValue(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 1 );
  EnumerationLiteral b;
  b = a;
  CPPUNIT_ASSERT( b == a );
  a = UniversalInteger(0);
  CPPUNIT_ASSERT( a.getIntValue() == 0 );
  CPPUNIT_ASSERT( a.toString() == "'0'" );
}

void
EnumerationLiteralTest::testDivide(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 1 );
  CPPUNIT_ASSERT( a / a == EnumerationLiteral( EnumerationTypeInfoTest::getBitInfo(), 1 ) );
  CPPUNIT_ASSERT( a / 1 == EnumerationLiteral( EnumerationTypeInfoTest::getBitInfo(), 1 ) );
  CPPUNIT_ASSERT( 0 / a == EnumerationLiteral( EnumerationTypeInfoTest::getBitInfo(), 0 ) );
}

void
EnumerationLiteralTest::testClone(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );

  EnumerationLiteral *cloned = dynamic_cast<EnumerationLiteral *>(a.clone());
  CPPUNIT_ASSERT( cloned != 0 );
  CPPUNIT_ASSERT( cloned != &a );
  CPPUNIT_ASSERT( *cloned == a );
  delete cloned;
}

void
EnumerationLiteralTest::testSerialization(){
//   EnumerationLiteral a( getBitInfo(), 0 );

//   SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
//   CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
//   EnumerationLiteral *deserialized = dynamic_cast<EnumerationLiteral *>( serialized->deserialize() );
//   CPPUNIT_ASSERT( deserialized != 0 );
//   CPPUNIT_ASSERT( deserialized != toTest );
//   CPPUNIT_ASSERT( *deserialized == *toTest );
//   delete deserialized;
}

void
EnumerationLiteralTest::testToString(){
  EnumerationLiteral a( EnumerationTypeInfoTest::getBitInfo(), 0 );
  EnumerationLiteral b( EnumerationTypeInfoTest::getBitInfo(), 1 );
  
  CPPUNIT_ASSERT( a.toString() == "'0'" );
  CPPUNIT_ASSERT( b.toString() == "'1'" );
}
