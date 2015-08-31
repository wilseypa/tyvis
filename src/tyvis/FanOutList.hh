#ifndef FAN_OUT_LIST_HH
#define FAN_OUT_LIST_HH

// Copyright (c) Clifton Labs, Inc.  All Rights Reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include "tyvis/tyvis-config.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::ostream;

class FanDest;
class SigEvent;
class SimulationObject;

class FanOutList {
public:
  FanOutList();
  ~FanOutList();

  FanOutList( const FanOutList & );
  FanOutList &operator=( const FanOutList &that );

  /**
     Send the event to all receivers in our fanout list.
  */
  void sendEvent( const SigEvent *toSend, SimulationObject *sender );

  /**
     Add this process to our fanout list.
  */
  void add( const string &processName, int destSigId );

  /**
     Add the FanOutList's processes to ours as well.
  */
  void addAll( const FanOutList &toAdd );

  /**
     Dump the this FanOutList - for debugging.
  */
  void dump( ostream & );
  void getIds( vector<int> & );
  int size() {return fanOut.size();}
private:
  vector<FanDest *> fanOut;
};

#endif
