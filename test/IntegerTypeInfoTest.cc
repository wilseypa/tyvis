#include "IntegerTypeInfoTest.hh"
#include "tyvis/std_standardPkg.hh"
#include "tyvis/ArrayInfo.hh"
#include "tyvis/EnumerationLiteral.hh"
#include "tyvis/IntegerTypeInfo.hh"
#include "tyvis/UniversalInteger.hh"

void
IntegerTypeInfoTest::setUp(){

}

void
IntegerTypeInfoTest::tearDown(){

}

void
IntegerTypeInfoTest::testCopyConstructor(){
  IntegerTypeInfo t1( 1, ArrayInfo::downto, -1 );
  IntegerTypeInfo t2(t1);

  CPPUNIT_ASSERT( t2.left().getIntValue() == 1 );
  CPPUNIT_ASSERT( t2.getDirection() == ArrayInfo::downto );
  CPPUNIT_ASSERT( t2.right().getIntValue() == -1 );
}

void
IntegerTypeInfoTest::testCreateObject(){
  const TypeInfo &t1 = IntegerTypeInfo( -10, ArrayInfo::to, 10 );
  RValue *object = t1.create(0);
  UniversalInteger *asInt = dynamic_cast<UniversalInteger *>(object);
  CPPUNIT_ASSERT( asInt != 0 );
  delete object;
}

void
IntegerTypeInfoTest::testIsCompositeType(){
  const TypeInfo &t1 = IntegerTypeInfo( -10, ArrayInfo::to, 10 );
  CPPUNIT_ASSERT( !t1.isCompositeType() );
}

void
IntegerTypeInfoTest::testIsPhysicalType(){
  const TypeInfo &t1 = IntegerTypeInfo( -10, ArrayInfo::to, 10 );
  CPPUNIT_ASSERT( !t1.isPhysicalType() );
}

void
IntegerTypeInfoTest::testIsScalarType(){
  const TypeInfo &t1 = IntegerTypeInfo( -10, ArrayInfo::to, 10 );
  CPPUNIT_ASSERT( t1.isScalarType() );
}

void
IntegerTypeInfoTest::testOperatorEqual(){
  IntegerTypeInfo t1( 1, ArrayInfo::downto, -1 );
  IntegerTypeInfo t2;
  t2 = t1;
  CPPUNIT_ASSERT( t2.left().getIntValue() == 1 );
  CPPUNIT_ASSERT( t2.getDirection() == ArrayInfo::downto );
  CPPUNIT_ASSERT( t2.right().getIntValue() == -1 );
}

void
IntegerTypeInfoTest::testAscending(){
  IntegerTypeInfo t1( 1, ArrayInfo::downto, -1 );
  CPPUNIT_ASSERT( t1.ascending() == SAVANT_BOOLEAN_FALSE );

  IntegerTypeInfo t2( -1, ArrayInfo::to, 1 );
  CPPUNIT_ASSERT( t2.ascending() == SAVANT_BOOLEAN_TRUE );
}

void
IntegerTypeInfoTest::testHigh(){
  IntegerTypeInfo t1( 1, ArrayInfo::downto, -1 );
  CPPUNIT_ASSERT( t1.high() == UniversalInteger(1) );
  IntegerTypeInfo t2( -1, ArrayInfo::to, 1 );
  CPPUNIT_ASSERT( t2.high() == UniversalInteger(1) );
}

void
IntegerTypeInfoTest::testLow(){
  IntegerTypeInfo t1( 1, ArrayInfo::downto, -1 );
  CPPUNIT_ASSERT( t1.low() == UniversalInteger(-1) );
  IntegerTypeInfo t2( -1, ArrayInfo::to, 1 );
  CPPUNIT_ASSERT( t2.low() == UniversalInteger(-1) );
}

void
IntegerTypeInfoTest::testPos(){
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pos( UniversalInteger(1) ) ==
		  UniversalInteger(1));
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pos( UniversalInteger(0) ) ==
		  UniversalInteger(0));
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pos( UniversalInteger(-1) ) ==
		  UniversalInteger(-1));
}

void
IntegerTypeInfoTest::testVal(){
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pos( UniversalInteger(1) ) ==
		  UniversalInteger(1));
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pos( UniversalInteger(0) ) ==
		  UniversalInteger(0));
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pos( UniversalInteger(-1) ) ==
		  UniversalInteger(-1));
}

void
IntegerTypeInfoTest::testSucc(){
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().succ( UniversalInteger(1) ) ==
		  UniversalInteger(2));
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().succ( UniversalInteger(-1) ) ==
		  UniversalInteger(0));
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().succ( UniversalInteger(0) ) ==
		  UniversalInteger(1));
}

void
IntegerTypeInfoTest::testPred(){
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pred( UniversalInteger(1) ) ==
		  UniversalInteger(0));
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pred( UniversalInteger(-1) ) ==
		  UniversalInteger(-2));
  CPPUNIT_ASSERT( std_standard_integerTypeInfo().pred( UniversalInteger(0) ) ==
		  UniversalInteger(-1));
}

void
IntegerTypeInfoTest::testLeftof(){
  IntegerTypeInfo t1( 1, ArrayInfo::to, 2 );
  CPPUNIT_ASSERT( t1.leftof( UniversalInteger(2) ) == UniversalInteger(1) );
  IntegerTypeInfo t2( 2, ArrayInfo::downto, 1 );
  CPPUNIT_ASSERT( t2.leftof( UniversalInteger(1) ) == UniversalInteger(2) );
}

void
IntegerTypeInfoTest::testRightof(){
  IntegerTypeInfo t1( 1, ArrayInfo::to, 2 );
  CPPUNIT_ASSERT( t1.rightof( UniversalInteger(1) ) == UniversalInteger(2) );
  IntegerTypeInfo t2( 2, ArrayInfo::downto, 1 );
  CPPUNIT_ASSERT( t2.rightof( UniversalInteger(2) ) == UniversalInteger(1) );
}

void
IntegerTypeInfoTest::testValue1(){
  IntegerTypeInfo t1( 1, ArrayInfo::downto, -1 );
  CPPUNIT_ASSERT( t1.value( VHDL_STRING("1") ) == UniversalInteger(1) );
}

void
IntegerTypeInfoTest::testValue2(){
  IntegerTypeInfo t1( 1, ArrayInfo::downto, -2 );
  CPPUNIT_ASSERT( t1.value( VHDL_STRING("-2") ) == UniversalInteger(-2) );
}
