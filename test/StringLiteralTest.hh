#ifndef STRING_LITERAL_TEST_HH
#define STRING_LITERAL_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "SliceTest.hh"

#include <vector>
using std::vector;
#include <string>
using std::string;

class StringLiteral;
class ArrayTypeInfo;

class StringLiteralTest : public SliceTest {
  CPPUNIT_TEST_SUITE( StringLiteralTest);
  CPPUNIT_TEST(testClone);
  CPPUNIT_TEST(testConcatenate);
  CPPUNIT_TEST(testCopyConstructor);
  CPPUNIT_TEST(testEqual);
  CPPUNIT_TEST(testNotEqual);
  CPPUNIT_TEST(testOperatorEqualRValue);
  CPPUNIT_TEST(testPrint);
  CPPUNIT_TEST(testSerialization);
  CPPUNIT_TEST(testToString);
  CPPUNIT_TEST(testGetSlice);
  CPPUNIT_TEST(testLeft);
  CPPUNIT_TEST(testRight);
  CPPUNIT_TEST(testHigh);
  CPPUNIT_TEST(testLow);
  CPPUNIT_TEST(testRange);
  CPPUNIT_TEST(testReverseRange);
  CPPUNIT_TEST(testLength);
  CPPUNIT_TEST(testAscending);
  CPPUNIT_TEST_SUITE_END();

private:
  static const ArrayTypeInfo &getStringInfo();

public:
  void setUp();
  void tearDown();

  void testClone();
  void testConcatenate();
  void testCopyConstructor();
  void testEqual();
  void testNotEqual();
//   void testMinus();
//   void testMultiply();
//   void testPlus();
  void testOperatorEqualRValue();
  void testPrint();
  void testSerialization();
  void testToString();
  void testGetSlice();

  /** These are unused. */
  void testGreater();
  void testGreaterEqual();
  void testLess();
  void testLessEqual();

  /**
     
  */
  void testLeft();
  void testRight();
  void testHigh();
  void testLow();
  void testRange();
  void testReverseRange();
  void testLength();
  void testAscending();
};

#endif
