#include "SigEventTest.hh"
#include "tyvis/SigEvent.hh"
#include "tyvis/STDTypes.hh"

const VHDLData &
SigEventTest::getDefaultVHDLData(){
  static const UniversalInteger defaultVHDLData;

  return defaultVHDLData;
}

void 
SigEventTest::setUp(){
  toTest1 = new SigEvent( getDefaultSendTime(),
			  getDefaultReceiveTime(),
			  getDefaultSender(),
			  getDefaultReceiver(),
			  getDefaultVHDLData() );
				 
}

void 
SigEventTest::tearDown(){
  delete toTest1;
}


void 
SigEventTest::testConstructor(){
  VHDLEventTest::testConstructor( toTest1 );
}

void 
SigEventTest::testSerialization(){
  VHDLEventTest::testSerialization( toTest1, SigEvent::getSigEventType() );
  
  SerializedInstance *serialized = static_cast<const Serializable *>(toTest1)->serialize();
  const SigEvent *deserialized = dynamic_cast<SigEvent *>(serialized->deserialize());
  CPPUNIT_ASSERT( deserialized != 0 );
  CPPUNIT_ASSERT( deserialized->getData() == getDefaultVHDLData() );
}

void 
SigEventTest::testEventCompare(){
  VHDLEventTest::testEventCompare( toTest1 );

  CPPUNIT_ASSERT( toTest1->eventCompare( toTest1 ) == true );
}
