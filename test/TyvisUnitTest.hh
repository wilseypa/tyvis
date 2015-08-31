#ifndef TYVIS_UNIT_TEST_HH
#define TYVIS_UNIT_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


class TyvisUnitTest : public CppUnit::TestFixture {
public:
  virtual void testCopyConstructor() = 0;

private:

};

#endif
