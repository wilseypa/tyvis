#ifndef UNIT_DECLARATION_HH
#define UNIT_DECLARATION_HH

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
#include <string>
using std::string;
class PhysicalLiteral;
class RValue;

class UnitDeclaration {
public:
  /**
     Used for handling primary units.
  */
  UnitDeclaration( const string &initIdentifier,
		   const RValue &initMultiplier );

  /**
     Used for the rest of the unit lists.
  */
  UnitDeclaration( const string &initIdentifier,
		   const PhysicalLiteral &initMultiplier );

  UnitDeclaration( const UnitDeclaration &that );
  /**
     Needed for STL stuff
  */
  UnitDeclaration();

  UnitDeclaration &operator=( const UnitDeclaration & that );

  bool operator==( const UnitDeclaration &that ) const;

  ~UnitDeclaration();

  /**
     Return this unit's identifier.  E.g. "fs", "ps", etc.
  */
  const string &getIdentifier() const;

  /**
     Return the unit's multipler.  For a primary unit identifier like "fs",
     this will return 1 fs.  For a secondary unit like ps, it would return
     "1000 fs".
  */
  const PhysicalLiteral &getMultiplier() const;

private:
  string myIdentifier;
  const PhysicalLiteral *myMultiplier;
};

#endif
