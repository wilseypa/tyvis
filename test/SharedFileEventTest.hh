#ifndef SHARED_FILE_EVENT_TEST_HH
#define SHARED_FILE_EVENT_TEST_HH

#include <cppunit/extensions/HelperMacros.h>
#include "VHDLEventTest.hh"

class SharedFileEvent;

class SharedFileEventTest : public VHDLEventTest {
  CPPUNIT_TEST_SUITE( SharedFileEventTest);
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testSerialization );
  CPPUNIT_TEST( testEventCompare );
  CPPUNIT_TEST_SUITE_END();

private:
  SharedFileEvent *toTest1;
  SharedFileEvent *toTest2;

  static const string &getDefaultData();


public:
  void setUp();
  void tearDown();

  void testConstructor( );
  void testSerialization();
  void testEventCompare();

};

#endif
