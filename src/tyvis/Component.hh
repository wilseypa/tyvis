#ifndef COMPONENT_HH
#define COMPONENT_HH

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
#include <iostream>
#include <string>
#include <list>

extern "C" {
#include "spmatrix.h"
}
#include "tyvis/vhdl.hh"
#include "tyvis/Wait.hh"
#include "tyvis/CallStack.hh"
#include "tyvis/Constants.hh"
#include "tyvis/AMSKernel_state.hh"
#include "tyvis/AMSKernel.hh"
#include "tyvis/Quantity.hh"
#include "tyvis/vhdl_ams.hh"
#include "tyvis/Tolerance.hh"

using std::list;

class AMSKernel;
class Quantity;
class branchEquation;
class AMSProcess;
class VHDLType;

template <class type> class set;

/** This class forms the basic unit used to store all the equations.
    branchEquation, freeEquation and differentialEquation class derive from
    this class.  A lot of functions are pure virtual functions implemented
    in the derived classes.  */
class component  {
public:

  /** Standard constructor. Sets the component name.*/ 
  component(const char* ); 

  /** Default destructor. */
  virtual ~component(void) { }

  /** Every equation has an initialize function. Defined as a pure virtual
      function here. */ 
  virtual void init()=0;

  /** Funtion to return the equation type. Every equation has this
      function. Defined as a pure virtual function. */ 
  virtual eqnType getEquationType()=0;

  /** Every equation has this function to determine the row for this
      equation in the matrix. */ 
  virtual void setBranch();

  /** Function returns the set of quantities in the equation. */ 
  virtual Quantity **getQuantities();

  /** Returns the number of quantities in the equation. */ 
  virtual int getNumberOfQuantities();

  /** Check to see if it is a conserved equation. */
  virtual bool getConsCheck();
  
  /** Add equation to set of equations in current simulation cycle */
  virtual void addToCurrentEquations();

  /** Builds the set of quantities for this equation.

      @param quantitySet a pointer to the set of quantities. */
  virtual void buildQuantitySet(list<Quantity *> * quantitySet);

  /** Returns the branchSetFlag */
  bool getBranchSetFlag() {
    return branchSetFlag;
  }

  /** Defines pointers to the matrix for the equation. */ 
  virtual void definePointers(void);

  /** Function to find the branch for the equation, when no 
      controlling through quantity is found across the same terminals as
      the first quantity. Required for dependant sources. 

      @return index of the through quantity required.
      @param posNode positive node of the quantity.
      @param negNode negative node of the quantity. */
  virtual int findBranch(int posNode, int negNode);
  
  /** Check for the controlling current, if any. 

      @return 1 if the quantity respresented by the third parameter
      is a controlling current/flow quantity for the equation, else return 0.
      @param posNode positive node of the quantity.
      @param negNode negative node of the quantity.
      @param index quantity index. */
  virtual int checkCurrent(int posNode, int negNode, int index);
  
  /** Sets the loaded flag for the quantities in the equation. */ 
  void setLoadedFlagInQtys();
  
  /** Allocates pointers in the matrix for the equation. */
  virtual int pointerAllocation() {
    return 0;  
  }
  
  /** Used to load values into the matrix. Every component has an load
      function. */ 
  virtual int load() = 0;
  
  /** Returns the island for current equation. */
  _savant_entity_elab *getEnclosingBlock();

  /** Sets the island for the current equation.

      @param toSet entity to which this component belongs. */
  void setEnclosingBlock(_savant_entity_elab * toSet);

  /** Returns number of through or free quantities. */
  virtual int getThroFreeNumber(){
    return (noOfThroOrFreeQuantities);
  }

  /** Returns the component name */ 
  const char* getName(){
    return name;
  }

  /** Returns a pointer to the next component/equation in the list. */
  component * getNext(){
    return next;
  }

  /** Sets the address to the next component/equation in the list. 

      @param nextP pointer to the next component. */
  void setNext(component * nextP ){
    next = nextP;
  } 

  /** Returns a pointer the circuit to which this component belongs. */
  AMSKernel * getCkt(){
    return ckt;
  }  
  
  /** Sets the address to the circuit to which this component belongs. 

      @param cktP pointer to the circuit to which this component belongs.*/    
  void setCkt(AMSKernel * cktP){
    ckt=cktP;
  }

  /** Updates values of all the quantities, called after solving the matrix. */
  virtual void updateQuantityValues();

  /** Used to add a differenital Equation to the implicitEquationList */
  virtual void add(component *comp);

  /** Returns the list of implicit equation associated with the current 
      equation. */
  virtual component* getImplicitEquationList();
  
  /** Checks for truncation error and updates the time step delta with the error correction.

      @return 1 if error, else returns 0.

      @param timeStep pointer to the current delta. */
  virtual int trunc(double* timeStep);

  /** Load differntial equation into the matrix during DC analysis

      @return returns 0 if sucessful, else 1.

      @param qty pointer to the differential quantity.
  
      @param index position of the differential quantiy in the parent equation. */
  virtual int load(Quantity* qty, int);

protected:
  /// Default constructor. Should never be used.
  component(){};

  /// Flag to indicate if the matrix row for this equation has been determined.  
  bool branchSetFlag;
  
  /// Stores number of through and free quantities in the equation.
  int noOfThroOrFreeQuantities;
  
  /// Stores the type of the equation. Branch, through or free. 
  eqnType equationType;

  /// The circuit with which this equation is associated.
  AMSKernel *ckt;     

private:
  /// Pointer to the next component.
  component* next; 
  
  /// Stores the island for the current equation.
  _savant_entity_elab *enclosingBlock;
  
  /// Store the component's name.
  const char* name;   

};

#endif
