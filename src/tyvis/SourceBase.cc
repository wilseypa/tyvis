// Copyright (c) Ohio Board of Regents and the University of Cincinnati.  
// All Rights Reserved.

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

#include "SourceData.hh"
#include "SourceBase.hh"

VHDLKernel* SourceBase::ANONYMOUS_PROCESS_ID = (VHDLKernel *) (-2);
VHDLKernel* SourceBase::DEFAULT_ID           = (VHDLKernel *) (-1);

TypeConversionFnId_t SourceBase::DEFAULT_TCF_ID  = -1;
ResolutionFnId_t     SourceBase::DEFAULT_RF_ID   = -2;

SourceBase::SourceBase() : 
  parent(0){
}

SourceBase::~SourceBase(){}

void 
SourceBase::setParent(SourceBase *myParent) {
  ASSERT(myParent != NULL);
  parent = myParent;
}

SourceBase &
SourceBase::operator[](const int) { 
  cerr << "SourceBase::operator[] called." << endl;
  abort();
  return *this;		// Just to keep CC from shouting.
};

Value SourceBase::resolve( VHDLKernel *, SigType ) { abort(); }
