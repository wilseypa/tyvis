//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "RecordTypeInfoTest.hh"
#include "tyvis/RecordTypeInfo.hh"
#include "tyvis/Record.hh"
#include "tyvis/std_standardPkg.hh"

const ElementInfo &
RecordTypeInfoTest::getElementInfo1(){
  static ElementInfo *retval = new ElementInfo( std_standard_integerTypeInfo(),
						"foo" );
  return *retval;  
}

const ElementInfo &
RecordTypeInfoTest::getElementInfo2(){
  static ElementInfo *retval = new ElementInfo( std_standard_stringTypeInfo(),
						"bar" );
  return *retval;
}

const RecordTypeInfo &
RecordTypeInfoTest::work_recordTypeInfo(){
  static RecordTypeInfo _work_recordTypeInfo;
  if( _work_recordTypeInfo.length() == 0 ){
    _work_recordTypeInfo.addField( getElementInfo1() );
    _work_recordTypeInfo.addField( getElementInfo2() );
  }
  return _work_recordTypeInfo;
}


void
RecordTypeInfoTest::setUp(){
  toTest = new RecordTypeInfo();
  toTest->addField( getElementInfo1() );
  toTest->addField( getElementInfo2() );
}

void
RecordTypeInfoTest::tearDown(){
  delete toTest;
}

void
RecordTypeInfoTest::testCopyConstructor(){
  RecordTypeInfo t1( *toTest );
  CPPUNIT_ASSERT( t1.length() == toTest->length() );
  for( int i = 0; i < t1.length(); i++ ){
    CPPUNIT_ASSERT( t1.getFieldInfo( i ) == toTest->getFieldInfo( i ) );
  }
}

void
RecordTypeInfoTest::testCreateObject(){
  RValue *object = toTest->create(0);
  Record *asRecord = dynamic_cast<Record*>(object);
  CPPUNIT_ASSERT( asRecord != 0 );
  CPPUNIT_ASSERT( asRecord->length().getIntValue() == 2 );
  CPPUNIT_ASSERT( asRecord->getField( 0 ).getTypeInfo() == std_standard_integerTypeInfo() );
  CPPUNIT_ASSERT( asRecord->getField( 1 ).getTypeInfo() == std_standard_stringTypeInfo() );
  
  delete object;
}

void
RecordTypeInfoTest::testIsCompositeType(){
  CPPUNIT_ASSERT( toTest->isCompositeType() );
}

void
RecordTypeInfoTest::testIsPhysicalType(){
  CPPUNIT_ASSERT( !toTest->isPhysicalType() );
}

void
RecordTypeInfoTest::testIsScalarType(){
  CPPUNIT_ASSERT( !toTest->isScalarType() );
}

void
RecordTypeInfoTest::testOperatorEqual(){
//   RecordTypeInfo t1( 1, RecordInfo::downto, -1 );
//   RecordTypeInfo t2;
//   t2 = t1;
//   CPPUNIT_ASSERT( t2.left() == 1 );
//   CPPUNIT_ASSERT( t2.getDirection() == RecordInfo::downto );
//   CPPUNIT_ASSERT( t2.right() == -1 );
}
