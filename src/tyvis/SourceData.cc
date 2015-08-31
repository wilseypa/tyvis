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

#include "ObjectBase.hh"
#include "SourceData.hh"
#include "SourceInfo.hh"
#include "VHDLKernel.hh"
#include "SignalBase.hh"

SourceData::SourceData() : SourceBase() {
  sourceProcess = NULL;
  sourceObject  = NULL;
  connected     = true;
  ambiguousSource = false;
  sourceProcessVec = NULL;
  sourceObjectVec = NULL;
}

SourceData::SourceData(VHDLKernel *sourceProcessPtr) : SourceBase() {
  sourceProcess = sourceProcessPtr;
  sourceObject          = NULL;
  connected     = true;
  ambiguousSource = false;
  sourceProcessVec = NULL;
  sourceObjectVec = NULL;
}

SourceData::~SourceData() {
  if (sourceProcessVec)
    delete sourceProcessVec;
  if (sourceObjectVec)
    delete sourceObjectVec;
}

VHDLKernel *
SourceData::getSourceId() const {
  return sourceProcess;
}

void 
SourceData::setSourceId(VHDLKernel* srcProcPtr) {
  sourceProcess = srcProcPtr;
}

SignalBase * 
SourceData::getData(){
  return sourceObject;
}

SourceData *
SourceData::getDriver( int processId ){
  ASSERT( sourceProcess != 0 );
  if( sourceProcess->getVHDLProcessId() == processId ) {
    return this;
  }
  
  return 0;
}

SourceData *
SourceData::getDriver( VHDLKernel* procPtr ){
  if ( sourceProcess == procPtr ){
    return this;
  }
  else if ( ambiguousSource ){
    if ( !sourceProcess ){
      vector<VHDLKernel *>::iterator p_i, p_end;
      vector<SignalBase *>::iterator o_i, o_end;
      p_i = sourceProcessVec->begin();
      o_i = sourceObjectVec->begin();
      p_end = sourceProcessVec->end();
      for ( ; p_i != p_end ; ++p_i, ++o_i ){
        if (*p_i == procPtr ){
          sourceProcess = *p_i;
          sourceObject = *o_i;
          break;
        }
      }
    }
    ASSERT ( sourceProcess == procPtr );
    return this;
  }
  
  return NULL;
}


SourceData *
SourceData::getDriveratIndex(int) {
  return this;
}

void 
SourceData::setData( SignalBase *newSourceObject ){
  sourceObject = newSourceObject;
}

bool
SourceData::_is_child_present(VHDLKernel *procPtr) {
  return (sourceProcess == procPtr);
}

bool
SourceData::isAnonymousDriver() {
  bool retval = false;
  if (sourceProcess == SourceBase::ANONYMOUS_PROCESS_ID) {
    retval = true;
  }
  return retval;
}

void 
SourceData::updateData( const RValue &newValue ){
  connected = true;
  sourceObject->updateVal( newValue );
}

int 
SourceData::addChild(ObjectBase *sourceSignal, VHDLKernel *srcProcPtr ){
  if ((sourceProcess && sourceProcess != SourceBase::ANONYMOUS_PROCESS_ID) || ambiguousSource) {
    if ( !ambiguousSource ){
      sourceObjectVec = new vector<SignalBase *>;
      sourceProcessVec = new vector<VHDLKernel *>;
      sourceObjectVec->push_back(sourceObject);
      sourceProcessVec->push_back(sourceProcess);
      sourceObject = NULL;
      sourceProcess = NULL;
      ambiguousSource = true;
    }
    if ( srcProcPtr != SourceBase::ANONYMOUS_PROCESS_ID ){
      sourceObjectVec->push_back(dynamic_cast<SignalBase *>(sourceSignal));
      sourceProcessVec->push_back(srcProcPtr);
    }
  }
  else {
    sourceObject          = dynamic_cast<SignalBase *>(sourceSignal);
    sourceProcess = srcProcPtr;
  }
  if ( parent != NULL && 
       srcProcPtr != SourceBase::ANONYMOUS_PROCESS_ID ){
    parent->addToRootDriverTable( this );
  }

  return 1;
}

int 
SourceData::addChild(SourceBase*) {
  cerr << "SourceData::addChild(SourceBase*) should never be called!!" << endl;
  abort();
  return 0;
}

void 
SourceData::print(ostream& os) const {
  os << "Source(" << sourceProcess << ") = ";
  sourceObject->print(os);
  os << endl;
}

Value
SourceData::resolve(VHDLKernel* procPtr, SigType) {
  Value retval;

  if ( ambiguousSource ){
    if (sourceObject) {
      retval = const_cast<RValue &>(getData()->readVal());
      return retval;
    }
    bool found = false;
    vector<VHDLKernel *>::iterator p_i, p_end;
    vector<SignalBase *>::iterator o_i, o_end;
    p_i = sourceProcessVec->begin();
    o_i = sourceObjectVec->begin();
    p_end = sourceProcessVec->end();
    for ( ; p_i != p_end ; ++p_i, ++o_i ){
      if (*p_i == procPtr ){
        found = true;
        retval = (*o_i)->readVal();
        break;
      }
    }
    if (!found ){
      o_i = sourceObjectVec->begin();
      retval = (*o_i)->readVal();
    }
  }
  else {
    retval = const_cast<RValue &>(getData()->readVal());
  }
  return retval;

  //This is where the difference between a composite resolved signal 
  //resolution and resolution of composite signal of resolved subtype lies
//   if( getData()->getParentSignal() == 0 ){
    retval = const_cast<RValue &>(getData()->readVal());
//   }
//   else {
//     retval = const_cast<RValue &>(getData()->getParentSignal()->readVal());
//   }
  return retval;
}

void
SourceData::addDriversToRootDriverTable( SourceBase * ){
  cerr << "SourceData::addDriversToRootDriverTable called - aborting!" << endl;
  abort();
}

void
SourceData::addToRootDriverTable( SourceData * ){
  cerr << "SourceData::addToRootDriverTable called - aborting!" << endl;
  abort();
}
