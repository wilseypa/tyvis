#include "VHDLEventTest.hh"
#include "tyvis/VHDLEvent.hh"

const VHDLVTime &
VHDLEventTest::getDefaultSendTime(){
  static const VHDLVTime defaultTime( 1, 1 );
  return defaultTime;
}
const VHDLVTime &
VHDLEventTest::getDefaultReceiveTime(){
  static const VHDLVTime defaultTime( 2, 2 );
  return defaultTime;
}

const string &
VHDLEventTest::getDefaultSender(){
  static const string defaultSender = "default sender";
  return defaultSender;
}

const string &
VHDLEventTest::getDefaultReceiver(){
  static const string defaultReceiver = "default receiver";
  return defaultReceiver;
}

void
VHDLEventTest::checkDefaults( const VHDLEvent *toTest ){
  CPPUNIT_ASSERT( toTest->getSendTime() == getDefaultSendTime() );
  CPPUNIT_ASSERT( toTest->getReceiveTime() == getDefaultReceiveTime() );
  CPPUNIT_ASSERT( toTest->getSender() == getDefaultSender() );
  CPPUNIT_ASSERT( toTest->getReceiver() == getDefaultReceiver() );
  
}

void 
VHDLEventTest::testConstructor( const VHDLEvent *toTest ){
  checkDefaults( toTest );
}

void 
VHDLEventTest::testSerialization( const VHDLEvent *toTest, const string &dataType ){
  checkDefaults( toTest );
  SerializedInstance *serialized = static_cast<const Serializable *>(toTest)->serialize();
  CPPUNIT_ASSERT( serialized != 0 );
  CPPUNIT_ASSERT( dataType == serialized->getDataType() );
  VHDLEvent *deserialized = dynamic_cast<VHDLEvent *>(serialized->deserialize());
  CPPUNIT_ASSERT( deserialized != 0 );
  CPPUNIT_ASSERT( deserialized != toTest );
  checkDefaults( deserialized );
}

void 
VHDLEventTest::testEventCompare( const VHDLEvent *toTest ){
  checkDefaults( toTest );
}
