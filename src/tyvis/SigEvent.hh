#ifndef SIG_EVENT_HH
#define SIG_EVENT_HH

// Copyright (c) Clifton Labs, Inc.  All Rights Reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

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
#include <iostream>

/** class for sending signals around */
class SigEvent: public VHDLEvent {
public:
  SigEvent( const std::string& receiver_name,
	    int value,
	    const std::string& signal_name,
	    const VTime& timestamp ) :
   VHDLEvent(timestamp, receiver_name),
   value_(value), signal_name_(signal_name) {}

  ~SigEvent() {}

  int getValue() const { return value_; }
  const std::string& signalName() const { return signal_name_; }

private:
  const int             value_;
  const std::string     signal_name_;
};

inline std::ostream&
operator<<(std::ostream& os, const SigEvent& s) {
  os << static_cast<const VHDLEvent &>(s)
     << "\ns.getValue(): " << s.getValue()
     << "\ns.signalName(): " << s.signalName() << "\n";
  
  return os;
}

inline int
SigEventCompareRecv(const SigEvent* a, const SigEvent* b) {
  register int retVal;  
  if ( a->timestamp() > b->timestamp() ) {
    retVal = 1;
  }
  else if ( a->timestamp() < b->timestamp()) {
    retVal = -1;
  }
  else {
    retVal = 0;
  }
  return retVal;
}


#endif
