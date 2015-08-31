#ifndef TYVIS_LIST_HH
#define TYVIS_LIST_HH

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

#include "Tyvis.hh"
#include "savant/IIRBase_List.hh"

class TyvisList : public virtual Tyvis, public virtual IIRBase_List {
public:

  void _publish_cc_headers( published_file &_cc_out );
  
  /** Append all elements in this non-NULL list. */
  virtual void _append( IIR_List * );

  /**
     Call _publish_cc_state_object_init on every element.  Put the separator between
     the elements (except the last).
  */
  void _publish_cc_associations( published_file &_cc_out,
			    PublishData *declarations,
			    const string &separator);
  void _publish_cc_l_associations( published_file &_cc_out,
			    PublishData *declarations,
			    const string &separator);

  /**
     Call _publish_cc_rvalue on every element.  Put the separator between
     the elements (except the last).
  */
  void _publish_cc_rvalues( published_file &_cc_out,
			    PublishData *declarations,
			    const string &separator );

  void _publish_cc_elaborate( published_file &_cc_out,
			      PublishData *declarations );

  /**
     Publish any TypeInfos defined in our members.
  */
  void _publish_cc_static_type_info_fn( published_file &_cc_out, PublishData *declarations );

  enum SeparatorMode { EVEN_LAST_ELEMENT, NOT_LAST_ELEMENT, NO_SEPARATOR };

  void _publish_cc_list( published_file &_cc_out,
			 PublishData *declarations,
			 void (Tyvis::*publish_method)( published_file &, PublishData *),
			 SeparatorMode seperatorMode,
			 const string &separator );

protected:
  void _publish_cc_separator( published_file &_cc_out,
			      bool hasAnother,
			      SeparatorMode separatorMode,
			      const string &separator );

private:

};
#endif
