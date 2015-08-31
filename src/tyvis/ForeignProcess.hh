#ifndef FOREIGN_PROCESS_HH
#define FOREIGN_PROCESS_HH

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
#include "tyvis/VHDLProcess.hh"
#include <string>

using std::string;
class _savant_entity_elab;

class ForeignProcess : public VHDLProcess {
public:
  /**
     This method takes a .so filename as a parameter and loads a module
     using it.  This is the only intended method of instantiating a foreign
     process.
  */
  static ForeignProcess *instantiate( const string &so_file,
				      const string &processName, 
				      _savant_entity_elab *initElabObj );
  
  /**
     Destructor
  */
  virtual ~ForeignProcess() = 0;

  /**
     Initialization hook.
  */
  virtual void initialize() = 0;

  void createNetInfo();

protected:
  /**
     Constructor
  */
  ForeignProcess( const string &processName, 
		  _savant_entity_elab *initElabObj );
  
private:
  const string myProcessName;
};

#endif
