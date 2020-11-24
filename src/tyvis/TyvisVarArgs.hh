#ifndef TYVIS_VAR_ARGS_HH
#define TYVIS_VAR_ARGS_HH

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

class RValue;

#include "tyvis/tyvis-config.h"
#include <VarArgs.h>
#include <tyvis/ElementAssociation.hh>

enum VAType { RVALUE, LVALUE };

class TyvisVarArgs {
public:
  TyvisVarArgs( VAType = RVALUE );
  virtual ~TyvisVarArgs();
  bool isLValueArgs() const;

  TyvisVarArgs &operator<<( const RValue &toAdd );
  TyvisVarArgs &operator<<( const ElementAssociation &toAdd );
  const ArrayInfo & choice();
  RValue *next();
  bool hasNext() const;

protected:
  vector<RValue *> myData;
  vector<ArrayInfo> myChoice;
  int pos;

private:
  VAType myType;
};

#endif
