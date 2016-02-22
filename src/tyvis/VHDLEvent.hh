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
#include <Event.hpp>
#include <VTime.hpp>
#include <iostream>

// this the base class that VHDL sends around
class VHDLEvent: public warped::Event {
public:
  VHDLEvent( const VTime& receivetime,
	     std::string receiver_name,
	     warped::EventType type = warped::EventType::POSITIVE) :
     ts_(&receivetime),
     receiver_name_(receiver_name),
     type_(type) {}

  virtual const std::string& receiverName() const override final { return receiver_name_; }
  virtual const VTime& timestamp() const override final { return *ts_; }
  
  ~VHDLEvent() {}
  
protected:

private:
  std::string receiver_name_;
  const VTime* ts_;
  warped::EventType type_;
};

inline std::ostream&
operator<<(std::ostream& os, const VHDLEvent& s) {
  os << "\ns.timestamp(): " << s.timestamp()
     << "\ns.receiverName(): " << s.receiverName() << "\n";
  
  return os;
}
#endif
