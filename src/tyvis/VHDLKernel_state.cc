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

#include "VHDLKernel_state.hh"
#include "Block.hh"
#include "AccessObject.hh"
#include "Wait.hh"
#include "VHDLKernel.hh"

// tyVis needs the new style of state saving mechanism of warped to implement
// types defined using generic constants etc. The information about these
// constants is available in the elab-class of the process to which the
// state belongs. This elab class pointer is available as part of the state
// that pointer will be used to copy the information from the elab class to
// to a set of global objects and the types will be based off those values
// of global constants.

VHDLKernel_state::VHDLKernel_state() : 
  procWait( 0 ),
  wait( 0 ),
  waitLabel( -1 ),
  waitExpired( false ),
  sharedReadPending( false ),
  myProc( 0 ),
  waitTime( dynamic_cast<VHDLVTime *>(VHDLVTime::getVHDLVTimeZero().clone()) ){}

VHDLKernel_state::~VHDLKernel_state() {
  delete waitTime;
  if (wait != NULL)  {
    delete [] wait;
  }
}

void
VHDLKernel_state::initState( _savant_entity_elab *initProc, VHDLKernel *) {
  waitLabel         = -1;
  waitExpired       = true;
  sharedReadPending = false;
  myProc = initProc;
}

VHDLKernel_state&
VHDLKernel_state::operator=(const VHDLKernel_state &s) {
  (State &)*this = (const State &) s;

  // numWaits = s.numWaits;
  stack             = s.stack;
  procWait          = s.procWait;
  waitLabel         = s.waitLabel;
  waitTime          = s.waitTime;
  waitExpired       = s.waitExpired;
  sharedReadPending = s.sharedReadPending;
  
  return *this;
}

// The set of functions needed for the new style of state saving in
// warped kernel. This new style is needed to handle types defined using
// generic constants etc.

void
VHDLKernel_state::copyState( const State *toCopy ) {
  const VHDLKernel_state *vhdlState = dynamic_cast<const VHDLKernel_state *>(toCopy);
  stack = vhdlState->stack;
  procWait = vhdlState->procWait;
  wait = vhdlState->wait;
  waitLabel = vhdlState->waitLabel;
  waitExpired = vhdlState->waitExpired;
  sharedReadPending = vhdlState->sharedReadPending;
  myProc = vhdlState->myProc;
  waitTime = dynamic_cast<VHDLVTime *>(vhdlState->waitTime->clone());
}

void
VHDLKernel_state::serialize(ofstream *, int) {
  cerr << "Error :: VHDLKernel_state::serialize(ofstream *, int) called\n";
  abort();
}

void
VHDLKernel_state::deserialize(ifstream *) {
  cerr << "Error :: VHDLKernel_state::deserialize(ifstream *) called\n";
  abort();
}

void
VHDLKernel_state::deserialize( ifstream *inFile, SimulationObject *simObj ){
  ASSERT ( simObj != NULL );
  VHDLKernel *base = (VHDLKernel *)simObj;
  initState( myProc, base );
  deserialize(inFile);
}
