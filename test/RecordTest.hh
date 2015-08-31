#ifndef RECORD_TEST_HH
#define RECORD_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "CompositeLiteralTest.hh"

#include <vector>
using std::vector;
#include <string>
using std::string;

class Record;
class ArrayTypeInfo;

class RecordTest : public CompositeLiteralTest {
  CPPUNIT_TEST_SUITE( RecordTest);
  CPPUNIT_TEST(testClone);
  CPPUNIT_TEST(testConstructor);
  CPPUNIT_TEST(testCopyConstructor);
  CPPUNIT_TEST(testEqual);
  CPPUNIT_TEST(testNotEqual);
  CPPUNIT_TEST(testLength);
  CPPUNIT_TEST(testOperatorEqualRValue);
  CPPUNIT_TEST(testPrint);
  CPPUNIT_TEST(testSerialization);
  CPPUNIT_TEST( testToString );
  CPPUNIT_TEST_SUITE_END();

private:
  

public:
  void setUp();
  void tearDown();

  void testClone();
  void testConstructor();
  void testCopyConstructor();
  void testEqual();
  void testNotEqual();
  void testLength();
//   void testMinus();
//   void testMultiply();
//   void testPlus();
  void testOperatorEqualRValue();
  void testPrint();
  void testSerialization();
  void testToString();

  /** These are unused. */
  void testGreater();
  void testGreaterEqual();
  void testLess();
  void testLessEqual();

};

#endif
