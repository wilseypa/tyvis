#include "WaitEventTest.hh"
#include "tyvis/WaitEvent.hh"

int
WaitEventTest::getDefaultWaitId(){
  static int defaultWaitId = 17;

  return defaultWaitId;
}

void 
WaitEventTest::setUp(){
  vector<char> data;

  toTest1 = new WaitEvent( getDefaultSendTime(),
			   getDefaultReceiveTime(),
			   getDefaultSender(),
			   getDefaultReceiver(),
			   getDefaultWaitId() );
				 
}

void 
WaitEventTest::tearDown(){
  delete toTest1;
}


void 
WaitEventTest::testConstructor(){
  VHDLEventTest::testConstructor( toTest1 );
}

void 
WaitEventTest::testSerialization(){
  VHDLEventTest::testSerialization( toTest1, WaitEvent::getWaitEventType() );
  
  SerializedInstance *serialized = static_cast<const Serializable *>(toTest1)->serialize();
  const WaitEvent *deserialized = dynamic_cast<WaitEvent *>(serialized->deserialize());
  CPPUNIT_ASSERT( deserialized != 0 );
  CPPUNIT_ASSERT( deserialized->getWaitId() == getDefaultWaitId() );
}

void 
WaitEventTest::testEventCompare(){
  VHDLEventTest::testEventCompare( toTest1 );

  CPPUNIT_ASSERT( toTest1->eventCompare( toTest1 ) == true );
}
