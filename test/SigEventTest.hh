#ifndef SIG_EVENT_TEST_HH
#define SIG_EVENT_TEST_HH

#include <cppunit/extensions/HelperMacros.h>
#include "VHDLEventTest.hh"

class SigEvent;
class VHDLData;

class SigEventTest : public VHDLEventTest {
  CPPUNIT_TEST_SUITE( SigEventTest);
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testSerialization );
  CPPUNIT_TEST( testEventCompare );
  CPPUNIT_TEST_SUITE_END();
  
private:
  SigEvent *toTest1;

  static const VHDLData &getDefaultVHDLData();

public:
  void setUp();
  void tearDown();

  void testConstructor( );
  void testSerialization();
  void testEventCompare();

};

#endif
