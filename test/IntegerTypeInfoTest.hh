#ifndef INTEGER_TYPE_INFO_TEST_HH
#define INTEGER_TYPE_INFO_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include <cppunit/extensions/HelperMacros.h>
#include "TypeInfoTest.hh"

class IntegerTypeInfoTest : public TypeInfoTest {
  CPPUNIT_TEST_SUITE( IntegerTypeInfoTest );
  CPPUNIT_TEST( testCopyConstructor );
  CPPUNIT_TEST( testCreateObject );
  CPPUNIT_TEST( testIsCompositeType );
  CPPUNIT_TEST( testIsPhysicalType );
  CPPUNIT_TEST( testIsScalarType );
  CPPUNIT_TEST( testOperatorEqual );
  CPPUNIT_TEST( testAscending );
  CPPUNIT_TEST( testHigh );
  CPPUNIT_TEST( testLow );
  CPPUNIT_TEST( testAscending );  
  CPPUNIT_TEST( testValue1 );
  CPPUNIT_TEST( testValue2 );
  CPPUNIT_TEST( testPos );
  CPPUNIT_TEST( testVal );
  CPPUNIT_TEST( testSucc );
  CPPUNIT_TEST( testPred );
  CPPUNIT_TEST( testLeftof );
  CPPUNIT_TEST( testRightof );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  // TyvisUnitTests
  void testCopyConstructor();

  // TypeInfo tests
  void testCreateObject();
  void testIsCompositeType();
  void testIsPhysicalType();
  void testIsScalarType();
  void testOperatorEqual();
  void testHigh();
  void testLow();
  void testAscending();  
  void testValue1();
  void testValue2();
  void testPos();
  void testVal();
  void testSucc();
  void testPred();
  void testLeftof();
  void testRightof();
};
#endif
