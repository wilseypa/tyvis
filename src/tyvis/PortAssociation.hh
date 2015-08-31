#ifndef _PORT_ASSOCIATION_HH_
#define _PORT_ASSOCIATION_HH_

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
#include "tyvis/Types.hh"
class SignalNetinfo;

class PortAssociation {

public:
  PortAssociation();
  PortAssociation( SignalNetinfo *actual, SignalNetinfo *formal, PortType );
  ~PortAssociation();
  
  void setActual(SignalNetinfo *actual) {
    actualPort = actual;
  }

  void setFormal(SignalNetinfo *formal) {
    formalPort = formal;
  }

  void setMode(PortType portMode) {
    mode = portMode;
  }

  SignalNetinfo *getActual() const {
    return actualPort;
  }

  SignalNetinfo *getFormal() const {
    return formalPort;
  }

  PortType getMode() const {
    return mode;
  }

private:
  SignalNetinfo *actualPort;
  SignalNetinfo *formalPort;
  PortType mode;
};
#endif
