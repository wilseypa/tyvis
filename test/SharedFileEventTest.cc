#include <vector>
using std::vector;

#include "SharedFileEventTest.hh"
#include "tyvis/SharedFileEvent.hh"

const string &
SharedFileEventTest::getDefaultData(){
  static const string defaultData( "foo" );
  return defaultData;
}

void 
SharedFileEventTest::setUp(){
  vector<char> data;

  toTest1 = new SharedFileEvent( getDefaultSendTime(),
				 getDefaultReceiveTime(),
				 getDefaultSender(),
				 getDefaultReceiver(),
				 getDefaultData() );
				 
}

void 
SharedFileEventTest::tearDown(){
  delete toTest1;
}


void 
SharedFileEventTest::testConstructor(){
  VHDLEventTest::testConstructor( toTest1 );
}

void 
SharedFileEventTest::testSerialization(){
  VHDLEventTest::testSerialization( toTest1, SharedFileEvent::getSharedFileEventType() );
  
  SerializedInstance *serialized = static_cast<const Serializable *>(toTest1)->serialize();
  const SharedFileEvent *deserialized = dynamic_cast<SharedFileEvent *>(serialized->deserialize());
  CPPUNIT_ASSERT( deserialized != 0 );
  CPPUNIT_ASSERT( deserialized->getData() == getDefaultData() );
}

void 
SharedFileEventTest::testEventCompare(){
  VHDLEventTest::testEventCompare( toTest1 );

  CPPUNIT_ASSERT( toTest1->eventCompare( toTest1 ) == true );
}
