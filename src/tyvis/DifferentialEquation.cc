
#ifndef DIFFERENTIAL_EQUATION_CC
#define DIFFERENTIAL_EQUATION_CC

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

#include "DifferentialEquation.hh"

differentialEquation::differentialEquation(component **&parent_equation, const char* name, 
					   VHDLType* implicitQty,VHDLType* quantityTickDot) {
  ASSERT(((Quantity *)implicitQty->getObject())->getType() == IMPLICIT);
  ASSERT(quantityTickDot->getObject()->getKind() == ObjectBase::QUANTITY); 
  component *equation = NULL;
  if (implicitQty->is_scalar_type() == true) {
   equation = new differentialEquation(name, implicitQty, quantityTickDot );
   ASSERT (parent_equation[0] != NULL) ;
   parent_equation[0]->add(equation);
  }
  else {
   cerr<<"ERROR:differentialEquation.cc:Array Quantities not handled yet."<<endl;
   exit(1);
  }
}

differentialEquation::differentialEquation(const char* name, VHDLType* implicitQty,
                                           VHDLType* quantityTickDot) : component(name) {

  ASSERT(implicitQty->getObject()->getKind() == ObjectBase::QUANTITY);
  ASSERT(((Quantity *)implicitQty->getObject())->getType() == IMPLICIT);
  ASSERT(quantityTickDot->getObject()->getKind() == ObjectBase::QUANTITY);
  branchQty = new (Quantity*)[2];
  branchQty[0] = (Quantity*)implicitQty->getObject();
  branchQty[1] = (Quantity*)quantityTickDot->getObject();
  equationType = DIFFERENTIAL_EQN;
  int matrixCount;
  if (branchQty[1]->getType() == ACROSS){
   matrixCount = 3;
  }
  else{
   matrixCount = 2; 
  }
  transientMatrixPointers = new spREAL*[matrixCount];  
  firstTime = 1;
  if (branchQty[1]->getType()==THROUGH) {
   branchQty[1]->setUsed();
   branchQty[1]->setSourceCurrent(true);
  }
}


differentialEquation::~differentialEquation() {
  delete [] transientMatrixPointers;
  delete [] branchQty;
}   

int
differentialEquation::load() {
  double valueInMatrix;
  double valueInRhs;
  int branch;
  int error; 
  long mode = ckt->getMode();   
  double* state0 = ckt->getStateVector(0);
  double* state1 = ckt->getStateVector(1);
  double* rhs    = ckt->getRhs();
  if(mode & MODEINITPRED){
    state0[stateVectorIndex] = state1[stateVectorIndex];   
  }   
  else{
    state0[stateVectorIndex] = branchQty[1]->getValue(ckt);
    if((mode & MODEINITTRAN)) {
      state1[stateVectorIndex] = state0[stateVectorIndex];
    }
  }
  error = ckt->integrate(&valueInMatrix, &valueInRhs, stateVectorIndex);
  if(error){
    return(error);
  }
  if(mode & MODEINITTRAN) {
    state1[stateVectorIndex+1] = state0[stateVectorIndex+1];
  }
  *transientMatrixPointers[0] -= 1.0;
  if (branchQty[1]->getType() == ACROSS){
    *transientMatrixPointers[1] += valueInMatrix;
    *transientMatrixPointers[2] -= valueInMatrix;
   }
  else{
      *transientMatrixPointers[1] += valueInMatrix;
  }
  branch = ckt->addNodeCond(branchQty[0]->getIndex(), FLOW, branchQty[0]->getName());
  *(rhs+branch) += valueInRhs;
  return(0);
}

int
differentialEquation::load(Quantity *qty, int index) {
  int branch,branch1;
  double* state0 = ckt->getStateVector(0);
  double* rhs    = ckt->getRhs();
  int tempNode1, tempNode2;
  spREAL *dcMatrixPointer = NULL;
  char *dcmatrix = ckt->getDcMatrix();
  ASSERT(ckt->getAnalysis() == 0);
  branch1 = ckt->addNodeCond(branchQty[0]->getIndex(), FLOW, branchQty[0]->getName());
  dcMatrixPointer = spGetElement(dcmatrix, branch1, branch1);
  *dcMatrixPointer += 1.0;
  rhs[branch1] += branchQty[0]->getValue(ckt);
  switch(qty->get_quantity_type()) {
  case ACROSS: {
    branch = index;
    tempNode1 = ckt->addNodeCond(qty->getPosNode(), EFFORT,qty->getPosTerminal()->getTerminalName());
    tempNode2 = ckt->addNodeCond(qty->getNegNode(), EFFORT,qty->getNegTerminal()->getTerminalName());
    spREAL *dcMatrixPointer2;
    dcMatrixPointer2 = spGetElement(dcmatrix,branch,branch);
    *dcMatrixPointer2 += 1.0;
    rhs[branch] += branchQty[1]->getValue(ckt);
    break;
  }
  case THROUGH: {
    branch = ckt->addNodeCond(qty->getIndex(), FLOW,qty->getName());
    tempNode1 = ckt->addNodeCond(qty->getPosNode(), FLOW,qty->getPosTerminal()->getTerminalName());
    tempNode2 = ckt->addNodeCond(qty->getNegNode(), FLOW,qty->getNegTerminal()->getTerminalName());
    spREAL *dcMatrixPointer0 = spGetElement(dcmatrix,tempNode1 ,branch);
    spREAL *dcMatrixPointer1 = spGetElement(dcmatrix,tempNode2 ,branch);
    *dcMatrixPointer0 -= 1.0;
    *dcMatrixPointer1 += 1.0;
    rhs[branch] += branchQty[1]->getValue(ckt);
    break;
  }
  default:{
    break;
  }
  }
  state0[stateVectorIndex] = branchQty[1]->getValue(ckt); 
  return(0);
}


void
differentialEquation::init()  {
  int branch;
  branch = ckt->addNodeCond(branchQty[0]->getIndex(), FLOW,branchQty[0]->getName());
  if  (branchQty[1]->getType() == ACROSS) {
    
ckt->addNodeCond(branchQty[1]->getPosNode(),EFFORT,branchQty[1]->getPosTerminal()->getTerminalName());
    
ckt->addNodeCond(branchQty[1]->getNegNode(),EFFORT,branchQty[1]->getNegTerminal()->getTerminalName());
  }
  else{
    ckt->addNodeCond(branchQty[1]->getIndex(), FLOW,branchQty[1]->getName());
  }
  if(firstTime == 1){  
   stateVectorIndex = ckt->getStateLength();
   ckt->incrementState();
   firstTime=2;
  }
}


int
differentialEquation::pointerAllocation() {
  char* transmatrix = ckt->getTransientMatrix();
  int branch1 = ckt->addNodeCond(branchQty[0]->getIndex(),FLOW,branchQty[0]->getName());
  transientMatrixPointers[0] = spGetElement(transmatrix, branch1,branch1);
  if (branchQty[1]->getType() == ACROSS) { 
    int branch2 = ckt->addNodeCond(branchQty[1]->getPosNode(), EFFORT,branchQty[1]->getPosTerminal()->getTerminalName());
    int branch3 = ckt->addNodeCond(branchQty[1]->getNegNode(), EFFORT,branchQty[1]->getNegTerminal()->getTerminalName());
    transientMatrixPointers[1] = spGetElement(transmatrix,branch1,branch2);
    transientMatrixPointers[2] = spGetElement(transmatrix,branch1,branch3);
  }
  else {
    int branch2 = ckt->addNodeCond(branchQty[1]->getIndex(), FLOW,branchQty[1]->getName());
    transientMatrixPointers[1] = spGetElement(transmatrix, branch1, branch2);
  }
  return(0);
}


int
differentialEquation::trunc(double *timeStep) {
  ckt->truncError(stateVectorIndex,timeStep);
  return(0);
}

eqnType
differentialEquation::getEquationType(){
  return equationType;
}


#endif
