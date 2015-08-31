//---------------------------------------------------------------------------
// Copyright (c) 2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "VariableTest.hh"
#include "EnumerationTypeInfoTest.hh"
#include "tyvis/Variable.hh"
#include "tyvis/EnumerationTypeInfo.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/IntegerTypeInfo.hh"
#include "tyvis/ArrayTypeInfo.hh"
#include "tyvis/UniversalInteger.hh"
#include "tyvis/std_standardPkg.hh"

const ArrayTypeInfo &
VariableTest::work_bv8TypeInfo(){
  static const IntegerTypeInfo bv8RangeTypeInfo( 7, ArrayInfo::downto, 0 );
  static const ArrayTypeInfo bv8TypeInfo( std_standard_bitTypeInfo(),
					  bv8RangeTypeInfo );
  return bv8TypeInfo;
}

void
VariableTest::setUp(){
  scalarVariable = new Variable( EnumerationTypeInfoTest::getColorInfo(),
				 "scalarVariable" );
  arrayVariable = new Variable( work_bv8TypeInfo(),
				"arrayVariable" );

}

void
VariableTest::tearDown(){
  delete scalarVariable;
  scalarVariable = 0;
  delete arrayVariable;
  arrayVariable = 0;
}


void 
VariableTest::testSetUp(){
  ObjectBaseTest::testConstructor1( *scalarVariable );
  CPPUNIT_ASSERT( scalarVariable->getName() == "scalarVariable" );
  CPPUNIT_ASSERT( scalarVariable->getTypeInfo() == EnumerationTypeInfoTest::getColorInfo() );
  CPPUNIT_ASSERT( scalarVariable->readVal() == EnumerationTypeInfoTest::getColorInfo().left() );

  CPPUNIT_ASSERT( arrayVariable->getName() == "arrayVariable" );
  CPPUNIT_ASSERT( arrayVariable->getTypeInfo() == work_bv8TypeInfo() );  
  CPPUNIT_ASSERT( arrayVariable->readVal().length(UniversalInteger(1)).getIntValue() == 8 );

  EnumerationLiteral zero( std_standard_bitTypeInfo(), 0 );
  CPPUNIT_ASSERT( (*arrayVariable)[7] == zero );
  CPPUNIT_ASSERT( (*arrayVariable)[6] == zero );
  CPPUNIT_ASSERT( (*arrayVariable)[5] == zero );
  CPPUNIT_ASSERT( (*arrayVariable)[4] == zero );
  CPPUNIT_ASSERT( (*arrayVariable)[3] == zero );
  CPPUNIT_ASSERT( (*arrayVariable)[2] == zero );
  CPPUNIT_ASSERT( (*arrayVariable)[1] == zero );
  CPPUNIT_ASSERT( (*arrayVariable)[0] == zero );

  CPPUNIT_ASSERT( dynamic_cast<Variable *>(&(*arrayVariable)[7]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Variable *>(&(*arrayVariable)[6]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Variable *>(&(*arrayVariable)[5]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Variable *>(&(*arrayVariable)[4]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Variable *>(&(*arrayVariable)[3]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Variable *>(&(*arrayVariable)[2]) != 0 ); 
  CPPUNIT_ASSERT( dynamic_cast<Variable *>(&(*arrayVariable)[1]) != 0 );
  CPPUNIT_ASSERT( dynamic_cast<Variable *>(&(*arrayVariable)[0]) != 0 );
}

void
VariableTest::testAssign(){
  Variable::assign( *scalarVariable, UniversalInteger( 1 ) );
  CPPUNIT_ASSERT( scalarVariable->readVal() == UniversalInteger( 1 ) );
//   CPPUNIT_ASSERT( arrayVali == UniversalInteger( 1 ) );
}
