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
#include <clutils/Debug.h>

using clutils::debug;

list<VHDLType *> globalAcrossQuantityList;
list<VHDLType *> globalThroughQuantityList;
list<VHDLType *> globalFreeQuantityList;
list<VHDLType *> globalImplicitQuantityList;
list<contributionNode *> globalContributionList;
int globalAMSId;

component* _savant_entity_ams_elab::basicSet = NULL;

int freeEquationId,oldFreeEquationId;

_savant_entity_ams_elab::_savant_entity_ams_elab() {}

void
_savant_entity_ams_elab::connectTerminals(int, ...) {
  cerr << "connectTerminals() called for an unbounded "
       << "component/entity/configuration" << endl;
  cerr << "Aborting VHDL-AMS elaboration." << endl;
  abort();
}

void
_savant_entity_ams_elab::setTerminalInfo(VHDLType& formal, VHDLType& actual) {
  switch(formal.get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    if ((((Terminal *) formal.getObject()) != NULL) && (((Terminal *) actual.getObject()) != NULL) ) {
      ((Terminal *) formal.getObject())->setTerminalId( ((Terminal *) actual.getObject())->getTerminalId() );
      ((Terminal *) formal.getObject())->setTerminalName( ((Terminal *) actual.getObject())->getTerminalName() );
    }
    break;
  case ARRAY_TYPE: 
    cerr << "Array of terminals currently not implemented" << endl;
    break;
  case RECORD_TYPE: 
    cerr << "Record of terminals currently not implemented" << endl;
    break;
  default:
    cerr << "_savant_entity_ams_elab::setTerminalInfo Wrong Type passed" << endl;
    break;
  }
}

void
_savant_entity_ams_elab::setPlusTerminal(VHDLType *toSet, VHDLType *plusTerminal , char *qName) {
  ASSERT(toSet->getObject() != NULL);
  ASSERT(toSet->getObject()->getKind() == ObjectBase::QUANTITY);
  ASSERT(plusTerminal->getObject() != NULL);
  ASSERT(plusTerminal->getObject()->getKind() == ObjectBase::TERMINAL);
  Quantity *temp = ((Quantity*)toSet->getObject());
  temp->setPosTerminal((Terminal*)plusTerminal->getObject());
  temp->setName(qName);
}

void
_savant_entity_ams_elab::setMinusTerminal(VHDLType *toSet, VHDLType *minusTerminal) {
  ASSERT(toSet->getObject() != NULL);
  ASSERT(toSet->getObject()->getKind() == ObjectBase::QUANTITY);
  ASSERT(minusTerminal->getObject() != NULL);
  ASSERT(minusTerminal->getObject()->getKind() == ObjectBase::TERMINAL);
  Quantity *temp = ((Quantity*)toSet->getObject());
  temp->setNegTerminal((Terminal*)minusTerminal->getObject());
}

void
_savant_entity_ams_elab::assignIndex() {
  // Setting unique Id for through quantities.
  list<VHDLType *>::iterator thruIterator;
  list<contributionNode *>::iterator contribIterator;
  
  for (thruIterator = globalThroughQuantityList.begin();
       thruIterator != globalThroughQuantityList.end();
       thruIterator++) {
    ASSERT( (*thruIterator)->getObject()->getKind() == ObjectBase::QUANTITY);
    ((Quantity*)(*thruIterator)->getObject())->setIndex(globalAMSId++);
    
    // This part of code helps in formations of a list of through
    // quantities associated with each terminal(contribution node). 
    for(contribIterator = globalContributionList.begin();
	contribIterator != globalContributionList.end();
	contribIterator++) {
      if(((((Quantity *)(*thruIterator)->getObject())->getPosNode()) == ((*contribIterator)->getTerminal()).getTerminalId()) ||
	 ((((Quantity *)(*thruIterator)->getObject())->getNegNode()) == ((*contribIterator)->getTerminal()).getTerminalId())) {
	(*contribIterator)->insertQty((*thruIterator));
      }
    }
  }

  // Setting Contribution for through Quantities.
  for (contribIterator = globalContributionList.begin();
       contribIterator != globalContributionList.end();
       contribIterator++) {
    (*contribIterator)->setContribution();
  }
  
  for (contribIterator = globalContributionList.begin();
       contribIterator != globalContributionList.end();
       contribIterator++) {
    debug<<"traversing the list of terminals"<<endl;
    (*contribIterator)->traverse();
  }

  // Making sure globalId is not 0 before assigning Id for free quantities.
  if (globalAMSId == 0) {
   globalAMSId++;
  }
  
  freeEquationId = globalAMSId;
  cout << "FREE EQUATION ID is " << freeEquationId << endl;
  oldFreeEquationId = freeEquationId;

  debug << "Now assigning index to free quantities" << endl;
  list<VHDLType *>::iterator freeIterator;
  for (freeIterator = globalFreeQuantityList.begin();
       freeIterator!=globalFreeQuantityList.end();
       freeIterator++) {
    ((Quantity*)(*freeIterator)->getObject())->setIndex(globalAMSId++);
  }
  
  debug << "Now assigning index to implicit quantities" << endl;
  list<VHDLType *>::iterator implicitIterator;
  for (implicitIterator = globalImplicitQuantityList.begin();
       implicitIterator!=globalImplicitQuantityList.end();
       implicitIterator++) {
    ((Quantity*)(*implicitIterator)->getObject())->setIndex(globalAMSId++);
  }
}

void
_savant_entity_ams_elab::formCharacteristicExpressions() {
  cerr << "Elaboration called for an unbounded "
       << "component/entity/configuration" << endl
       << "Aborting VHDL-AMS elaboration..." << endl;
  abort();
}

void
_savant_entity_ams_elab::add(component *cmp) {
  cmp->setNext(basicSet);
  basicSet = cmp;
}

void
_savant_entity_ams_elab::formStepLimitSpecs() {}

void
_savant_entity_ams_elab::formBreakSet() {}

void
_savant_entity_ams_elab::createAMSProcess() {
  if (basicSet!=NULL) {
    cout<<"Creating Analog Process..."<<endl;
    AMSProcess *contProcess = new AMSProcess("Island_1");
    cout <<"after create cont process"<<endl;
    component *currentEquation;
    list<component *> *tempList = new list<component *>;
    currentEquation = basicSet;
    while (currentEquation != NULL) {
      tempList->push_back(currentEquation);
      currentEquation->setCkt(contProcess);
      currentEquation = currentEquation->getNext();
    }

    list<component *>::iterator eqn_iterator;
    for (eqn_iterator = tempList->begin(); eqn_iterator != tempList->end();eqn_iterator++) {
      contProcess->add((*eqn_iterator));
    }
    delete tempList;
    cout <<"End AMSProcess"<<endl;
  }
}

void
_savant_entity_ams_elab::printBasicSet() {
  component *currentEquation = basicSet;
  debug << "Printing Basic Set :" << endl;
  while (currentEquation != NULL) {
    debug << "Equation pointer : " << currentEquation << " Equation name : " << currentEquation->getName() << endl;
    currentEquation=currentEquation->getNext();
  }
}

int
getFreeEquationId() {
  return freeEquationId++;
}

