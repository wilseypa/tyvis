#ifndef AMSKERNEL_STATE_HH
#define AMSKERNEL_STATE_HH

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
#include "tyvis/CallStack.hh"
#include "tyvis/Block.hh"
#include "tyvis/Variable.hh"
#include "tyvis/VHDLKernel_state.hh"
#include "tyvis/Constants.hh"
#include "warped/SimulationObject.h"
#include "warped/State.h"

class Wait;
class SignalBase;
class VHDLKernel;
class AMSKernel;

typedef long long int discreteTime;

/** Class stores the variables needed in tha Analog cycle which change over
    the different time steps. */
class AMSKernel_state: public VHDLKernel_state {

public:
  AMSKernel_state();
  virtual ~AMSKernel_state();

  /** Returns the rhs pointer. */  
  double* getRhs();

  /** Returns oldrhs pointer. */
  double* getOldRhs();

  /** Cleans all the analog state. */
  void cleanState();

  /** Function is defined as pure virtual in base class VHDLKernel_state.
      Not implemented here. Defined purely to satisfy  pure virtual definition. */
  virtual SignalBase* locateSig(int);

  /** Returns the state machine state vector. */
  long getMode();

  /** Sets the state machine state vector. 

      @param newMode vector indicating mode of the AMS Kernel.*/
  void setMode(long newMode);

  /**Returns iteration state machine state vector. */
  int getIterMode();

  /* Sets the iteration state machine state vector. 

     @param newMode vector indicating iteration state vector.*/
  void setIterMode(int newMode);

  /** Sets the rhs vector. 

      @param newRhs pointer to the new rhs vector. */
  void setRhs(double* newRhs );

  /** Sets the current value for the specified row of the rhs vector. 

      @param position indicates the row to be sent.
      @param value indicates the value to be set to. */
  void setRhsValue(int position, double value);

  /** Sets the old rhs vector.

      @param oldRhs pointer to the old rhs vector. */
  void setOldRhs(double* newRhs);

  /** Sets the value for the old rhs vector. 

      @param position indicates the row to be sent.
      @param value indicates the value to be set to. */
  void setOldRhsValue(int position, double value);

  /** Returns the size of the rhs and oldRhs vectors. */
  int getVectorSize();

  /** Sets the size of the rhs and oldRhs vectors. 

      @param size the size of the vectors. */
  void setVectorSize(int size);

  /** Initialize state variables. */
  void initState();
  
  /** Retruns current simulation time */
  discreteTime getCurTime();

  /** Sets the current simulation time

      @param value the value to be set to the current simulation time. */
  void setCurTime(discreteTime value);

  /** Returns ith state vector.
 
      @return the ith state vector.

      @param i index of the required state vector. */
  double* getStateVector(int i);

  /** Returns individual state vector length */
  int getStateLength();

  /** Sets the state vector length

      @param value the value to be set to the state vector length. */
  void setStateLength(int value);

  /** Returns the order of integration */
  int getIntegOrder();

  /** Sets the order of integration. 

      @param value the value to be set to the integeration order */
  void setIntegOrder(int value);

  /** Returns the current time step increment. */
  double getDelta();

  /** Sets the current time step increment value.

      @param value the value to be set to the time step increment. */
  void setDelta(double value);  

  /** Returns the previous delta value saved before i time steps.

      @return the delta value saved before i time steps

      @param i position of the delta in the saved array of old deltas. */
  double getOldDelta(int i);

  /** Sets the ith old delta value

      @param i index of the old delta value to be set.
 
      @param value the value to be set to the old delta. */ 
  void setOldDelta(int i, double value);

  /** Returns the ith integration coefficient.

      @return returns the ith integration coefficient.
 
      @param i the index of integration coefficient. */
  double getIntegCoeff(int i);

  /** Sets the ith integration coefficient value.

      @param i the index of the integeration coefficient to be set.

      @param value the value to be set. */
  void setIntegCoeff(int i, double value);

  /** Returns the ith state vector array.

      @return returns the ith state vector array.

      @param i Index of the state vector array. */
  double* getStates(int i);

  /** Sets the ith state vector array.

      @param i Index of the state vector array to be set.

      @param value the value to be set to the state vector array. */
  void setStates(int i, double* value);

  /** Sets the i,j state vector array.

      @param i Index of the state vector array.

      @param j Index of the state vector array.

      @param value the value to be set to the state vector array. */
  void setStates(int i, int j, double value);

  /** Returns the final time of the simulation cycle. */
  discreteTime getFinalTime();

  /** Sets the final simulation time.
      
  @param value the value to be set to the final simulation time. */
  void setFinalTime(discreteTime value);

  /** Return the size of the state class. */
  unsigned int getStateSize();

  
  
  
protected:

private:

  /// State machine state vector. 
  long mode;

  /// Iteration state machine state vector.
  int iterMode;

  /// Holds the current right hand side value
  double* rhs;

  /// Holds the previous rhs value for convergence testing
  double* oldRhs;

  /// Stores the size of the rhs and oldRhs vectors. 
  int vectorSize;

  /// Individual state vector length 
  int stateLength;

  /// State vector array 
  double* states[4];

  /// Order of integeration in the trapezoidal rule.
  int integOrder;

  /// Coefficients of integeration.
  double integCoeff[7];

  /// End of the simulation cycle.
  discreteTime finalTime;

  /// Start of the simulation time, initially set to zero.
  discreteTime curTime;

  /// contains current timestep.
  double delta;

  /// History of previous delta. 
  double oldDelta[7];

};

#endif
