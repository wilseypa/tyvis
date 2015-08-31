//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------

#include "ArrayTypeInfoTest.hh"
#include "tyvis/ArrayInfo.hh"
#include "tyvis/ArrayTypeInfo.hh"
#include "tyvis/std_standardPkg.hh"
#include "tyvis/IntegerTypeInfo.hh"
#include "tyvis/EnumerationLiteral.hh"

void
ArrayTypeInfoTest::setUp(){
  toTestOneD = new ArrayTypeInfo( std_standard_bitTypeInfo(), std_standard_integerTypeInfo() );
}

void
ArrayTypeInfoTest::tearDown(){
  delete toTestOneD;
}

void
ArrayTypeInfoTest::testCopyConstructor(){
  ArrayTypeInfo t1( *toTestOneD );
  CPPUNIT_ASSERT( t1 == *toTestOneD );
}

void
ArrayTypeInfoTest::testCreateObject(){
  RValue *object = toTestOneD->create(0);
  CompositeLiteral *asArray = dynamic_cast<CompositeLiteral *>(object);
  CPPUNIT_ASSERT( asArray != 0 );
  delete object;
}

void
ArrayTypeInfoTest::testIsCompositeType(){
  CPPUNIT_ASSERT( toTestOneD->isCompositeType() );
}

void
ArrayTypeInfoTest::testIsConstrained(){
  IntegerTypeInfo constrainedIndex( 7, ArrayInfo::downto, 0 );
  ArrayTypeInfo constrained( std_standard_bitTypeInfo(),
			     constrainedIndex );
  CPPUNIT_ASSERT( constrained.isConstrained() );
  CPPUNIT_ASSERT( !constrained.isUnconstrained() );

  ArrayTypeInfo unconstrained( std_standard_bitTypeInfo(),
			       std_standard_naturalTypeInfo() );

  CPPUNIT_ASSERT( unconstrained.isUnconstrained() );
  CPPUNIT_ASSERT( !unconstrained.isConstrained() );
}


void
ArrayTypeInfoTest::testIsPhysicalType(){
  CPPUNIT_ASSERT( !toTestOneD->isPhysicalType() );
}

void
ArrayTypeInfoTest::testIsScalarType(){
  CPPUNIT_ASSERT( !toTestOneD->isScalarType() );
}

void
ArrayTypeInfoTest::testLeft(){
  CPPUNIT_ASSERT( toTestOneD->left() == std_standard_integerTypeInfo().left() ); 
}


void
ArrayTypeInfoTest::testOperatorEqual(){
//   ArrayTypeInfo t1( 1, ArrayInfo::downto, -1 );
//   ArrayTypeInfo t2;
//   t2 = t1;
//   CPPUNIT_ASSERT( t2.left() == 1 );
//   CPPUNIT_ASSERT( t2.getDirection() == ArrayInfo::downto );
//   CPPUNIT_ASSERT( t2.right() == -1 );
}

void
ArrayTypeInfoTest::testRight(){
  CPPUNIT_ASSERT( toTestOneD->right() == std_standard_integerTypeInfo().right() ); 
}


void
ArrayTypeInfoTest::testStorageIndex(){
  IntegerTypeInfo indexType1( 7, ArrayInfo::downto, 0 );
  ArrayTypeInfo constrained1( std_standard_bitTypeInfo(), indexType1 );
  CPPUNIT_ASSERT( constrained1.storageIndex( 7 ) == 7 );
  CPPUNIT_ASSERT( constrained1.storageIndex( 0 ) == 0 );

  IntegerTypeInfo indexType2( 8, ArrayInfo::downto, 1 );
  ArrayTypeInfo constrained2( std_standard_bitTypeInfo(), indexType2 );
  CPPUNIT_ASSERT( constrained2.storageIndex( 8 ) == 7 );
  CPPUNIT_ASSERT( constrained2.storageIndex( 1 ) == 0 );

  IntegerTypeInfo indexType3( 6, ArrayInfo::downto, -1 );
  ArrayTypeInfo constrained3( std_standard_bitTypeInfo(), indexType3 );
  CPPUNIT_ASSERT( constrained3.storageIndex( 6 ) == 7 );
  CPPUNIT_ASSERT( constrained3.storageIndex( -1 ) == 0 );

  IntegerTypeInfo indexType4( 0, ArrayInfo::to, 7 );
  ArrayTypeInfo constrained4( std_standard_bitTypeInfo(), indexType4 );
  CPPUNIT_ASSERT( constrained4.storageIndex( 7 ) == 7 );
  CPPUNIT_ASSERT( constrained4.storageIndex( 0 ) == 0 );

  IntegerTypeInfo indexType5( 1, ArrayInfo::to, 8 );
  ArrayTypeInfo constrained5( std_standard_bitTypeInfo(), indexType5 );
  CPPUNIT_ASSERT( constrained5.storageIndex( 8 ) == 7 );
  CPPUNIT_ASSERT( constrained5.storageIndex( 1 ) == 0 );

  IntegerTypeInfo indexType6( -1, ArrayInfo::to, 6 );
  ArrayTypeInfo constrained6( std_standard_bitTypeInfo(), indexType6 );
  CPPUNIT_ASSERT( constrained6.storageIndex( 6 ) == 7 );
  CPPUNIT_ASSERT( constrained6.storageIndex( -1 ) == 0 );
}


void
ArrayTypeInfoTest::testValue1(){
  IntegerTypeInfo indexType1( 7, ArrayInfo::downto, 0 );
  ArrayTypeInfo constrained1( std_standard_bitTypeInfo(), indexType1 );
  Value literal1 = constrained1.value( "10101010" );
  CPPUNIT_ASSERT( literal1[7] == EnumerationLiteral( std_standard_bitTypeInfo(), 1 ) );
  CPPUNIT_ASSERT( literal1[6] == EnumerationLiteral( std_standard_bitTypeInfo(), 0 ) );
  CPPUNIT_ASSERT( literal1[5] == EnumerationLiteral( std_standard_bitTypeInfo(), 1 ) );
  CPPUNIT_ASSERT( literal1[4] == EnumerationLiteral( std_standard_bitTypeInfo(), 0 ) );
  CPPUNIT_ASSERT( literal1[3] == EnumerationLiteral( std_standard_bitTypeInfo(), 1 ) );
  CPPUNIT_ASSERT( literal1[2] == EnumerationLiteral( std_standard_bitTypeInfo(), 0 ) );
  CPPUNIT_ASSERT( literal1[1] == EnumerationLiteral( std_standard_bitTypeInfo(), 1 ) );
  CPPUNIT_ASSERT( literal1[0] == EnumerationLiteral( std_standard_bitTypeInfo(), 0 ) );

//   Value literal2 = constrained1.value( "('1','0','1','0','1','0','1','0')" );
//   CPPUNIT_ASSERT( literal2[7] == EnumerationLiteral( std_standard_bitTypeInfo(), 1 ) );
//   CPPUNIT_ASSERT( literal2[6] == EnumerationLiteral( std_standard_bitTypeInfo(), 0 ) );
//   CPPUNIT_ASSERT( literal2[5] == EnumerationLiteral( std_standard_bitTypeInfo(), 1 ) );
//   CPPUNIT_ASSERT( literal2[4] == EnumerationLiteral( std_standard_bitTypeInfo(), 0 ) );
//   CPPUNIT_ASSERT( literal2[3] == EnumerationLiteral( std_standard_bitTypeInfo(), 1 ) );
//   CPPUNIT_ASSERT( literal2[2] == EnumerationLiteral( std_standard_bitTypeInfo(), 0 ) );
//   CPPUNIT_ASSERT( literal2[1] == EnumerationLiteral( std_standard_bitTypeInfo(), 1 ) );
//   CPPUNIT_ASSERT( literal2[0] == EnumerationLiteral( std_standard_bitTypeInfo(), 0 ) );
}
