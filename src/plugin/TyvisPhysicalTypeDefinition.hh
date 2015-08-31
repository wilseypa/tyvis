#ifndef TYVIS_PHYSICALTYPEDEFINITION_HH
#define TYVIS_PHYSICALTYPEDEFINITION_HH

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

#include "TyvisScalarTypeDefinition.hh"
#include "savant/IIRBase_PhysicalTypeDefinition.hh"

class TyvisUnitList;
class TyvisPhysicalUnit;

class TyvisPhysicalTypeDefinition : public virtual TyvisScalarTypeDefinition, public virtual IIRBase_PhysicalTypeDefinition {
public:
  TyvisPhysicalTypeDefinition();
  ~TyvisPhysicalTypeDefinition();

  void _publish_cc_elaborate( published_file &_cc_out, PublishData *declarations );
  void _publish_cc_universal_type( published_file &_cc_out );

  const string _get_cc_tyvis_type();
  /**
     Returns the name of the method that is generated that contains the
     unit list for this type.
  */
  static string _get_cc_units_method( const string &type_name );
  void _publish_cc_type_info_prereq( published_file &_cc_out, PublishData *declarations, const string &type_name );

  void _publish_cc_type_info_args( published_file &_cc_out, PublishData *declarations, const string &type_name );

  bool _is_time();

  // Helper Functions
  TyvisUnitList *           _get_units();
  TyvisPhysicalUnit *       _get_primary_unit();
protected:

private:

};
#endif
