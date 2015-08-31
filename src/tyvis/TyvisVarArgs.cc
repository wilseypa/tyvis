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

#include "tyvis/TyvisVarArgs.hh"
#include "tyvis/RValue.hh"

TyvisVarArgs::TyvisVarArgs( VAType initType ):
  pos(0),
  myType( initType ){}

TyvisVarArgs::~TyvisVarArgs(){}

bool
TyvisVarArgs::isLValueArgs() const {
  return myType == LVALUE;
}

TyvisVarArgs &
TyvisVarArgs::operator<<( const RValue &toAdd ){
  if( isLValueArgs() ){
    myData.push_back(const_cast<RValue *>(&toAdd));
  }
  else{
    myData.push_back(toAdd.clone());
  }
  myChoice.push_back(nullInfo());
  return *this;
}

TyvisVarArgs &
TyvisVarArgs::operator<<( const ElementAssociation &toAdd ){
  if( isLValueArgs() ){
    myData.push_back(toAdd.value);
    myChoice.push_back(toAdd.choice);
  }
  else{
    myData.push_back(toAdd.value->clone());
    myChoice.push_back(toAdd.choice);
  }
  return *this;
}

const ArrayInfo &
TyvisVarArgs::choice(){
  return myChoice[pos];
}

RValue *
TyvisVarArgs::next(){
  RValue *retval = myData[pos];
  pos++;
  return retval;
}

bool
TyvisVarArgs::hasNext() const {
  bool retval = (pos < (int)myData.size());
  return retval;
}
