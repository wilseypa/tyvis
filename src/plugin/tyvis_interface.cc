// Copyright (c) 2003-2004 The University of Cincinnati.  
// All rights reserved.
// 
// UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
// LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
// 
// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.
// 
// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.
// 
// Authors: Philip A. Wilsey	philip.wilsey@ieee.org
// 	    Dale E. Martin	dmartin@cliftonlabs.com
// 
// --------------------------------------------------------------------------

#include "tyvis_interface.hh"
#include "tyvis_plugin_class_factory.hh"

#include "savant/IIRBase.hh"
#include "TyvisDesignFile.hh"

IIR* 
tyvis_interface::process_tree(IIR *old_tree, int, char *argv[]) {
  IIR                   *new_tree = NULL;

  new_tree =
    dynamic_cast<IIRBase *>(old_tree)->convert_tree(tyvis_plugin_class_factory::instance());
  bool last_tree = static_cast<bool>(argv[0]);
  dynamic_cast<TyvisDesignFile *>(new_tree)->_publish_cc( last_tree );

  return new_tree;
}

/** @return a string saying what the module is for. */
const string 
tyvis_interface::getPluginType() const { 
  return string("Savant AIRE Plugin"); 
}

/** @return the name of the plugin. */
const string 
tyvis_interface::getPluginName() const { 
  return string("tyvis publishing plugin"); 
}

extern "C" {
  void *allocatelibtyvisplugin() {
    return new tyvis_interface();
  }
}
