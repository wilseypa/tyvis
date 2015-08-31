//---------------------------------------------------------------------------
// Copyright (c) 2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "SignalTest.hh"
#include "tyvis/Signal.hh"
#include "tyvis/std_standardPkg.hh"
#include "VariableTest.hh"

void 
SignalTest::setUp(){
  scalarSignal = new Signal( std_standard_bitTypeInfo(), "scalarSignal" );
  arraySignal = new Signal( VariableTest::work_bv8TypeInfo(), "arraySignal" );
}

void 
SignalTest::tearDown(){
  delete scalarSignal;
  delete arraySignal;
}

void 
SignalTest::testSetup(){
  EnumerationLiteral zero(std_standard_bitTypeInfo(), 0);
  CPPUNIT_ASSERT( scalarSignal->getType() == EXPLICIT );
  CPPUNIT_ASSERT( scalarSignal->getAttributeList().empty() );
  CPPUNIT_ASSERT( scalarSignal->readVal() == zero );
  CPPUNIT_ASSERT( scalarSignal->getName() == "scalarSignal" );
  CPPUNIT_ASSERT( arraySignal->getType() == EXPLICIT );
  CPPUNIT_ASSERT( arraySignal->getAttributeList().empty() );

  CPPUNIT_ASSERT( arraySignal->readVal() == StringLiteral( VariableTest::work_bv8TypeInfo(),
							   "00000000"));
  CPPUNIT_ASSERT( arraySignal->getName() == "arraySignal" );

  CPPUNIT_ASSERT( (*arraySignal)[7] == zero );
  CPPUNIT_ASSERT( (*arraySignal)[6] == zero );
  CPPUNIT_ASSERT( (*arraySignal)[5] == zero );
  CPPUNIT_ASSERT( (*arraySignal)[4] == zero );
  CPPUNIT_ASSERT( (*arraySignal)[3] == zero );
  CPPUNIT_ASSERT( (*arraySignal)[2] == zero );
  CPPUNIT_ASSERT( (*arraySignal)[1] == zero );
  CPPUNIT_ASSERT( (*arraySignal)[0] == zero );

  CPPUNIT_ASSERT( dynamic_cast<Signal *>(&(*arraySignal)[7]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Signal *>(&(*arraySignal)[6]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Signal *>(&(*arraySignal)[5]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Signal *>(&(*arraySignal)[4]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Signal *>(&(*arraySignal)[3]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Signal *>(&(*arraySignal)[2]) != 0 ); 
  CPPUNIT_ASSERT( dynamic_cast<Signal *>(&(*arraySignal)[1]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Signal *>(&(*arraySignal)[0]) != 0 );
}
