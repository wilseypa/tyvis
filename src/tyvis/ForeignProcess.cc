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

#include "ForeignProcess.hh"
#include "ForeignProcessFactory.hh"
#include <clutils/PluginManager.h>
#include <clutils/Debug.h>

ForeignProcess::ForeignProcess( const string &processName, 
				_savant_entity_elab *initElabObj ) :
  VHDLProcess( processName, initElabObj ){
  clutils::debug << "ForeignProcess::ForeignProcess( " << processName << ", " 
		 << initElabObj << " ) called" << endl;
}

ForeignProcess::~ForeignProcess(){}

ForeignProcess * 
ForeignProcess::instantiate( const string &soFile, 
			     const string &processName, 
			     _savant_entity_elab *owner ){
  clutils::debug << "ForeignProcess::instantiate called!" << endl;

  ForeignProcess *retval = 0;
  PluginBase *module = PluginManager::instance()->loadPlugin( soFile );
  if( module == 0 ){
    cerr << "Error loading module from " << soFile << ":" 
	 << PluginManager::instance()->getLastError() << endl;
  }
  else{
    // Now we have a factory, which we can use to instiate the real thing.
    ForeignProcessFactory *factory = static_cast<ForeignProcessFactory *>( module ); 
    retval = factory->createProcess( processName, owner );
  }

  return retval;
}

void
ForeignProcess::createNetInfo(){
  cerr << "ForeignProcess::createNetInfo() called!" << endl;
  abort();
}
