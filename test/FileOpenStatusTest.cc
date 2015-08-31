#include "FileOpenStatusTest.hh"
#include <warped/SerializedInstance.h>
#include <clutils/StringUtilities.h>

int
FileOpenStatusTest::getDefaultValue(){
  static int defaultValue = 3;
  return defaultValue;
}

void 
FileOpenStatusTest::setUp(){
  toTest = new FileOpenStatus( FileOpenStatus::NAME_ERROR );
}

void 
FileOpenStatusTest::tearDown(){
  delete toTest;
}

void
FileOpenStatusTest::testGetUniversalKind(){
  CPPUNIT_ASSERT( toTest->getUniversalKind() == VHDLData::FILE_OPEN_STATUS );
}

void
FileOpenStatusTest::testSize(){
  CPPUNIT_ASSERT( toTest->size() == 1 );
}

void
FileOpenStatusTest::testPrint(){
  ostringstream printStream;
  toTest->print( printStream );
  CPPUNIT_ASSERT( "NAME_ERROR" == printStream.str() );
}

void
FileOpenStatusTest::testEqual(){
  FileOpenStatus a(FileOpenStatus::STATUS_ERROR);
  FileOpenStatus b(FileOpenStatus::STATUS_ERROR);
  
  CPPUNIT_ASSERT( a == b );
}

void
FileOpenStatusTest::testNotEqual(){
  FileOpenStatus a(FileOpenStatus::STATUS_ERROR);
  FileOpenStatus b(FileOpenStatus::OPEN_OK);
  
  CPPUNIT_ASSERT( a != b );
}

void
FileOpenStatusTest::testGreater(){
  FileOpenStatus a(FileOpenStatus::OPEN_OK);
  FileOpenStatus b(FileOpenStatus::STATUS_ERROR);
  
  CPPUNIT_ASSERT( b > a );
}

void
FileOpenStatusTest::testGreaterEqual(){
  FileOpenStatus a(FileOpenStatus::OPEN_OK);
  FileOpenStatus b(FileOpenStatus::NAME_ERROR);
  FileOpenStatus c(FileOpenStatus::NAME_ERROR);
  
  CPPUNIT_ASSERT( b >= a );
  CPPUNIT_ASSERT( b >= c );
}

void
FileOpenStatusTest::testLess(){
  FileOpenStatus a(FileOpenStatus::OPEN_OK);
  FileOpenStatus b(FileOpenStatus::STATUS_ERROR);
  
  CPPUNIT_ASSERT( a < b );
}

void
FileOpenStatusTest::testLessEqual(){
  FileOpenStatus a(FileOpenStatus::OPEN_OK);
  FileOpenStatus b(FileOpenStatus::MODE_ERROR);
  FileOpenStatus c(FileOpenStatus::MODE_ERROR);
  
  CPPUNIT_ASSERT( a <= c );
  CPPUNIT_ASSERT( b <= c );
}

void
FileOpenStatusTest::testClone(){
  FileOpenStatus *cloned = dynamic_cast<FileOpenStatus *>(toTest->clone());
  CPPUNIT_ASSERT( cloned != 0 );
  CPPUNIT_ASSERT( cloned != toTest );
  CPPUNIT_ASSERT( *cloned == *toTest );
  delete cloned;
}

void
FileOpenStatusTest::testSerialization(){
  SerializedInstance *serialized = static_cast<Serializable *>(toTest)->serialize();
  CPPUNIT_ASSERT( serialized->getDataType() == toTest->getDataType() );
  FileOpenStatus *deserialized = dynamic_cast<FileOpenStatus *>( serialized->deserialize() );
  CPPUNIT_ASSERT( deserialized != 0 );
  CPPUNIT_ASSERT( deserialized != toTest );
  CPPUNIT_ASSERT( *deserialized == *toTest );
  delete deserialized;
}
