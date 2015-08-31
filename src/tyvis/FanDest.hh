#ifndef FANDEST_HH
#define FANDEST_HH

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
#include "tyvis/VHDLKernel.hh"
#include <iostream>

using std::ostream;
class SigEvent;
class SimulationObject;

/** Signal fanout data structure--allows for slices of a signal to have
    different ID's. */

class FanDest {
public:
  FanDest( const FanDest &that );

  FanDest( const string &initProcessName, int initSigId );
  FanDest( SimulationObject *processPtr_in, int initSigId );

  FanDest &operator=( const FanDest &fd ) {
    processName = fd.getProcessName();
    return *this;
  }

  /**
     Destructor.  Since the SimulationObject pointer belongs to a
     SimulationManager, we don't do anything here.
  */
  ~FanDest(){}


  const string &getProcessName() const {
    return processName;
  }

  int getDestSigId() const {
    return destSigId;
  }

  void sendEvent( const SigEvent *toSend, 
		  SimulationObject *sourceProcess );

  FanDest *clone() const;

private:
  string processName;
  SimulationObject *processPtr;
  int destSigId;
};

#endif
