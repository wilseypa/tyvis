#ifndef WAIT_HH
#define WAIT_HH

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
#include "tyvis/vhdl.hh"
#include "tyvis/std_standardPkg.hh"
#include <vector>
using std::vector;

extern bool defaultWaitCond( VHDLKernel * );
extern PhysicalLiteral defaultWaitTimeout( VHDLKernel * );

typedef bool (*waitConditionPtr)( VHDLKernel * );
typedef PhysicalLiteral (*waitTimeoutPtr)( VHDLKernel * );

class Wait {
public:  
  unsigned int getSensSize() const { return sensList.size(); }

  Wait() :
    cond( defaultWaitCond ),
    timeout( getDefaultWaitTimeOut ){}

  void destructSensitivityList();
  void addSensitivityTo( SignalBase * );

  Wait& operator=(const Wait &);
  
  bool sensitiveTo(int sigid);
  void print(ostream& = cout) const;

  static PhysicalLiteral getDefaultWaitTimeOut( VHDLKernel * );

private:
  vector<SignalBase *> sensList;
  waitConditionPtr cond;
  waitTimeoutPtr timeout;

  bool isSensitive(int sigid, SignalBase *signal );
};
#endif
