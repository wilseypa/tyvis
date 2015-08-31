#ifndef LVALUE_HH
#define LVALUE_HH

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
#include "tyvis/RValue.hh"

class SignalNetinfo;
class VHDLKernel;

class LValue : public RValue {
public:
  using RValue::readVal;
  virtual RValue &readVal() = 0;
  virtual void updateVal( const RValue & ) = 0;
  virtual ~LValue(){}

  //@{
  /** SignalNetinfo stuff */
  virtual SignalNetinfo &addToFanOut( VHDLKernel * ) = 0;
  //@}
  //DRH - problems with addChild
  virtual SignalNetinfo &addChild( SignalNetinfo &, VHDLKernel * ) = 0;

protected:
  LValue(){}
};

#endif

