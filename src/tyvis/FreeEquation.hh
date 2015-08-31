#ifndef FREE_EQUATION_HH
#define FREE_EQUATION_HH

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
#include "tyvis/Component.hh"
#include "tyvis/VHDLType.hh"
#include "tyvis/adouble.h"
#include "tyvis/adutils.h"

class _savant_entity_ams_elab;

/** Class stores all related information for free equations, ie
    Non-conserved equations.  If a simultaneous equation has a FREE
    quantity, then a Free Equation is necessarily published. */
class freeEquation : public component {
public:
  
  /** Generalized constructor for free equations. 

      @param designPtr elaboration class pointer.
      @param parent_equation Pointer to the implicit equation (if any) created due to
      'dot quantities in the current equation.
      @param caseValue Indicates the condition of the equation. Takes values YES/NO/-1(non-conditional).
      @param name name of the branch equation.
      @param functionPtr pointer to the function which represents characteristic
      equation in ADOLC format to evaluate partial derivatives.
      @param noOfVariables Number of quantities in the branch equation. */
   freeEquation(_savant_entity_ams_elab *designPtr, component **&parent_equation, int caseValue, const char* name, 
                adouble (*functionPtr) (component *, adouble*), int noOfVariables, ...);

  /** This constructor is called for each scalar free equation to be elaborated. 

      @param name Name of the branch equation.
      @param functionPtr pointer to the function which represents characteristic
      equation in ADOLC format to evaluate partial derivatives.
      @param noOfVariables Number of quantities in the branch equation.
      @param Qty Array holding pointers to the quantites present in the equation. */
  freeEquation(const char* name, adouble (*functionPtr) (component *, adouble*),
               int noOfVariables, VHDLType **Qty);

  /** Gets the type of the Equation. */
  virtual eqnType getEquationType() {
    return equationType;
  }

  /** Destructor. */
  virtual ~freeEquation(void);

  /** Every component has an init function. Helps out in assigning new indices. */
  virtual void init();

  /** Allocates pointers into the Jacobian matrix and rhs as appropriate for the equation. */
  virtual int pointerAllocation();

  /** Find the branch from the specified nodes, if any. 

      @return a 1 if found, else retuns 0.

      @param posNode positive node of the branch.
      @param negNode negative node of the branch. */
  virtual int findBranch(int posNode, int negNode);

  /** Loads appropriate values into the matrix locations. */ 
  virtual int load();

  /** Returns the type of the equation. ie FREE. */
  virtual char * getEquationKind(){
    return "FREE";   
  }

  /** Return list of quantities for this equation. */ 
  Quantity **getQuantities();
   
  /** Return the total number of quantities in this equation. */ 
  int getNumberOfQuantities();

  /** Returns the nodes for the quantities in this equation. */
  list<int*> *formNodeSet();

  /** Check to see if it is a conserved equation. */ 
  virtual bool getConsCheck(){
    return conservativeCheck;
  }
  
  /** Find the specified quantity in the quantity list.

      @return 0, if not equal to branch value else return branch value.   
      @param toCheck the branch which needs to be checked. */
  int findFreeQuantity(int toCheck);

   /** Returns the list of implicit equation associated with the current
      equation. */
  virtual component* getImplicitEquationList();

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

  /** Performs initializatrion for all the implicit equations. */
  void initializeImplicitEqns();
  
  /** Allocates Matrix pointers for implicit equations. */
  int pointerAllocationForImplicitEqns();

protected:

private:
  /** Declares a pointer to a function. */
  adouble (*functionPointer) (component *, adouble*);
   
  /// Pointer of pointers to the sparse matrix during dc analysis.
  spREAL **dcMatrixPointers;

  /// Pointer of pointers to the sparse matrix during transient analysis.
  spREAL **transientMatrixPointers;

  /// Branch in the matrix for this equation. 
  int branch;

  /// Total number of quantities in the equation.
  int numberOfQuantities;

  /// Reference to the free quantities.
  Quantity** freeQty;
    
  /// Check for conservative equations.
  bool conservativeCheck;

  /// Array which maintains position, of each quantity, in the dcMatrixPointers array.
  int* base;

  /// List of implicit equations for current equation.
  component *implicitEquationList;

};

#endif
