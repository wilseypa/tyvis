#ifndef VHDLKERNEL_CC
#define VHDLKERNEL_CC

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

#include <clutils/StringUtilities.h>
#include <clutils/Debug.h>
#include <warped/SimulationStream.h>
#include "VHDLKernel.hh"
#include "AccessObject.hh"
#include "SigEvent.hh"
#include "WaitEvent.hh"
#include "STDTypes.hh"
#include "FileTypeInfo.hh"

using std::streampos;
using std::cin;
using std::flush;

vector<VHDLKernel*> listOfProcesses;
map<int, VHDLKernel*> procMap;

static
VHDLKernel *
getProcessForId( int id ){
  VHDLKernel *retval;
  retval = procMap[id];
  if (!retval) {
    procMap.clear();
    for( vector<VHDLKernel *>::iterator i = listOfProcesses.begin();
         i < listOfProcesses.end();
         i++ ){
      procMap[(*i)->getVHDLProcessId()] = *i;
      if( (*i)->getVHDLProcessId() == id ){
        retval = *i;
      }
    }
  }
  return retval;
}

/* Implements the global object identifier algorithm. Will produce an integer
   number which is ensured to be globally unique and thus may be used as an
   identifier of the object. */

int 
VHDLKernel::getGlobalObjectId(){
  static int globalId = 0;
  return globalId++;
}

const string &
VHDLKernel::getName( const OBJECT_ID& id ){ 
  return listOfProcesses[id.getSimulationObjectID()]->getName(); 
}

void
VHDLKernel::initializeGlobalObjectId(){}

VHDLKernel::VHDLKernel( const string &name, _savant_entity_elab *ptr) : proc(ptr){
  initializeGlobalObjectId();
  vhdlProcessId = getGlobalObjectId();
  processName = string( name + "." + intToString( vhdlProcessId )).c_str();

  listOfProcesses.push_back(this);
  postponed = false;
}

VHDLKernel::~VHDLKernel(){}

void
VHDLKernel::initialize() {
  getVHDLState()->initState(proc, this);
  // set GUARD signals to result of their expression
  if( getGuard() != NULL ){
    updateGuard(getGuard());
  }
  executeVHDL( *(VHDLKernel_state *)getState() );
}

void
VHDLKernel::finalize() {
  getVHDLState()->cleanState();
}

Set<VHDLEvent> *
VHDLKernel::getEventsToExecute(){
  // A transaction that has been posted to a process could be cancelled
  // during a marking process The events to be cancelled are received are
  // informed by CANCELTRANSACTION event eventsToCancel is tha array that
  // holds the CANCELTRANSACTION events
  Set<SigEvent> eventsToCancel;
  
  // In a simulation cycle, we could get different events that correspond
  // to several transactions posted for this cycle eventsInCurrentCycle
  // collects all these events from the simulation
  // kernel(TimeWarp/Sequential/...)
  Set<VHDLEvent> *eventsInCurrentCycle = new Set<VHDLEvent>;
  
  //Collect all the events to be processed in this
  //simulation time cycle
  //Define an expandable array to hold the events in the current
  //simulation cycle

  map <unsigned int, map <unsigned int, VHDLEvent *> > eventM;

  while ( haveMoreEvents() ){
    //get the elements from the inputQ 
    //add them to the expandable array of pointers
    const VHDLEvent *currentEvent = dynamic_cast<const VHDLEvent *>(getEvent());
    ASSERT( currentEvent != 0 );
    //Check if the currentEvent is a Cancellation Event 
    //that has been sent during the Marking process
    if( currentEvent->isCancelTransactionEvent() ) { //CANCELTRANSACTION
      eventsToCancel.add( const_cast<SigEvent *>(static_cast<const SigEvent *>(currentEvent)));
    }
    else {
      VHDLEvent * curr = const_cast<VHDLEvent *>(currentEvent);
      SigEvent *postedEvent = dynamic_cast<SigEvent *>(curr);
      if (postedEvent) {
        eventM[postedEvent->getSenderSigId()][postedEvent->getSequenceNumber()] = curr;
      }
      else
        eventsInCurrentCycle->add( curr );
    }
  }
    
  // Only test time, not time & delta, for a postponed process.
  // Have to introduce this change in the WARPED API to handle this case...

  //CANCEL out the messages by looking at the events to be processed
  //in this cycle and the remove them
  SigEvent *toCancel = eventsToCancel.getElement();
  bool matchFound = true;
  while( toCancel != 0 ){
    matchFound = false;
    if (eventM.find(toCancel->getSenderSigId()) == eventM.end())
      break;
    if (eventM[toCancel->getSenderSigId()].find(toCancel->getSequenceNumber()) ==
        eventM[toCancel->getSenderSigId()].end())
      break;
    matchFound =true;
    eventM[toCancel->getSenderSigId()].erase(toCancel->getSequenceNumber());
    toCancel = eventsToCancel.getNextElement();
  }
  if( matchFound == false ){
    cerr << getProcessName() << " -- Error: can't find Transaction to cancel after marking\n";
    cerr << "Simulation may not be functionally correct\n";
    abort();
  }

  map <unsigned int, map <unsigned int, VHDLEvent *> >::iterator mm_i, mm_end;
  mm_i = eventM.begin();
  mm_end = eventM.end();
  for ( ; mm_i != mm_end ; ++mm_i ) {
    map <unsigned int, VHDLEvent *>::iterator m_i, m_end, m_last;
    m_i = (*mm_i).second.begin();
    m_end = (*mm_i).second.end();
    if (m_i != m_end) {
      for ( ; m_i != m_end ; ++m_i)
      {
        m_last = m_i;
      }
      eventsInCurrentCycle->add( (*m_last).second );
    }
  }
  
  return eventsInCurrentCycle;
}

void
VHDLKernel::executeProcess() {
  Set<VHDLEvent> *eventsInCurrentCycle = getEventsToExecute();
  // process all events at the current time
  
  VHDLEvent *event = eventsInCurrentCycle->getElement();
  while( event != 0 ){
    if (event != NULL) {
      event->execute( this );
    }
    event = eventsInCurrentCycle->getNextElement();
  }
  delete eventsInCurrentCycle;

  executeVHDL( *getVHDLState() );
}

void
VHDLKernel::reportError(const string& msg, const SEVERITY level)  {
  char option = 'n';
  
  switch (level) {
  case NOTE:
    cerr << "Note: " << msg << endl;
    break;

  case WARNING:
    cerr << "Warning: " << msg << endl;
    break;

  case ERROR:
    cerr << "Error: " << msg << endl;
    cerr << "Do you wish to continue? (y/n) " << flush;
    cin  >> option;
    if ((option != 'y') && (option != 'Y')) {
      abort();
    }
    
    break;

  case ABORT:
  default:
    cerr << "Abort: " << msg << endl;
    abort();
    break;
  }
}

const VHDLVTime &
VHDLKernel::getTimeNow() const {
  return dynamic_cast<const VHDLVTime &>(getSimulationTime());
}

void
VHDLKernel::deallocateState(State *trash) {
  delete trash;
}

void
VHDLKernel::reclaimEvent( const Event *e ) {
  delete e;
}

void
VHDLKernel::reclaimSerializedInstance(SerializedInstance *si) {
  delete si;
}

void
VHDLKernel::disconnectDriver( SigEvent *event ){
  SignalBase *signal = getVHDLState()->locateSig(event->getSigId()); 
  int processId = event->getSigSrc();
  
  disconnectDriver( signal, processId );
  
}

void
VHDLKernel::updateDriver( SigEvent *event ){
  //  int sigId = event->getSigId();
  int sigSrc = event->getSigSrc();
  int senderSigId = event->getSenderSigId();

  const RValue &data = event->getData();
  ArrayInfo dInfo = event->getDestBounds();
  ArrayInfo sInfo = event->getSourceBounds();

  SignalBase *signal = getVHDLState()->locateSig(event->getSigId()); 
  if( signal == 0 ){
    cerr << getName() << " asked to update signal id |" << 
      event->getSigId() << "|, but can't locate it in my state." << endl;
    abort();
  }

  updateDriver( signal, getProcessForId( sigSrc ), senderSigId, &data, &sInfo, &dInfo );
}

void
VHDLKernel::updateWait( WaitEvent* event ){
  if( event->getWaitId() != getVHDLState()->waitLabel ){
    // This case is NOT an error case.  It could happen that I was waiting
    // with the following VHDL statement:
    //    wait on sig until <cond> for <time> ns;
    // If the signal has an event on it, I will no longer wait here, but
    // the wait-resume event that I sent out earlier WILL arrive here.  I
    // should not abort on that.  I have to check if I was waiting on this
    // wait stmt.  If not, just return without doing anything.
    return;
  }

  updateWait( dynamic_cast<const VHDLVTime &>(event->getSendTime()) );
}

void
VHDLKernel::updateWait( const VHDLVTime & ){ abort(); }

bool
VHDLKernel::resumeWait(int, const EnumerationLiteral & ){ abort(); }

void
VHDLKernel::executeWait(int, const PhysicalLiteral & ){ abort(); }

void
VHDLKernel::setProcedureWait( Wait * ){ abort(); }

void
VHDLKernel::assignSignal( SignalBase &, 
			  VHDLKernel *, 
			  const RValue &,
			  const PhysicalLiteral &,
			  const PhysicalLiteral &,
			  const ArrayInfo &,
			  const ArrayInfo & ){ 
  abort(); 
}

void
VHDLKernel::assignSignal( SignalBase &, 
			  VHDLKernel *, 
			  const RValue &,
			  const VHDLVTime &, 
			  const VHDLVTime &,
			  const ArrayInfo &, 
			  const ArrayInfo & ){ 
  abort(); 
}


void
VHDLKernel::updateSignal( SignalBase*, bool ){ abort(); }

Value
VHDLKernel::locateDelayedAttribute( RValue *){
  abort();
}

EnumerationLiteral
VHDLKernel::locateStableAttribute( RValue *, 
				   const VHDLVTime ){
    abort(); 
}

EnumerationLiteral
VHDLKernel::locateQuietAttribute( RValue *, 
				  const VHDLVTime ){ 
  abort();
}

UniversalInteger *
VHDLKernel::locateTransactionAttribute( RValue *){
  abort(); 
}

EnumerationLiteral
VHDLKernel::locateEventAttribute( RValue * ){
  abort();
}

EnumerationLiteral
VHDLKernel::locateActiveAttribute( RValue *){
  abort();
}

PhysicalLiteral 
VHDLKernel::locateLastEventAttribute( RValue *){ 
  abort(); 
}

PhysicalLiteral 
VHDLKernel::locateLastActiveAttribute( RValue *){ 
  abort(); 
}

Value
VHDLKernel::locateLastValueAttribute( RValue *){
  abort(); 
}

EnumerationLiteral
VHDLKernel::locateDrivingAttribute( RValue *){
  abort();
}

Value
VHDLKernel::locateDrivingValueAttribute( RValue *){
  abort();
}

void
VHDLKernel::scheduleWait( int waitId, const VHDLVTime &waitTime ){
  // schedule resumption from this wait by sending event to self
  WaitEvent *waitEv = new WaitEvent( getTimeNow(),
				     waitTime, 
				     this,
				     this,
				     waitId );
  receiveEvent( waitEv );
}

#endif

