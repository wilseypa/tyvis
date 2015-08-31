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

#include "AdditionalDrivers.hh"
#include "SignalBase.hh"
#include <iostream>
using std::cerr;
using std::endl;

AdditionalDrivers::AdditionalDrivers( const AdditionalDrivers &that ) :
  drivers( that.drivers ){}

AdditionalDrivers &
AdditionalDrivers::operator=( const AdditionalDrivers &that ){
  drivers = that.drivers;
  return *this;
}

void
AdditionalDrivers::addDriver( SignalBase *toAdd ){
  drivers.add( toAdd );
}

void
AdditionalDrivers::addDrivers( const AdditionalDrivers &toAdd ){
  drivers.add( const_cast<Set<SignalBase> *>(&toAdd.drivers) );
}

void
AdditionalDrivers::updateDriver( int sigId,
				 VHDLKernel *srcProcess,
				 const RValue *data,
				 const ArrayInfo *sInfo, 
				 const ArrayInfo *dInfo ){
  SignalBase *driverInBlock = 0;
  SignalBase *currentDriver = drivers.getElement();
  while( currentDriver != 0 ){
    SignalBase *driverInBlock = currentDriver->findSigInBlock( sigId, srcProcess );
    if ( driverInBlock != 0 ){
      driverInBlock->updateDriver( sigId, srcProcess, data, sInfo, dInfo );
      break;
    }
    currentDriver = drivers.getNextElement();
  }
  if( driverInBlock == 0 ){
    cerr << "AdditionalDrivers::updateDriver( " << sigId << ", " << srcProcess->getName()
	 << ", " << *data << ", " << *sInfo << ", " << *dInfo << " ) did not "
	 << "find driver." << endl;
    abort();
  }
}

int
AdditionalDrivers::length() const {
  return drivers.size();
}
