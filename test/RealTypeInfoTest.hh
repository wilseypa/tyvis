#ifndef REAL_TYPE_INFO_TEST_HH
#define REAL_TYPE_INFO_TEST_HH

#include <cppunit/extensions/HelperMacros.h>
#include "TypeInfoTest.hh"

class RealTypeInfoTest : public TypeInfoTest {
  CPPUNIT_TEST_SUITE( RealTypeInfoTest );
  CPPUNIT_TEST( testCopyConstructor );
  CPPUNIT_TEST( testCreateObject );
  CPPUNIT_TEST( testIsCompositeType );
  CPPUNIT_TEST( testIsPhysicalType );
  CPPUNIT_TEST( testIsScalarType );
  CPPUNIT_TEST( testOperatorEqual );
  CPPUNIT_TEST( testValue1 );
  CPPUNIT_TEST( testValue2 );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  // TyvisUnitTests
  void testCopyConstructor();

  // TypeInfo tests
  void testCreateObject();
  void testIsCompositeType();
  void testIsPhysicalType();
  void testIsScalarType();
  void testOperatorEqual();
  void testValue1();
  void testValue2();
};
#endif
