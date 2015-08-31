//
//
//  Test.cc
//
//  This is a test to verify correct compilation of the cppunit stuff
//
//

#include "main.hh"

// CppUnit Headers
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "tyvis/vhdl.hh"
#include <warped/WarpedMain.h>

// Add test case headers here
// #include "ArrayTypeTest.hh"
#include "AggregateTest.hh"
#include "ArrayTypeInfoTest.hh"
#include "BlockTest.hh"
#include "EnumerationLiteralTest.hh"
#include "EnumerationTypeInfoTest.hh"
#include "IntegerTypeInfoTest.hh"
#include "PhysicalLiteralTest.hh"
#include "PhysicalTypeInfoTest.hh"
#include "RealTypeInfoTest.hh"
#include "RecordTypeInfoTest.hh"
#include "RecordTest.hh"
// #include "SharedFileEventTest.hh"
// #include "SigEventTest.hh"
#include "SignalTest.hh"
#include "StringLiteralTest.hh"
#include "UniversalIntegerTest.hh"
#include "UniversalRealTest.hh"
#include "VariableTest.hh"
// #include "WaitEventTest.hh"

// Add your tests to the suite here
// CPPUNIT_TEST_SUITE_REGISTRATION( ArrayTypeTest );
CPPUNIT_TEST_SUITE_REGISTRATION( AggregateTest );
CPPUNIT_TEST_SUITE_REGISTRATION( ArrayTypeInfoTest );
CPPUNIT_TEST_SUITE_REGISTRATION( BlockTest );
CPPUNIT_TEST_SUITE_REGISTRATION( EnumerationLiteralTest );
CPPUNIT_TEST_SUITE_REGISTRATION( EnumerationTypeInfoTest );
CPPUNIT_TEST_SUITE_REGISTRATION( IntegerTypeInfoTest );
CPPUNIT_TEST_SUITE_REGISTRATION( PhysicalLiteralTest );
CPPUNIT_TEST_SUITE_REGISTRATION( PhysicalTypeInfoTest );
CPPUNIT_TEST_SUITE_REGISTRATION( RealTypeInfoTest );
CPPUNIT_TEST_SUITE_REGISTRATION( RecordTest );
CPPUNIT_TEST_SUITE_REGISTRATION( RecordTypeInfoTest );
// CPPUNIT_TEST_SUITE_REGISTRATION( SharedFileEventTest );
CPPUNIT_TEST_SUITE_REGISTRATION( SignalTest );
// CPPUNIT_TEST_SUITE_REGISTRATION( SigEventTest );
CPPUNIT_TEST_SUITE_REGISTRATION( StringLiteralTest );
CPPUNIT_TEST_SUITE_REGISTRATION( UniversalIntegerTest );
CPPUNIT_TEST_SUITE_REGISTRATION( UniversalRealTest );
CPPUNIT_TEST_SUITE_REGISTRATION( VariableTest );
// CPPUNIT_TEST_SUITE_REGISTRATION( WaitEventTest );

int main() { 
  WarpedMain::registerKernelDeserializers();
  registerTyvisDeserializers();

  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest(registry.makeTest());
  bool result =  runner.run();

  return result;
}
