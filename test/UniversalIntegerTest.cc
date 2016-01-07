#include "UniversalIntegerTest.hh"
#include "tyvis/UniversalInteger.hh"
#include "tyvis/UniversalReal.hh"
#include "tyvis/Value.hh"
#include "tyvis/EnumerationLiteral.hh"
#include <SerializedInstance.h>
#include <StringUtilities.h>
#include <sstream>
using std::ostringstream;

int
UniversalIntegerTest::getDefaultValue(){
  static int defaultValue = 3;
  return defaultValue;
}

void 
UniversalIntegerTest::setUp(){
  toTest = new UniversalInteger( getDefaultValue() );
}

void 
UniversalIntegerTest::tearDown(){
  delete toTest;
}

void
UniversalIntegerTest::testCopyConstructor(){
  UniversalInteger i1( 42 );
  UniversalInteger i2( i1 );
  CPPUNIT_ASSERT( i1 == i2 );
}

void
UniversalIntegerTest::testLength(){
  CPPUNIT_ASSERT( toTest->length(UniversalInteger(1)).getIntValue() == 1 );
}

void
UniversalIntegerTest::testPrint(){
  ostringstream printStream;
  toTest->print( printStream );
  CPPUNIT_ASSERT( printStream.str() == intToString( getDefaultValue() ));
}

void
UniversalIntegerTest::testEqual(){
  UniversalInteger a(10);
  UniversalInteger b(10);
  
  CPPUNIT_ASSERT( a == b );
  CPPUNIT_ASSERT( a.vhdlEqual(b) == EnumerationLiteral::getEnumerationTrue() );
}

void
UniversalIntegerTest::testNotEqual(){
  UniversalInteger a(10);
  UniversalInteger b(11);
  
  CPPUNIT_ASSERT( a != b );
}

void
UniversalIntegerTest::testGreater(){
  UniversalInteger a(10);
  UniversalInteger b(11);
  
  CPPUNIT_ASSERT( b > a );
}

void
UniversalIntegerTest::testGreaterEqual(){
  UniversalInteger a(10);
  UniversalInteger b(11);
  UniversalInteger c(11);
  
  CPPUNIT_ASSERT( b >= a );
  CPPUNIT_ASSERT( b >= c );
}

void
UniversalIntegerTest::testLess(){
  UniversalInteger a(10);
  UniversalInteger b(11);
  
  CPPUNIT_ASSERT( a < b );
}

void
UniversalIntegerTest::testLessEqual(){
  UniversalInteger a(10);
  UniversalInteger b(11);
  UniversalInteger c(11);
  
  CPPUNIT_ASSERT( a <= c );
  CPPUNIT_ASSERT( b <= c );
}

void
UniversalIntegerTest::testPlus(){
  const UniversalInteger minusTwo( -2 );
  const UniversalInteger zero(0);
  const UniversalInteger two(2);
  const UniversalInteger four(4);

  CPPUNIT_ASSERT( two.vhdlPlus(two) == four );
  CPPUNIT_ASSERT( two.vhdlPlus(minusTwo) == zero );
  CPPUNIT_ASSERT( four.vhdlPlus(minusTwo) == two );
}

void
UniversalIntegerTest::testMinus(){
  const UniversalInteger minusTwo(-2);
  const UniversalInteger zero(0);
  const UniversalInteger two(2);
  const UniversalInteger four(4);
  const UniversalInteger six(6);

  CPPUNIT_ASSERT( two.vhdlMinus(two) == zero );
  CPPUNIT_ASSERT( two.vhdlMinus(minusTwo) == four );
  CPPUNIT_ASSERT( four.vhdlMinus(minusTwo) == six );
  CPPUNIT_ASSERT( four.vhdlMinus(two) == two );
}

void
UniversalIntegerTest::testMultiply(){
  const UniversalInteger minusTwo(-2);
  const UniversalInteger zero(0);
  const UniversalInteger two(2);
  const UniversalInteger four(4);
  const UniversalInteger six(6);

  CPPUNIT_ASSERT( two.vhdlMultiply( two ) == four );
  CPPUNIT_ASSERT( minusTwo.vhdlMultiply( minusTwo ) == four );
  CPPUNIT_ASSERT( two.vhdlMultiply( minusTwo ) == UniversalInteger( -4 ) );
  CPPUNIT_ASSERT( zero.vhdlMultiply( minusTwo ) == zero );
}

void
UniversalIntegerTest::testOperatorEqualRValue(){
  UniversalInteger i1( 1 );
  UniversalInteger i2;
  i2 = i1;
  CPPUNIT_ASSERT( i2 == i1 );
  UniversalReal r1( 3.0 );
  i2 = r1;
  CPPUNIT_ASSERT( i2 == r1 );
}

void
UniversalIntegerTest::testDivide(){
  const UniversalInteger minusTwo(-2);
  const UniversalInteger zero(0);
  const UniversalInteger one(1);
  const UniversalInteger two(2);
  const UniversalInteger four(4);

  CPPUNIT_ASSERT( four.vhdlDivide( one ) == four );
  CPPUNIT_ASSERT( four.vhdlDivide( one ) == UniversalInteger( 4 ) );
  CPPUNIT_ASSERT( four.vhdlDivide( two ) == two );
  CPPUNIT_ASSERT( four.vhdlDivide( four ) == one );
  CPPUNIT_ASSERT( zero.vhdlDivide( four ) == zero );

}

void
UniversalIntegerTest::testClone(){
  UniversalInteger *cloned = dynamic_cast<UniversalInteger *>(toTest->clone());
  CPPUNIT_ASSERT( cloned != 0 );
  CPPUNIT_ASSERT( cloned != toTest );
  CPPUNIT_ASSERT( *cloned == *toTest );
  delete cloned;
}

void
UniversalIntegerTest::testSerialization(){
  SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
  CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
  UniversalInteger *deserialized = dynamic_cast<UniversalInteger *>( serialized->deserialize() );
  CPPUNIT_ASSERT( deserialized != 0 );
  CPPUNIT_ASSERT( deserialized != toTest );
  CPPUNIT_ASSERT( *deserialized == *toTest );
  delete deserialized;
  delete serialized;
}

void
UniversalIntegerTest::testToString(){
  UniversalInteger zero(0), minusOne(-1), twelve(12);
  CPPUNIT_ASSERT( zero.toString() == "0" );
  CPPUNIT_ASSERT( minusOne.toString() == "-1" );
  CPPUNIT_ASSERT( twelve.toString() == "12" );
}
