#ifndef VHDLPROCESS_HH
#define VHDLPROCESS_HH

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
#include "tyvis/VHDLKernel.hh"
#include "tyvis/vhdl.hh"
#include "tyvis/EventSigs.hh"

class StringLiteral;

/** This class implements the LRM definition of a VHDL'93 process.  The
   code defined inside of the method "executeVHDL" represents the
   sequential code defined by a VHDL process.  Some forms of VHDL
   sequential statements have been defined within this class as a method
   call.  */

class VHDLProcess : public VHDLKernel {
public:

  /** This method executes a VHDL assertion statement.  

      @param assertStatement "true" if this is an assert statement, "false"
      if it's a report statement.  Merely changes formatting of output
      slightly.

      @param condition This is the expression that the assertion "trips"
      on.  If it's true, the assertion trips, false it doesn't.  For report
      statments, you must always pass "true".

      @param message The message that this statement will print when it
      trips.

      @param severityString Will be reported as the severity of the
      violation.
      
      @param locatorInfo Will be reported as where the error occurred.
 */
  void vhdlAssertOrReport( bool assertStatement, 
			   const RValue &condition, 
			   const PhysicalLiteral &timeOccurred,
			   const RValue &message,
			   const RValue &severityLevel, 
			   const string &locatorInfo );

  /**
     This method is an upcall to generated code.  It gets called after the
     signal updation phase occurs.  Therefore, all input signals have been
     updated, and the code executed in the upcall needs to access these
     signals and other elements of this process's state, and generate
     output signal updates.
  */
  virtual void executeVHDL() = 0;  // VHDL process simulation code


  /**
     This method is the entry point from VHDLKernel.  That is, all
     warped/event related processing happens in VHDLKernel, and then it
     hands of execution to us through this upcall.
  */
  virtual void executeVHDL( VHDLKernel_state &currentState );

  /// @see VHDLKernel#updateSignal
  void updateSignal(SignalBase*, bool initializingSimulation = false);

  /** Resolution functions are not called from updateSignal.  The driver
      which modifies the value of the signal has the responsibility to
      update the values of all the fanouts of the signals and send the
      corresponding messages to them.  This is done to avoid multiple
      resolution function computation.  If the destination were to resolve
      its signals, the same values will be computed by each of the fanouts.
      This is why the check of resolved funciton has been removed from
      updateSignal. */
  void updateSignal( int sigId, 
		     VHDLProcess *srcId, 
		     VHDLData *data, 
		     const ArrayInfo *dInfo, 
		     const ArrayInfo *sInfo );


  //@{
  /**
     Assigns the signal \dest the value of \src (having the source identifier 
     srcID). The assigment is done with the delay of \delay. Optionally may the
     rejection time \rejTime be assigned. For array assignments can the array
     information of the source (\sinfo) and the destination (\dinfo) be given.

     Each member on the fanout list of the \dest will have an event appended
     to its private event queue for that signal. This is required for the
     distributed parallell simulation of Warped.

     \brief Assigns the signal \dest the value of \src.

     \param dest The destination signal.
     \param srcId The source process identifier.
     \param src The source value.
     \param delay The delay before the signal becomes effective.
     \param rejTime The rejection time.
     \param dinfo The destination array info.
     \param sinfo The source array info.
   */

  void assignSignal( SignalBase &dest, 
		     VHDLKernel *srcId, 
		     const RValue &src,
		     const RValue &delay,
		     const RValue &rejTime = SAVANT_ZERO_TIME, 
		     const ArrayInfo &dinfo = defaultInfo(),
		     const ArrayInfo &sinfo = defaultInfo() );
  
  void assignSignal( SignalBase &dest, 
		     VHDLKernel *srcId, 
		     const RValue &src,
		     const VHDLVTime &delay, 
		     const VHDLVTime &rejTime = VHDLVTime::getVHDLVTimeZero(),
		     const ArrayInfo &dinfo = defaultInfo(), 
		     const ArrayInfo &sinfo = defaultInfo() );

  //@}

  //@{
  /**
     Assigns the signal \dest a null transaction (having the source identifier 
     srcID). The assigment is done with the delay of \delay. Optionally may the
     rejection time \rejTime be assigned. For array assignments can the array
     information of the source (\sinfo) and the destination (\dinfo) be given.

     Each member on the fanout list of the \dest will have an event appended
     to its private event queue for that signal. This is required for the
     distributed parallell simulation of Warped.

     \brief Assigns the signal \dest a null transaction.

     \param dest The destination signal.
     \param srcId The source process identifier.
     \param delay The delay before the signal becomes effective.
     \param rejTime The rejection time.
     \param dinfo The destination array info.
     \param sinfo The source array info.
   */
  
  virtual void assignNullTransaction( SignalBase &dest,
				      VHDLProcess * srcId,
				      const PhysicalLiteral &delay,
				      const PhysicalLiteral &rejTime = SAVANT_ZERO_TIME, 
				      const ArrayInfo &dinfo = defaultInfo(),
				      const ArrayInfo &sinfo = defaultInfo());
  
  virtual void assignNullTransaction( SignalBase &dest,
				      VHDLProcess *srcId,
				      const VHDLVTime &delay, 
				      const VHDLVTime &rejTime = VHDLVTime::getVHDLVTimeZero(),
				      const ArrayInfo &dinfo = nullInfo(), 
				      const ArrayInfo &sinfo = nullInfo());
  //@}

  void updateImplicit( SignalBase*, const RValue * );
  void updateImplicitAttributes( SignalBase*, const RValue*, bool);
  void updateImplicitAttributes( SignalBase *, const RValue * );
  
  /** The following function updates the driver of the signal. */
  void updateDriver( SignalBase *signal, 
		     VHDLKernel *srcProcess,
		     int senderSigId,
		     const RValue *newData,
		     ArrayInfo *sourceInfo,
		     ArrayInfo *destInfo );

  /**
     Updates the wait state for the current time to "expired" if the wait
     time has passed.
  */
  void updateWait( const VHDLVTime &waitTime );

  bool resumeWait(int, const VHDLData & = SAVANT_BOOLEAN_TRUE);
  void executeWait(int, const PhysicalLiteral& = SAVANT_INFINITY_TIME);
  void executeWait(int, const Variable&); 

  void setProcedureWait( Wait *myWait ){
    VHDLKernel_state *state = getVHDLState();
    state->procWait = myWait;
  }

  /**
     Disconnects the process specified from the signal specified.
  */
  void disconnectDriver( SignalBase *signal, int processId );

  /** The second argument is a dummy argument. */
  void updateAttribute(Attribute *attribute,
		       const RValue &lastValue = SAVANT_BOOLEAN_FALSE ) const;

  void updateDefaultAttributes( SignalBase *signal,
				const RValue *oldEffVal,
				bool eventOccurred );


  int getWaitLabel() {
    return getVHDLState()->waitLabel;
  }

//  Value locateDelayedAttribute( RValue * );
  EnumerationLiteral locateStableAttribute( RValue *, 
					    const VHDLVTime = VHDLVTime::getVHDLVTimeZero()) const;
  EnumerationLiteral locateQuietAttribute( RValue *,
					   const VHDLVTime = VHDLVTime::getVHDLVTimeZero()) const;
  UniversalInteger *locateTransactionAttribute(RValue *);
  EnumerationLiteral locateEventAttribute( RValue * ) const;
  EnumerationLiteral locateActiveAttribute( RValue * ) const;
  PhysicalLiteral locateLastEventAttribute( RValue * ) const;
  PhysicalLiteral locateLastActiveAttribute(RValue *) const;
  Value locateLastValueAttribute( RValue * );
//  EnumerationLiteral locateDrivingAttribute( RValue * );
//  Value locateDrivingAttribute( RValue * );

protected:
  /**
     Constructor.
     
     @param name - simply passed on to parent class.
   */
  VHDLProcess( const string &name, _savant_entity_elab * = NULL );
  
  /**
     Destructor.
   */
  ~VHDLProcess( );

private:
  /**
     This method does the updating of the input signals.
  */
  void executeSignalUpdationPhase( Block &scalarSignalsUpdateArray,
				   Block &compositeResolvedSignalsUpdateArray );


  /**
     Method used by public version to do the dirty work of assertion
     handling.
  */
  void vhdlAssertOrReport( bool assertStatement, 
			   bool condition, 
			   const PhysicalLiteral &timeOccurred,
			   const string &message,
			   const string &severityString, 
			   const string &locatorInfo );
  

  void updateGuard( RValue * );
  virtual RValue *getGuard() { return NULL; };
  virtual RValue *getGuardExpression() { return NULL;}

  /** The scalar signals updated this simulation cycle. */
  Block scalarSignalsUpdateArray;
  /** The composited signals updated this simulation cycle. */
  Block compositeResolvedSignalsUpdateArray;

  /** array of signal ID's that had events on them this sim cycle.  Used
      for wait resumption. */
  EventSigs eventSigs;
};

#endif

