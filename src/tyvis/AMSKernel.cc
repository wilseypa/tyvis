#ifndef AMSKERNEL_CC
#define AMSKERNEL_CC

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

#include "_savant_entity_ams_elab.hh"
#include "AMSKernel.hh"
#include "vhdl_ams.hh"
#include "Constants.hh"
#include "Component.hh"
#include <iomanip>
#include <clutils/Debug.h>

using clutils::debug;

extern list<contributionNode *> globalContributionList;
extern list<VHDLType *> globalThroughQuantityList;
int convergedCount;
int iterationCount;

AMSKernel::AMSKernel()
: VHDLKernel("Analog")  {
  int perror;
  rhsNodes = new nodeList;
  dcMatrix = spCreate(0,0,&perror);
  transientMatrix = spCreate(0,0,&perror);
  analysis = DC;
  currentEquations = NULL;
  freeLoadEqnCounter = 0;
  comp = NULL;
  pivotAbsTol = 1e-13;
  pivotRelTol = 1e-3;
  setOfQuantities = new list<Quantity *>;
  maxStep  = 0.0;
  startTime = 0;
  delMin = 1e-15;  
  maxIntegOrder = 2;
  voltTol = 1e-6;
  absTol = 1e-12;  
  relTol = 1e-3;
  chgTol = 1e-14;  
  trTol = 7;
}

AMSKernel::~AMSKernel() {
}

component* 
AMSKernel::getComp() const {
 return comp;
}

inline nodeList* 
AMSKernel::getRhsNodes() const { 
  return rhsNodes;
}

double* 
AMSKernel::getOldRhs(){
  return getSolverState()->getOldRhs();
}

list<component *> * 
AMSKernel::getCurrentEquations() { 
  return currentEquations;
}

int 
AMSKernel::getMatrixSize() const { 
  return rhsNodes->getSize();
}

char* 
AMSKernel::getDcMatrix(){
  return dcMatrix;
}

char*
AMSKernel::getTransientMatrix(){
  return transientMatrix;
}

int 
AMSKernel::getFreeLoadEqnCounter(){
return freeLoadEqnCounter;
}

discreteTime
AMSKernel::getCurTime(){
  return getSolverState()->getCurTime();
}

int
AMSKernel::getStateLength(void){
  return getSolverState()->getStateLength();
}

void 
AMSKernel::incrementFreeLoadEqnCounter(){
freeLoadEqnCounter++;
}

long
AMSKernel::getMode(){
  return getSolverState()->getMode();
}

double*
AMSKernel::getStateVector(int i){
  return getSolverState()->getStateVector(i);
}

void
AMSKernel::incrementState(){
  getSolverState()->setStateLength(getSolverState()->getStateLength()+2);
}

inline int 
AMSKernel::addNode(int nodeId, nodeConstants type){
  return rhsNodes->addNode(nodeId,type);
}

int 
AMSKernel::addNodeCond(int nodeId, nodeConstants type) {
  return rhsNodes->addNodeCond(nodeId,type);
}

int
AMSKernel::addNodeCond(int nodeId, nodeConstants type, char* nam) {
  return rhsNodes->addNodeCond(nodeId,type,nam);
}

void 
AMSKernel::initialSettingUp() {
  int converged = 0;
  int size = 0;
  int firsttime;
  cout << "->ENTERED INITIAL DC SETTING UP" << endl;
  setup();
  debug << "Finished setup()" << endl;
  completeDynamicState();
  checkSolvability();  
  formCurrentEquations();
  rhsNodes->display();
  maxStep = 3.333333333e-10;
  definePointersAcross();
  newSetBranch();
  // This is the order of the matrix.
  size = rhsNodes->getSize();
  getSolverState()->setRhs(new double[size+1]);
  getSolverState()->setOldRhs(new double[size+1]);       
  for(int i=0; i<size+1; i++) { 
    getSolverState()->setRhsValue(i,0.0);
    getSolverState()->setOldRhsValue(i,0.0);
  }
  dcMaxIter = 300;
  transientMaxIter = 100;
  firsttime = 1;
  getSolverState()->setIterMode(NIUNINITIALIZED); 
  converged = findDCOp( ( getSolverState()->getMode() & MODEUIC)
			| MODETRANOP | MODEINITJCT , dcMaxIter);
  analysis = TRANSIENT;
  if (converged!=0){
    cerr << "Initial Find DC operating point failed" << endl;
  }
  getSolverState()->setIntegOrder(1);
  for (int i=0; i<7; i++){
    getSolverState()->setOldDelta(i,maxStep);
  }
  getSolverState()->setDelta(maxStep/10);
  getSolverState()->setMode((getSolverState()->getMode() & MODEUIC) | MODETRAN | MODEINITTRAN);
  getSolverState()->setIterMode(NISHOULDREORDER | NIACSHOULDREORDER | NIPZSHOULDREORDER);
  setupFlag = true;
  copyStates(0,1);
  getSolverState()->setIntegCoeff(0,0);
  getSolverState()->setIntegCoeff(1,0);
}

void 
AMSKernel::definePointersAcross(void) {
  component* index=getComp();
  while (index!=NULL) {
    if (index->getConsCheck() == false && index->getEquationType() != FREE_EQN) {
      index->definePointers();
    }
    index=index->getNext();
  }
}

void 
AMSKernel::newSetBranch() {
  list<component *>::iterator eqn_iterator;   
  for (eqn_iterator = currentEquations->begin(); eqn_iterator != currentEquations->end();eqn_iterator++) {
    if (((*eqn_iterator)->getEquationType() == BRANCH_EQN)&&(!((*eqn_iterator)->getBranchSetFlag()))) {
      (*eqn_iterator)->setBranch();   
    }
  }
}

int 
AMSKernel::newFindBranch(int posNode, int negNode) {
  int addNode = 0;
  list<component *>::iterator eqn_iterator; 
  for (eqn_iterator = currentEquations->begin(); eqn_iterator != currentEquations->end();eqn_iterator++) {
    addNode = (*eqn_iterator)->findBranch(posNode,negNode);
  }
  return(addNode);
}

list <Quantity *>* 
AMSKernel::getSetOfQuantities() {
  return setOfQuantities;
}

int AMSKernel::findNodeRHS(int name) {
  return rhsNodes->find(name);
}

void 
AMSKernel::checkSolvability() {
  int count=0,temp=0;
  int solFlag = 0;
  component *index = getComp();
  while (index!=NULL) {
    count+=index->getThroFreeNumber();
    temp++;
    index = index->getNext();
  }
  if (temp<count) {
    solFlag = 1; 
  }
  if (solFlag == 0) {
    cout << "SYSTEM IS SOLVABLE" << endl;
    cout << "..Continuing with simulation" << endl;
  }
  else{
    cout << " SYSTEM IS NOT SOLVABLE" << endl;
    abort();
  }
  cout<<endl;
}

double* 
AMSKernel::getRhs() {
  return getSolverState()->getRhs();
}

void
AMSKernel::formCurrentEquations() {
  delete currentEquations;
  currentEquations = new list<component *>;
  component *index = getComp();
  while(index != NULL) {   
    index->addToCurrentEquations();
    index = index->getNext();
  } 
}

void 
AMSKernel::completeDynamicState() {
  getSolverState()->setVectorSize(getMatrixSize() + 1);
  getSolverState()->setRhs(new double[getSolverState()->getVectorSize()]);
  getSolverState()->setOldRhs(new double[getSolverState()->getVectorSize()]);
  createStates();
}

void
AMSKernel :: createStates(){
  if ( getSolverState()->getStateLength() > 0){
    for(int i=0; i<4; i++){
      getSolverState()->setStates(i,new double[getSolverState()->getStateLength()]);
      for(int j=0; j<getSolverState()->getStateLength(); j++){
        getSolverState()->setStates(i,j,0.0);
      }
    }
  }
}


int 
AMSKernel::findDCOp(long firstMode, int iterLimit) {
  analysis = DC;
  int converged = 0;
  getSolverState()->setMode(firstMode);
  converged = iterate(iterLimit, dcMatrix);
  return (converged);
}

int AMSKernel::iterate(int maxIter, char *matrix){
  int iterno,error;
  int ipass;
  iterno = 0;
  ipass = 0;
  double *temp;
  // Condition to check and make sure that itermode is initialized during
  // simulation.
  if (getSolverState()->getIterMode() & NIUNINITIALIZED) {
    getSolverState()->setIterMode(NISHOULDREORDER | NIACSHOULDREORDER | NIPZSHOULDREORDER);
  }
  for (;;) {
    noConv = 0;
    // During first iteration do this
    if (iterno == 0) {
      formCurrentEquations();
      newSetBranch();
    }
    error = matrixLoad(matrix);
    spPrint(matrix,0,1,1);
    iterno++;
    iterationCount++;
    if (error) {
      cout << "Error in Circuit load occured" << endl;
      return(error);
    }
    // Checking whether preorder has been done.If preorder is not done do
    // the preordering of the matrix.
    if (!( getSolverState()->getIterMode() & NIDIDPREORDER)) {
      cout << "PERFORMING PREORDER OF MATRIX" << endl;
      spMNA_Preorder(matrix);
      error = spError(matrix);
      if(error){
        cout << "Error occured in preorder" << endl;
        return(error);
      }
      // After preordering set the preorder flag.
      getSolverState()->setIterMode(getSolverState()->getIterMode() | NIDIDPREORDER);
    }
    if (( getSolverState()->getMode() & MODEINITJCT) || ((getSolverState()->getMode() & MODEINITTRAN) &&(iterno == 1))) {
      getSolverState()->setIterMode(getSolverState()->getIterMode() | NISHOULDREORDER);
    }
    if (getSolverState()->getIterMode() & NISHOULDREORDER) {
      spSetReal(matrix);
      error = spOrderAndFactor(matrix, NULL, pivotRelTol, pivotAbsTol, 1);
      if (error) {
        int i,j;
        cout << "Reordering produced error" << endl;
        spWhereSingular(matrix, &i, &j);
        cout << "The problem is that A is singular" << endl;
        cout << "Check the Nodes " << i << " and " << j << endl;
        return(error);
      }
      getSolverState()->setIterMode(getSolverState()->getIterMode() & ~NISHOULDREORDER);
    }
    else {
      spSetReal(matrix);
      error = spFactor(matrix);
      if (error) {
        // Error occured during LU-Factorization.
        if (error == E_SINGULAR) {
          cout << "Reorder flag is set for the matrix" << endl;
          getSolverState()->setIterMode(getSolverState()->getIterMode() | NISHOULDREORDER );
          continue;
        }
        cout << "LU-Factorization returned error" << endl;
        return(error);
      }
    }
    spSolve(matrix,getSolverState()->getRhs(),getSolverState()->getRhs());
    getSolverState()->setRhsValue(0,0.0);
    getSolverState()->setOldRhsValue(0,0.0);
    // If number of iterations is greater than maximum allowed
    if (iterno > maxIter) {
      cout << "->TOO MANY ITERATIONS WITHOUT CONVERGENCE" << endl;
      return(E_ITERLIM);
    }
    if (noConv==0 && iterno!=1) {
      noConv = convergenceTest();
    }
    else {
      //Convergence has not been achieved.
      noConv = 1;
    }
    if (getSolverState()->getMode() & MODEINITFLOAT) {
      if (getSolverState()->getMode() & MODEDC) {
        if (ipass) {
          noConv = ipass;}
        ipass = 0;
      }
      if (noConv == 0) {
        // Converged
	printFunction();
        return(0);
      }
    }
    else if( getSolverState()->getMode() & MODEINITJCT) {
	getSolverState()->setMode((getSolverState()->getMode() & (~INITF)) | MODEINITFIX);
	getSolverState()->setIterMode(getSolverState()->getIterMode() | NISHOULDREORDER );
    }
    else if (getSolverState()->getMode() & MODEINITFIX) {
	if (noConv == 0) {
	  getSolverState()->setMode((getSolverState()->getMode() & (~INITF)) | MODEINITFLOAT);
	}
	ipass =1 ;
    }
    else if (getSolverState()->getMode() & MODEINITSMSIG) {
	  getSolverState()->setMode(( getSolverState()->getMode() & (~INITF)) | MODEINITFLOAT);
    }
    else if (getSolverState()->getMode() & MODEINITTRAN) {
	if (iterno <=1 ) {
	  getSolverState()->setIterMode(getSolverState()->getIterMode() | NISHOULDREORDER );
	}
	// Reset mode and set INITFLOAT.
	getSolverState()->setMode(( getSolverState()->getMode() & (~INITF)) | MODEINITFLOAT);
    }
    else if (getSolverState()->getMode() & MODEINITPRED) {
	getSolverState()->setMode(( getSolverState()->getMode() & (~INITF)) | MODEINITFLOAT);
    }
    else {
      cout << "Bad INITF state" << endl;
      return(E_INTERN);
    }
    temp = getSolverState()->getOldRhs();
    getSolverState()->setOldRhs(getSolverState()->getRhs());
    getSolverState()->setRhs(temp);
  }
}

int 
AMSKernel::matrixLoad(char *matrix) {
  list <Quantity *> *islandQuantities = getSetOfQuantities();
  if (islandQuantities ==NULL) {
    cout << "FATAL ERROR in matrix load. No island quantities." << endl;
    abort();
  }
  int error;
  error = newPointerAllocation();
  if (error) {
    cout << "Pointer allocation returned error" << endl;
    return(error);
  }
  // We set loaded flag of all quantities in the Island to false set it to
  // true after all equations are loaded. 
  list<Quantity *>::iterator currentQuantity;
  for (currentQuantity = islandQuantities->begin(); currentQuantity != islandQuantities->end();currentQuantity++) {
    (*currentQuantity)->setLoadedFlag(0);
  }
  error = newLoad(matrix);
  if (error) {
    cout << "Load error." << endl;
    return(error);
  }
  else{
    return(0);
  }
}

int 
AMSKernel::newPointerAllocation() {
  int error;
  list<component *>::iterator eqn_iterator;
  for (eqn_iterator = currentEquations->begin(); eqn_iterator != currentEquations->end();eqn_iterator++) {
    if ((*eqn_iterator)->getConsCheck() == false) {
      error=(*eqn_iterator)->pointerAllocation();
      if(error) {
	return(error);
      }
    }
  }
  return(0);
}

int 
AMSKernel::newCheckCurrent(int posNode, int negNode, int indexVal) {
  int val=0;
  list<component *>::iterator eqn_iterator;
  for (eqn_iterator = currentEquations->begin(); eqn_iterator  != currentEquations->end();eqn_iterator++) {
    val= (*eqn_iterator)->checkCurrent(posNode, negNode, indexVal);
    if(val==-1){
      return(0);
    }
  }
  return(val);
}

int 
AMSKernel::newLoad(char *matrix) {
  char* dcmatrix = getDcMatrix();
  char* transientmatrix = getTransientMatrix();
  list<component *>::iterator eqn_iterator;
  int size = rhsNodes->getSize();
  int error;
  spREAL *tempDC,*tempTrans;    
  for (int i=0; i<=size; i++) {
    getSolverState()->setRhsValue(i,0);
  }
  freeLoadEqnCounter=1;
  spClear( (char*)matrix);
  for (eqn_iterator = currentEquations->begin(); eqn_iterator != currentEquations->end();eqn_iterator++) {
    if ((*eqn_iterator)->getConsCheck()==false) {
      error=(*eqn_iterator)->load();
      if (error) {
	return(error);
      }
    }
  }
  cout<<"->Traversing terminal list now to load conservative eqns"<<endl;
  list<contributionNode *>::iterator contributionIterator;
  for (contributionIterator = globalContributionList.begin();
       contributionIterator != globalContributionList.end();
       contributionIterator++) {
    if (getAnalysis() == 0) {
      quantityNode *tempnode1 = (*contributionIterator)->getNode(); 
      if (tempnode1 == NULL) {  
        // No quantities for terminal. Do nothing
      }
      else {
        while (tempnode1 !=NULL) {
          if (((Quantity*)(tempnode1->getQuantity())->getObject())->getUsed() == 1) {
            tempDC=spGetElement(dcmatrix,
                                addNodeCond(((*contributionIterator)->getTerminal()).getTerminalId(),EFFORT) ,
                                addNodeCond(((Quantity*)(tempnode1->getQuantity())->getObject())->getIndex(),FLOW));
            *tempDC += tempnode1->getContribution();
          }
          tempnode1 = tempnode1->getNext();
        }
      }
    }

    quantityNode *tempnode1 = (*contributionIterator)->getNode();  
    if (tempnode1 == NULL) {  
      // No quantities for terminal. Do nothing
    }  
    else {  
      while (tempnode1 !=NULL) {     
        if ( ((Quantity*)(tempnode1->getQuantity())->getObject())->getUsed() == 1) {
          tempTrans=spGetElement(transientmatrix,
                                addNodeCond(((*contributionIterator)->getTerminal()).getTerminalId(),EFFORT) ,
                                addNodeCond(((Quantity*)(tempnode1->getQuantity())->getObject())->getIndex(),FLOW));
          *tempTrans += tempnode1->getContribution();
        }
        tempnode1 = tempnode1->getNext();
      }
    }

  } // End of traversing STL list.
  return(0);
}

void
AMSKernel::add(component* newcomp) {
  newcomp->setNext(comp);
  comp = newcomp;
}

int
AMSKernel::simulate(discreteTime endTime){
  int firsttime;
  int error;
  int converged;
  double newval, localOldDelta;
  startTime = getSolverState()->getFinalTime();
  getSolverState()->setFinalTime(endTime);
  firsttime = 0;
if(setupFlag == true){
    setupFlag  = false;
nextTime:
   if(getSolverState()->getFinalTime() <= getSolverState()->getCurTime() ){
     getSolverState()->setFinalTime(getSolverState()->getCurTime());
     return(0);
   }
  }
  for(int i=5; i>=0; i--){
    getSolverState()->setOldDelta(i+1,getSolverState()->getOldDelta(i));
  }
  getSolverState()->setOldDelta(0,getSolverState()->getDelta());
  rotateStates();
  if(maxStep < getSolverState()->getDelta()){
    getSolverState()->setDelta(maxStep);
  }
  //INFINITE LOOP
  for(;;){
    localOldDelta = getSolverState()->getDelta();
    // getSolverState()->setOldCurTime(getSolverState()->getCurTime());
    //Increment current time point by delta
    getSolverState()->setCurTime(getSolverState()->getCurTime() + (discreteTime)((getSolverState()->getDelta())*1e15));
    getSolverState()->setOldDelta(0,getSolverState()->getDelta());
    compCoeff();
    converged = iterate(transientMaxIter, transientMatrix);
    updateQuantityValues();
    getSolverState()->setMode(( getSolverState()->getMode() & MODEUIC)| MODETRAN | MODEINITPRED);
    if (firsttime){
      copyStates(1,2);
      copyStates(1,3);
    }
    if (converged!=0){
      getSolverState()->setCurTime(getSolverState()->getCurTime() -(discreteTime)((getSolverState()->getDelta())*1e15));
      getSolverState()->setDelta(getSolverState()->getDelta() / 8);
      if (firsttime){
        //If it was during first time, mode must be set back.
        getSolverState()->setMode (( getSolverState()->getMode() )| MODETRAN | MODEINITTRAN );
      }
      getSolverState()->setIntegOrder(1);
    }
    else {
      //Convergence has been achieved.
      convergedCount++;
      if (firsttime){
        firsttime = 0;
      }
      newval = getSolverState()->getDelta();
      error = trunc(&newval);
      if (error){
        cout<<"Truncation error occured in 1"<<endl;
      }
      if (newval > 0.9 * getSolverState()->getDelta()){
        if ( getSolverState()->getIntegOrder() == 1){
          newval = getSolverState()->getDelta();
          getSolverState()->setIntegOrder(2);
          error = trunc(&newval);
          if (error){
            cout<<"Truncation returned error in 2 "<<endl;
            return(error);
          }
          if ( newval<= 1.05* getSolverState()->getDelta() ){
            getSolverState()->setIntegOrder(1);
          }
        }
        getSolverState()->setDelta(newval);
        goto nextTime;
      }
      else {
        getSolverState()->setCurTime(getSolverState()->getCurTime() - (discreteTime)(getSolverState()->getDelta() * 1e15));
        getSolverState()->setDelta(newval);
      }
    }
    if((getSolverState()->getDelta()) <= delMin){
      cout <<"->DELTA IS TOO SMALL"<<endl;
      if (localOldDelta > delMin){
        getSolverState()->setDelta(delMin);
      }
      else{
          cout<<"->Delta Min = "<<delMin<<endl;
          cerr<<"->Time Step is too small, so aborting Simulation\n";
          abort();
      }
    }
  }//end of infinite for loop.  
}//end of simulate routine.


void
AMSKernel::updateQuantityValues(){
  list<component *> *currentEquations = getCurrentEquations();
  list<component *>::iterator eqn_iterator;
  for(eqn_iterator = currentEquations->begin(); eqn_iterator != currentEquations->end();eqn_iterator++){
    (*eqn_iterator)->updateQuantityValues();
  }
}
 
int
AMSKernel::trunc(double *timeStep){
  component* index = getComp();
  component* temp;
  double timetemp = 3.40282e+38;
  int error;
  while (index!=NULL){
    component* index1 = index->getImplicitEquationList();
    while (index1 ){
      temp = index1->getNext();
      error = index1->trunc(&timetemp);
      if (error){
        return(error);
      }
      index1 = temp;
    }
    index = index->getNext();
  }
  if ((2 * *timeStep) < timetemp){
    *timeStep = 2 * *timeStep;
  }
  else{
    *timeStep = timetemp;
  }
  return(0);
}

void   
AMSKernel::truncError(int qcap, double* timeStep) {
  double volttol;
  double chargetol;
  double tol;
  double del;
  double diff[8];
  double deltmp[8];
  double factor;
  int i;
  int j;
  double trapCoeff[] = {
    .5,
    .08333333333
  };
  int ccap = qcap+1;
  double temp1 = fabs(*(getSolverState()->getStates(0)+ccap));
  double temp2 = fabs(*(getSolverState()->getStates(1)+ccap));
  if( temp1 > temp2){
    volttol = absTol + relTol *temp1;
  }
  else{
    volttol = absTol + relTol *temp2;
  }
  temp1 = fabs(*(getSolverState()->getStates(0)+qcap));
  temp2 = fabs(*(getSolverState()->getStates(1)+qcap)); 
  if( temp1 > temp2){
    chargetol = temp1;
  }
  else{
    chargetol = temp2;
  }
  if(chargetol > chgTol){
    chargetol = relTol * chargetol/getSolverState()->getDelta();
  }
  else{
    chargetol = relTol * chgTol/getSolverState()->getDelta();
  }
  if(volttol > chargetol){
    tol = volttol;
  }
  else{
    tol = chargetol;
  }
  for(i=getSolverState()->getIntegOrder()+1; i>=0 ;i--){
    diff[i] = *(getSolverState()->getStates(i)) + qcap;
  }

  for(i=0 ; i <= getSolverState()->getIntegOrder() ; i++){
    deltmp[i] = getSolverState()->getOldDelta(i);
  }
  j = getSolverState()->getIntegOrder();
  while(1){
    for(i=0; i <= j; i++){
      diff[i] = (diff[i] - diff[i+1])/deltmp[i];
    }
    if(--j < 0){ 
      break;
    }
    for(i=0; i <= j; i++){
      deltmp[i] = deltmp[i+1] + getSolverState()->getOldDelta(i);
    }
  }
  factor = trapCoeff[getSolverState()->getIntegOrder() - 1] ;
  if(absTol > factor * fabs(diff[0])){
    del = trTol * tol/absTol;
  }
  else {
    del = trTol * tol/( factor * fabs(diff[0]));
  }
  if(getSolverState()->getIntegOrder() == 2){
    del = sqrt(del);
  }
  else if (getSolverState()->getIntegOrder() > 2){   
    del = exp(log(del)/getSolverState()->getIntegOrder());
  }  
  if( (*timeStep) > del){
    *timeStep = del;
  }
  return;
}

int
AMSKernel::convergenceTest(){
  double oldValue,newValue,tol,relTolerance,absTolerance;
  list <Quantity *>*islandQuantities = getSetOfQuantities();
  list<Quantity *>::iterator currQuantity;
  for(currQuantity = islandQuantities->begin(); currQuantity != islandQuantities->end();currQuantity++){
    relTolerance = 1e-03;
    if ((*currQuantity)->get_quantity_type() == ACROSS || (*currQuantity)->get_quantity_type() == FREE){
       absTolerance = voltTol;
    }
    else{
        absTolerance = 1e-12;
    }
    oldValue = getPrevIterValueOfQuantity((*currQuantity));
    newValue = getNewIterValueOfQuantity((*currQuantity));
    if(fabs(oldValue) > fabs(newValue)){
      tol = relTolerance * fabs(oldValue)+ absTolerance;
    }
    else{
      tol = relTolerance * fabs(newValue)+ absTolerance;
    }
    if (fabs(newValue - oldValue) > tol){
      return(1);
    }
  }
  return(0);
}

double
AMSKernel::getPrevIterValueOfQuantity(Quantity* quantityPtr){
  double retval = 0.0;
  double* old_rhs = getSolverState()->getOldRhs();
  if (quantityPtr != NULL){
    if (quantityPtr->get_quantity_type() == ACROSS){
      int pos = addNodeCond(quantityPtr->getPosNode(), EFFORT);
      int neg = addNodeCond(quantityPtr->getNegNode(), EFFORT);
      retval = old_rhs[pos] - old_rhs[neg]; 
    }
    else{  
      //through or free.
      int ftq = addNodeCond(quantityPtr->getIndex(), FLOW);
      retval = old_rhs[ftq];
    }
    return retval;
  }
  else{
    return(0.0);
  }
}

double
AMSKernel::getNewIterValueOfQuantity(Quantity* quantityPtr){
  double retval = 0.0;
  double* new_rhs = getSolverState()->getRhs();
  if (quantityPtr != NULL){
    if (quantityPtr->get_quantity_type() == ACROSS){
      int pos = addNodeCond(quantityPtr->getPosNode(), EFFORT);
      int neg = addNodeCond(quantityPtr->getNegNode(), EFFORT);
      retval = new_rhs[pos] - new_rhs[neg];
    }
    else{
      int ftq = addNodeCond(quantityPtr->getIndex(), FLOW);
      retval = new_rhs[ftq];
    }
    return retval;  
  }
  else{
    return (0.0);
  }
}

void
AMSKernel::rotateStates(){
  double *temp;
  temp = getSolverState()->getStates(maxIntegOrder+1);
  for(int i=maxIntegOrder; i>=0; i--){
    getSolverState()->setStates(i+1,getSolverState()->getStates(i));
  }
  getSolverState()->setStates(0,temp);
}
  
int   
AMSKernel::integrate(double* matrixValue,double* rhsValue, int stateIndex){
  int stateIndex2 = stateIndex+1;
  switch ( getSolverState()->getIntegOrder()){
  case 1 :
    *( getSolverState()->getStates(0) + stateIndex2) =(getSolverState()->getIntegCoeff(0) * (*( getSolverState()->getStates(0) + stateIndex)) ) +
      (getSolverState()->getIntegCoeff(1) * (*( getSolverState()->getStates(1) + stateIndex)));
    break; 
  case 2 :
    *(getSolverState()->getStates(0) + stateIndex2) =
      (-*( getSolverState()->getStates(1) + stateIndex2) * getSolverState()->getIntegCoeff(1)) + (getSolverState()->getIntegCoeff(0) * (*(getSolverState()->getStates(0) + stateIndex) - *( getSolverState()->getStates(1) + stateIndex)));
    break;
  default:
    cerr<<"Illegal Integ Order"<<endl;
  }   
  *(rhsValue) =( getSolverState()->getIntegCoeff(0) * (*(getSolverState()->getStates(0) + stateIndex)) - *(getSolverState()->getStates(0) + stateIndex2) );
  *matrixValue = getSolverState()->getIntegCoeff(0);
  return(0);
}


int
AMSKernel::compCoeff(){
  switch ( getSolverState()->getIntegOrder()){
  case 1 :
    getSolverState()->setIntegCoeff(0,(1.0/getSolverState()->getDelta()));
    getSolverState()->setIntegCoeff(1,(-1.0/getSolverState()->getDelta()));
    break;
  case 2 :
    getSolverState()->setIntegCoeff(0,(2.0/getSolverState()->getDelta()));
    getSolverState()->setIntegCoeff(1,1.0);
    break;
  default:
    return(E_ORDER);
  }
  return(0);
}


void
AMSKernel:: copyStates(int from, int to){
  memcpy( getSolverState()->getStates(to),getSolverState()->getStates(from),getSolverState()->getStateLength() );
}



#endif
