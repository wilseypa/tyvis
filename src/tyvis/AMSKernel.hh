#ifndef AMSKERNEL_HH
#define AMSKERNEL_HH

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
#include <math.h>
#include <list>
#include "tyvis/VHDLKernel.hh"
#include "tyvis/NodeList.hh"
#include "tyvis/Constants.hh"
#include "tyvis/ContributionNode.hh"
#include "tyvis/AMSKernel_state.hh"
extern "C" {
#include "spmatrix.h"
}

using std::list;

class component;
class Quantity;

/** Kernel that performs the simulation for analog processes. */
class AMSKernel: public VHDLKernel {

public:
  AMSKernel();
  virtual ~AMSKernel();
  
  /* Returns the pointer to the current set of equations in the analog
     process for the present time point. */
  list<component *> *getCurrentEquations() ;
  
  /** Routine for loading the matrix with the appropriate values. 

      @return a 1 if error occured in matrix load, else returns 0.

      @param matrix Pointer to the system matrix. */
  int matrixLoad(char *matrix);
      
  /** Iteration through the circuit components, to integrate, fill the
      matrix and solve the system. 

      @return a 1 if error occured, else returns 0.

      @param MaxIter indicates the maximum number of iteration for the current analysis.
      @param matrix Pointer to the system matrix.*/
  int iterate(int MaxIter, char *matrix);
      
  /** Returns whether current analysis is a DC or Transient analysis. */
  int getAnalysis() {
    return analysis;
  }
  
  /** Returns the Matrix size. */
  int getMatrixSize() const;
  
  /** Returns the pointer to the DC Matrix. */
  char* getDcMatrix();

  /** Returns the pointer to the Transient Matrix. */
  char* getTransientMatrix();

  /** Adds a node to the rhsnodeslist. 

      @return The node number for a future node in the system.

      @param nodeId a user defined id.
      @param type type of the node(EFFORT or FLOW). */
  int addNode(int nodeId, nodeConstants type);
  
  /** Adds the node to the rhsnodelist, if the node is not already present
      in the list.  

      @return The current position of the node in the rhs list. 

      @param nodeId a user defined id.
      @param type type of the node(EFFORT or FLOW). */
  int addNodeCond(int nodeId, nodeConstants type);
  
  /** Adds the node to the rhsnodelist, if the node is not already present
      in the list.  

      @return The current position of the node in the rhs list.    
      
      @param nodeId a user defined id.
      @param type type of the node(EFFORT or FLOW).
      @param nam name of the node. */
  int addNodeCond(int nodeId, nodeConstants type,char* nam);
      
  /** This function searches through the rhsnodelist for the given name.

      @return The equation number if found in the list, else returns
      NONODE constant. 

      @param nodeId id to be looked up in the list. */
  int findNodeRHS(int nodeId);   
  
  /** Forms the list of current equations of the analog process for the
      present Time point. */
  void formCurrentEquations();

  /** Determines wether the system is solvable or not. */
  void checkSolvability();
      
  /** Initializes the Analog process, also finds the DC operating point of
      the process. */
  void initialSettingUp();
  
  /** Returns the component pointer which contains the current equation
      set. */
  component* getComp() const;
  
  /** Returns the set of all the quantities in the current equation set. */
  list<Quantity *>* getSetOfQuantities(); 
  
  /** Routine which allocates memory for rhs and oldrhs. */
  void completeDynamicState();
  
  /** Allocates memory for storing the matrix values of each equation. */
  void definePointersAcross();
      
  /** Sets the row for each equation in Matrix. */
  void newSetBranch();
  
  /** Determines DC Operating Point of the system. 

      @return a 1 if error occured , else returns 0.

      @param firstMode Initial mode of the DC analysis.
      @param iterLimit maximum number of iteration for the DC analysis. */
  int findDCOp(long firstMode, int iterLimit);
   
  /** Initial setting up of continous process is done here. */
  virtual void setup(void)=0;   
  
  /** Loads the matrix by calling the load function of all equations. 

      @return a 1 if error occured , else returns 0.

      @param matrix Pointer to the system matrix. */
  int newLoad(char *matrix);

  /** Allocates pointer in the Matrix for various elements of each
      equation. */
  int newPointerAllocation();
  
  /** Used for checking the presence of a FLOW  between the 2 nodes
      posNode and negNode. 

      @return a 1 if found, else retuns 0.

      @param posNode positive node of the branch.
      @param negNode negative node of the branch.
      @param indexVal Id of the branch. */
  int newCheckCurrent(int posNode, int negNode, int indexVal);
  
  /** Returns the rhs array 'b' of the Ax=b system of equations. */
  double* getRhs();
      
  /** Returns previous iteration's rhs value. */
  double* getOldRhs();
      
  /** It finds the branch corresponding to a controlling current if present
      between the specified nodes. 

      @return a 1 if found, else retuns 0.

      @param posNode positive node of the branch.
      @param negNode negative node of the branch. */
  int newFindBranch(int posNode, int negNode);
      
  /** Returns RHS Nodes. */   
  inline nodeList* getRhsNodes() const;
  
  /** Function calls execute for Analog Process. */
  void executeVHDL() {
    executeAnalogVHDL();
  }
  
  /** Simulation cyle of a analog VHDL process. */
  int executeAnalogVHDL() {  
    initialSettingUp();
    return 1;
  }
  
  /** Add an equation to the circuit. 

      @param newcomp new component added to the equation list */
  void add(component* newcomp);
  
  /** Function for debug purposes. */
  virtual void printFunction() = 0;
      
  /** Returns the counter to the number of free equations loaded into the
      DC matrix. */
  int getFreeLoadEqnCounter();
  
  /** Increases the counter to the number of free equations loaded into the
      DC matrix by one. */
  void incrementFreeLoadEqnCounter();

  /** Returns the current simulation Time. */
  discreteTime getCurTime() ;

  /** Checks for truncation error and updates the delta with the error correction.

      @return 1 if error, else returns 0.

      @param timeStep pointer to the current delta. */
  int trunc(double *timeStep);

  /** Determines if a truncation error occured. If yes it returnes the new 
      time step.

      @param qcap index into state for which truncation error has to be 
      checked
      
      @param timeStep pointer to the current delta. */
  void truncError(int qcap, double* timeStep);

  /** Updates values of all the quantities, called after solving the matrix. */
  void updateQuantityValues(); 

  /** Returns the present value of the quantity

      @return the present quantity value 
      
      @param quantityPtr pointer to the quantity of interest. */
  double getNewIterValueOfQuantity(Quantity *quantityPtr);

  /** Returns the previous iteration value of the quantity

      @return the previous iteration value of the quantity 
      
      @param quantityPtr pointer to the quantity of interest. */
  double getPrevIterValueOfQuantity(Quantity *);

  /** This is the Analog simulation routine. The simulation is started 
      initially at zero time point and contined till the end time, which is 
      given as an argument. In every additional simulation cycle the 
      simulation will continue from the time point where the previous 
      simulation stopped. The function is based on the dctran function 
      of the spice3f4 version.

      @return 0 if analog simulation from current time point to the end time 
      is succesfull, else returns 1.
      
      @param endTime end time of this simulation cycle. */
  int simulate(discreteTime endTime);

  /** Returns state vector of specific integeration order,which holds the 
      previous iteration delta values. It is used for the integeration of 
      differntial equations.

      @return returns the state vector.

      @param i indicates the integeration order. */
  double* getStateVector(int i);

  /** This function creates all state vectors. */
  void createStates(); 

  /** Returns the length of the state vector */
  int getStateLength();

  /** Incerements state length by 2 */
  void incrementState();

  /** Copies one state to other.

      @param from the source state array.
      @param to the destination state array. */
  void copyStates(int from, int to);

  /**  rotate state vectors towards right. */  
  void rotateStates();

  /** Routine which performs the integeration to linearize differntial 
      equations of the form y_dot = y'dot. 

      @return returns 0 if succesfull, else returns 1.

      @param matrixValue pointer in the matrix for the 'dot quantity.
      @param rhsValue pointer for the differential equation,in B vector in the 
      Ax = B system of equations.
      @param stateIndex Index in the statevector for the current 'dot quantity. */
  int integrate(double* matrixValue,double* rhsValue, int stateIndex);

  /** Returns the state machine state vector. */
  long getMode();

  /** Calculates the integration coefficients depending on the current degree 
      of integration. */
  int compCoeff();

protected:
  
  /** This method wraps warped's getState() called and does the appropriate
      casting for us to return a AMSKernel_state. */
  AMSKernel_state *getSolverState(){ return (AMSKernel_state *)getState(); }

private:

  /// Tolerance for the pivot element.
  double pivotAbsTol;

  /// Tolerance for the pivot element.
  double pivotRelTol;

  /// List which holds reference to the B vector in the Ax = B system of
  /// equations.
  nodeList* rhsNodes;
  
  /// System Matrix during DC analysis.
  char* dcMatrix;

  /// System Matrix during transient analysis.
  char* transientMatrix;

  /// To store the mode of analysis : DC->0 ; TRANSIENT->1.
  enum {DC,TRANSIENT} analysis;

  /// Maximum number of iterations for dc analysis.
  int dcMaxIter;

  /// Maximum number of iterations for dc analysis.
  int transientMaxIter;

  /// Pointer to all components.
  component* comp;

  /// Flag to indicate if convergence was reached or not.
  int noConv;

  /// Counter to number of free equations loaded in the matrix.
  int freeLoadEqnCounter;

  /** Holds the List of equations analog process need to solve for the
      present Time point. */
  list<component *> *currentEquations;

  /// Set of all the quantities in the current equation set.
  list<Quantity *> *setOfQuantities;

  /** Function to test for convergence using user defined tolerances. */
  int convergenceTest();

  /// Maximum Timestep value.
  double maxStep;
  
  /// Minimum timestep value. It is set to one femto second. 
  double delMin;

  /// Time at which the analog process starts.
  discreteTime startTime;

  /// Flag to indicate if initial setting up is done or not.
  bool setupFlag;

  /// Indicates Maximum order to which integeration is performed. 
  /// Default is set to 2.
  int maxIntegOrder;

  /// Tolerance for voltage.
  double voltTol;

  /// Absolute Tolerance.
  double absTol;

  /// Relative tolerance.
  double relTol;

  /// Charge tolerance.
  double chgTol;

  /// Correction factor for truncation error estimation.
  double trTol;


};
#endif
