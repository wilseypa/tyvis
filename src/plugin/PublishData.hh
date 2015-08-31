#ifndef PUBLISH_DATA
#define PUBLISH_DATA

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

#include <set>
#include <vector>
#include "savant/IIR_Declaration.hh"

using std::vector;

/** This class is used by the publishing methods to store the declarations they find. */
class PublishData {
public:
  PublishData();
  ~PublishData();

  /// Get the set based on it's declaration type
  std::set<IIR_Declaration*> *get_set( IIR_Declaration::declaration_type );

  /// Check the entire PublishData collection for the declaration
  bool in_collection( IIR_Declaration * );
  /// Add a new declaration to the collections
  void add_declaration( IIR_Declaration * );
  
  /// Returns a set containing all the declarations with that name
  std::set<IIR_Declaration*> *find_set( IIR_TextLiteral *name );

private:
  vector<std::set<IIR_Declaration*> *> collection;
};
#endif
