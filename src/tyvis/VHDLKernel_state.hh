#ifndef VHDL_KERNEL_STATE_HH
#define VHDL_KERNEL_STATE_HH

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
#include <fstream>
#include "tyvis/CallStack.hh"
#include "tyvis/Block.hh"
#include "tyvis/Variable.hh"
#include "tyvis/VHDLVTime.hh"
#include <SimulationObject.h>
#include <State.h>

using std::ifstream;

class Wait;
class SignalBase;
class _savant_entity_elab;
class VHDLKernel;

#define CHECKPOINT_DELIMITER '|'

/** This class has been created for savant interface alone. This has some
    basic members needed for any state in Savant. */
class VHDLKernel_state : public State {
public:
  VHDLKernel_state();
  ~VHDLKernel_state();
  virtual void initState(_savant_entity_elab *, VHDLKernel *);
  VHDLKernel_state& operator=(const VHDLKernel_state&);

  const VHDLVTime &getWaitTime() const { ASSERT( waitTime != 0 ); return *waitTime; }
  void setWaitTime( const VHDLVTime &newWaitTime ){
    delete waitTime;
    waitTime = dynamic_cast<const VHDLVTime *>(newWaitTime.clone());
    ASSERT( waitTime != 0 );
  }

  CallStack stack;
  Wait *procWait;
  Wait *wait;
  int waitLabel;
  bool waitExpired;

  /** To check for updates while reading from shared files... */
  bool sharedReadPending;
  
  // The new set of functions for the new style of state saving
  // mechanism that is going to be in-place.

  /** This is the copyState function that merely does what operator= used
      to do. This will be the new interface. Obviously savant should
      erloads this guy with the published code. */
  virtual void copyState( const State * );

  virtual SignalBase* locateSig(int) = 0;
  virtual void cleanState() = 0;

  virtual void serialize(ofstream *, int);
  virtual void deserialize(ifstream *);
  virtual void deserialize(ifstream *, SimulationObject *);

private:
  _savant_entity_elab *myProc;
  const VHDLVTime *waitTime;
};

#endif

