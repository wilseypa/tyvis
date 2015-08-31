#ifndef WAIT_EVENT_TEST_HH
#define WAIT_EVENT_TEST_HH

#include <cppunit/extensions/HelperMacros.h>
#include "VHDLEventTest.hh"

class WaitEvent;

class WaitEventTest : public VHDLEventTest {
  CPPUNIT_TEST_SUITE( WaitEventTest);
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testSerialization );
  CPPUNIT_TEST( testEventCompare );
  CPPUNIT_TEST_SUITE_END();

private:
  WaitEvent *toTest1;

  static int getDefaultWaitId();


public:
  void setUp();
  void tearDown();

  void testConstructor( );
  void testSerialization();
  void testEventCompare();

};

#endif
