#ifndef TYPECONVERT_HH_
#define TYPECONVERT_HH_

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

#include "tyvis/tyvis-config.h"
#include "tyvis/SourceBase.hh"

class VHDLProcess;
class _savant_entity_elab;
class ObjectBase;
class SignalBase;
class SignalNetinfo;

// This class is used to hold some temporary information and some permanent
// information which are needed during the TypeConversion. The Signal it points
// to is the signal which has the resolution tree for signal of a formal or
// actual on which type conversion is done ( whether up type or down type ).
// The Signal Netinfo is of the destination type where the type converted value
// should go. The Variable is used as a storage location where the type
// converted value is stored and SourceTypeConvert node reads it from that
// storage location.
class TypeConvert {
public:
  TypeConvert(_savant_entity_elab *proc = NULL);
  
  ~TypeConvert();

  void setSignal( SignalBase* tmpSignal ){
    lowerSignal = tmpSignal;
  }
  
  void setSignalNetinfo( SignalNetinfo *tmpSignalNetinfo ){
    upperSignalnetInfo = tmpSignalNetinfo;
  }
  
  void setVariable( ObjectBase *tmpVariable ){
    upperDriver = tmpVariable;
  }
  
  void setConversionFunctionId(TypeConversionFnId_t id) {
    conversionFunctionId = id;
  }
  
  RValue *resolve( VHDLKernel * );
  
  void setSourceBaseDeleteFlag(bool flag){
    sourceBaseDeleteFlag = flag;
  }

  _savant_entity_elab* getEnclosingScpe() const {
    return enclosingScope;
  }

  void setEnclosingScope(_savant_entity_elab* proc) {
    enclosingScope = proc;
  }
  
private:
  SignalNetinfo *upperSignalnetInfo;
  ObjectBase *upperDriver;
  SignalBase *lowerSignal;
  TypeConversionFnId_t conversionFunctionId;
  bool sourceBaseDeleteFlag;

  _savant_entity_elab *enclosingScope;
};
#endif
