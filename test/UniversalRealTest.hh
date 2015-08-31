#ifndef UNIVERSAL_REAL_TEST_HH
#define UNIVERSAL_REAL_TEST_HH

#include <cppunit/extensions/HelperMacros.h>
#include "VHDLDataTest.hh"

class UniversalReal;

class UniversalRealTest : public VHDLDataTest {
  CPPUNIT_TEST_SUITE( UniversalRealTest);
  CPPUNIT_TEST(testClone);
  CPPUNIT_TEST(testCopyConstructor);
  CPPUNIT_TEST(testEqual);
  CPPUNIT_TEST(testLength);
  CPPUNIT_TEST(testGreater);
  CPPUNIT_TEST(testGreaterEqual);
  CPPUNIT_TEST(testLess);
  CPPUNIT_TEST(testLessEqual);
  CPPUNIT_TEST(testNotEqual);
  CPPUNIT_TEST(testOperatorEqualRValue);
  CPPUNIT_TEST(testPrint);
  CPPUNIT_TEST(testSerialization);
  CPPUNIT_TEST(testToString);
  CPPUNIT_TEST_SUITE_END();

private:
  UniversalReal *toTest;

  static double getDefaultValue();

public:
  void setUp();
  void tearDown();

  void testClone();
  void testCopyConstructor();
  void testEqual();
  void testLength();
  void testGreater();
  void testGreaterEqual();
  void testLess();
  void testLessEqual();
  void testNotEqual();
  void testOperatorEqualRValue();
  void testPrint();
  void testSerialization();
  void testToString();
};

#endif
