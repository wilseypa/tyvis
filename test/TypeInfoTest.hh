#ifndef TYPE_INFO_TEST_HH
#define TYPE_INFO_TEST_HH

#include "TyvisUnitTest.hh"

class TypeInfoTest : public TyvisUnitTest {

public:

  virtual void testCreateObject() = 0;
  virtual void testIsCompositeType() = 0;
  virtual void testIsPhysicalType() = 0;
  virtual void testIsScalarType() = 0;
  virtual void testOperatorEqual() = 0;

private:

};

#endif
