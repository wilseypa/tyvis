#ifndef VHDLKERNEL_HH
#define VHDLKERNEL_HH

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
#include <cassert>
#include <vector>
#include "tyvis/vhdl.hh"
#include "tyvis/Wait.hh"
#include "tyvis/VHDLKernel_state.hh"
#include "tyvis/VHDLVTime.hh"
#include "tyvis/EnumerationLiteral.hh"
#include <Set.h>

using std::vector;
using std::istream;
using clutils::Set;

// Class prototypes
class _savant_entity_elab;
class CallStack;
class Record;
class ScalarType;
class SigEvent;
class VHDLEvent;
class WaitEvent;

/** This class includes the methods and data required by a VHDL Object to
   interface to warped.  It is intended to be the glue between a VHDL
   process and Warped.  */

class VHDLKernel;
extern vector<VHDLKernel*> listOfProcesses;

class VHDLKernel : public SimulationObject {
public:
  /** Get a unique global object identifier. Will produce an integer number
      which is ensured to be globally unique and thus may be used as an
      identifier of the object.  Global id with increment each time this is
      called. */
  static int getGlobalObjectId();

  virtual void initialize();
  void finalize();
  virtual void executeProcess();           

  virtual void deallocateState(State *state);
  virtual void reclaimEvent(const Event* event);
  virtual void reclaimSerializedInstance(SerializedInstance *instance);

  
  void cancelSingleMessage(Event*);
  void sendNegEvent(Event*);

  // Some of the new methods that is required by warped 2.0 API...
  void reportError( const string &, const SEVERITY );


  virtual void executeVHDL( VHDLKernel_state & ){ abort(); }
  
  const VHDLVTime &getTimeNow() const;
  
  /** Implicitly assumes that a variable called "stack" is defined in the
      user-defined state of type CallStack. */
  CallStack* getCallStack() {
    return &getVHDLState()->stack;
  }

  const string &getName() const { return processName; }
  int getVHDLProcessId() const { return vhdlProcessId; }

  static const string &getName( const OBJECT_ID& id );
  static void initializeGlobalObjectId();

  /** Handle this "disconnect driver" event. */
  void disconnectDriver( SigEvent * );
  
  /** Defer the disconnect to our superclass */
  virtual void disconnectDriver( SignalBase *, int ){ abort(); }

  void setSharedData( const string &newData ){ sharedData = newData; }

  /** The following Function updates the driver of the signal corresponding
      to its sig id. Also adds this signal to the list of signals that
      should be updated in the current simulation cycle. */
  void updateDriver( SigEvent * );
  
  /**
     The actual driver updation is deferred to the VHDLProcess.
  */
  virtual void updateDriver( SignalBase *signal, 
			     VHDLKernel *srcProcess,
			     int senderSigId,
			     const RValue *newData,
			     ArrayInfo *sourceInfo,
			     ArrayInfo *destInfo ) = 0;

  /**
     Handles a wait event for us.
  */
  void updateWait( WaitEvent * );

  //@{
  /**
     Wait updation is deferred to the VHDLProcess.
  */
  virtual void updateWait( const VHDLVTime & );
  virtual bool resumeWait( int, const EnumerationLiteral & = SAVANT_BOOLEAN_TRUE );
  virtual void executeWait(int, const PhysicalLiteral& = SAVANT_INFINITY_TIME);
  virtual void setProcedureWait( Wait *myWait );
  //@}

  /** The following are the methods that operate over signals.  They are
      all upcalls to superclasses. */
  //@{
  /** Okay, immaterial of the original type of the signal we are now
      assiging values to a scalar signal.  Note that assigning a value to a
      signal does not change the value of the signal, but merely generates
      transactions on the signal.  The transactions ultimately get
      converted to events and get shipped out to the various (VHDL)
      processes that are sensitive to this signal.
      
      The process of posting transactions on signals also involves a phase
      called marking.  Each scalar signal does its own marking and
      generates the transactions to be sent out.  The actual events are
      sent out by the process.  */
  virtual void assignSignal( SignalBase &dest, 
			     VHDLKernel *srcId, 
			     const RValue &newValue,
			     const PhysicalLiteral &delay,
			     const PhysicalLiteral &rejTime = SAVANT_ZERO_TIME, 
			     const ArrayInfo &dinfo = defaultInfo(),
			     const ArrayInfo &sinfo = defaultInfo() );

  /** Okay, immaterial of the original type of the signal we are now
      assiging values to a scalar signal.  Note that assigning a value to a
      signal does not change the value of the signal, but merely generates
      transactions on the signal.  The transactions ultimately get
      converted to events and get shipped out to the various (VHDL)
      processes that are sensitive to this signal.
      
      The process of posting transactions on signals also involves a phase
      called marking.  Each scalar signal does its own marking and
      generates the transactions to be sent out.  The actual events are
      sent out by the process.  */
  virtual void assignSignal( SignalBase &dest, 
			     VHDLKernel *srcId, 
			     const RValue &newValue,
			     const VHDLVTime &delay, 
			     const VHDLVTime &rejTime = VHDLVTime::getVHDLVTimeZero(),
			     const ArrayInfo &dinfo = defaultInfo(), 
			     const ArrayInfo &sinfo = defaultInfo() );


  /**  The following function calls the resolution function and The type
       conversion functions if any for a signal and updates the values of
       its dependent signals and attributes. 

      This method is public because the type code defers to this method
      sometimes.
  */
  virtual void updateSignal( SignalBase*, bool initializingSimulation );

  virtual int getWaitLabel(){ abort(); }

  virtual Value locateDelayedAttribute( RValue *);
  virtual EnumerationLiteral locateStableAttribute( RValue *, 
						  const VHDLVTime = VHDLVTime::getVHDLVTimeZero());
  virtual EnumerationLiteral locateQuietAttribute( RValue *, 
						const VHDLVTime = VHDLVTime::getVHDLVTimeZero());
  virtual UniversalInteger *locateTransactionAttribute( RValue *);
  virtual EnumerationLiteral locateEventAttribute( RValue * );
  virtual EnumerationLiteral locateActiveAttribute( RValue *);
  virtual PhysicalLiteral locateLastEventAttribute( RValue *);
  virtual PhysicalLiteral locateLastActiveAttribute( RValue *);
  virtual Value locateLastValueAttribute( RValue *);
  virtual EnumerationLiteral locateDrivingAttribute( RValue *);
  virtual Value locateDrivingValueAttribute( RValue *);

  //@}
  
  void setProc( _savant_entity_elab *newProc ){ proc = newProc; }
  _savant_entity_elab *getProc(){ return proc; }

  const string &getProcessName(){ return processName; }

  void deallocateState( const State *toDeallocate ){ delete toDeallocate; }
  
protected:
  /**
     Schedules a wait for the waitId at the specified time.
  */
  void scheduleWait( int waitId, const VHDLVTime &waitTime );

  /** This method wraps warped's getState() called and does the appropriate
      casting for us to return a VHDLKernel_state */
  VHDLKernel_state *getVHDLState(){ return dynamic_cast<VHDLKernel_state *>(getState()); }
  
  VHDLKernel( const string &name, _savant_entity_elab* = NULL);
  virtual ~VHDLKernel();

private:
  bool postponed;

  bool eatwhite( AccessObject & );
  virtual void updateGuard( RValue * ){ abort(); }

  /**
     This method processes the events that warped has passed us.  It
     handles all of the cancellations for us, and only gives back a set
     of events which we actually need to execute.     
  */
  Set<VHDLEvent> *getEventsToExecute();

  virtual RValue *getGuard() { return NULL; };
  virtual RValue *getGuardExpression() { return NULL;}
  int   vhdlProcessId;
  string processName;
  string sharedData;
  _savant_entity_elab *proc;
};

#endif
