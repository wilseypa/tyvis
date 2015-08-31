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

#include "VHDLProcess.hh"
#include "SignalBase.hh"
#include <warped/warped.h>
#include "UniversalReal.hh"
#include "StringLiteral.hh"
#include "VCD.hh"

VHDLProcess::VHDLProcess( const string &name, _savant_entity_elab *proc ) : 
  VHDLKernel(name, proc){}

VHDLProcess::~VHDLProcess( ){}

void 
VHDLProcess::vhdlAssertOrReport( bool assertStatement, 
				 bool condition, 
				 const PhysicalLiteral &timeOccurred,
				 const string &message,
				 const string &severityString, 
				 const string &locatorInfo ){
  if( condition == false ){
    if( assertStatement == true ){
      *wout << "Assertion failure at ";
    }
    else{
      *wout << "Report at ";
    }
    *wout << "Time: ";
    *wout << timeOccurred;
    *wout << " Report: " << endl
	  << "    ";
    
    *wout << message;
    
    *wout << endl 
	  << "Severity Level: " << severityString 
	  << locatorInfo << endl;
  }
}

void 
VHDLProcess::vhdlAssertOrReport( bool assertStatement, 
				 const RValue &condition, 
				 const PhysicalLiteral &timeOccurred,
				 const RValue &message,
				 const RValue &severityLevel, 
				 const string &locatorInfo ) {
  
  bool tempValue = condition.getIntValue();
  
  vhdlAssertOrReport( assertStatement, 
		      tempValue,
		      timeOccurred,
		      message.toString(),
		      severityLevel.toString(),
		      locatorInfo);

  if( tempValue == false && severityLevel.getIntValue() >= 2 ){
    reportError( "VHDL assertion severity >= ERROR, shutting down", ABORT );
  }
}

void 
VHDLProcess::executeVHDL( VHDLKernel_state & ){
  executeSignalUpdationPhase( scalarSignalsUpdateArray, 
			      compositeResolvedSignalsUpdateArray );
  executeVHDL();
  scalarSignalsUpdateArray.reset();
  compositeResolvedSignalsUpdateArray.reset();
  eventSigs.reset();
}

void
VHDLProcess::executeSignalUpdationPhase( Block &scalarSignals,
					 Block &compositeResolvedSignals ){
  // Call updateSignal() of all the unresolved scalar Signals,
  // Or on elements of composite unresolved signals or
  // on elements of composite resolved signals
  for( int i = 0; i < scalarSignals.getNumberOfElements(); i++ ){
    SignalBase *currentSignal = static_cast<SignalBase*>(scalarSignals.getElement(i));
    updateSignal( currentSignal );
  }

  //Call updateSignal() of all signals that are composite resolved signals
  for(int i = 0; i < compositeResolvedSignals.getNumberOfElements(); i++ ){
    SignalBase *currentSignal = static_cast<SignalBase*>(compositeResolvedSignals.getElement(i));
    updateSignal( currentSignal );
  }
  
  // Update the implicit guard signal if it exists
  if (getGuard() != NULL) {
    updateGuard(getGuard());
  }
}

void
VHDLProcess::updateSignal( SignalBase *signal, 
			   bool initializingSimulation ){
  const RValue &oldEffVal = signal->readEffVal();
  bool eventOccurred = false;
  const Value newEffVal = signal->resolve(this);
//   if ( newEffVal == NULL ){
//     // This could happen in the case of signals of register kind where if
//     // all the drivers are disconnected the register value doesn't change
//     return;
//   }
  if ( oldEffVal != newEffVal ){  
    // update sensitivity list
    if( signal->isSensitive() ){
      eventSigs.add( signal->getSigId() );
    }
    eventOccurred = true;
  }
  
#ifdef VHDLDBG
  cout << getName() << " updating signal |" << signal->name << 
    "| with drvVal |" << *signal->readDrvVal() << "|, effVal |" <<
    *newEffVal << "|, and oldEffVal |" << oldEffVal <<
    "| at " << getTimeNow() << endl;
#endif

  if( initializingSimulation == false ){
    // update the implicit attributes iff simulation is in progress.
    // other dont. 
    updateImplicitAttributes(signal, &oldEffVal, eventOccurred);
  }
  
  //Update the effective Value of the signal if there is an event
  if(eventOccurred == true) {
    if (getTimeNow().getMajor() == 0 && GlobalControl::getVCD()) {
      GlobalControl::getVCD()->updateSignal(signal->getSigId(), newEffVal);
    }
    signal->updateEffVal( newEffVal );
  }
}

void
VHDLProcess::assignSignal( SignalBase &dest, 
			   VHDLKernel *srcId, 
			   const RValue &src,
			   const RValue &delay, 
			   const RValue &rejTime, 
			   const ArrayInfo &dinfo,
			   const ArrayInfo &sinfo ){  
  ASSERT( srcId != 0 );
  if (delay.getInt64Value() == 0) {
    dest.assignSignal( srcId, 
		     src,
		     VHDLVTime(0,1),
		     rejTime.getInt64Value(),
		     dinfo, 
		     sinfo );
  }
  else {
    dest.assignSignal( srcId, 
		     src,
		     delay.getInt64Value(),
		     rejTime.getInt64Value(),
		     dinfo, 
		     sinfo );
  }
}

void
VHDLProcess::assignSignal( SignalBase &destSignal, 
			   VHDLKernel *srcProcess, 
			   const RValue &newValue,
			   const VHDLVTime &delay, 
			   const VHDLVTime &rejTime, 
			   const ArrayInfo &dinfo,
			   const ArrayInfo &sinfo) {
  ASSERT( srcProcess != 0 );
  if (delay.getMajor() == 0 && delay.getMinor() == 0 ){
    destSignal.assignSignal( srcProcess,
			   newValue.readVal(), 
                           VHDLVTime(0,1),
                           rejTime,
			   dinfo,
			   sinfo);
  }
  else {
    destSignal.assignSignal( srcProcess,
			   newValue.readVal(), 
			   delay,
			   rejTime,
			   dinfo,
			   sinfo);
  }
}

void
VHDLProcess::assignNullTransaction( SignalBase &dest, 
				    VHDLProcess *srcId, 
				    const PhysicalLiteral &delay, 
				    const PhysicalLiteral &rejTime, 
				    const ArrayInfo&,
				    const ArrayInfo &){

  if( dest.isCompositeType() ){
    for(int i = 0; i <= dest.length().getIntValue(); i++) {
      assignNullTransaction( dynamic_cast<SignalBase &>(dest.getField(i)),
			     srcId,
			     delay, rejTime, nullInfo(), nullInfo());
    }
  }
  else{
    assignNullTransaction( dest,
			   srcId, 
			   delay.toVTime(), 
			   rejTime.toVTime(), nullInfo(), nullInfo());
  }
}

void
VHDLProcess::assignNullTransaction( SignalBase &dest, 
				    VHDLProcess *srcId, 
				    const VHDLVTime &delay, 
				    const VHDLVTime &rejTime, 
				    const ArrayInfo &dinfo,
				    const ArrayInfo &sinfo) {

  abort();
  // The process of posting transactions on signals also involves a phase called
  // marking.  Each scalar signal does its own marking and generates the transactions to
  // be sent out.  The actual events are sent out by the process.
//   dest.assignSignal( NULL_TRANSACTION, 
// 		     srcId, 
// 		     UniversalInteger(),
// 		     delay,
// 		     rejTime, 
// 		     dinfo, 
// 		     sinfo );
}

void
VHDLProcess::disconnectDriver( SignalBase *signal, int processId ){
  VHDLProcess *toDisconnect = dynamic_cast<VHDLProcess *>( listOfProcesses[ processId ] );
  signal->disconnectDriver( toDisconnect );

#ifdef VHDLDBG
  cout << getName() << " Applying a NULL TRANSACTION to signal " << signal->name;
  cout << " at " << getTimeNow() << endl;
#endif

  //Update the signals driver corresponding to the source ID.
  if( signal->isCompositeResolvedSignal() == false ){
    //Add this element into the list of signals that are to be 
    //updated this simulation cycle
    scalarSignalsUpdateArray.addElement(signal);
  }
  else {
    compositeResolvedSignalsUpdateArray.addElement( signal->getParentSignal() );
  }
}

void
VHDLProcess::updateDriver( SignalBase *signal,
			   VHDLKernel *srcProcess,
			   int senderSigId,
			   const RValue *newData,
			   ArrayInfo *sourceInfo,
			   ArrayInfo *destInfo ){
#ifdef VHDLDBG
  cout << getName() << " updating signal " << signal->name << " senderSigId " 
       << senderSigId << " slice " << *destInfo << " with value " << *newData
       << " slice " << *sourceInfo << " at " << getTimeNow() << endl;
#endif

  signal->getAllAttributes().last_active =  getTimeNow();
  signal->getAllAttributes().active = true;

  //Update the signals driver corresponding to the source ID.
  signal->updateDriver( senderSigId, srcProcess, newData, sourceInfo, destInfo );
  
  if( signal->isCompositeResolvedSignal() ){
    compositeResolvedSignalsUpdateArray.addElement(signal->getParentSignal());
  }
  else {
    //updated this simulation cycle
    scalarSignalsUpdateArray.addElement(signal);
  }
}

//The following function updates the implicit signal attributes
//of the currently updated composite resolved signal

void
VHDLProcess::updateImplicitAttributes( SignalBase *signal,
				       const RValue *newEffVal ){
  ASSERT( signal != 0 );

  VHDLData *newValue = NULL;
  bool eventOccurred = false;
  if( signal->isCompositeType() ){
    for(int i = 0; i <= signal->length().getIntValue(); i++) {
      updateImplicitAttributes( &dynamic_cast<SignalBase &>(signal->getField(i)),
				&newEffVal->getField(i) );
    }
  }
  else{
    if( signal->readEffVal() != *newValue ){
      eventOccurred = true;
    }
    updateImplicitAttributes( signal, &signal->readEffVal(), eventOccurred );
  }
}

//The following function updates the implicit signal attributes
//of the currently updated function

void
VHDLProcess::updateImplicitAttributes( SignalBase *signal,
				       const RValue *oldEffVal,
				       bool eventOccurred ){
  for(int i = 0; i < signal->getNumAttributes(); i++) {
    switch(signal->getAttributeList()[i]->getType()) {
    case LAST_EVENT: 
      if(eventOccurred == true) {
	updateAttribute(signal->getAttributeList()[i]);
      }
      break;
    case LAST_VALUE: 
      if(eventOccurred == true) {
	updateAttribute( signal->getAttributeList()[i], *oldEffVal );
      }
      break;
    case LAST_ACTIVE:
      updateAttribute( signal->getAttributeList()[i] );
      break;
    case DRIVING :
      cerr << getProcessName()
	   << ": VHDLProcess::updateSignal : Not yet implemented" << endl;
      break;
    case TRANSACTION: 
      updateAttribute( signal->getAttributeList()[i] );
      break;
    case EVENT: 
    case DRIVING_VALUE:
    case INVALID:
    case ACTIVE:
      break;
    case STABLE:
      if( signal->getAttributeList()[i]->getSignal().isSensitive() && eventOccurred ){
	eventSigs.add( signal->getAttributeList()[i]->getSignal().getSigId() );
      }
      break;
    case QUIET:
      if( signal->getAttributeList()[i]->getSignal().isSensitive() ){
	eventSigs.add( signal->getAttributeList()[i]->getSignal().getSigId());
      }
      break;
    default:
      cerr << getProcessName()
	   << ": VHDLProcess::updateSignal : (invalid attribute)" << endl;
      break;
    } // switch
  } // for
  updateDefaultAttributes(signal, oldEffVal, eventOccurred);
}

void
VHDLProcess::updateDefaultAttributes( SignalBase *signal,
				      const RValue *oldEffVal,
				      bool eventOccurred ){
  if(eventOccurred == true) {
    signal->getAllAttributes().last_event =  getTimeNow();
    signal->getAllAttributes().event = true;
  }

  signal->getAllAttributes().last_active = getTimeNow();

  switch(signal->getAllAttributes().sigtype) {
  case DefaultAttributes::INT:
    signal->getAllAttributes().last_value.inttype = oldEffVal->getIntValue();
    break;
  case DefaultAttributes::DOUBLE:
    signal->getAllAttributes().last_value.floattype = oldEffVal->getDoubleValue();
    break;
  case DefaultAttributes::INT64:
    signal->getAllAttributes().last_value.physicaltype = oldEffVal->getInt64Value();
    break;
  default:
    break;
  }
}

void
VHDLProcess::updateSignal( int sigId, 
			   VHDLProcess *srcProcess, 
			   VHDLData *data,
			   const ArrayInfo *dInfo, 
			   const ArrayInfo *sInfo ){

  bool eventOccurred = false;	// Set if an event occurs on this signal.
  SignalBase *sig = getVHDLState()->locateSig(sigId); 
  
#ifdef VHDLDBG
  cout << getName() << " updating signal " << sig->name << " driver " 
       << srcProcess << " slice " << *dInfo << " with value ";
  data->print(cout);
  cout << " slice " << *sInfo << " at " << getTimeNow() << endl;
#endif

  const RValue &oldEffVal = sig->readEffVal();
  const RValue *newEffVal;

  // Update the driver as specified by srcProcess, and resolve the signal.
  // The resolve is an overloaded function in ImplicitSignal and Signal.
  // If the Signal has to be resolved, the signal will resolve itself and
  // update the effective value of the signal.
  if(sig->isImplicitSignal()){
    newEffVal = data;
  }
  else{
    sig->updateDriver( sigId, srcProcess, data, sInfo, dInfo );
    const RValue &tempVal = sig->readDriver(srcProcess)->readVal();
    sig->updateDrvVal(tempVal, sInfo, dInfo);
    newEffVal = sig->resolve(this).clone();
  }
  // check if there's been an event
  if ( oldEffVal != *newEffVal ){  
    // update sensitivity list
    if (sig->isSensitive() == true) {
      eventSigs.add(sig->getSigId());
    }
    eventOccurred = true;
  }

#ifdef VHDLDBG
  cout << getName() << " updating signal " << sig->name << " with drvVal(";
  sig->readDrvVal()->print(cout);
  cout << ") and effVal(";
  sig->readEffVal()->print(cout);
  cout << ") and oldEffVal(";
  oldEffVal->print(cout);
  cout <<") at " << getTimeNow() << endl;
#endif
  for( int i = 0; i < sig->getNumAttributes(); i++ ){
    switch(sig->getAttributeList()[i]->getType()) {
    case LAST_EVENT: 
      if(eventOccurred == true) {
	updateAttribute( sig->getAttributeList()[i] );
      }
      break;
    case LAST_VALUE: 
      if(eventOccurred == true) {
	updateAttribute( sig->getAttributeList()[i], oldEffVal);
      }
      break;
    case LAST_ACTIVE:
      updateAttribute( sig->getAttributeList()[i] );
      break;
    case DRIVING :
      cerr << getProcessName()
	   << ": VHDLProcess::updateSignal : Not yet implemented" << endl;
      break;
    case TRANSACTION: 
      updateAttribute( sig->getAttributeList()[i] );
      break;
    case EVENT: 
    case DRIVING_VALUE:
    case INVALID:
    case ACTIVE:
      break;
    case STABLE:
      if( sig->getAttributeList()[i]->getSignal().isSensitive() && eventOccurred == true) {
	eventSigs.add( sig->getAttributeList()[i]->getSignal().getSigId() );
      }
      break;
    case QUIET:
      if( sig->getAttributeList()[i]->getSignal().isSensitive() ){
	eventSigs.add( sig->getAttributeList()[i]->getSignal().getSigId() );
      }
      break;
    default:
      cerr << getProcessName()
	   << ": VHDLProcess::updateSignal : (invalid attribute)" << endl;
      break;
    } // switch
  } // for
  if(eventOccurred == true) {
    sig->updateEffVal( *newEffVal );
  }
}

void
VHDLProcess::updateGuard( RValue *s ){
  SignalBase *signal = dynamic_cast<SignalBase *>(s);
  const RValue &oldEffVal = signal->readEffVal();
  const RValue *newEffVal = NULL;
  bool eventOccurred = false;

  newEffVal = getGuardExpression();

  if ( newEffVal == NULL ){
    return;
  }
  if ( oldEffVal != *newEffVal ){  
    // update sensitivity list
    if( signal->isSensitive() ){
      eventSigs.add(signal->getSigId());
    }
    eventOccurred = true;
  }
  
#ifdef VHDLDBG
  cout << getName() << " updating signal " << signal->name
       << " with drvVal(";
  signal->readDrvVal()->print(cout);
  cout << ") and effVal(";
  signal->readEffVal()->print(cout);
  cout << ") and oldEffVal(";
  oldEffVal->print(cout);
  cout <<") at " << getTimeNow() << endl;
#endif
  
  //Update the effective Value of the signal if there is an event
  if(eventOccurred == true) {
    signal->updateEffVal( *newEffVal );
  }
}

void
VHDLProcess::executeWait( int wId, const PhysicalLiteral &timeout ){
  VHDLVTime delay = VHDLVTime::getVHDLVTimePositiveInfinity();
  
  // record which wait we're waiting on
  getVHDLState()->waitLabel = wId;
  
  // This is required in update Wait.
  getVHDLState()->setWaitTime( getTimeNow() );

  if ( &timeout != &SAVANT_INFINITY_TIME ){
    delay = timeout.toVTime();
  }

#ifdef VHDLDBG
  cout << getName() << " executing wait "<< wId << " at time " 
       << getTimeNow() << ", will resume in: " << delay << endl;
#endif

  getVHDLState()->waitExpired = false;
  if( delay != VHDLVTime::getVHDLVTimePositiveInfinity() ){
    scheduleWait( wId, getTimeNow() + delay );
  }
}

void
VHDLProcess::executeWait( int wld, const Variable &timeout ){
  executeWait( wld, PhysicalLiteral(timeout.getValue(), timeout.getUnit()) );
}

void
VHDLProcess::updateWait( const VHDLVTime &waitTime ){
  if( getVHDLState()->getWaitTime() == waitTime ){
    getVHDLState()->waitExpired = true;
  }
#ifdef VHDLDBG
  cout << getName() << " updating wait time to " << waitTime
       << ", waiting on "
       << getVHDLState()->waitLabel << " from "
       << getVHDLState()->getWaitTime();
  if(getVHDLState()->waitExpired == true) {
    cout << " wait expired." << endl;
  } else {
    cout << " wait has not expired, waitTime = " << getVHDLState()->getWaitTime();
  }
#endif
}

bool
VHDLProcess::resumeWait( int wId, const VHDLData &cond ){
  bool resume = false;
  Wait *wait;

  if (wId != getVHDLState()->waitLabel) {
    cerr << getProcessName() << " resumeWait: wId (" << wId 
	 << ") not the one I'm waiting for! (" << getVHDLState()->waitLabel 
	 << ")" << endl;
    abort();
  }

  if( getVHDLState()->waitExpired == true ){
    // timeout has been satisfied
    resume = true;
#ifdef VHDLDBG
    cout << getName() << ": wait timeout has expired-resuming\n";
#endif
  } else {
    if( wId == WAITING_IN_PROC ){
      wait = getVHDLState()->procWait;
    } 
    else {
      wait = (getVHDLState()->wait + wId);
    }
    if( wait->getSensSize() > 0 ){
      // wait has a sensitivity list
      for( int i = 0; i < eventSigs.num(); i++ ){
	if( wait->sensitiveTo(eventSigs.get(i)) ){
	  // this wait is sensitive to a signal that has had an event this
	  // cycle.  Only resume the wait if there was a hit on the
	  // sensList _and_ the condition clause evaluates to true.
	  resume = (cond == SAVANT_BOOLEAN_TRUE);
#ifdef VHDLDBG
	  cout << getName() << " had event on sens. sig " << eventSigs.get(i)
		<< ": condition evals to " << (resume ? "true":"false") << endl;
#endif
	  break;
	} // if
      }	// for
    } // if(wait->getSensSize() > 0)
  } // else 

  if (resume == true) {
    getVHDLState()->waitLabel = NOT_WAITING;
#ifdef VHDLDBG
    cout << getName() << " resuming wait " << wId << " at " << getTimeNow() <<endl;
  } 
  else {
    cout << getName() << " not resuming wait " << wId << " at " 
	 << getTimeNow() << endl;
#endif
  }

  return resume;
}

void
VHDLProcess::updateAttribute( Attribute *attribute, 
			      const RValue &lastValue ) const {
  switch(attribute->getType()) {
  case LAST_EVENT:
  case LAST_ACTIVE:
    attribute->getSignal().updateVal(PhysicalLiteral(std_standard_timeTypeInfo(), getTimeNow().getMajor()));
    break;
  case LAST_VALUE:
    if(attribute->getSignal().isScalarType() == true) {
      attribute->getSignal().updateVal(lastValue);
    } else {
      abort();
    }
    break;
  case QUIET:
  case EVENT:
  case ACTIVE:
  case STABLE:
    attribute->getSignal().updateVal(lastValue);
    break;
  case TRANSACTION:{
    const RValue &oldValue = attribute->getSignal().readVal();
    attribute->getSignal().updateVal( dynamic_cast<const EnumerationLiteral &>(oldValue).vhdlNot() );
    break;
  }
  case DRIVING :
  case DRIVING_VALUE:
    cerr << "VHDLProcess::updateAttribute : Not yet implemented" << endl;
    break;
  default:
    cerr << "VHDLProcess::updateAttribute : (unknown attribute)" << endl;
    break;
  } // switch(attribute->getType());
}

EnumerationLiteral
VHDLProcess::locateStableAttribute( RValue *asRVal, 
				    const VHDLVTime time ) const {
  EnumerationLiteral retval;
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);
 
  VHDLVTime lastEventTime = sig->getAllAttributes().last_event;
  if (getTimeNow().getMajor() - lastEventTime.getMajor() > time.getMajor()){
    retval = SAVANT_BOOLEAN_TRUE;
  } 
  else {
    retval = SAVANT_BOOLEAN_FALSE;
  }
/*
  EnumerationLiteral retval;
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);

  vector<Attribute *> attributes;
  Attribute *attribute = 0;
  PhysicalLiteral lastEventTime( std_standard_timeTypeInfo() );

  attributes = const_cast<SignalBase *>(sig)->getAttributeList();
  for( int i = 0; i < sig->getNumAttributes(); i++ ){
    if(attributes[i]->getType() == LAST_EVENT){
      attribute = attributes[i];
      break;
    }
  }

  ASSERT((attribute != NULL && attribute->getType() == LAST_EVENT));
  lastEventTime = attribute->getSignal();
  
  attributes = const_cast<SignalBase *>(sig)->getAttributeList();
  for(int i = 0; i <sig->getNumAttributes(); i++) {
    if(attributes[i]->getType() == STABLE){
      attribute = attributes[i];
      break;
    }
  }

  PhysicalLiteral lvt = VHDL_TIME(getTimeNow().getMajor());

  ASSERT((attribute != NULL && attribute->getType() == STABLE));
  if (time == VHDLVTime::getVHDLVTimeZero()) {
    retval = EnumerationLiteral::toBoolean( lastEventTime < lvt );
  } 
  else {
    retval = EnumerationLiteral::toBoolean( lastEventTime <=
					    lvt.vhdlMinus( VHDL_TIME(time.getMajor())));
  }
  updateAttribute(attribute, retval);
*/
  
  return retval;
}

EnumerationLiteral
VHDLProcess::locateQuietAttribute( RValue *asRVal,
				   const VHDLVTime time ) const {
  EnumerationLiteral retval;
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);
 
  VHDLVTime lastActiveTime = sig->getAllAttributes().last_active;
  if (getTimeNow().getMajor() - lastActiveTime.getMajor() > time.getMajor()){
    retval = SAVANT_BOOLEAN_TRUE;
  } 
  else {
    retval = SAVANT_BOOLEAN_FALSE;
  }
/*
  vector<Attribute *> attributes;
  Attribute *attribute = NULL;
  EnumerationLiteral retval;
  PhysicalLiteral lastActiveTime( std_standard_timeTypeInfo() );
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);

  attributes = const_cast<SignalBase *>(sig)->getAttributeList();
  for(int i = 0; i < sig->getNumAttributes(); i++ ){
    if( attributes[i]->getType() == LAST_ACTIVE ){
      attribute = attributes[i];
    }
  }
  ASSERT((attribute != NULL && attribute->getType() == LAST_ACTIVE));
  lastActiveTime = attribute->getSignal().readVal();
 
  attributes = const_cast<SignalBase *>(sig)->getAttributeList();
  for(int i = 0; i <sig->getNumAttributes(); i++) {
    if(attributes[i]->getType() == QUIET) 
      attribute = attributes[i];
  }
  ASSERT((attribute != NULL && attribute->getType() == QUIET));

  if(time == VHDLVTime::getVHDLVTimeZero()) {
    if( lastActiveTime < VHDL_TIME(getTimeNow().getMajor()) ){
      retval = SAVANT_BOOLEAN_TRUE;
    } 
    else {
      retval = SAVANT_BOOLEAN_FALSE;
    }
  } 
  else {
    if( lastActiveTime < VHDL_TIME(getTimeNow().getMajor() - time.getMajor()) ){
      retval = SAVANT_BOOLEAN_TRUE;
    } 
    else {
      retval = SAVANT_BOOLEAN_FALSE;
    }
  }
  updateAttribute(attribute, retval );
*/
    
  return retval;
}

UniversalInteger *
VHDLProcess::locateTransactionAttribute(RValue *asRVal) {
  vector<Attribute *> attribute;
  Attribute *retval = NULL;
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);
  
  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for (int i = 0; i <sig->getNumAttributes(); i++) {
    if(attribute[i]->getType() == TRANSACTION){
      retval = attribute[i];
    }
  }
  ASSERT((retval != NULL && retval->getType() == TRANSACTION));
  return new UniversalInteger(retval->getSignal().readVal());
}

EnumerationLiteral
VHDLProcess::locateEventAttribute( RValue *asRVal ) const {
  EnumerationLiteral retval;
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);

  VHDLVTime lastEventTime = sig->getAllAttributes().last_event;
  
  // taking the default stuff
  if ( lastEventTime == getTimeNow() ){ 
    retval = SAVANT_BOOLEAN_TRUE;
  } 
  else  {
    retval = SAVANT_BOOLEAN_FALSE;
  }

  return retval;
}

EnumerationLiteral
VHDLProcess::locateActiveAttribute( RValue *asRVal ) const {
  EnumerationLiteral retval;
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);

  VHDLVTime lastActiveTime = sig->getAllAttributes().last_active;
  
  // taking the default stuff
  if ( lastActiveTime == getTimeNow() ){ 
    retval = SAVANT_BOOLEAN_TRUE;
  } 
  else  {
    retval = SAVANT_BOOLEAN_FALSE;
  }

  return retval;
}

PhysicalLiteral
VHDLProcess::locateLastEventAttribute( RValue *asRVal ) const {
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);

  VHDLVTime lastEventTime = sig->getAllAttributes().last_event;

  PhysicalLiteral retval(std_standard_timeTypeInfo(),
       getTimeNow().getMajor() - lastEventTime.getMajor());

  return retval;
}

PhysicalLiteral
VHDLProcess::locateLastActiveAttribute( RValue *asRVal ) const {
  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);

  VHDLVTime lastActiveTime = sig->getAllAttributes().last_active;

  PhysicalLiteral retval(std_standard_timeTypeInfo(),
       getTimeNow().getMajor() - lastActiveTime.getMajor());

  return retval;
}

Value
VHDLProcess::locateLastValueAttribute( RValue *asRVal ){

  SignalBase *sig = dynamic_cast<SignalBase *>(asRVal);
  ASSERT(sig != NULL);

  Value retval;
  const TypeInfo *signalType = &sig->getTypeInfo();
  if( dynamic_cast<const ScalarTypeInfo *>(signalType) ){
    retval = UniversalInteger( sig->getAllAttributes().last_value.inttype );
  }
  else if( dynamic_cast<const RealTypeInfo *>(signalType) ){
    retval = UniversalReal( sig->getAllAttributes().last_value.floattype );
  }
  else if( dynamic_cast<const PhysicalTypeInfo *>(signalType) ){
    retval = PhysicalLiteral( *dynamic_cast<const PhysicalTypeInfo *>(signalType),
			      sig->getAllAttributes().last_value.physicaltype );
  }
  else{
    abort();
  }
  return retval;
}
