#ifndef TERMINAL_CC
#define TERMINAL_CC

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

#include "Terminal.hh"
#include "ContributionNode.hh"
#include <list>

using std::list;

extern int globalAMSId;
extern list<contributionNode *> globalContributionList;

Terminal::Terminal() {
   terminalName = NULL;
   terminalId = 0;
}

Terminal::Terminal(char *name) {
  terminalName = name;
  if (strcmp(terminalName,"ground") == 0) {
    terminalId = 1;
    cout<<"assigning terminal id of ground terminal as one."<<endl;
  }
  else {
    terminalId = globalAMSId++;
  }
  value.val = terminalId;
  contributionNode *cNode = new contributionNode();
  cNode->setTerminal(*this);
  globalContributionList.push_back(cNode);
}

Terminal::Terminal(char *name,  int newId) {
  terminalName = name;
  terminalId = newId;
  used = 0;
}

Terminal::~Terminal() {}

ObjectBase::ObjectType 
Terminal::getKind() const {
  return TERMINAL;
}

int 
Terminal::getTerminalId() {
  return terminalId;
}

void 
Terminal::print(ostream& os) const {
    os << "Terminal Name : " << terminalName << " Terminal Id : " << terminalId << endl;
}

ObjectBase* 
Terminal::clone() const {
  Terminal *retval = new Terminal();
  *retval = *this;
  return ((ObjectBase *) retval);
}

void
Terminal::updateVal(const VHDLData& val) {}

const VHDLData&
Terminal::readVal() const {
  return value;
}

Terminal& Terminal::operator=(const Terminal& term) {
  terminalName = term.terminalName;
  used = term.used;   
  terminalId = term.terminalId;
  return *this;
}

#endif
