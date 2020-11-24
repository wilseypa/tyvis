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
#include "tyvis/VHDLEvent.hh"
#include "tyvis/VHDLKernel.hh"
#include "tyvis/VHDLVTime.hh"

/** class for a VHDLProcess to send wait expirations to itself */
class WaitEvent: public VHDLEvent {
public:
  WaitEvent( VHDLVTime &receiveTime, 
	     std::string& receiver, 
	     int initWaitId ) : 
    VHDLEvent( receiveTime, receiver ),
    waitId( initWaitId ){}

  ~WaitEvent() {}

  int getWaitId() const { return waitId; }

  static const string &getWaitEventType(){
    static string waitEventId = "WaitEvent";
    return waitEventId;
  }
  
  const string &getDataType() const { return getWaitEventType(); }

  unsigned int getEventSize() const {
    return sizeof(WaitEvent);
  }
 
private:
  int waitId;
};

#endif
