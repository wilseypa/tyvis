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

#include "SourceTypeConvert.hh"
#include "TypeConvert.hh"
#include "SignalBase.hh"

SourceTypeConvert::SourceTypeConvert() {
  source = NULL;
  conversionNode = NULL;
}

SourceTypeConvert::~SourceTypeConvert() {
  conversionNode = NULL;
}

Value
SourceTypeConvert::resolve( VHDLKernel *processPtr, SigType ){
  Value retval;

  conversionNode->resolve(processPtr);
  if ( source->getParentSignal() != NULL ){
    retval = *source->getParentSignal();
  }
  else {
    retval = *source;
  }

  return retval;
}

void
SourceTypeConvert::setData( SignalBase *inData ){
  source = inData;
}

void
SourceTypeConvert::setTypeConvert(TypeConvert *nodePtr){
  conversionNode = nodePtr;
}

SignalBase *
SourceTypeConvert::getData(){
  cerr << "SourceTypeConvert::getData() called!" << endl;
  abort();
  return 0;
}

void
SourceTypeConvert::updateData( const RValue & ){
  cerr << "SourceTypeConvert::updateData called!" << endl;
  abort();
}

void
SourceTypeConvert::addDriversToRootDriverTable( SourceBase * ){
  cerr << "SourceTypeConvert::addDriversToRootDriverTable called!" << endl;
  abort();
}

void
SourceTypeConvert::addToRootDriverTable( SourceData * ){
  cerr << "SourceTypeConvert::addToRootDriverTable called!" << endl;
  abort();
}
