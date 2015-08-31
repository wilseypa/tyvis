#include "FileOpenKindTest.hh"
#include <warped/SerializedInstance.h>
#include <clutils/StringUtilities.h>

int
FileOpenKindTest::getDefaultValue(){
  static int defaultValue = 3;
  return defaultValue;
}

void 
FileOpenKindTest::setUp(){
  toTest = new FileOpenKind( FileOpenKind::WRITE_MODE );
}

void 
FileOpenKindTest::tearDown(){
  delete toTest;
}

void
FileOpenKindTest::testGetUniversalKind(){
  CPPUNIT_ASSERT( toTest->getUniversalKind() == VHDLData::FILE_OPEN_KIND );
}

void
FileOpenKindTest::testSize(){
  CPPUNIT_ASSERT( toTest->size() == 1 );
}

void
FileOpenKindTest::testPrint(){
  ostringstream printStream;
  toTest->print( printStream );
  CPPUNIT_ASSERT( "WRITE_MODE" == printStream.str() );
}

void
FileOpenKindTest::testEqual(){
  FileOpenKind a(FileOpenKind::READ_MODE);
  FileOpenKind b(FileOpenKind::READ_MODE);
  
  CPPUNIT_ASSERT( a == b );
}

void
FileOpenKindTest::testNotEqual(){
  FileOpenKind a(FileOpenKind::READ_MODE);
  FileOpenKind b(FileOpenKind::WRITE_MODE);
  
  CPPUNIT_ASSERT( a != b );
}

void
FileOpenKindTest::testGreater(){
  FileOpenKind a(FileOpenKind::READ_MODE);
  FileOpenKind b(FileOpenKind::WRITE_MODE);
  
  CPPUNIT_ASSERT( b > a );
}

void
FileOpenKindTest::testGreaterEqual(){
  FileOpenKind a(FileOpenKind::READ_MODE);
  FileOpenKind b(FileOpenKind::WRITE_MODE);
  FileOpenKind c(FileOpenKind::WRITE_MODE);
  
  CPPUNIT_ASSERT( b >= a );
  CPPUNIT_ASSERT( b >= c );
}

void
FileOpenKindTest::testLess(){
  FileOpenKind a(FileOpenKind::READ_MODE);
  FileOpenKind b(FileOpenKind::WRITE_MODE);
  
  CPPUNIT_ASSERT( a < b );
}

void
FileOpenKindTest::testLessEqual(){
  FileOpenKind a(FileOpenKind::READ_MODE);
  FileOpenKind b(FileOpenKind::WRITE_MODE);
  FileOpenKind c(FileOpenKind::WRITE_MODE);
  
  CPPUNIT_ASSERT( a <= c );
  CPPUNIT_ASSERT( b <= c );
}

void
FileOpenKindTest::testClone(){
  FileOpenKind *cloned = dynamic_cast<FileOpenKind *>(toTest->clone());
  CPPUNIT_ASSERT( cloned != 0 );
  CPPUNIT_ASSERT( cloned != toTest );
  CPPUNIT_ASSERT( *cloned = *toTest );
  delete cloned;
}

void
FileOpenKindTest::testSerialization(){
  SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
  CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
  FileOpenKind *deserialized = dynamic_cast<FileOpenKind *>( serialized->deserialize() );
  CPPUNIT_ASSERT( deserialized != 0 );
  CPPUNIT_ASSERT( deserialized != toTest );
  CPPUNIT_ASSERT( *deserialized = *toTest );
  delete deserialized;
}
