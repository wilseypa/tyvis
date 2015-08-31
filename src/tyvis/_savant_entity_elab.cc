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

#include "_savant_entity_elab.hh"
#include "SignalNetinfo.hh"
#include "SignalBase.hh"
#include "Signal.hh"
#include "SourceData.hh"
#include "SourceInfo.hh"
#include "PortAssociation.hh"
#include "TypeConvert.hh"
#include "SourceTypeConvert.hh"
#include "VCD.hh"
#include <stdarg.h>

int _savant_entity_elab::resolutionFnIdCounter = 0;
int _savant_entity_elab::typeConversionFnIdCounter = 0;

ResolutionFnPtr *savantResolutionFn;
TypeConversionFnPtr *savantTypeConversionFn;

_savant_entity_elab::_savant_entity_elab(){}

_savant_entity_elab::~_savant_entity_elab() {
  if ( savantTypeConversionFn != NULL ){
    delete [] savantTypeConversionFn;
    savantTypeConversionFn = NULL;
  }
}

void
_savant_entity_elab::instantiate( Hierarchy * ){
  cerr << "Dynamic Instantiation called for an unbounded component/entity/configuration" << endl;
}

void
_savant_entity_elab::instantiate( Hierarchy *, const string, const char * ){
  cerr << "Dynamic Instantiation called for an unbounded component/entity/configuration" << endl;
}

void 
_savant_entity_elab::createNetInfo() {
  cerr << "Dynamic Elaboration called for an unbounded component/entity/configuration" << endl;
}

void 
_savant_entity_elab::connect(int inputSignals, int outputSignals, ...){
  int noOfSignals = inputSignals + outputSignals;
  va_list ap;
  va_start(ap, outputSignals);
  for( int i=0; i < noOfSignals; i++ ){
    addToFanOut( va_arg(ap, SignalNetinfo*) );
  }
  va_end(ap);
}


// void
// _savant_entity_elab::Add(ObjectBase &val, ObjectBase &rhs) {
//   val.Add( rhs );
// }

// void
// _savant_entity_elab::Add(ObjectBase &val, SignalNetinfo &sig) {
//   val.Add( sig );
// }

// void
// _savant_entity_elab::Add(ObjectBase &val, VHDLKernel *processPtr) {
//   ASSERT( processPtr != 0 );
//   val.Add( processPtr );
// }

// void
// _savant_entity_elab::Add(ObjectBase &val, VHDLKernel *processPtr, int sigId) {
//   val.Add( processPtr, sigId );
// }


// void
// _savant_entity_elab::Add(ObjectBase &val, SignalNetinfo *ptr) {
//   val.Add( ptr );
// }

int
_savant_entity_elab::addResolutionFn(ResolutionFnPtr resolve) {
  ResolutionFnPtr *tempPtr = new ResolutionFnPtr[resolutionFnIdCounter + 1];
  int i;
  int resolutionFnId =0;
  int found = 0;
  for(i = 0; i < resolutionFnIdCounter; i++) {
    if(savantResolutionFn[i] == resolve) {
      resolutionFnId = i;
      found = 1;
    }
    tempPtr[i] = savantResolutionFn[i];
  }
  if(found == 0) {
    tempPtr[resolutionFnIdCounter] = resolve;
    if(savantResolutionFn != NULL) {
      delete [] savantResolutionFn;
    }
    savantResolutionFn = tempPtr;
    resolutionFnIdCounter++;
  } else {
    delete [] tempPtr;
  }
  return resolutionFnId;
}

int
_savant_entity_elab::addTypeConversionFn(TypeConversionFnPtr upConvert) {
  TypeConversionFnPtr *tempPtr = new TypeConversionFnPtr[typeConversionFnIdCounter + 1];
  int i;
  int typeConversionFnId =0;
  int found = 0;
  for(i = 0; i < typeConversionFnIdCounter; i++) {
    if(savantTypeConversionFn[i] == upConvert) {
      typeConversionFnId = i;
      found = 1;
    }
    tempPtr[i] = savantTypeConversionFn[i];
  }
  if(found == 0) {
    tempPtr[typeConversionFnIdCounter] = upConvert;
    typeConversionFnId = typeConversionFnIdCounter;
    if(savantTypeConversionFn != NULL) {
      delete [] savantTypeConversionFn;
    }
    savantTypeConversionFn = tempPtr;
    typeConversionFnIdCounter++;
  } else {
    delete [] tempPtr;
  }
  return typeConversionFnId;
}


void
_savant_entity_elab::setResolutionFunctionId(ObjectBase &sigInfo,
					     ResolutionFnPtr resolve) {
  int resolutionFnId = addResolutionFn(resolve);
  //Set the resolution Function Id foa all the elements of the ObjectBase
  //If the ObjectBase is a composite type the, previously set functionId,
  //will be overwritten, if there is any. This is valid as per
  //Resoultion Functions section of the LRM
  sigInfo.setResolutionFunctionId(resolutionFnId);
}

void
_savant_entity_elab::setUpConversionFunctionId(ObjectBase &sigInfo,
					       TypeConversionFnPtr upConvert){
  int typeConversionFnId = addTypeConversionFn(upConvert);
  
  //Set the type conversion Function Id to all the elements of the ObjectBase
  sigInfo.setTypeConversionFunctionId(typeConversionFnId);
}
  
void 
_savant_entity_elab::setDownConversionFunctionId( SignalNetinfo &sigInfo, 
						  TypeConversionFnPtr downConvert){
  ASSERT( sigInfo.isScalarType() );
  SourceBase *source = sigInfo.getSource();
  if(source == NULL) {
    source = new SourceInfo;
  }
  savantTypeConversionFn[typeConversionFnIdCounter] = downConvert;
  source->setDownTypeConversionFnId(typeConversionFnIdCounter++);
}
  
void 
_savant_entity_elab::addChild( SignalNetinfo *targetSigInfo, 
			       ObjectBase *sourceSignal, 
			       VHDLKernel *sourceProcess ){

    if(targetSigInfo->getSource() == NULL) {
      targetSigInfo->setSource( new SourceData );
      targetSigInfo->getSource()->addChild(sourceSignal, sourceProcess);
    } 
    else if ( !targetSigInfo->isResolvedSignal() ){
      targetSigInfo->getSource()->addChild(sourceSignal, sourceProcess);
    }
    else {
      if( targetSigInfo->getSource()->getNumChildren() == 0 ){
	SourceBase *source = targetSigInfo->getSource();
	if( dynamic_cast<SourceData *>(source) ){
	  if( sourceProcess == SourceBase::ANONYMOUS_PROCESS_ID) {
	    cerr << "Warning !! _savant_entity_elab::addChild() called with"
		 << "ANONYMOUS_PROCESS_ID but this is not the first guy to be"
		 << " added" << endl;
	    abort();
	  }
	  else {
	    if (source->isAnonymousDriver() == true) {
	      sourceSignal = targetSigInfo->getSource()->getData();
	      targetSigInfo->setSource( new SourceData );
	      targetSigInfo->getSource()->addChild(sourceSignal, sourceProcess);
	    }
	    else {
	      targetSigInfo->setSource( new SourceInfo );
	      targetSigInfo->getSource()->addChild( source->getData(),
						    source->getSourceId() );
	      targetSigInfo->getSource()->addChild(sourceSignal, sourceProcess);
	    }
	  }
	  delete source;	
	}
	else if ( dynamic_cast<SourceInfo *>(source) ){
	  targetSigInfo->getSource()->addChild(sourceSignal, sourceProcess);
	}
	else if ( dynamic_cast<SourceTypeConvert *>(source) ){
	  targetSigInfo->getSource()->addChild(source);
	  targetSigInfo->getSource()->addChild(sourceSignal, sourceProcess);
	}
      }
      else if (sourceProcess != SourceBase::ANONYMOUS_PROCESS_ID) {
	// Check if there is an anonymous driver already present in the tree
	// is so then delete that guy and add the new guy.
	if (targetSigInfo->getSource()->getNumChildren() == 1) {
	  // Check if the first guy is an anonymous driver. If so pitch him
	  // after copying the default initial value.
	  ASSERT ( dynamic_cast<SourceInfo *>(targetSigInfo->getSource()) );
	  SourceInfo *tempSourceInfo = (SourceInfo *) targetSigInfo->getSource();
	  if (tempSourceInfo->getChild(0)->isAnonymousDriver() == true) {
	    ASSERT( dynamic_cast<SourceData *>(tempSourceInfo->getChild(0)) );
	    ObjectBase *tempData =
	      dynamic_cast<ObjectBase *>(tempSourceInfo->getChild(0)->getData());
	    // tempSourceInfo->child = NULL;
	    delete tempSourceInfo;
	    targetSigInfo->setSource( new SourceInfo );
	    targetSigInfo->getSource()->addChild(tempData, sourceProcess);
	  }
	  else {
	    targetSigInfo->getSource()->addChild(sourceSignal, sourceProcess);
	  }
	}
	else {
	  targetSigInfo->getSource()->addChild(sourceSignal, sourceProcess);
	}
      }
    }
    if ( sourceProcess != SourceBase::ANONYMOUS_PROCESS_ID){
      targetSigInfo->setDriverAddedFlag();
    }
}  

void 
_savant_entity_elab::addChild( SignalNetinfo &targetSigInfo, 
			       ObjectBase &sourceSignal, 
			       VHDLKernel *sourceProcess ){
  if( targetSigInfo.isCompositeType() &&
      (!targetSigInfo.isDriverAlreadySet() || targetSigInfo.isResolvedSignal())){
    for( int i = 0; i < targetSigInfo.length().getIntValue(); i++ ){
      if (targetSigInfo.getField(i).isCompositeType()) {
        addChild( dynamic_cast<SignalNetinfo &>(targetSigInfo.getField(i)),
		dynamic_cast<ObjectBase &>(sourceSignal.getField(i)), 
		sourceProcess );
      }
      else {
        addChild( &dynamic_cast<SignalNetinfo &>(targetSigInfo.getField(i)),
		&dynamic_cast<ObjectBase &>(sourceSignal.getField(i)), 
		sourceProcess );
      }
    }
  }
  else{
    if( !targetSigInfo.isDriverAlreadySet() || targetSigInfo.isResolvedSignal() ){
      addChild( &targetSigInfo,
		&sourceSignal,
		sourceProcess );
      if ( sourceProcess != SourceBase::ANONYMOUS_PROCESS_ID){
	targetSigInfo.setDriverAddedFlag();
      }
    }
  }
}

void 
_savant_entity_elab::addChild(SignalNetinfo &targetSigInfo, ObjectBase &sourceSigInfo) {
  if( !targetSigInfo.isCompositeType() ){
    addChild( &targetSigInfo, &dynamic_cast<SignalNetinfo &>(sourceSigInfo) );
    //    sourceSigInfo.setSourceBaseDeleteFlag(false);
  }
  else{
    for( int i = 0; i < targetSigInfo.length().getIntValue(); i++ ){
      addChild( dynamic_cast<SignalNetinfo &>(targetSigInfo.getField(i)),
		dynamic_cast<SignalNetinfo &>(sourceSigInfo.getField(i)) );
    }
  }
}

void 
_savant_entity_elab::addChild( SignalNetinfo *parentSigInfo, SignalNetinfo *childDriverInfo ){
  // If the childDriverInfo does not have any driver for this signal, we have
  // nothing to do here.
  if( childDriverInfo->getSource() == NULL ){
    return;
  }
  if( parentSigInfo->getSource() == NULL ){
    // this coud be just parentSigInfo->source = childDriverInfo->source, but why this way
    parentSigInfo->setSource( new SourceInfo );
    parentSigInfo->getSource()->addChild(childDriverInfo->getSource());
  } 
  else if( dynamic_cast<SourceData *>(parentSigInfo->getSource()) || 
	   dynamic_cast<SourceTypeConvert *>(parentSigInfo->getSource()) ){
    if ( dynamic_cast<SourceTypeConvert *>(parentSigInfo->getSource()) ){
      SourceBase *src = parentSigInfo->getSource();
      parentSigInfo->setSource( new SourceInfo );
      parentSigInfo->getSource()->addChild(src);
      parentSigInfo->getSource()->addChild(childDriverInfo->getSource());
    } 
    else {
      if ( parentSigInfo->getSource()->isAnonymousDriver() == false ){
	SourceBase *src = parentSigInfo->getSource();
	parentSigInfo->setSource( new SourceInfo );
	parentSigInfo->getSource()->addChild(src);
	parentSigInfo->getSource()->addChild(childDriverInfo->getSource());
      } 
      else {
	parentSigInfo->setSource( new SourceInfo );
	parentSigInfo->getSource()->addChild(childDriverInfo->getSource());
      }
    }
  } 
  else {
    parentSigInfo->getSource()->addChild(childDriverInfo->getSource());
  }    
}  

// Move the source pointer to point to the root of the source tree.  This
// required since the resolution has to start from the root of the source
// tree.
void 
_savant_entity_elab::setSourceToRoot( SignalNetinfo &sigInfo ){
  ASSERT( sigInfo.isScalarType() );
  SourceBase *sigsrc = sigInfo.getSource();
  while(sigsrc->getParent() != NULL) {
    sigsrc = sigsrc->getParent();
  }
  sigInfo.setSource( sigsrc );
}


void
_savant_entity_elab::setSourceInfo( SignalNetinfo &val, const RValue &rhs ){
  //Assumes val and rhs are of same types
  if( val.isCompositeType() ){
    for( int i = 0; i < val.length().getIntValue(); i++ ){
      setSourceInfo( dynamic_cast<SignalNetinfo &>(val.getField(i)), 
		     rhs.getField(i) );
    }
  }
  else{
    if (GlobalControl::getVCD() && dynamic_cast<const SignalNetinfo *>(&rhs) ) {
      GlobalControl::getVCD()->registerSignalInfo(dynamic_cast<const SignalNetinfo &>(rhs), val);
    }
    if ( val.getSource() != NULL && val.getSourceBaseDeleteFlag() ){
      // delete ((SignalNetinfo *) val.getObject())->source;
    }
    if (dynamic_cast<const SignalNetinfo *>(&rhs)) {
      val.setSource( dynamic_cast<const SignalNetinfo &>(rhs).getSource() );
      val.setSourceBaseDeleteFlag(false);
      val.addAdditionalDrivers( dynamic_cast<const SignalNetinfo &>(rhs).getAdditionalDrivers() );
    }
    else {
      SourceData * sigsrc = new SourceData();
      sigsrc->setData(new Signal (rhs.getTypeInfo(), rhs.getName(), rhs.readVal()));
      val.setSource( sigsrc);
    }
  }
}

Block *
_savant_entity_elab::getDownTypeConversionFnList( SignalNetinfo &signal ){
  Block *retval = 0;

  if( signal.isCompositeType() ){
    retval = getDownTypeConversionFnList( dynamic_cast<SignalNetinfo &>(signal.getField(0)) );
  }
  else{
    retval = const_cast<Block *>(&signal.getDownTypeConversionFnList());
  }

  return retval;
}

void
_savant_entity_elab::addDownTypeConversionFn( SignalNetinfo &lhs,
					      SignalNetinfo &rhs,
					      TypeConversionFnPtr typeConversionFn) {
  Block *rhsDownTypeConversionFnList;
  int   *newFnId;
  int i;

  rhsDownTypeConversionFnList = getDownTypeConversionFnList(rhs);
  if( lhs.isCompositeType() ){
    for(i = 0; i < lhs.length().getIntValue(); i++ ){
      addDownTypeConversionFn( dynamic_cast<SignalNetinfo &>(lhs.getField(i)),
			       rhs,
			       typeConversionFn );
    }
  }
  else{
    for(i = 0; (i < rhsDownTypeConversionFnList->getNumberOfElements()); i++) {
      lhs.addDownTypeConversionFn(rhsDownTypeConversionFnList->getElement(i));
    }
    newFnId = new int;
    *newFnId = addTypeConversionFn(typeConversionFn);
    lhs.addDownTypeConversionFn(newFnId);
  }
}

void 
_savant_entity_elab::buildSignalTree() {
  const PortAssociation *portAssociation = portMapAspect.getFirstAssociation();
  while( portAssociation != 0 ){
    portAssociation->getFormal()->addToFanOut( *portAssociation->getActual() );

    switch(portAssociation->getMode()) {
    case OUT:
    case INOUT:
      addChild( dynamic_cast<SignalNetinfo &>(*portAssociation->getFormal()),
		*portAssociation->getActual() );
      break;
    case IN:
      break;
    default:
      cerr << "Port declarations of mode IBUFFFER and LINKAGE" << endl
	   << "Not Yet Supported" << endl;
      break;
    }
    portAssociation = portMapAspect.getNextAssociation();
  }
}

void
_savant_entity_elab::collectFanout( SignalNetinfo *sigNetInfo, const SignalNetinfo &val ){
  if( val.isCompositeType() ){
    for(int i=0; i < val.length().getIntValue(); i++) {
      collectFanout( dynamic_cast<SignalNetinfo *>(&sigNetInfo->getField(i)),
                     dynamic_cast<const SignalNetinfo &>(val.getField(i)) );
    }
  }
  else{
    sigNetInfo->addToFanOut( dynamic_cast<const SignalNetinfo &>(val) );
  }
}

void
_savant_entity_elab::Addall( SignalNetinfo &dest, const SignalNetinfo &src ){
  SignalNetinfo tmp(src.getTypeInfo(),string("tmp"));
  collectFanout(&tmp, src);
  dest.addToFanOut( tmp );
}

void
_savant_entity_elab::addUpConvertDriver( ObjectBase &sigInfo,
					 ObjectBase &data,
					 TypeConvert *typeConvertPtr ){

  if( sigInfo.isCompositeType() ){
    for( int i = 0; i < sigInfo.length().getIntValue(); i++){
      addUpConvertDriver( dynamic_cast<ObjectBase &>(sigInfo.getField(i)),
			  dynamic_cast<ObjectBase &>(data.getField(i)),
			  typeConvertPtr );
    }
  }
  else{
    addUpConvertDriver( dynamic_cast<SignalNetinfo *>(&sigInfo),
			dynamic_cast<SignalBase *>(&data),
			dynamic_cast<TypeConvert *>(typeConvertPtr));
  }
}
 
void
_savant_entity_elab::addUpConvertDriver( SignalNetinfo *sigInfo,
					 SignalBase *data,
					 TypeConvert *typeConvertPtr){
  SourceTypeConvert *tmp_stc = NULL;
  
  tmp_stc = new SourceTypeConvert ;
  tmp_stc->setData(data);
  tmp_stc->setTypeConvert(typeConvertPtr);
  if ( sigInfo->getSource() == NULL ){
    sigInfo->setSource( tmp_stc );
  } else {
    sigInfo->getSource()->addChild(tmp_stc);
  }
}

void
_savant_entity_elab::addDriver( SignalNetinfo &sigInfo, SignalBase &driverInfo ){
  AdditionalDrivers additionalDrivers;

  static int callBlockPresent = 0;

  if ( callBlockPresent == 0 && !isBlockPresent(sigInfo) ) {
    additionalDrivers.addDriver( &driverInfo );
  }

  callBlockPresent++;
  if( !sigInfo.isCompositeType() ){
    if ( additionalDrivers.length() > 0 ){
      sigInfo.addAdditionalDrivers( additionalDrivers );
    }
    else {
      sigInfo.addAdditionalDriver( &driverInfo );
    }
  }
  else{
    if ( additionalDrivers.length() == 0 ){
      for ( int i = 0; i < sigInfo.length().getIntValue(); i++ ){
	addDriver( dynamic_cast<SignalNetinfo &>(sigInfo.getField(i)), driverInfo );
      }
    } 
    else {
      for ( int i = 0; i < sigInfo.length().getIntValue(); i++ ){
	addDriver( dynamic_cast<SignalNetinfo &>(sigInfo.getField(i)),
		   additionalDrivers );
      }
    }
  }

  callBlockPresent--;
}

void
_savant_entity_elab::addDriver( SignalNetinfo &sigInfo, const AdditionalDrivers &toAdd ){
  if( !sigInfo.isCompositeType() ){
    sigInfo.addAdditionalDrivers( toAdd );
  }
  else{
    for( int i = 0; i < sigInfo.length().getIntValue(); i++ ){
      addDriver( dynamic_cast<SignalNetinfo &>(sigInfo.getField(i)), toAdd );
    }
  }
}

bool
_savant_entity_elab::isBlockPresent( SignalNetinfo &sigInfo){
  bool retval = true;
  if( sigInfo.isCompositeType() ){
    for( int i = 0; i < sigInfo.length().getIntValue(); i++ ){
      if ( !isBlockPresent( dynamic_cast<SignalNetinfo &>(sigInfo.getField(i)) )){
	retval = false;
	break;
      }
    }
  }
  else{
    if ( sigInfo.getAdditionalDrivers().length() == 0 ){
      retval = false;
    }
  }
  return retval;
}

int
_savant_entity_elab::checkSetSourceInfo( SignalNetinfo &dest, 
					 int srcStart, 
					 vector<RValue *> &src ){
  ASSERT ( dest.length().getIntValue() > 0 );
  RValue *vsrc = src[srcStart];
  ASSERT ( vsrc->length().getIntValue() > 0 );
  
//  cerr << "DNS: at srcStart = " << srcStart << endl;
//  cerr << "DNS: " << dest.length().getIntValue() << " vs " << vsrc->length().getIntValue() << endl;
//  dest.debugPrint(cerr);
//  vsrc->debugPrint(cerr);
  if (dest.length().getIntValue() == vsrc->length().getIntValue()) {
    setSourceInfo( dest,  *vsrc );
    return (srcStart + 1);
  }

  if (dest.length().getIntValue() > vsrc->length().getIntValue()) {
    ObjectBase *newType = dest.clone();
    int elements_copied_so_far = 0;

    while (elements_copied_so_far < dest.length().getIntValue()) {
      vsrc = src[srcStart];
      newType->getField( elements_copied_so_far ) = *(vsrc);
      elements_copied_so_far += vsrc->length().getIntValue();
      srcStart++;
    }

    setSourceInfo(dest, dynamic_cast<RValue &>(*newType));
    delete newType;
  }
  else {
    cerr << "Error : Unhandled case in checkSetSourceInfo()\n";
    ASSERT (false);
  }
  
  return srcStart;
}

int
_savant_entity_elab::checkAdd( SignalNetinfo &dest,
			       int srcStart,
			       vector<RValue *> &src ){
  ASSERT ( dest.length().getIntValue() > 0 );
  RValue *vsrc = src[srcStart];
  ASSERT ( vsrc->length().getIntValue() > 0 );
  
  if (dest.length().getIntValue() == vsrc->length().getIntValue()) {
    if ( dynamic_cast<SignalNetinfo *>(vsrc) ){
      dest.addToFanOut( dynamic_cast<SignalNetinfo &>(*vsrc) );
      if (GlobalControl::getVCD()) {
        GlobalControl::getVCD()->registerSignalInfo(dynamic_cast<SignalNetinfo &>(*vsrc), dest);
      }
    }
    return (srcStart + 1);
  }
  
  if (dest.length().getIntValue() > vsrc->length().getIntValue()) {
    RValue *newType = dest.clone();
    int elements_copied_so_far = 0;
    
    while (elements_copied_so_far < dest.length().getIntValue()) {
      vsrc = dynamic_cast<SignalNetinfo *>(src[srcStart]);
      newType->getField(elements_copied_so_far) = *(vsrc);
      elements_copied_so_far += vsrc->length().getIntValue();
      srcStart++;
    }
    
    dest.addToFanOut( dynamic_cast<SignalNetinfo &>(*newType) );
    delete newType;
  }
  else {
    cerr << "Error : Unhandled case in checkSetSourceInfo()\n";
  }

  return srcStart;
}

void
_savant_entity_elab::addToFanOut( RValue *newFanOut ){
  fanOutInfo.push_back( newFanOut );
}
