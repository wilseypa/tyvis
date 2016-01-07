#include "UniversalRealTest.hh"
#include "tyvis/UniversalReal.hh"
#include "tyvis/UniversalInteger.hh"
#include "tyvis/Value.hh"
#include <SerializedInstance.h>
#include <StringUtilities.h>
#include <sstream>
using std::ostringstream;

double
UniversalRealTest::getDefaultValue(){
  static double defaultValue = 5.55;
  return defaultValue;
}

void 
UniversalRealTest::setUp(){
  toTest = new UniversalReal( getDefaultValue() );
}

void 
UniversalRealTest::tearDown(){
  delete toTest;
}

void
UniversalRealTest::testCopyConstructor(){
  UniversalReal r1( 5.0 );
  UniversalReal r2( r1 );
  
  UniversalReal r3 = r2 - r1;
  CPPUNIT_ASSERT( r3.vhdlAbs() < 0.0001 );
}

void
UniversalRealTest::testLength(){
  CPPUNIT_ASSERT( toTest->length(UniversalInteger(1)).getIntValue() == 1 );
}

void
UniversalRealTest::testPrint(){
  ostringstream printStream;
  toTest->print( printStream );
  CPPUNIT_ASSERT( printStream.str() == doubleToString( getDefaultValue() ));
}

void
UniversalRealTest::testEqual(){
  UniversalReal a(10);
  UniversalReal b(10);
  
  CPPUNIT_ASSERT( a == b );
}

void
UniversalRealTest::testNotEqual(){
  UniversalReal a(10);
  UniversalReal b(11);
  
  CPPUNIT_ASSERT( a != b );
}

void
UniversalRealTest::testOperatorEqualRValue(){
  UniversalReal r1( 10.0 );
  UniversalReal r2;
  r2 = r1;
  CPPUNIT_ASSERT( r2 == r1 );
  UniversalInteger i1( 1 );
  r2 = i1;
  CPPUNIT_ASSERT( r2 == i1 );
}

void
UniversalRealTest::testGreater(){
  UniversalReal a(10);
  UniversalReal b(11);
  
  CPPUNIT_ASSERT( b > a );
}

void
UniversalRealTest::testGreaterEqual(){
  UniversalReal a(10);
  UniversalReal b(11);
  UniversalReal c(11);
  
  CPPUNIT_ASSERT( b >= a );
  CPPUNIT_ASSERT( b >= c );
}

void
UniversalRealTest::testLess(){
  UniversalReal a(10);
  UniversalReal b(11);
  
  CPPUNIT_ASSERT( a < b );
}

void
UniversalRealTest::testLessEqual(){
  UniversalReal a(10);
  UniversalReal b(11);
  UniversalReal c(11);
  
  CPPUNIT_ASSERT( a <= c );
  CPPUNIT_ASSERT( b <= c );
}

void
UniversalRealTest::testClone(){
  UniversalReal *cloned = dynamic_cast<UniversalReal *>(toTest->clone());
  CPPUNIT_ASSERT( cloned != 0 );
  CPPUNIT_ASSERT( cloned != toTest );
  CPPUNIT_ASSERT( *cloned == *toTest );
  delete cloned;
}

void
UniversalRealTest::testSerialization(){
  SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
  CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
  UniversalReal *deserialized = dynamic_cast<UniversalReal *>( serialized->deserialize() );
  CPPUNIT_ASSERT( deserialized != 0 );
  CPPUNIT_ASSERT( deserialized != toTest );
  CPPUNIT_ASSERT( *deserialized == *toTest );
  delete deserialized;
  delete serialized;
}

void
UniversalRealTest::testToString(){
  UniversalReal one(1.1), zero(0.0), minusOne(-1.0);
  CPPUNIT_ASSERT( one.toString() == "1.1" );
  CPPUNIT_ASSERT( zero.toString() == "0" );
  CPPUNIT_ASSERT( minusOne.toString() == "-1" );
}
