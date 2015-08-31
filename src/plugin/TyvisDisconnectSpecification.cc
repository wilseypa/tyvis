
// Copyright (c) 1996-2003 The University of Cincinnati.
// All rights reserved.

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

// Authors: Philip A. Wilsey    philip.wilsey@ieee.org
//          Dale E. Martin      dmartin@cliftonlabs.com

//---------------------------------------------------------------------------

#include "TyvisDesignFile.hh"
#include "TyvisDisconnectSpecification.hh"
#include "TyvisPhysicalSubtypeDefinition.hh"
#include "TyvisDesignatorList.hh"
#include "TyvisDesignator.hh"
#include "savant/error_func.hh"
#include "savant/resolution_func.hh"
#include "savant/set.hh"
#include "savant/StandardPackage.hh"
#include <sstream>

TyvisDisconnectSpecification::TyvisDisconnectSpecification() {
  set_guarded_signal_list(new TyvisDesignatorList());
}

TyvisDisconnectSpecification::~TyvisDisconnectSpecification(){
  //Release the list memory
  delete get_guarded_signal_list();
}

TyvisDesignatorList *
TyvisDisconnectSpecification::_get_guarded_signal_list() {
  return dynamic_cast<TyvisDesignatorList *>(get_guarded_signal_list());
}

TyvisTypeDefinition *
TyvisDisconnectSpecification::_get_type_mark() {
  return dynamic_cast<TyvisTypeDefinition *>(get_type_mark());
}

Tyvis *
TyvisDisconnectSpecification::_get_time_expression() {
  return dynamic_cast<Tyvis *>(get_time_expression());
}
