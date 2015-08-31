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

#include "FanDest.hh"
#include "SigEvent.hh"
#include "VHDLKernel.hh"
#include <string>
using std::string;

FanDest::FanDest( const FanDest &that ) :
  processName( that.processName ),
  processPtr( that.processPtr ),
  destSigId( that.destSigId ){}

FanDest::FanDest( const string &initProcessName, int initDestSigId ) :
  processName( initProcessName ),
  processPtr(0),
  destSigId( initDestSigId ){}

FanDest::FanDest( SimulationObject *processPtr_in, int initSigId ):
  processName( processPtr_in->getName() ),
  processPtr( processPtr_in ),
  destSigId( initSigId ){}

void 
FanDest::sendEvent( const SigEvent *toSend, 
		    SimulationObject *sourceProcess ){
  ASSERT( toSend != 0 );
  ASSERT( sourceProcess != 0 );

  SigEvent *copy = toSend->clone();
  copy->setDestSigId( destSigId );
  
  if( processPtr == 0 ){
    processPtr = sourceProcess->getObjectHandle( processName );
  }
  ASSERT( processPtr != 0 );

  copy->setReceiver( processPtr );
  processPtr->receiveEvent( copy );
}


FanDest *
FanDest::clone() const {
  return new FanDest( *this );
}
