#ifndef BRANCH_EQUATION_CC
#define BRANCH_EQUATION_CC

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
#include "BranchEquation.hh"
#include "_savant_entity_ams_elab.hh"

extern list<contributionNode *> globalContributionList;
extern list<VHDLType *> globalThroughQuantityList;
extern int globalAMSId;

using clutils::debug;

eqnType
branchEquation::getEquationType() {
  return equationType;
}

bool 
branchEquation::getConsCheck() {
  return conservativeCheck;
}

branchEquation::~branchEquation() {
  delete [] dcMatrixPointers;
  delete [] transientMatrixPointers;
  delete [] base;
  delete [] branchQty;
}

branchEquation::branchEquation(_savant_entity_ams_elab *designPtr, component **&parent_equation, int caseValue , const char* name,
                               adouble(*functionPtr) (component *, adouble *),int noOfVariables, ...) {
  component *equation = NULL;
  numberOfQuantities = noOfVariables;

  va_list ap;
  va_start(ap, noOfVariables);
  VHDLType **Qty = new VHDLType*[numberOfQuantities];
  for (int i=0; i<numberOfQuantities; i++) {
    VHDLType *currentQty = va_arg(ap, VHDLType *);
    Qty[i] = currentQty;
  }
  
  va_end(ap);
  if (Qty[0]->is_scalar_type() == 1 ) {
    equation = new branchEquation(name,functionPtr,numberOfQuantities,Qty);
    equation->setEnclosingBlock(designPtr);
    if ( caseValue != -1) {
      cerr << "ERROR: Conditional Simultaneous Statements not done yet. " << endl;
      exit(1); 
    }
    else {
      designPtr->add(equation);
    }
    parent_equation = new component*[1];
    parent_equation[0] = equation;
  }
  else {
    cerr << "ERROR : Array quantities not handled yet for simulataneous equations. " << endl;
    exit(1);
  }
}

branchEquation::branchEquation(const char* name, adouble(*functionPtr) (component *, adouble *),
                               int noOfVariables, VHDLType **Qty) : component(name) {
  
  numberOfQuantities = noOfVariables;
  functionPointer = functionPtr;
  branchQty = new (Quantity*)[numberOfQuantities];
  base = new int[numberOfQuantities];
  debug << "Equation name --> " << name << endl;
  debug << "RHSQtys : " << endl;
  for (int i=0; i<numberOfQuantities; i++) {
    VHDLType *currentQty = Qty[i];
    ASSERT(currentQty->getObject()->getKind() == ObjectBase::QUANTITY);
    currentQty->print(debug);
    branchQty[i] = (Quantity *)currentQty->getObject();
  }
  matrixCount = 0;
  equationType = BRANCH_EQN;
  int numberOfCorrectValues = 0;
  noOfThroOrFreeQuantities = 0;
  if (numberOfQuantities == 1) {
    noOfThroOrFreeQuantities++;
  }
  for (int i=0; i<numberOfQuantities; i++) {
    base[i] = matrixCount;
    if (branchQty[i]->getType() == ACROSS) {
      matrixCount += 2;
    }
    else {
      matrixCount += 1;
      if (branchQty[i]->getType()!=IMPLICIT) {
       if ((branchQty[i]->getUsed()==0)||
	   (branchQty[i]->getSourceCurrent()==true) ) {
        noOfThroOrFreeQuantities++;
       }
       branchQty[i]->setUsed();
      }
    }
    if (branchQty[i]->getType() == THROUGH) {
      branchQty[i]->setUsed();
      if ((branchQty[i]->getPosNode() != branchQty[0]->getPosNode()) ||
          (branchQty[i]->getNegNode() != branchQty[0]->getNegNode()) ) {
	numberOfCorrectValues++;
      }
    }
  }
  correctValue = new int[numberOfCorrectValues];
  for (int i=0; i<numberOfCorrectValues; i++) {
    correctValue[i] = 0;
  }
  branch = 0;
  dcMatrixPointers = NULL;
  transientMatrixPointers = NULL;
  implicitEquationList = NULL;
  conservativeCheck = false;
  branchSetFlag = false;
  sourceCurrent = 0;
}

void 
branchEquation::init() {
  component* index = NULL;
  if (getImplicitEquationList() != NULL){
    index = getImplicitEquationList();
    while (index != NULL){
      index->setCkt(this->ckt);
      index = index->getNext();
    }
  }
  for (int i=0;i<numberOfQuantities;i++) {
    if (branchQty[i]->getType() == ACROSS) {
      ckt->addNodeCond(branchQty[i]->getPosNode(), EFFORT,branchQty[i]->getPosTerminal()->getTerminalName());
      ckt->addNodeCond(branchQty[i]->getNegNode(), EFFORT,branchQty[i]->getNegTerminal()->getTerminalName());
    }
    else {
      ckt->addNodeCond(branchQty[i]->getIndex(), FLOW, branchQty[i]->getName());
    }
  }
  if (branch != 0) {
    branch = ckt->addNodeCond(sourceCurrent,FLOW); 
  }
  if (implicitEquationList != NULL){
    initializeImplicitEqns();
  }
}

void 
branchEquation::definePointers () {
  dcMatrixPointers = new spREAL*[matrixCount]; 
  transientMatrixPointers = new spREAL*[matrixCount];
}

Quantity**
branchEquation::getQuantities() {
  return branchQty;
}

int
branchEquation::getNumberOfQuantities() {
  return numberOfQuantities;
}

void
branchEquation::setBranch() {
  branchSetFlag=true;
  int tmp1 =  branchQty[0]->getPosNode();
  int tmp2 =  branchQty[0]->getNegNode();
  for (int i=0;i<numberOfQuantities;i++) {
    if (branchQty[i]->getType() == THROUGH) {
      if ((tmp1 == branchQty[i]->getPosNode()) &&
	  (tmp2 == branchQty[i]->getNegNode())) {
        branch = ckt->addNodeCond(branchQty[i]->getIndex(),FLOW,branchQty[i]->getName());
        if (branch !=0) {
	  sourceCurrent = branchQty[i]->getIndex();
	}
        return;
      }
    }
  }
  branch = ckt->addNodeCond( ckt->newFindBranch(branchQty[0]->getPosNode(),branchQty[0]->getNegNode()), FLOW );
  if (branch!=0) {
    sourceCurrent = ckt->newFindBranch(branchQty[0]->getPosNode(), branchQty[0]->getNegNode()) ;
    return;
  }
  else {
    list<VHDLType *>::iterator thruIterator;
    for (thruIterator = globalThroughQuantityList.begin();
	 thruIterator != globalThroughQuantityList.end();
	 thruIterator++) {     
      if ((((Quantity*)(*thruIterator)->getObject())->getPosNode() == branchQty[0]->getPosNode())&&
          (((Quantity*)(*thruIterator)->getObject())->getNegNode() == branchQty[0]->getNegNode())
          &&( (((Quantity*)(*thruIterator)->getObject())->getUsed() == 0) ||
	      ( (((Quantity*)(*thruIterator)->getObject())->getUsed() == 1)&&
		( ((Quantity*)(*thruIterator)->getObject())->getSourceCurrent() == true) ))) {  
        branch = ckt->addNodeCond(((Quantity*)(*thruIterator)->getObject())->getIndex(), FLOW,
                                  ((Quantity*)(*thruIterator)->getObject())->getName());
        if (branch!=0) {
          ((Quantity*)(*thruIterator)->getObject())->setUsed();
          sourceCurrent = ((Quantity*)(*thruIterator)->getObject())->getIndex();
          ((Quantity*)(*thruIterator)->getObject())->setSourceCurrent(true);
        }
        break;
      }
    }
    
    if (branch!=0) {
      return;
    }
    else {
      branch = ckt->addNodeCond(globalAMSId++, FLOW, "Is");
      AMSType tempTerm1(ObjectBase::TERMINAL ,"tempPos",tmp1),
	tempTerm2(ObjectBase::TERMINAL ,"tempNeg",tmp2);
      AMSType *temp = new AMSType(ObjectBase::QUANTITY,"Is",0,0,THROUGH,&tempTerm1,&tempTerm2);
      branchQty[numberOfQuantities] = (Quantity*)(temp)->getObject();
      ((Quantity*)(temp)->getObject())->setIndex(globalAMSId - 1);
      sourceCurrent = globalAMSId - 1;
      ((Quantity*)(temp)->getObject())->setUsed();
      ((Quantity*)(temp)->getObject())->setSourceCurrent(true);
      list<contributionNode *>::iterator contributionIterator;
      for (contributionIterator =  globalContributionList.begin();
	   contributionIterator !=  globalContributionList.end();
	   contributionIterator++) {
        if (((((Quantity*)(temp)->getObject())->getPosNode())==((*contributionIterator)->getTerminal()).getTerminalId())
	    || ((((Quantity*)(temp)->getObject())->getNegNode())==((*contributionIterator)->getTerminal()).getTerminalId())) {
	  (*contributionIterator)->insertQty(temp);
	  (*contributionIterator)->setContribution();
        }
      }
      return;
    }
  }
}

int 
branchEquation::findBranch(int posNode, int negNode) {
  if (numberOfQuantities > 1) {
    for (int i=1;i< numberOfQuantities ;i++) {
      if (branchQty[i]->getType() == THROUGH) {
        if ((branchQty[i]->getPosNode() != branchQty[0]->getPosNode()) ||
	    (branchQty[i]->getNegNode() != branchQty[0]->getNegNode()) ) {
          if ((branchQty[i]->getPosNode() == posNode) &&
              (branchQty[i]->getNegNode() == negNode) ) {
            //this is the required branch quantity that we are looking for.
            return(branchQty[i]->getIndex());
          }
        }
      }
    }
  }
  return(0);
} 

int 
branchEquation::pointerAllocation() {
  char* dcmatrix = ckt->getDcMatrix();   
  char* transientmatrix = ckt->getTransientMatrix();
  if (implicitEquationList != NULL){
    pointerAllocationForImplicitEqns();
  }
  int Base = 0;
  int correctIndex = 0;
  for (int i=0;i<numberOfQuantities;i++) {
    Base = base[i];
    if (branchQty[i]->getType() == ACROSS) {
      int rhs1= ckt->addNodeCond(branchQty[i]->getPosNode(), EFFORT, branchQty[i]->getPosTerminal()->getTerminalName());
      int rhs2= ckt->addNodeCond(branchQty[i]->getNegNode(), EFFORT, branchQty[i]->getNegTerminal()->getTerminalName());
      if (ckt->getAnalysis() == 0) {
        dcMatrixPointers[Base] = spGetElement(dcmatrix, branch, rhs1);
        dcMatrixPointers[Base+1] = spGetElement(dcmatrix, branch, rhs2);
      }
      transientMatrixPointers[Base]   = spGetElement(transientmatrix, branch, rhs1);
      transientMatrixPointers[Base+1] = spGetElement(transientmatrix, branch, rhs2);
    }
    else {
      if (branchQty[i]->getType() == THROUGH) {
        if ((branchQty[i]->getPosNode() != branchQty[0]->getPosNode()) ||
            (branchQty[i]->getNegNode() != branchQty[0]->getNegNode()) ) {
          if (!correctValue[correctIndex]) {				
            correctValue[correctIndex] = ckt->newCheckCurrent(branchQty[i]->getPosNode(), 
                                                              branchQty[i]->getNegNode(), 
                                                              branchQty[i]->getIndex());
          }              
          if (!correctValue[correctIndex]) {
            cout <<"improper current"<<endl;
            return(-1);
          }
          int tmp = ckt->addNodeCond(branchQty[i]->getIndex(), FLOW,branchQty[i]->getName());
          if (ckt->getAnalysis() == 0) {
            dcMatrixPointers[Base] = spGetElement(dcmatrix, branch, tmp);  
	  }
	  transientMatrixPointers[Base] = spGetElement(transientmatrix, branch, tmp);
        }
        else { 
          if (ckt->getAnalysis() == 0) {
            dcMatrixPointers[Base] = spGetElement(dcmatrix, branch, branch);
	  }
	  transientMatrixPointers[Base] = spGetElement(transientmatrix, branch, branch);
        }
      }
      else {
        int temNode = ckt->addNodeCond(branchQty[i]->getIndex(), FLOW ,branchQty[i]->getName());
        if (ckt->getAnalysis() == 0) {
          dcMatrixPointers[Base] = spGetElement(dcmatrix, branch, temNode);
	}
	transientMatrixPointers[Base] = spGetElement(transientmatrix, branch, temNode);
      }
    }
  }
  return(0);
}

int 
branchEquation::checkCurrent(int posNode,int negNode, int index) {
  if ((branchQty[0]->getPosNode()==posNode)&&(branchQty[0]->getNegNode()==negNode)
      &&(branch=ckt->findNodeRHS(index))) {
    return(1);
  }
  else {
    return(0);
  }
}

int 
branchEquation::load() {
  double* rhs = ckt->getRhs();
  double branchQuantity;
  int noOfQtys = numberOfQuantities;
  double* qtyValues = new double[noOfQtys];
  // Starting ADOL-C
  int tag = 1;
  adouble* indepVariables = new adouble[noOfQtys];
  adouble dependentVariable;
  // Start active section now
  trace_on(1);
  for (int i=0;i<noOfQtys;i++) {
    qtyValues[i] = branchQty[i]->getValue(ckt);
    indepVariables[i] <<= qtyValues[i];
  }
  dependentVariable = functionPointer(this,indepVariables);
  dependentVariable >>= branchQuantity;   
  trace_off();   
  double lhsQty = branchQuantity;
  int Base = 0; 
  if (numberOfQuantities == 0) {
    *(rhs+branch) = lhsQty;
  }
  else {
    double* partialDerivatives = new double[noOfQtys];
    gradient(tag, noOfQtys, qtyValues, partialDerivatives);
    for (int i=0;i<noOfQtys;i++) {
      lhsQty = lhsQty - partialDerivatives[i] * qtyValues[i];
    }     
    *(rhs+branch) -= lhsQty;
    for (int i=0;i<numberOfQuantities;i++) {
      Base = base[i];
      if (ckt->getAnalysis() == 0) {
        // DC mode 
        if (branchQty[i]->getType() == ACROSS) {
          // VCVS
          *dcMatrixPointers[Base]   += partialDerivatives[i];  
          *dcMatrixPointers[Base+1] -= partialDerivatives[i];
        }
        else {
          *dcMatrixPointers[Base]   += partialDerivatives[i];  
        }
      }
      else {
        if (branchQty[i]->getType() == ACROSS) {
	  *transientMatrixPointers[Base]   += partialDerivatives[i];
          *transientMatrixPointers[Base+1] -= partialDerivatives[i];
        }
        else {
          *transientMatrixPointers[Base]   += partialDerivatives[i];
        }
      }

    }
    delete[] partialDerivatives;
  }
  delete[] qtyValues;
  delete[] indepVariables;
  setLoadedFlagInQtys();
  if (implicitEquationList != NULL){
    if( (ckt->getAnalysis()==0)&&(ckt->getCurTime()==0) ){
      loadImplicitEqns(branchQty[0]);
    }
    else{
      loadImplicitEqns();
    }
  }
  return(0);
}


void
branchEquation::add(component* newcomp) {
  newcomp->setNext(implicitEquationList);
  implicitEquationList = newcomp;
}

int
branchEquation::loadImplicitEqns(){
  int error;
  component *indext; 
  indext = implicitEquationList;
  while (indext != NULL){
    error = indext->load();
    if (error){
      return error;
    }
    indext = indext->getNext();
  }
  return(0);
}  
  

int
branchEquation::loadImplicitEqns(Quantity *qty){
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
branchEquation::initializeImplicitEqns(){
  component* indext = implicitEquationList;
  while (indext != NULL){
    indext->init();
    indext = indext->getNext();
  }
}  

int
branchEquation::pointerAllocationForImplicitEqns(){
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

component*
branchEquation :: getImplicitEquationList(){
  return implicitEquationList;
}


#endif

