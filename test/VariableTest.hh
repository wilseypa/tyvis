#ifndef VARIABLE_TEST_HH
#define VARIABLE_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "ObjectBaseTest.hh"

#include <string>
using std::string;

class Variable;
class VTime;
class ArrayTypeInfo;

class VariableTest : public ObjectBaseTest {
  CPPUNIT_TEST_SUITE(VariableTest);
  CPPUNIT_TEST(testAssign);
  CPPUNIT_TEST(testSetUp);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testSetUp();
  void testAssign();

  static const ArrayTypeInfo &work_bv8TypeInfo();

protected:

private:

  Variable *scalarVariable;
  Variable *arrayVariable;
};
#endif
