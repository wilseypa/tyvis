#include <cppunit/testfixture.hh>
#include <cppunit/extensions/HelperMacros.h>

// Step 1, rename test to <class/feature>Test
class Test : public CppUnit::TestFixture {
  // Step 2, rename suite with new class name
  CPPUNIT_TEST_SUITE( Test);

  // Step 3, add new tests here, by adding their function names
  CPPUNIT_TEST(test);

  // <--Add new block tests here!  

  CPPUNIT_TEST_SUITE_END();

private:
  // Add new variables for the test here

public:
  // Create anything needed for the tests here
  void setUp();

  // Release any allocated resources here
  void tearDown();

  // Step 4, add new tests here, starting by renaming test() to testFoo()
  void test();
};
