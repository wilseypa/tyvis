#ifndef FILE_OPEN_STATUS_TEST_HH
#define FILE_OPEN_STATUS_TEST_HH

#include <cppunit/extensions/HelperMacros.h>
#include "VHDLDataTest.hh"
#include "tyvis/_savant_file_open_status.hh"

typedef _savant_file_open_status FileOpenStatus;

class FileOpenStatusTest : public VHDLDataTest {
  CPPUNIT_TEST_SUITE( FileOpenStatusTest);
  CPPUNIT_TEST(testGetUniversalKind);
  CPPUNIT_TEST(testSize);
  CPPUNIT_TEST(testPrint);
  CPPUNIT_TEST(testEqual);
  CPPUNIT_TEST(testNotEqual);
  CPPUNIT_TEST(testGreater);
  CPPUNIT_TEST(testGreaterEqual);
  CPPUNIT_TEST(testLess);
  CPPUNIT_TEST(testLessEqual);
  CPPUNIT_TEST(testClone);
  CPPUNIT_TEST(testSerialization);
  CPPUNIT_TEST_SUITE_END();

private:
  FileOpenStatus *toTest;

  static int getDefaultValue();

public:
  void setUp();
  void tearDown();

  void testGetUniversalKind();
  void testSize();
  void testPrint();
  void testEqual();
  void testNotEqual();
  void testGreater();
  void testGreaterEqual();
  void testLess();
  void testLessEqual();
  void testClone();
  void testSerialization();
};

#endif
