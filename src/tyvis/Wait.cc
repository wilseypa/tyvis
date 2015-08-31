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

#include "Wait.hh"
#include "SignalBase.hh"
#include "PhysicalLiteral.hh"

bool
defaultWaitCond(VHDLKernel*) {
  return true;
}

PhysicalLiteral
Wait::getDefaultWaitTimeOut( VHDLKernel * ) {
  static PhysicalLiteral defaultTimeOut( std_standard_timeTypeInfo(),
					 VHDLVTime::getVHDLVTimePositiveInfinity().getMajor() );
  return defaultTimeOut;					 
}

Wait&
Wait::operator=(const Wait &rhs) {
  sensList = rhs.sensList;
  cond     = rhs.cond;
  timeout  = rhs.timeout;

  return *this;
}

bool
Wait::sensitiveTo(int sigid) {
  bool retval = false;
  for(unsigned int i = 0; i < getSensSize(); i++ ){
    if( isSensitive(sigid,sensList[i]) ){
      retval = true;
      break;
    }
  }
  return retval;
}

bool
Wait::isSensitive(int sigid, SignalBase *signal ){
  return (signal->matchId(sigid));
}

void
Wait::destructSensitivityList(){
  for( vector<SignalBase*>::iterator i = sensList.begin();
       i < sensList.begin();
       i++ ){
    delete (*i);
  }
  sensList.clear();
}

void
Wait::addSensitivityTo( SignalBase *toAdd ){
  sensList.push_back( toAdd );
}

void
Wait::print(ostream& os) const {
  if (getSensSize() > 0) {
    os << " signals: ";
    for( unsigned int i = 0; i < getSensSize(); i++ ){
      os << sensList[i] << endl;
      sensList[i]->print(os);
    }
  }
  else {
    os << "(empty senslist)";
  }
}
