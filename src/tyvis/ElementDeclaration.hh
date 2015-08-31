#ifndef ELEMENT_DECLARATION_HH
#define ELEMENT_DECLARATION_HH

// Copyright (c) Clifton Labs, Inc.  All Rights Reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "tyvis/tyvis-config.h"
#include "tyvis/ObjectBase.hh"

#include <string>
using std::string;

class TypeInfo;
class RValue;

class ElementDeclaration : public ObjectBase {
public:
  ElementDeclaration( const TypeInfo &initTypeInfo,
		      const string &initIdentifier );

  ElementDeclaration( const ElementDeclaration &that );
  ElementDeclaration &operator=( const ElementDeclaration & that );

  bool operator==( const ElementDeclaration &that ) const;

  ~ElementDeclaration();

  /**
     Return this element's identifier.
  */
  const string &getIdentifier() const;

protected:
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  string myIdentifier;
};

#endif
