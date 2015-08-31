#ifndef SHARED_FILE_EVENT_HH
#define SHARED_FILE_EVENT_HH

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
#include "tyvis/VHDLVTime.hh"
#include "tyvis/VHDLEvent.hh"
#include "tyvis/VHDLKernel.hh"
#include <warped/DeserializerManager.h>
#include <vector>

using std::vector;

class SharedFileEvent : public VHDLEvent {
public:
  SharedFileEvent( const VHDLVTime &initSendTime,
		   const VHDLVTime &initRecvTime,
		   const string &initSender,
		   const string &initReceiver,
		   const string &initData ) :
    VHDLEvent( initSendTime,
	       initRecvTime,
	       initSender,
	       initReceiver ),
    data( initData ),
    readFlag( false ){}

  explicit SharedFileEvent( const SharedFileEvent *sfe ) : 
    VHDLEvent( dynamic_cast<const VHDLVTime &>(sfe->getSendTime()),
               dynamic_cast<const VHDLVTime &>(sfe->getReceiveTime()),
	       sfe->getSender(),
	       sfe->getReceiver() ),
    data( sfe->data ),
    readFlag( sfe->readFlag ){}
  
  ~SharedFileEvent() {}
  
  static const string &getSharedFileEventType(){
    static const string sharedFileEventType = "SharedFileEvent";
    return sharedFileEventType;
  }

  const string &getDataType() const {
    return getSharedFileEventType();
  }

  void serialize( SerializedInstance *serializeInto ) const {
    VHDLEvent::serialize( serializeInto );
    serializeInto->addString( data );
  }

  static Serializable *deserialize( SerializedInstance *si ){
    VHDLVTime *sendTime = dynamic_cast<VHDLVTime *>(si->getSerializable());
    VHDLVTime *recvTime = dynamic_cast<VHDLVTime *>(si->getSerializable());
    const string sender = si->getString();
    const string receiver = si->getString();
    const unsigned int eventId = si->getUnsigned();
    const string data = si->getString();

    SharedFileEvent *retval = new SharedFileEvent( *sendTime,
						   *recvTime,
						   sender,
						   receiver,
						   eventId,
						   data );
    
    return retval;
  }

  bool isReadRequest() const {
    return readFlag;
  }

  void execute( VHDLKernel *executeOn ){
    ASSERT( executeOn != 0 );
    if( executeOn->hasSharedReadPending() ){
      executeOn->setSharedData( getData() );
    }
  }

  const string &getData() const { return data; }

  static void registerDeserializer(){
    DeserializerManager::instance()->registerDeserializer( getSharedFileEventType(),
							   &SharedFileEvent::deserialize );
  }

protected:
  
private:
  /**
     Constructor used by deserializers.
  */
  SharedFileEvent( const VHDLVTime &initSendTime,
		   const VHDLVTime &initRecvTime,
		   const string &initSender,
		   const string &initReceiver,
		   const unsigned int eventId,
		   const string &initData ) :
    VHDLEvent( initSendTime,
	       initRecvTime,
	       initSender,
	       initReceiver,
	       eventId ),
    data( initData ){}


  const string data;
  bool readFlag;
};

#endif
