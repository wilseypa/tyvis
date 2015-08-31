#ifndef DIFFERENTIAL_EQUATION_HH
#define DIFFERENTIAL_EQUATION_HH

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

/** This class describes Differential Equations. A differential Equation of 
    the form " q_dot == q'dot " is created for every q'dot in the model. Here, 
    q represents a quantity and 'dot is an AMS attribute.

    When a quantity 'q' is used in a model in the form of q'dot, a new IMPLICIT 
    quantity called "q_dot" is created. Only one differentialEquation is  
    published for all occurrences of q'dot 

    A differential equation does not have an equationType. It is an equation 
    created by us. The equation where the 'dot occurs is named by our usual 
    conventions. */

class differentialEquation : public component {

public:
  /** Generalized constructor for elaborating all differential equations in 
      the model. 

      @param parent_equation Pointer to the parent equation form which current 
      differential equation was formed.
      @param name name of the branch equation.
      @param implicitQty the implicit quantity q_dot created fot the differntial equation 
      q_dot = q'dot
      @param quantityTickDot the differential quantity q in the equation q_dot = q'dot. */     
   differentialEquation(component **&parent_equation , const char* name, VHDLType * implicitQty,
			VHDLType* quantityTickDot);

  /** This constructor is called for every scalar differntial equation to be 
      elaborated.

      @param name name of the branch equation.
      @param implicitQty the implicit quantity q_dot created fot the differntial 
      equation q_dot = q'dot
      @param quantityTickDot the differential quantity q in the equation q_dot = q'dot. */
  differentialEquation(const char* name, VHDLType* implicitQty,
                       VHDLType* quantityTickDot);
  
  /** Destructor for the class */
  virtual ~differentialEquation(void); 

  /** Return type of equation. */
  virtual eqnType getEquationType();

  /** Initialisation function for each component. */
  virtual void init();

  /** Load equation into the matrix.Called during transient analysis */  
  virtual int load();

  /** Load equation into the matrix during DC analysis 

      @return returns 0 if sucessful, else 1.

      @param qty pointer to the differential quantity.

      @param index position of the differential quantiy in the parent equation. */
  int load(Quantity *qty, int index);

  /** Pointer allocation for the matrices. */
  virtual int pointerAllocation();

  /** Checks for truncation error and updates the time step delta with the error correction.
      
      @return 1 if error, else returns 0.

      @param timeStep pointer to the current delta. */
  virtual int  trunc(double *timeStep);

protected:

private:

  /// Pointer of pointers to the sparse matrix during transient analysis.
  spREAL **transientMatrixPointers;

  /// Reference to the quantities in this equation.
  Quantity** branchQty;

  /// Index into the state vector array for the current differential equation.
  int stateVectorIndex;

  /// Flag to indicate first iteration 
  int firstTime;

};

#endif
