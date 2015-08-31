
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
#include "TyvisTypeDefinition.hh"

#include "savant/savant.hh"
#include "published_file.hh"

void 
TyvisList::_publish_cc_headers( published_file &_cc_out ) {
  Tyvis *list_element = dynamic_cast<Tyvis *>(first()); 
  while( list_element != NULL ){
    ASSERT(list_element->is_resolved());
    list_element->_publish_cc_headers( _cc_out );
    list_element = dynamic_cast<Tyvis *>(successor(list_element));
  }
}


void
TyvisList::_publish_cc_static_type_info_fn( published_file &_cc_out,
					    PublishData *declarations ){
  CC_REF( _cc_out, "TyvisList::_publish_cc_type_info" );
  PublishedUnit oldUnit = _get_currently_publishing_unit();
  _publish_cc_list( _cc_out, declarations,
		    &Tyvis::_publish_cc_static_type_info_fn,
		    NO_SEPARATOR, "" );
  _set_currently_publishing_unit(oldUnit);
}

void
TyvisList::_publish_cc_associations( published_file &_cc_out,
				PublishData *declarations,
				const string &separator ){
  CC_REF( _cc_out, "TyvisList::_publish_cc_rvalues" );
  _publish_cc_list( _cc_out,
		    declarations,
		    &Tyvis::_publish_cc_state_object_init,
		    NOT_LAST_ELEMENT,
		    separator );
}

void
TyvisList::_publish_cc_l_associations( published_file &_cc_out,
				PublishData *declarations,
				const string &separator ){
  CC_REF( _cc_out, "TyvisList::_publish_cc_rvalues" );
  _publish_cc_list( _cc_out,
		    declarations,
		    &Tyvis::_publish_cc_state_l_object_init,
		    NOT_LAST_ELEMENT,
		    separator );
}

void
TyvisList::_publish_cc_rvalues( published_file &_cc_out,
				PublishData *declarations,
				const string &separator ){
  CC_REF( _cc_out, "TyvisList::_publish_cc_rvalues" );
  _publish_cc_list( _cc_out,
		    declarations,
		    &Tyvis::_publish_cc_rvalue,
		    NOT_LAST_ELEMENT,
		    separator );
}

void
TyvisList::_publish_cc_elaborate( published_file &_cc_out, PublishData *declarations ){
  CC_REF( _cc_out, "TyvisList::_publish_cc_elaborate" );
  _publish_cc_list( _cc_out, declarations,
		    &Tyvis::_publish_cc_elaborate,
		    NO_SEPARATOR, "" );
}

void 
TyvisList::_append( IIR_List *to_append ){
  ASSERT( to_append != NULL );
  // This is kind of inefficient - we could support it at a lower level and
  // hook the whole chain on at once.
  Tyvis *current = dynamic_cast<Tyvis *>(to_append->first());
  while( current != NULL ){
    append( current );
    current = dynamic_cast<Tyvis *>(to_append->successor( current ));
  }
}

void
TyvisList::_publish_cc_list( published_file &_cc_out,
			     PublishData *declarations,
			     void (Tyvis::*publish_method)( published_file &, PublishData *),
			     SeparatorMode separatorMode,
			     const string &separator ){
  Tyvis *current = dynamic_cast<Tyvis *>(first());
  while( current ){
    (current->*publish_method)( _cc_out, declarations );
    current = dynamic_cast<Tyvis *>(successor(current));
    _publish_cc_separator( _cc_out, current != 0, separatorMode, separator );
  }
}

void
TyvisList::_publish_cc_separator( published_file &_cc_out,
				  bool hasAnother,
				  SeparatorMode separatorMode,
				  const string &separator ){
  switch( separatorMode ){
  case EVEN_LAST_ELEMENT:
    _cc_out << separator << NL();
    break;
  case NOT_LAST_ELEMENT:
    if( hasAnother ){
      _cc_out << separator << NL();
    }
    break;
  case NO_SEPARATOR:
    break;
  default:
    abort();
  }
}
