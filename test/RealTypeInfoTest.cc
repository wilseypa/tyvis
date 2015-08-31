#include "RealTypeInfoTest.hh"
#include "tyvis/ArrayInfo.hh"
#include "tyvis/RealTypeInfo.hh"
#include "tyvis/UniversalReal.hh"
#include "tyvis/std_standardPkg.hh"

void
RealTypeInfoTest::setUp(){

}

void
RealTypeInfoTest::tearDown(){

}

void
RealTypeInfoTest::testCopyConstructor(){
  RealTypeInfo t1( 1.0, ArrayInfo::downto, -1.0 );
  RealTypeInfo t2(t1);

  CPPUNIT_ASSERT( t2.left() == UniversalReal( 1.0 ) );
  CPPUNIT_ASSERT( t2.getDirection() == ArrayInfo::downto );
  CPPUNIT_ASSERT( t2.right() == UniversalReal( -1.0 ) );
}

void
RealTypeInfoTest::testCreateObject(){
  const TypeInfo &t1 = RealTypeInfo( -10.0, ArrayInfo::to, 10.0 );
  RValue *object = t1.create(0);
  UniversalReal *asInt = dynamic_cast<UniversalReal *>(object);
  CPPUNIT_ASSERT( asInt != 0 );
  delete object;
}

void
RealTypeInfoTest::testIsCompositeType(){
  const TypeInfo &t1 = RealTypeInfo( -10.0, ArrayInfo::to, 10.0 );
  CPPUNIT_ASSERT( !t1.isCompositeType() );
}

void
RealTypeInfoTest::testIsPhysicalType(){
  const TypeInfo &t1 = RealTypeInfo( -10.0, ArrayInfo::to, 10.0 );
  CPPUNIT_ASSERT( !t1.isPhysicalType() );
}

void
RealTypeInfoTest::testIsScalarType(){
  const TypeInfo &t1 = RealTypeInfo( -10.0, ArrayInfo::to, 10.0 );
  CPPUNIT_ASSERT( t1.isScalarType() );
}

void
RealTypeInfoTest::testOperatorEqual(){
  RealTypeInfo t1( 1.0, ArrayInfo::downto, -1.0 );
  RealTypeInfo t2;
  t2 = t1;
  CPPUNIT_ASSERT( t2.left() == UniversalReal( 1.0 ) );
  CPPUNIT_ASSERT( t2.getDirection() == ArrayInfo::downto );
  CPPUNIT_ASSERT( t2.right() == UniversalReal( -1.0 ) );
}

void
RealTypeInfoTest::testValue1(){
  RealTypeInfo t1( 1.0, ArrayInfo::downto, -1.0 );
  CPPUNIT_ASSERT( t1.value( VHDL_STRING("1.0") ) == UniversalReal( 1.0 ) );
  CPPUNIT_ASSERT( t1.value( VHDL_STRING("-1.0") ) == UniversalReal( -1.0 ) );
}

void
RealTypeInfoTest::testValue2(){
  RealTypeInfo t1( 1.0, ArrayInfo::downto, -1.0 );
  CPPUNIT_ASSERT( t1.value( "1.0" ) == UniversalReal( 1.0 ) );
  CPPUNIT_ASSERT( t1.value( "-1.0" ) == UniversalReal( -1.0 ) );
}
