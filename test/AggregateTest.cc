//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "AggregateTest.hh"
#include "tyvis/Aggregate.hh"
#include "tyvis/ArrayTypeInfo.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/TyvisVarArgs.hh"
#include "tyvis/Value.hh"
#include "tyvis/std_standardPkg.hh"
#include <SerializedInstance.h>
#include <StringUtilities.h>
#include <sstream>
using std::ostringstream;

void 
AggregateTest::setUp(){}

void 
AggregateTest::tearDown(){}

const ArrayTypeInfo &
AggregateTest::getStringInfo(){
  return std_standard_stringTypeInfo();
}

void
AggregateTest::testConstructor(){
  Aggregate a1( TyvisVarArgs() <<
		UniversalInteger(0) <<
		UniversalInteger(1) <<
		UniversalInteger(2) );
  CPPUNIT_ASSERT( a1.length().getIntValue() == 3 );
  CPPUNIT_ASSERT( a1[1] == UniversalInteger(0) );
  CPPUNIT_ASSERT( a1[2] == UniversalInteger(1) );
  CPPUNIT_ASSERT( a1[3] == UniversalInteger(2) );
}

void
AggregateTest::testCopyConstructor(){
  Aggregate a1( TyvisVarArgs(  ) << UniversalInteger(0) << UniversalInteger(1) << UniversalInteger(2) );
  Aggregate a2( a1 );
  CPPUNIT_ASSERT( a2.length().getIntValue() == 3 );
  CPPUNIT_ASSERT( a2[1] == UniversalInteger(0) );
  CPPUNIT_ASSERT( a2[2] == UniversalInteger(1) );
  CPPUNIT_ASSERT( a2[3] == UniversalInteger(2) );
}

void
AggregateTest::testConcatenate(){
  Aggregate a1( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  Aggregate a2( TyvisVarArgs() << UniversalInteger(2) << UniversalInteger(3) );
  Value a3 = a1.vhdlConcatenate( a2 );

  CPPUNIT_ASSERT( a3.length().getIntValue() == 4 );
  CPPUNIT_ASSERT( a3[1] == UniversalInteger(0) );
  CPPUNIT_ASSERT( a3[2] == UniversalInteger(1) );
  CPPUNIT_ASSERT( a3[3] == UniversalInteger(2) );
  CPPUNIT_ASSERT( a3[4] == UniversalInteger(3) );
}

void
AggregateTest::testLength(){
  Aggregate a1( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  CPPUNIT_ASSERT( a1.length().getIntValue() == 2 );
  a1.append( UniversalInteger(2) );
  CPPUNIT_ASSERT( a1.length().getIntValue() == 3 );
}

void
AggregateTest::testOperatorEqualRValue(){
  Aggregate a1( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  Aggregate a2( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  const RValue &rvalue = a2;
  CPPUNIT_ASSERT( a1 == rvalue );
}

void
AggregateTest::testPrint(){
  Aggregate a1( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  ostringstream os1;
  a1.print(os1);
  CPPUNIT_ASSERT( os1.str() == "(0, 1)" );
  Aggregate a2( TyvisVarArgs() << UniversalInteger(1) <<
		Aggregate( TyvisVarArgs() << UniversalInteger(2) << UniversalInteger(3) ));
  ostringstream os2;
  a2.print(os2);
  CPPUNIT_ASSERT( os2.str() == "(1, (2, 3))" );
}

void
AggregateTest::testEqual(){
  Aggregate a1( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  Aggregate a2( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );

  CPPUNIT_ASSERT( a1 == a2 );
}

void
AggregateTest::testNotEqual(){
  Aggregate a1( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  Aggregate a2( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(0) );
  Aggregate a3( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(0) << UniversalInteger(0) );

  CPPUNIT_ASSERT( a1 != a2 );
  CPPUNIT_ASSERT( a2 != a3 );
}

void
AggregateTest::testGreater(){
  abort();
}

void
AggregateTest::testGreaterEqual(){
  abort();
}

void
AggregateTest::testLess(){
  abort();
}

void
AggregateTest::testLessEqual(){
  abort();
}

void
AggregateTest::testClone(){
  Aggregate a1( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  RValue *a2 = a1.clone();
  CPPUNIT_ASSERT( a2 != &a1 );
  CPPUNIT_ASSERT( *a2 == a1 );
}

void
AggregateTest::testSerialization(){
//   Aggregate a( getStringInfo(), 0 );

//   SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
//   CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
//   Aggregate *deserialized = dynamic_cast<Aggregate *>( serialized->deserialize() );
//   CPPUNIT_ASSERT( deserialized != 0 );
//   CPPUNIT_ASSERT( deserialized != toTest );
//   CPPUNIT_ASSERT( *deserialized == *toTest );
//   delete deserialized;
}

void
AggregateTest::testToString(){
  Aggregate a1( TyvisVarArgs() << UniversalInteger(0) << UniversalInteger(1) );
  CPPUNIT_ASSERT( a1.toString() == "(0, 1)" );
  Aggregate a2( TyvisVarArgs() << UniversalInteger(1) <<
		Aggregate( TyvisVarArgs() << UniversalInteger(2) << UniversalInteger(3) ));
  CPPUNIT_ASSERT( a2.toString() == "(1, (2, 3))" );
}
