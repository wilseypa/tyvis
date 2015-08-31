#ifndef VHDL_EVENT_HH
#define VHDL_EVENT_HH

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
#include <warped/DefaultEvent.h>
#include <warped/SerializedInstance.h>
#include "tyvis/ArrayInfo.hh"

class VHDLKernel;
//#include "tyvis/vhdl.hh"

// this the base class that VHDL sends around
class VHDLEvent: public DefaultEvent {
public:
  VHDLEvent( const VHDLVTime &sendTime,
	     const VHDLVTime &receiveTime,
	     SimulationObject *sender,
	     SimulationObject *receiver ) : 
    DefaultEvent( sendTime, receiveTime, sender, receiver ){}
  
  explicit VHDLEvent(const VHDLEvent *v) : DefaultEvent( v->getSendTime(),
							 v->getReceiveTime(),
							 v->getSender(),
							 v->getReceiver(),
                                                         v->getEventId() ){}
  
  ~VHDLEvent() {}
  
  void serialize( SerializedInstance *serializeInto ) const {
    Event::serialize( serializeInto );
  }
  
  virtual void execute( VHDLKernel *onProcess ) = 0;

  bool eventCompare(const Event* event) {
    return (getReceiveTime() == event->getReceiveTime());
  }
  
  virtual bool isCancelTransactionEvent() const { return false; }
  virtual bool isSigEvent() const { return false; }

protected:
  /** Constructor used by deserializers. */
  VHDLEvent( const VHDLVTime &sendTime,
	     const VHDLVTime &receiveTime,
	     const ObjectID &sender,
	     const ObjectID &receiver,
	     unsigned int eventId ) : 
    DefaultEvent( sendTime, receiveTime, sender, receiver, eventId ){}

private:
};

#endif
