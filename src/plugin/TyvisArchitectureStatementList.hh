#ifndef TYVIS_ARCHITECTURESTATEMENTLIST_HH
#define TYVIS_ARCHITECTURESTATEMENTLIST_HH

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

#include "TyvisList.hh"
#include "savant/IIRBase_ArchitectureStatementList.hh"

class TyvisArchitectureStatementList : public virtual TyvisList, public virtual IIRBase_ArchitectureStatementList {
public:
  void _publish_cc( published_file &_cc_out, PublishData *declarations  );
  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations );
  
  /** Call "_publish_cc_instantiate" on all of the members of the list. */
  void _publish_cc_instantiate_calls( published_file &_cc_out, PublishData *declarations );

  /** Function which helps in publishing the characteristic expressions */
  void _publish_cc_characteristic_expressions(published_file &);

  /** Publish the ams function's body and prototype */
  void _publish_cc_ams_function_body(published_file &);
  void _publish_cc_ams_function_prototype(published_file &);

  void _publish_cc_createNetInfo( published_file &_cc_out, PublishData *declarations );

  int _get_number_of_component_instantiations(  );

protected:

private:

};
#endif
