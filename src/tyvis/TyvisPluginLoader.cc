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

#include "TyvisPluginLoader.hh"
#include "VHDLApplication.hh"
#include <PluginManager.h>

TyvisPluginLoader &
TyvisPluginLoader::instance(){
  static TyvisPluginLoader *singleton = new TyvisPluginLoader();
  return *singleton;
}

VHDLApplication *
TyvisPluginLoader::getVHDLApplication( const string &pluginFileName ){
  VHDLApplication *retval = 0;
  
  _savant_entity_elab *entityElab = loadEntityElab( pluginFileName );
  if( entityElab != 0 ){
    retval = new VHDLApplication( entityElab );
  }

  return retval;
}

TyvisPluginLoader::TyvisPluginLoader(){}

TyvisPluginLoader::~TyvisPluginLoader(){}

_savant_entity_elab *
TyvisPluginLoader::loadEntityElab( const string &pluginFileName ){
  _savant_entity_elab *retval = 0;

  PluginManager *mm = PluginManager::instance();
  retval = reinterpret_cast<_savant_entity_elab *>(mm->loadPlugin( pluginFileName ));
  if( retval == 0 ){
    std::cerr << pluginFileName << " could not be loaded, error is \"" <<
      mm->getLastError() << "\"." << std::endl;
  }

  return retval;
}
