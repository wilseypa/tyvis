#ifndef BRANCH_EQUATION_HH
#define BRANCH_EQUATION_HH

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
#include <stdarg.h>
#include "tyvis/Component.hh"
#include "tyvis/vhdl_ams.hh"
#include "tyvis/Quantity.hh"
#include "tyvis/VHDLType.hh"
#include "tyvis/AMSType.hh"
#include "tyvis/adouble.h"
#include "tyvis/adutils.h"

class _savant_entity_ams_elab;
class component;
class AMSType;

/** This is the class for all branch equations i.e.  equations in which all
    quantities obey conservative equations such as Kirchoff's Laws.

    A branch equation is published only when all the quantities in a
    simultaneous equation are branch quantities. If there is a FREE
    quantity in a simultaneous equation, then necessarily a free Equation
    is published. */
class branchEquation : public component {

public:
  /** Default constructor. */
  branchEquation();   
  
  /** Default destructor. */
  virtual ~branchEquation();
  
  /** This constructor is called for every scalar equation. 

      @param name name of the branch equation.
      @param functionPtr pointer to the function which represents the characteristic 
      equation in ADOLC format to evaluate partial derivatives.
      @param noOfVariables Number of quantities in the branch equation.
      @param Qty Array holding pointers to the quantites present in the equation. */
  branchEquation(const char* name, adouble (*functionPtr) (component *, adouble*),
                 int noOfVariables, VHDLType **Qty);

  /** General equation constructor for every branch equation. 

      @param designPtr elaboration class pointer.
      @param parent_equation Pointer to the implicit equation (if any) created due to 
      'dot quantities in the current equation.
      @param caseValue Indicates the condition of the equation. Takes values YES/NO/-1(non-conditional).
      @param name Name of the branch equation.
      @param functionPtr pointer to the function which represents characteristic
      equation in ADOLC format to evaluate partial derivatives.
      @param noOfVariables Number of quantities in the branch equation. */
  branchEquation(_savant_entity_ams_elab *designPtr, component **&parent_equation, int caseValue, const char* name,
                 adouble (*functionPtr) (component *, adouble*),int noOfVariables, ...);

  /** Initialisation function for each component. */
  virtual void init();
  
  /** Check to see if it is a conserved equation. */
  virtual bool getConsCheck(); 

  /** Find branch ie row in matrix number for current equation. */
  virtual void setBranch();
  
  /** Define pointers to the various matrix elements for this equation. */
  virtual void definePointers();
  
  /** Find the branch from the specified nodes, if any. 

      @return a 1 if found, else retuns 0.

      @param posNode positive node of the branch.
      @param negNode negative node of the branch. */
  virtual int findBranch(int posNode, int negNode);

  /** Return type of equation. */  
  virtual eqnType getEquationType();

  /** Return list of quantities for this equation. */
  virtual Quantity** getQuantities();

  /** Return number of quantities in this equation. */
  virtual int getNumberOfQuantities();

  /** Check for the controlling current, if any. 

      @return a 1 if found, else retuns 0.

      @param posNode positive node of the branch.
      @param negNode negative node of the branch.
      @param indexVal Id of the branch. */
  virtual int checkCurrent(int posNode, int negNode, int index);

  /** Load equation into the matrix. */
  virtual int load();
 
  /** Pointer allocation for the matrices. */
  virtual int pointerAllocation();

  /** Used to add a differenital Equation to the implicitEquationList */
  virtual void add(component *comp);

  /** Loads implicit equation, corresponding to the implicit quantity in 
      the current equation during DC Analysis.

      @return returns 0 if imlicit load is successful, else returns 1.

      @param qty implicit quantity in the branch equation. */
  int loadImplicitEqns(Quantity* qty); 

  /** Loads implicit equation, corresponding to the implicit quantity in 
      the current equation. */ 
  int loadImplicitEqns();

  /** Returns the list of implicit equation associated with the current 
      equation. */
  virtual component* getImplicitEquationList(void);  

  /** Performs initializatrion for all the implicit equations. */
  void initializeImplicitEqns();

  /** Allocates Matrix pointers for implicit equations. */
  int pointerAllocationForImplicitEqns();


protected:

private:
  /// Number of quantities in f(x) = LHS - RHS.
  int numberOfQuantities;

  /// List of implicit equations for current equation. 
  component *implicitEquationList;

  /// Check for conservative equations. 
  bool conservativeCheck;

  /// Function Pointer to the f(x) = LHS-RHS function.
  adouble (*functionPointer) (component *, adouble*);

  /// Pointer of pointers to the sparse matrix during DC analysis.
  spREAL **dcMatrixPointers;

  /// Pointer of pointers to the sparse matrix during transient analysis.
  spREAL **transientMatrixPointers;

  /// Stores the size of the matrix.
  int matrixCount;

  /// Array which maintains the position of each quantity, in the dcMatrixPointers array.
  int* base;

  /// To check if controlling currents are correct. 
  int* correctValue;

  /// Reference to the quantities in this equation. 
  Quantity** branchQty;

  /// Branch in the matrix for this equation. 
  int branch;

  /// Holds 'true' if current quantity is a source current. 
  int sourceCurrent;

};

#endif

