#ifndef WAIT_EVENT_HH
#define WAIT_EVENT_HH

// Copyright (c) Ohio Board of Regents and the University of Cincinnati.  
// All Rights Reserved.

// UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
// LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR
// DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "tyvis/tyvis-config.h"
#include <DeserializerManager.h>
#include "tyvis/VHDLEvent.hh"
#include "tyvis/VHDLKernel.hh"
#include "tyvis/VHDLVTime.hh"

/** class for a VHDLProcess to send wait expirations to itself */
class WaitEvent: public VHDLEvent {
public:
  WaitEvent( const VHDLVTime &sendTime, 
	     const VHDLVTime &receiveTime, 
	     SimulationObject *sender,
	     SimulationObject *receiver, 
	     int initWaitId ) : 
    VHDLEvent( sendTime, receiveTime, sender, receiver ),
    waitId( initWaitId ){}

  explicit WaitEvent(const WaitEvent *we) : VHDLEvent( we ) {
    waitId = we->waitId;
  }

  ~WaitEvent() {}

  void serialize( SerializedInstance *serializeInto ) const {
    VHDLEvent::serialize( serializeInto );
    serializeInto->addInt( waitId );
  }

  static Serializable *deserialize( SerializedInstance *si ){
    VHDLVTime *sendTime = dynamic_cast<VHDLVTime *>(si->getSerializable());
    VHDLVTime *recvTime = dynamic_cast<VHDLVTime *>(si->getSerializable());
    unsigned int senderSimManID = si->getUnsigned();
    unsigned int senderSimObjID = si->getUnsigned();
    unsigned int receiverSimManID = si->getUnsigned();
    unsigned int receiverSimObjID = si->getUnsigned();
    unsigned int eventId = si->getUnsigned();
    int waitId = si->getInt();
    
    ObjectID sender(senderSimObjID, senderSimManID);
    ObjectID receiver(receiverSimObjID, receiverSimManID);

    return new WaitEvent( *sendTime,
			  *recvTime,
			  sender,
			  receiver,
			  eventId,
			  waitId );
  }

//   WaitEvent& operator=(const WaitEvent& src) {
//     VHDLEvent::operator=((const VHDLEvent &) src);
//     waitId = src.waitId;

//     return *this;
//   }

  virtual void execute( VHDLKernel *onProcess ){
    onProcess->updateWait( this );
  }

  int getWaitId() const { return waitId; }

  static const string &getWaitEventType(){
    static string waitEventId = "WaitEvent";
    return waitEventId;
  }
  
  const string &getDataType() const { return getWaitEventType(); }

  static void registerDeserializer(){
    DeserializerManager::instance()->registerDeserializer( getWaitEventType(),
							   &WaitEvent::deserialize );
  }

  unsigned int getEventSize() const {
    return sizeof(WaitEvent);
  }

protected:
 
private:
  /**
     Constructor used by deserializer.
  */
  WaitEvent( const VHDLVTime &sendTime, 
	     const VHDLVTime &receiveTime, 
	     const ObjectID &sender,
	     const ObjectID &receiver, 
	     unsigned int eventId,
	     int initWaitId ) : 
    VHDLEvent( sendTime, receiveTime, sender, receiver, eventId ),
    waitId( initWaitId ){}

  int waitId;
};

#endif
