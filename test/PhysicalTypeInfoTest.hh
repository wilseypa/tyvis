#ifndef PHYSICAL_TYPE_INFO_TEST_HH
#define PHYSICAL_TYPE_INFO_TEST_HH

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

class PhysicalTypeInfo;

class PhysicalTypeInfoTest : public TypeInfoTest {
  CPPUNIT_TEST_SUITE( PhysicalTypeInfoTest );
  CPPUNIT_TEST( testCopyConstructor );
  CPPUNIT_TEST( testCreateObject );
  CPPUNIT_TEST( testIsCompositeType );
  CPPUNIT_TEST( testIsPhysicalType );
  CPPUNIT_TEST( testIsScalarType );
  CPPUNIT_TEST( testOperatorEqual );
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

  static const vector<UnitDeclaration> &getTestUnits();
  
private:
  const PhysicalTypeInfo *toTest;
};
#endif
