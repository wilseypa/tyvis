#ifndef OBJECT_BASE_TEST_HH
#define OBJECT_BASE_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <string>
using std::string;

class ObjectBase;
class VTime;

class ObjectBaseTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( ObjectBaseTest);
  CPPUNIT_TEST_SUITE_END();

private:

protected:
  static void testConstructor1( const ObjectBase &toCheck );

public:

};

#endif
