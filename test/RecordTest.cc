//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "RecordTest.hh"
#include "tyvis/Record.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/RecordTypeInfo.hh"
#include "tyvis/std_standardPkg.hh"
#include "RecordTypeInfoTest.hh"
#include <StringUtilities.h>
#include <sstream>
using std::ostringstream;

void 
RecordTest::setUp(){}

void 
RecordTest::tearDown(){}

void
RecordTest::testConstructor(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  CPPUNIT_ASSERT( r1.length().getIntValue() == 2 );
  CPPUNIT_ASSERT( r1.getField(0) == std_standard_integerTypeInfo().left() );
  CPPUNIT_ASSERT( r1.getField(1) == VHDL_STRING("") );
}

void
RecordTest::testCopyConstructor(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  Record r2( r1 );
  CPPUNIT_ASSERT( r1 == r2 );
}

void
RecordTest::testLength(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  CPPUNIT_ASSERT( r1.length().getIntValue() == 2 );
}

void
RecordTest::testOperatorEqualRValue(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  r1.getField(0) = UniversalInteger(42);
  r1.getField(1) = VHDL_STRING("string value");

  Record r2( RecordTypeInfoTest::work_recordTypeInfo() );
  r2.getField(0) = UniversalInteger(42);
  r2.getField(1) = VHDL_STRING("string value");

  CPPUNIT_ASSERT( r1 == static_cast<const RValue &>(r2) );
}

void
RecordTest::testPrint(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  r1.getField(0) = UniversalInteger(42);
  r1.getField(1) = VHDL_STRING("string value");

  ostringstream os;
  r1.print( os );
  string expected = "(foo=>42, bar=>string value)";
  CPPUNIT_ASSERT( os.str() == expected );
}

void
RecordTest::testEqual(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  r1.getField(0) = UniversalInteger(42);
  r1.getField(1) = VHDL_STRING("string value");

  Record r2( RecordTypeInfoTest::work_recordTypeInfo() );
  r2.getField(0) = UniversalInteger(42);
  r2.getField(1) = VHDL_STRING("string value");

  CPPUNIT_ASSERT( r1 == r2 );
}

void
RecordTest::testNotEqual(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  r1.getField(0) = UniversalInteger(42);
  r1.getField(1) = VHDL_STRING("string value");

  Record r2( RecordTypeInfoTest::work_recordTypeInfo() );
  r2.getField(0) = UniversalInteger(42);
  r2.getField(1) = VHDL_STRING("string value!");

  CPPUNIT_ASSERT( r1 != r2 );
}

void
RecordTest::testGreater(){
  abort();
}

void
RecordTest::testGreaterEqual(){
  abort();
}

void
RecordTest::testLess(){
  abort();
}

void
RecordTest::testLessEqual(){
  abort();
}

void
RecordTest::testClone(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  r1.getField(0) = UniversalInteger(42);
  r1.getField(1) = VHDL_STRING("string value");

  RValue *r2 = r1.clone();
  CPPUNIT_ASSERT( r1 == *r2 );
  delete r2;  
}

void
RecordTest::testSerialization(){
//   Record a( getStringInfo(), 0 );

//   SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
//   CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
//   Record *deserialized = dynamic_cast<Record *>( serialized->deserialize() );
//   CPPUNIT_ASSERT( deserialized != 0 );
//   CPPUNIT_ASSERT( deserialized != toTest );
//   CPPUNIT_ASSERT( *deserialized == *toTest );
//   delete deserialized;
}

void
RecordTest::testToString(){
  Record r1( RecordTypeInfoTest::work_recordTypeInfo() );
  r1.getField(0) = UniversalInteger(42);
  r1.getField(1) = VHDL_STRING("string value");

  string expected = "(foo=>42, bar=>string value)";
  CPPUNIT_ASSERT( r1.toString() == expected );
}
