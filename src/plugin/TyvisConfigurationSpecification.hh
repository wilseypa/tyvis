#ifndef TYVIS_CONFIGURATIONSPECIFICATION_HH
#define TYVIS_CONFIGURATIONSPECIFICATION_HH

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

#include "TyvisDeclaration.hh"
#include "savant/IIRBase_ConfigurationSpecification.hh"

class TyvisComponentInstantiationStatement;

class TyvisConfigurationSpecification : public virtual TyvisDeclaration, public virtual IIRBase_ConfigurationSpecification {
public:
  TyvisConfigurationSpecification();
  ~TyvisConfigurationSpecification();

  void _publish_cc_port_map_aspect( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_headers( published_file & ) {}

  IIR_Boolean _has_same_component(TyvisComponentInstantiationStatement* conc_stmt);
  IIR_Boolean _configures_this_component(TyvisComponentInstantiationStatement* conc_stmt);

  Tyvis *_get_component_name( );
  TyvisLibraryUnit *_get_entity_aspect();

  // Helper Functions
  TyvisDesignatorList *_get_instantiation_list();
  TyvisAssociationList * _get_generic_map_aspect();
  TyvisAssociationList * _get_port_map_aspect();
protected:
private:
};
#endif
