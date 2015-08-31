#include "PhysicalTypeInfoTest.hh"
#include "tyvis/ArrayInfo.hh"
#include "tyvis/PhysicalTypeInfo.hh"
#include "tyvis/PhysicalLiteral.hh"

//---------------------------------------------------------------------------
// Copyright (c) 1995-2004 Clifton Labs, Inc.  All Rights Reserved.

// You may modify, distribute, and use the software contained in this package
// under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE" version 2,
// June 1991. A copy of this license agreement can be found in the file
// "LGPL", distributed with this archive.
//---------------------------------------------------------------------------


const vector<UnitDeclaration> &
PhysicalTypeInfoTest::getTestUnits(){
  static vector<UnitDeclaration> *literals = new vector<UnitDeclaration>;
  if( literals->size() == 0 ){
    literals->push_back( UnitDeclaration( "in", PhysicalLiteral( 1, "in" )) );
    literals->push_back( UnitDeclaration( "foot", PhysicalLiteral( 12, "in")) );
    literals->push_back( UnitDeclaration( "yard", PhysicalLiteral( 3, "foot" )) );
    literals->push_back( UnitDeclaration( "mile", PhysicalLiteral( 1760, "yard" )) );
  }
  return *literals;
}

void
PhysicalTypeInfoTest::setUp(){
  toTest = new PhysicalTypeInfo( getTestUnits() );
}

void
PhysicalTypeInfoTest::tearDown(){
  delete toTest;
}

void
PhysicalTypeInfoTest::testCopyConstructor(){
  PhysicalTypeInfo t1( *toTest );

  CPPUNIT_ASSERT( t1.left() == toTest->left() );
  CPPUNIT_ASSERT( t1.getDirection() == toTest->getDirection() );
  CPPUNIT_ASSERT( t1.right() == toTest->right() );

  CPPUNIT_ASSERT( t1.getUnits() == toTest->getUnits() );
}

void
PhysicalTypeInfoTest::testCreateObject(){
  RValue *object = toTest->create(0);
  PhysicalLiteral *asPhys = dynamic_cast<PhysicalLiteral *>(object);
  CPPUNIT_ASSERT( asPhys != 0 );
  delete object;
}

void
PhysicalTypeInfoTest::testIsCompositeType(){
  CPPUNIT_ASSERT( !toTest->isCompositeType() );
}

void
PhysicalTypeInfoTest::testIsPhysicalType(){
  CPPUNIT_ASSERT( toTest->isPhysicalType() );
}

void
PhysicalTypeInfoTest::testIsScalarType(){
  CPPUNIT_ASSERT( toTest->isScalarType() );
}

void
PhysicalTypeInfoTest::testOperatorEqual(){
  CPPUNIT_ASSERT( toTest->left() == PHYSICAL_MIN( *toTest ));
  CPPUNIT_ASSERT( toTest->getDirection() == ArrayInfo::to );
  CPPUNIT_ASSERT( toTest->right() == PHYSICAL_MAX( *toTest ));
}
