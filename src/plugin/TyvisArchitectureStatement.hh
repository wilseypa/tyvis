#ifndef TYVIS_ARCHITECTURESTATEMENT_HH
#define TYVIS_ARCHITECTURESTATEMENT_HH

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

#include "TyvisStatement.hh"
#include "savant/IIRBase_ArchitectureStatement.hh"

class TyvisArchitectureStatement : public virtual TyvisStatement, public virtual IIRBase_ArchitectureStatement {
public:
  TyvisArchitectureStatement();
  ~TyvisArchitectureStatement();

  Tyvis* _transmute();

  void _publish_cc( published_file &_cc_out, PublishData *declarations  );
  void _publish_cc_scoping_prefix( ostream &_cc_out, Tyvis *, Tyvis * );
  void _publish_cc_state( published_file &_cc_out );
  virtual void _publish_cc_binding_name( ostream &outstream );
  virtual void _publish_cc_concurrent_stmt_init( published_file &_cc_out, TyvisDeclarationList*, PublishData * );

  /**
     Implements the publication of the call to the instantiate method for a
     concurrent statement to the published file @_cc_out. The instantiate
     method will instantiate a concurrent statement into the actual
     hierarchial description at the time of static elaboration prior to the
     dynamic elaboration of the simulation. This is where a concurrent
     statement may do its appropiate preparations.

     /param _cc_out The stream to publish the call to.
  */
  virtual void _publish_cc_instantiate_call( published_file &_cc_out, PublishData *declarations );
  
  /**
     Publish the connect calls that are used at elaborate time.
  */
  virtual void _publish_cc_connect_call( published_file &, PublishData * ){}

  virtual void _publish_createNetInfo( published_file &_cc_out, PublishData *declarations );

  void _publish_cc_headerfiles_for_cc_generate_statement( published_file &_cc_out );

  /**
     Implements _publish_cc_class for the two types of generate statements.
   */
  void _publish_cc_class_generate_stmt( published_file &_cc_out, PublishData *declarations );

  virtual void _publish_cc_characteristic_expressions(SimultaneousIfPublishingPart, published_file &);

  Tyvis* _get_declarative_region();
  const string &_get_enclosing_path() const;
  
  void _publish_cc_enclosing_stmt_to_architecture_path(ostream& outstream);

  virtual TyvisAssociationList *_get_generic_map_aspect() {
    _report_undefined_fn("_get_generic_map_aspect()");
    return NULL;
  }

  virtual Tyvis* _get_instantiated_unit() {
    _report_undefined_fn("_get_instantiated_unit()");
    return NULL;
  };

protected:
  string _get_enclosing_stmt_to_architecture_path();
private:
  string enclosing_path;

};
#endif
