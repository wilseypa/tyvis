#ifndef ARRAY_TYPE_INFO_TEST_HH
#define ARRAY_TYPE_INFO_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------


#include "TypeInfoTest.hh"
#include "tyvis/UnitDeclaration.hh"
#include <string>
using std::string;
#include <vector>
using std::vector;

class ArrayTypeInfo;

class ArrayTypeInfoTest : public TypeInfoTest {
  CPPUNIT_TEST_SUITE( ArrayTypeInfoTest );
  CPPUNIT_TEST( testCopyConstructor );
  CPPUNIT_TEST( testCreateObject );
  CPPUNIT_TEST( testIsCompositeType );
  CPPUNIT_TEST( testIsConstrained );
  CPPUNIT_TEST( testIsPhysicalType );
  CPPUNIT_TEST( testIsScalarType );
  CPPUNIT_TEST( testLeft );
  CPPUNIT_TEST( testOperatorEqual );
  CPPUNIT_TEST( testRight );
  CPPUNIT_TEST( testStorageIndex );
  CPPUNIT_TEST( testValue1 );
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp();
  void tearDown();

  // TyvisUnitTests
  void testCopyConstructor();

  // TypeInfo tests
  void testCreateObject();
  void testIsCompositeType();
  void testIsConstrained();
  void testIsPhysicalType();
  void testIsScalarType();
  void testLeft();
  void testOperatorEqual();
  void testRight();
  void testStorageIndex();
  void testValue1();

private:
  const ArrayTypeInfo *toTestOneD;
};
#endif
