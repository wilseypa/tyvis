#ifndef VHDL_DATA_TEST_HH
#define VHDL_DATA_TEST_HH

//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "RValueTest.hh"

class VHDLData;
class VTime;

class VHDLDataTest : public RValueTest {
private:

protected:

public:
  virtual void testLength() = 0;
  virtual void testPrint() = 0;
  virtual void testEqual() = 0;
  virtual void testNotEqual() = 0;
  virtual void testGreater() = 0;
  virtual void testGreaterEqual() = 0;
  virtual void testLess() = 0;
  virtual void testLessEqual() = 0;
  virtual void testClone() = 0;
  virtual void testSerialization() = 0;
};

#endif
