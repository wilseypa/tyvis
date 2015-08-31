#ifndef COMPONENT_CC
#define COMPONENT_CC

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

#include <iostream>
#include "Component.hh"
#include "AMSKernel_state.hh"
#include "_savant_entity_ams_elab.hh"

component::component(const char* compName) : name(compName) {}

void
component::buildQuantitySet(list<Quantity *> *quantitySet) {
  
  // Add the quantities in this equation...
  Quantity **currentQuantities = getQuantities();
  for (int i=0; i<getNumberOfQuantities(); i++) {
    if (currentQuantities[i]->getInTheSet() == false) {
      if (currentQuantities[i]->getType() == FREE) {
	quantitySet->push_back(currentQuantities[i]);
      }      
      else {
	quantitySet->push_back(currentQuantities[i]);
      }
    }
    currentQuantities[i]->setInTheSet(true);
  }
}

int 
component::checkCurrent(int, int, int) {
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
}

void 
component::definePointers(void) {
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
}  
  
void 
component::setBranch() {
  cerr << "Component's virtual method  called..." << endl
       << "Aborting simulation..." << endl;
  abort();
}

int
component::getNumberOfQuantities() {
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
}

Quantity **  
component::getQuantities() {
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
}

void 
component::add(component *comp){
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl; 
  abort();
}

component* 
component::getImplicitEquationList(){
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
}

int 
component::trunc(double* timeStep){
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
}

int
component::load(Quantity* qty, int i){
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
  return(1);
}

int
component::findBranch(int posNode, int negNode){
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
  return(1);
}

bool 
component::getConsCheck(){
  cerr << "Component's virtual method called..." << endl
       << "Aborting simulation..." << endl;
  abort();
  return(false);
}


void
component::addToCurrentEquations() {
  ASSERT(ckt != NULL);
  list<component *> *currentCktEquations = ckt->getCurrentEquations();
  currentCktEquations->push_back(this);
}

void
component::setLoadedFlagInQtys() {
  Quantity **currentQuantities = getQuantities();
  for (int i=0; i<getNumberOfQuantities(); i++) {
    currentQuantities[i]->setLoadedFlag(1);
  }
}

_savant_entity_elab *
component::getEnclosingBlock() {
  return enclosingBlock;
}
    
void
component::setEnclosingBlock(_savant_entity_elab *toSet) {
  enclosingBlock = toSet; 
}

void
component::updateQuantityValues(){
  Quantity **quantitiesToUpdate = getQuantities();
  for(int i=0; i< getNumberOfQuantities(); i++) {
    quantitiesToUpdate[i]->getValue(ckt);
  }
}
#endif
