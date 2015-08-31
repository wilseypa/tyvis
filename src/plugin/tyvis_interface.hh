#ifndef TYVIS_INTERFACE
#define TYVIS_INTERFACE

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

#include "savant/plugin_interface.hh"

/** TyVIS Implementation of the plugin interface. */
class tyvis_interface : public plugin_interface {
public:
  /** @return a string saying what the module is for. */
  const string getPluginType() const;

  /** @return the name of the plugin. */
  const string getPluginName() const; 

  /** Publishes C++.  This function is the start of the TyVIS C++
      code generation interface section. */
  IIR* process_tree(IIR*, int argc, char *argv[]);
};

#endif
