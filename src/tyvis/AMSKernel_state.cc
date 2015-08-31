#ifndef AMSKERNEL_STATE_CC 
#define AMSKERNEL_STATE_CC
 
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

#include "AMSKernel_state.hh"
#include <iostream>

using std::cout;

AMSKernel_state::AMSKernel_state() {
  rhs = NULL;
  oldRhs = NULL;
}

AMSKernel_state::~AMSKernel_state() {
  if (rhs) {
    delete[] rhs;
  }
  if (oldRhs) {
    delete[] oldRhs;
  }
}

double*
AMSKernel_state::getRhs() {
  return rhs;
}

double* 
AMSKernel_state::getOldRhs() {
  return oldRhs;
}

void
AMSKernel_state::cleanState() {
  cout << "Cleaning up analog process" << endl;
}

SignalBase*
AMSKernel_state::locateSig(int) {
  cerr << "AMSKernel_state::locateSig is called which has no signals"
       << endl;
  abort();
}

long
AMSKernel_state::getMode(){
  return mode;
}

void
AMSKernel_state::setMode(long newMode){
  mode = newMode;
}

int
AMSKernel_state::getIterMode(){
  return iterMode;
}

void
AMSKernel_state::setIterMode(int newMode){
  iterMode = newMode;
}

void 
AMSKernel_state::setRhs(double* newRhs){
  rhs = newRhs;
}

void
AMSKernel_state::setRhsValue(int position,double value){
  rhs[position]=value;
}

void
AMSKernel_state::setOldRhs(double* newRhs){
  oldRhs = newRhs;
}

void
AMSKernel_state::setOldRhsValue(int position,double value){
  oldRhs[position]=value;
}

int 
AMSKernel_state::getVectorSize(){
  return vectorSize;
}

void 
AMSKernel_state::setVectorSize(int size){
  vectorSize = size;
}


void 
AMSKernel_state::initState(){
  curTime = 0;
  finalTime = 0;
  mode = 0;
  stateLength = 0;
}

discreteTime
AMSKernel_state::getCurTime(){
  return curTime;
}
void
AMSKernel_state::setCurTime(discreteTime value){
  curTime = value;
}

double*
AMSKernel_state::getStateVector(int i){
  return states[i];
} 

int
AMSKernel_state::getStateLength(){
  return stateLength;
}

void
AMSKernel_state::setStateLength(int value){
  stateLength = value;
}

int
AMSKernel_state::getIntegOrder(){
  return integOrder;
}

void
AMSKernel_state::setIntegOrder(int value){
  integOrder = value;
}

double
AMSKernel_state::getDelta(){
  return delta;
}

void
AMSKernel_state::setDelta(double value){
  delta = value;
}

double
AMSKernel_state::getOldDelta(int i){
  return oldDelta[i];
}

void
AMSKernel_state::setOldDelta(int i, double value){
  oldDelta[i] = value;
}

double
AMSKernel_state::getIntegCoeff(int i){
  return integCoeff[i];
}

void
AMSKernel_state::setIntegCoeff(int i, double value){
  integCoeff[i] = value;
}

double*
AMSKernel_state::getStates(int i){
  return states[i];
}

void
AMSKernel_state::setStates(int i, double* value){
  states[i] = value;
}

void
AMSKernel_state::setStates(int i, int j, double value){
  states[i][j] = value;
}


discreteTime
AMSKernel_state::getFinalTime(){
  return finalTime;
}

void
AMSKernel_state::setFinalTime(discreteTime value){
  finalTime = value;
}

unsigned int AMSKernel_state::getStateSize(){
  return sizeof(AMSKernel_state);
}

#endif
