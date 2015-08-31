#ifndef ENUMERATION_TYPE_INFO_TEST_HH
#define ENUMERATION_TYPE_INFO_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------


#include "TypeInfoTest.hh"
#include <string>
using std::string;
#include <vector>
using std::vector;

class EnumerationTypeInfo;

class EnumerationTypeInfoTest : public TypeInfoTest {
  CPPUNIT_TEST_SUITE( EnumerationTypeInfoTest );
  CPPUNIT_TEST( testCopyConstructor );
  CPPUNIT_TEST( testCreateObject );
  CPPUNIT_TEST( testIsCompositeType );
  CPPUNIT_TEST( testIsPhysicalType );
  CPPUNIT_TEST( testIsScalarType );
  CPPUNIT_TEST( testOperatorEqual );
  CPPUNIT_TEST( testValue1 );
  CPPUNIT_TEST( testValue2 );
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
  void testValue1();
  void testValue2();

  static const EnumerationTypeInfo &getColorInfo();
  static const EnumerationTypeInfo &getBitInfo();
  
private:
  static const vector<string> &getBitLiterals();
  static const vector<string> &getColorLiterals();
  const EnumerationTypeInfo *toTest;
};
#endif
