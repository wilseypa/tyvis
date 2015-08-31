#ifndef PHYSICAL_LITERAL_TEST_HH
#define PHYSICAL_LITERAL_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "VHDLDataTest.hh"

#include <vector>
using std::vector;
#include <string>
using std::string;

class PhysicalLiteral;
class PhysicalTypeInfo;

class PhysicalLiteralTest : public VHDLDataTest {
  CPPUNIT_TEST_SUITE( PhysicalLiteralTest);
  CPPUNIT_TEST(testClone);
  CPPUNIT_TEST(testCopyConstructor);
  CPPUNIT_TEST(testDivide);
  CPPUNIT_TEST(testEqual);
  CPPUNIT_TEST(testNotEqual);
  CPPUNIT_TEST(testLength);
  CPPUNIT_TEST(testGreater);
  CPPUNIT_TEST(testGreaterEqual);
  CPPUNIT_TEST(testLess);
  CPPUNIT_TEST(testLessEqual);
  CPPUNIT_TEST(testMinus);
  CPPUNIT_TEST(testMultiply);
  CPPUNIT_TEST(testOperatorEqualRValue);
  CPPUNIT_TEST(testPlus);
  CPPUNIT_TEST(testPrint);
  CPPUNIT_TEST(testSerialization);
  CPPUNIT_TEST( testToString );
  CPPUNIT_TEST_SUITE_END();

private:
  static const PhysicalTypeInfo &getLengthInfo();

public:
  void setUp();
  void tearDown();

  void testClone();
  void testCopyConstructor();
  void testDivide();
  void testEqual();
  void testNotEqual();
  void testLength();
  void testGreater();
  void testGreaterEqual();
  void testLess();
  void testLessEqual();
  void testMinus();
  void testMultiply();
  void testOperatorEqualRValue();
  void testPlus();
  void testPrint();
  void testSerialization();
  void testToString();
};

#endif
