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

#include "SignalNetinfo.hh"
#include "SignalBase.hh"
#include "TypeConvert.hh"
#include "_savant_entity_elab.hh"
#include "SourceBase.hh"
#include "SourceInfo.hh"
#include "VHDLProcess.hh"

TypeConvert::TypeConvert(_savant_entity_elab *proc) {
  upperSignalnetInfo   = NULL;
  upperDriver          = NULL;
  lowerSignal          = NULL;
  conversionFunctionId = -1;
  sourceBaseDeleteFlag = false;
  enclosingScope       = proc;
}

TypeConvert::~TypeConvert() {
  if ( sourceBaseDeleteFlag == false ){
    upperSignalnetInfo->setSourceBaseDeleteFlag(false);
  }
  
  delete upperSignalnetInfo;
  delete upperDriver;
  delete lowerSignal;
}

RValue *
TypeConvert::resolve( VHDLKernel *processPtr ){
  RValue *retval = lowerSignal->resolve(processPtr).clone();
  RValue *typeconvertvalue = NULL;

  // We need to manually update the signal with the resolved value if the
  // type is a composite resolved type. If the composite type is not a
  // composite resolved type, then we update its value in the scalar type
  // itself
  
  if ( lowerSignal->isCompositeResolvedObject() ){
    lowerSignal->updateVal( *retval );
  }

  if( enclosingScope != NULL ){
    // All the operations within the typeconversion function must be performed
    // wihin the context of the enclosingScope.  However, the function could be
    // called from several different processes. So save and restore the enclosingScoope
    // to ensure that the operating region is correct within the TCF.
    _savant_entity_elab *oldProc = processPtr->getProc();
    processPtr->setProc( enclosingScope );
    typeconvertvalue = (savantTypeConversionFn[conversionFunctionId](static_cast<VHDLProcess *>(processPtr), *lowerSignal)).clone();
    processPtr->setProc( oldProc );
  }
  else {
    typeconvertvalue = (savantTypeConversionFn[conversionFunctionId](static_cast<VHDLProcess *>(processPtr), *lowerSignal)).clone();
  }

  ASSERT ( typeconvertvalue != NULL );
  upperDriver->updateVal( *typeconvertvalue );

  delete typeconvertvalue;
  return upperDriver;
}
