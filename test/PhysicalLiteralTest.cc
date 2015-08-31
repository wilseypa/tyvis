//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "PhysicalLiteralTest.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/PhysicalLiteral.hh"
#include "tyvis/PhysicalTypeInfo.hh"
#include <warped/SerializedInstance.h>
#include <clutils/StringUtilities.h>
#include "PhysicalTypeInfoTest.hh"
#include <sstream>
using std::ostringstream;

void 
PhysicalLiteralTest::setUp(){}

void 
PhysicalLiteralTest::tearDown(){}

const PhysicalTypeInfo &
PhysicalLiteralTest::getLengthInfo(){
  static PhysicalTypeInfo sizeInfo( PhysicalTypeInfoTest::getTestUnits() );
  return sizeInfo;
}

void
PhysicalLiteralTest::testCopyConstructor(){
  PhysicalLiteral l1( getLengthInfo(), 0 );
  PhysicalLiteral l2( l1 );
  CPPUNIT_ASSERT( l1 == l2 );
}

void
PhysicalLiteralTest::testLength(){
  PhysicalLiteral l1( getLengthInfo(), 0 );
  CPPUNIT_ASSERT( l1.length(UniversalInteger(1)).getIntValue() == 1 );
}

void
PhysicalLiteralTest::testPrint(){
  PhysicalLiteral a( getLengthInfo(), 0 );

  ostringstream printStream;
  a.print( printStream );
  CPPUNIT_ASSERT( printStream.str() == "0 in" );
}

void
PhysicalLiteralTest::testEqual(){
  PhysicalLiteral a( getLengthInfo(), 0 );
  PhysicalLiteral b( getLengthInfo(), 0 );
  
  CPPUNIT_ASSERT( a == b );
  CPPUNIT_ASSERT( a.vhdlEqual(b) == EnumerationLiteral::getEnumerationTrue() );

  PhysicalLiteral twelveInches( getLengthInfo(), 12, "in" );
  PhysicalLiteral oneFoot( getLengthInfo(), 1, "foot" );
  CPPUNIT_ASSERT( twelveInches == oneFoot );
  CPPUNIT_ASSERT( twelveInches.vhdlEqual(oneFoot) == EnumerationLiteral::getEnumerationTrue() );
  

}

void
PhysicalLiteralTest::testNotEqual(){
  PhysicalLiteral a( getLengthInfo(), 0 );
  PhysicalLiteral b( getLengthInfo(), 1 );
  
  CPPUNIT_ASSERT( a != b );
}

void
PhysicalLiteralTest::testGreater(){
  PhysicalLiteral a( getLengthInfo(), 0 );
  PhysicalLiteral b( getLengthInfo(), 1 );
    
  CPPUNIT_ASSERT( b > a );
}

void
PhysicalLiteralTest::testGreaterEqual(){
  PhysicalLiteral a( getLengthInfo(), 0 );
  PhysicalLiteral b( getLengthInfo(), 1 );
  PhysicalLiteral c( getLengthInfo(), 1 );
  
  CPPUNIT_ASSERT( b >= a );
  CPPUNIT_ASSERT( b >= c );
}

void
PhysicalLiteralTest::testLess(){
  PhysicalLiteral a( getLengthInfo(), 0 );
  PhysicalLiteral b( getLengthInfo(), 1 );
  
  CPPUNIT_ASSERT( a < b );
}

void
PhysicalLiteralTest::testLessEqual(){
  PhysicalLiteral a( getLengthInfo(), 0 );
  PhysicalLiteral b( getLengthInfo(), 1 );
  PhysicalLiteral c( getLengthInfo(), 1 );
  
  CPPUNIT_ASSERT( a <= c );
  CPPUNIT_ASSERT( b <= c );
}

void
PhysicalLiteralTest::testPlus(){
  PhysicalLiteral a( getLengthInfo(), 0 );
  CPPUNIT_ASSERT( a.vhdlPlus( UniversalInteger(1) ) == PhysicalLiteral( getLengthInfo(), 1 ) );
}

void
PhysicalLiteralTest::testMinus(){
  PhysicalLiteral a( getLengthInfo(), 1 );
  CPPUNIT_ASSERT(a.vhdlMinus( UniversalInteger(1)) == PhysicalLiteral( getLengthInfo(), 0 ) );
}

void
PhysicalLiteralTest::testMultiply(){
  PhysicalLiteral a( getLengthInfo(), 1 );
  CPPUNIT_ASSERT( a.vhdlMultiply( a ) == PhysicalLiteral( getLengthInfo(), 1 ) );
  CPPUNIT_ASSERT( a.vhdlMultiply(UniversalInteger(1)) == PhysicalLiteral( getLengthInfo(), 1 ) );
  CPPUNIT_ASSERT( a.vhdlMultiply(UniversalInteger(0)) == PhysicalLiteral( getLengthInfo(), 0 ) );
}

void
PhysicalLiteralTest::testOperatorEqualRValue(){
  PhysicalLiteral a( getLengthInfo(), 1 );
  PhysicalLiteral b;
  b = a;
  CPPUNIT_ASSERT( b == a );

  a = UniversalInteger( 2 );
  CPPUNIT_ASSERT( a.getInt64Value() == 2LL );
  CPPUNIT_ASSERT( a.getUnit() == "in" );
}


void
PhysicalLiteralTest::testDivide(){
  PhysicalLiteral a( getLengthInfo(), 1 );
  CPPUNIT_ASSERT( a.vhdlDivide(a) == PhysicalLiteral( getLengthInfo(), 1 ) );
  CPPUNIT_ASSERT( a.vhdlDivide(UniversalInteger(1)) == PhysicalLiteral( getLengthInfo(), 1 ) );
}

void
PhysicalLiteralTest::testClone(){
  PhysicalLiteral a( getLengthInfo(), 0 );

  PhysicalLiteral *cloned = dynamic_cast<PhysicalLiteral *>(a.clone());
  CPPUNIT_ASSERT( cloned != 0 );
  CPPUNIT_ASSERT( cloned != &a );
  CPPUNIT_ASSERT( *cloned == a );
  delete cloned;
}

void
PhysicalLiteralTest::testSerialization(){
//   PhysicalLiteral a( getLengthInfo(), 0 );

//   SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
//   CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
//   PhysicalLiteral *deserialized = dynamic_cast<PhysicalLiteral *>( serialized->deserialize() );
//   CPPUNIT_ASSERT( deserialized != 0 );
//   CPPUNIT_ASSERT( deserialized != toTest );
//   CPPUNIT_ASSERT( *deserialized == *toTest );
//   delete deserialized;
}

void
PhysicalLiteralTest::testToString(){
  PhysicalLiteral a( getLengthInfo(), 0 );
  PhysicalLiteral b( getLengthInfo(), 1 );
  
  CPPUNIT_ASSERT( a.toString() == "0 in" );
  CPPUNIT_ASSERT( b.toString() == "1 in" );
}
