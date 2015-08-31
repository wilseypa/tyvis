#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "tyvis/Block.hh"

class BlockTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( BlockTest);
  CPPUNIT_TEST(testAddRemoveElement);
  CPPUNIT_TEST(testEqual);
  CPPUNIT_TEST(testReset);

  // <--Add new block tests here!  

  CPPUNIT_TEST_SUITE_END();

private:
  Block block;
  int *ptr, *ptr2;
  // Add new variables for the test here

public:
  // Test interface
  void setUp();
  void tearDown();

  void testAddRemoveElement();
  void testEqual();
  void testReset();
};
