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

#include <clutils/Debug.h>
#include <list>

#include "FreeEquation.hh"
#include "_savant_entity_ams_elab.hh"

extern list<contributionNode *> globalContributionList;
extern list<VHDLType *> globalAcrossQuantityList;
extern int getFreeEquationId();
extern void setFreeEquationId(int);
extern int globalAMSId;

using clutils::debug;

freeEquation::freeEquation(_savant_entity_ams_elab *designPtr, component **&parent_equation, 
		 	   int caseValue,const char* name, adouble (*functionPtr) (component *, adouble*),
			   int noOfVariables, ...) {
  component *equation = NULL;

  // If the free equation has a scalar quantity on LHS, then this equation
  // is the only equation we need to process.  Equations with only some
  // quantities as array quantities should be treated as modelling errors.

  // Step through the list of unknown arguments and get the array of scalar
  // quantities on LHS-RHS.
  va_list ap;
  va_start(ap, noOfVariables);
  VHDLType **Qty = new VHDLType*[noOfVariables];
  for (int i=0; i<noOfVariables; i++) {
    VHDLType *currentQty = va_arg(ap, VHDLType *);
    Qty[i] = currentQty;
  }
  va_end(ap);

  if (Qty[0]->is_scalar_type() == true) {
    equation = new freeEquation(name, functionPtr, noOfVariables, Qty);
    equation->setEnclosingBlock(designPtr);
    if (caseValue != -1 ) {
      // This is the case when there are YES and NO conditions in Conditional
      // simultaneous statements. Code would be added later.
      cerr << "ERROR:freeEquation.cc:Conditional Simultaneous Statements not done yet." << endl;
      exit(1);
    }
    else {
      designPtr->add(equation);
    }
    // parent_equation pointer is set to this equation.
    // parent_equation pointer needs to be returned because
    // implicit/differential equations are not added to the global equation
    // set but to the parent equation to which they belong.
    parent_equation  = new component*[1];
    parent_equation[0] = equation ;
  }
  else {
    // code for handling array quantities in SSS needs to be put in here.
    cerr << "ERROR:freeEquation.cc:Array Quantities not handled yet." << endl; 
    exit(1);
  }
}

freeEquation::freeEquation(const char* name, adouble (*functionPtr) (component *, adouble*),
                           int noOfVariables, VHDLType **Qty) 
  :component(name) {
  
  // Assign the function pointer to the member.
  functionPointer = functionPtr;
  numberOfQuantities = noOfVariables;
    
  // Allocate memory for pointers to free quantities.
  freeQty = new (Quantity*)[numberOfQuantities];

  debug << "Equation name --> " << name << endl;
  debug << "RHSQtys : " << endl;

  for (int i=0;i<numberOfQuantities;i++) {
    VHDLType *currentQty = Qty[i];
    ASSERT(currentQty->getObject()->getKind() == ObjectBase::QUANTITY);
    currentQty->print(debug);
    freeQty[i] = (Quantity *)currentQty->getObject();
  }

  // We need to check if any quantity can be implicit in this situation.
  // If this freeQty[0] is related to what we are defining for the implicit
  // equation, then it can stay as in SIERRA.  The source current condition
  // may not be needed right now.  Check in view of line 129.
  if (freeQty[0]->getType() != IMPLICIT) {
    if ((freeQty[0]->getUsed() == 0)||(freeQty[0]->getSourceCurrent()==true)) {
      noOfThroOrFreeQuantities = 1;
    }
    else {
      noOfThroOrFreeQuantities = 0;
    }
  }
  freeQty[0]->setUsed();
  
  equationType = FREE_EQN;
  base = new int[numberOfQuantities];
  
  // Initializing the counter for number of pointers.
  int matrixCount = 0;
  
  for (int i=0; i<numberOfQuantities; i++) {
    // store the pointer base
    base[i] = matrixCount;
    if ( freeQty[i]->getType() == ACROSS) {
      matrixCount += 2;
    }
    else if (freeQty[i]->getType() == THROUGH) {
	if(freeQty[i]->getUsed()==0) {
	  noOfThroOrFreeQuantities++;
	}
      freeQty[i]->setUsed();
      matrixCount += 1;
    }
    else { // Free Quantity
    if (freeQty[i]->getType()!=IMPLICIT) {
      if (freeQty[i]->getUsed()==0) {
	noOfThroOrFreeQuantities++;
      }
    } 
    freeQty[i]->setUsed();
    matrixCount += 1;
    }
    conservativeCheck = false;
    branch = 0;
  } // end for
  dcMatrixPointers = new spREAL*[matrixCount];
  transientMatrixPointers = new spREAL*[matrixCount];
  implicitEquationList = NULL;  
}

freeEquation::~freeEquation() {
  delete [] dcMatrixPointers;
  delete [] transientMatrixPointers;
  delete [] base;
  delete [] freeQty;
}

void 
freeEquation::init() {
  component* index = NULL;
  if (getImplicitEquationList() != NULL){
    index = getImplicitEquationList();
    while (index != NULL){  
      index->setCkt(this->ckt);
      index = index->getNext();
    }
  }
  branch = ckt->addNodeCond(getFreeEquationId(), FLOW);
  for (int i=0; i<numberOfQuantities; i++) {
    if (freeQty[i]->getType() == ACROSS) {
      ckt->addNodeCond(freeQty[i]->getPosNode(), EFFORT,freeQty[i]->getPosTerminal()->getTerminalName());
      ckt->addNodeCond(freeQty[i]->getNegNode(), EFFORT,freeQty[i]->getNegTerminal()->getTerminalName());
    }
    else {
      ckt->addNodeCond(freeQty[i]->getIndex(), FLOW, freeQty[i]->getName());
    }
  }
  if (implicitEquationList != NULL){
    initializeImplicitEqns();
  }
}

int 
freeEquation::pointerAllocation() {
  char *dcmatrix = ckt->getDcMatrix();
  char *transientmatrix = ckt->getTransientMatrix();
  if (implicitEquationList != NULL){
    pointerAllocationForImplicitEqns();
  }
  // Now go through all the quantities present on the RHS.
  for (int i=0; i<numberOfQuantities; i++) {
    int Base = base[i];
    if (freeQty[i]->getType() == ACROSS) {
      int rhs1 = ckt->addNodeCond(freeQty[i]->getPosNode(), EFFORT,freeQty[i]->getPosTerminal()->getTerminalName()); 
      int rhs2 = ckt->addNodeCond(freeQty[i]->getNegNode(), EFFORT,freeQty[i]->getNegTerminal()->getTerminalName()); 
      // Pointers into DC Matrix.
      dcMatrixPointers[Base] = spGetElement(dcmatrix, branch, rhs1);
      dcMatrixPointers[Base+1] = spGetElement(dcmatrix, branch, rhs2);
      transientMatrixPointers[Base] = spGetElement(transientmatrix, branch, rhs1);
      transientMatrixPointers[Base+1] = spGetElement(transientmatrix, branch, rhs2);
    }
    else {
      // RHS quantity is either a free or a through quantity. Both are handled the same.
      int rhs1 = ckt->addNodeCond(freeQty[i]->getIndex(), FLOW,freeQty[i]->getName());
      // Pointers into DCMatrix locations.
      dcMatrixPointers[Base] = spGetElement(dcmatrix, branch, rhs1);
      transientMatrixPointers[Base] = spGetElement(transientmatrix, branch, rhs1);
    }
  } // End of for looping through rhs quantities.
  return(0);
}

int 
freeEquation::load() {
  double* rhs = ckt->getRhs();
  double freeQuantity;
  int noOfQtys = numberOfQuantities;
  double *qtyValues = new double[noOfQtys]; 
  
  // Start ADOL-C.
  int tag = 1;
  adouble* indepVariables = new adouble[noOfQtys]; 
  adouble dependentVariable;
  
  // Start active section of ADOL-C.
  trace_on(1);   
  
  for (int i=0; i<noOfQtys; i++) {
    // Assign the values of the rhs quantities to the independent variables
    // by the special adol-c assignment operator.
    qtyValues[i] = freeQty[i]->getValue(ckt);
    indepVariables[i] <<= qtyValues[i];
  }
  
  // Call the function corresponding to the rhs expression
  dependentVariable = functionPointer(this, indepVariables);
  
  dependentVariable >>= freeQuantity;
  
  // End active section
  trace_off();
  
  double lhsQty = freeQuantity;
  
  double *partialDerivatives = new double[noOfQtys];
  
  // This function is used to evaluate the partial derivatives w.r.t each
  // of the independent variables. 
  gradient(tag, noOfQtys, qtyValues, partialDerivatives);

  // Updating the LHS quantity.
  for(int i=0; i<noOfQtys; i++){
    lhsQty = lhsQty - partialDerivatives[i]*qtyValues[i];
  }
  if (ckt->getAnalysis() == 0) {
    // If dc mode.
    *(rhs+ckt->getFreeLoadEqnCounter()) = lhsQty;
  }
  else{
    //transient mode
    *(rhs+branch) = lhsQty;
  }

  // RHS quantities are very much present, so loop through them taking into
  // account their contribution to the Jacobian Matrix.
  for(int i=0; i<numberOfQuantities; i++) { 
    int Base = base[i];
    if (ckt->getAnalysis() == 0) {
      if (freeQty[i]->getType() == ACROSS) {
        *dcMatrixPointers[Base]   -= partialDerivatives[i];
        *dcMatrixPointers[Base+1] += partialDerivatives[i];
      }
      else {
        // RHS quantity is free or through.
        *dcMatrixPointers[Base] -=partialDerivatives[i];
      }
    }
    else {
      if (freeQty[i]->getType() == ACROSS){
        *transientMatrixPointers[Base]   -= partialDerivatives[i];
        *transientMatrixPointers[Base+1] += partialDerivatives[i];
      }
      else{
        //RHS quantity is through or free type
        *transientMatrixPointers[Base]  -=partialDerivatives[i];
      }
    }
  }
  
  delete [] partialDerivatives;
  delete [] qtyValues;
  delete [] indepVariables; 
  ckt->incrementFreeLoadEqnCounter();
  setLoadedFlagInQtys();
  if (implicitEquationList != NULL){
    if( (ckt->getAnalysis()==0)&&(ckt->getCurTime()==0) ){
      loadImplicitEqns(freeQty[0]);
    }  
    else{
      loadImplicitEqns();
    }
  }
  return(0);    
} 

Quantity ** freeEquation::getQuantities() {
  return freeQty;
}

int freeEquation::getNumberOfQuantities() {
  return numberOfQuantities;
}

int 
freeEquation::findBranch(int posNode, int negNode) {
  if (numberOfQuantities > 1) {
    // We have atleast one quantity on the RHS.
    for (int i=1; i<numberOfQuantities; i++) {
      if (freeQty[i]->getType() == THROUGH) {
        if( (freeQty[i]->getPosNode() == posNode) &&
            (freeQty[i]->getNegNode() == negNode) ) {
          // This is the required quantity.
          return(freeQty[i]->getIndex());
        }
      }
    }
  }
  return(0);
}

list<int *> *
freeEquation :: formNodeSet() {
  list <int *> *newNodeSet = new list<int *>;
  return newNodeSet;
}

int 
freeEquation::findFreeQuantity(int toCheck) {
  if (branch == toCheck) {
    return branch;
  }
  else {
    return(0);
  }
}

component*
freeEquation :: getImplicitEquationList(){
  return implicitEquationList;
}

void
freeEquation::add(component* newcomp) {
  newcomp->setNext(implicitEquationList);
  implicitEquationList = newcomp;
}

int
freeEquation::loadImplicitEqns(){
  int error;
  component *indext;
  indext = implicitEquationList;
  while (indext != NULL){
    error = indext->load();
    if(error){
      return error;
    }
    indext = indext->getNext();
  }
  return(0);
}

int
freeEquation::loadImplicitEqns(Quantity *qty){
  int error;
  component* indext = implicitEquationList;
  int index = 0;
  while (indext != NULL){
    error = indext->load(qty, index);
    if(error){
      return error;
    }
    indext = indext->getNext();
  }
  return(0);
}

void 
freeEquation::initializeImplicitEqns(){
  component* indext = implicitEquationList;
  while (indext != NULL){
    indext->init();
    indext = indext->getNext();
  } 
}

int
freeEquation::pointerAllocationForImplicitEqns(){
  int error;
  component* indext;
  indext = implicitEquationList;   
  while (indext != NULL){
    if (ckt->getAnalysis() == 1){
      error = indext->pointerAllocation();
      if(error){
        return error;
      }
    }
    indext=indext->getNext();
  }
  return(0);
}

