#ifndef SIGNAL_TEST_HH
#define SIGNAL_TEST_HH

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

#include "tyvis/Signal.hh"

class SignalTest : public ObjectBaseTest {
  CPPUNIT_TEST_SUITE( SignalTest);
  CPPUNIT_TEST( testSetup );
  CPPUNIT_TEST_SUITE_END();

private:
  Signal *scalarSignal;
  Signal *arraySignal;

protected:

public:
  void setUp();
  void tearDown();

  void testSetup();

};

#endif
