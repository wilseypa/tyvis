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

#include "FanOutList.hh"
#include "FanDest.hh"
#include "SigEvent.hh"

FanOutList::FanOutList(){}

FanOutList::~FanOutList(){
  for( vector<FanDest *>::iterator i = fanOut.begin();
       i < fanOut.end();
       i++ ){
    delete (*i);
  }
  fanOut.clear();
}

FanOutList::FanOutList( const FanOutList &that ){
  *this = that;
}


FanOutList &
FanOutList::operator=( const FanOutList &that ){
  for( vector<FanDest *>::const_iterator i = that.fanOut.begin();
       i < that.fanOut.end();
       i++ ){
    fanOut.push_back( (*i)->clone() );
  }
  return *this;
}


void 
FanOutList::sendEvent( const SigEvent *toSend, SimulationObject *sender ){
  for( vector<FanDest *>::iterator i = fanOut.begin();
       i < fanOut.end();
       i++ ){
    (*i)->sendEvent( toSend, sender );
  }
}

void
FanOutList::add( const string &toAdd, int destSigId ){
  bool found = false;
  for( vector<FanDest *>::const_iterator i = fanOut.begin();
       i < fanOut.end();
       i++ ){
    if( (*i)->getProcessName() == toAdd && (*i)->getDestSigId() == destSigId ){
      found = true;
      break;
    }
  }
  if( !found ){
    fanOut.push_back( new FanDest( toAdd, destSigId ));
  }
}

void
FanOutList::addAll( const FanOutList &toAdd ){
  for( vector<FanDest *>::const_iterator i = toAdd.fanOut.begin();
       i < toAdd.fanOut.end();
       i++ ){
    add( (*i)->getProcessName(), (*i)->getDestSigId() );
  }
}

void
FanOutList::dump( ostream &os ){
  for( vector<FanDest *>::const_iterator i = fanOut.begin();
       i < fanOut.end();
       i++ ){
    os << (*i)->getProcessName() << " : " << (*i)->getDestSigId() << endl;
  }
}

void
FanOutList::getIds( vector<int> & f ){
  for( vector<FanDest *>::const_iterator i = fanOut.begin();
       i < fanOut.end();
       i++ ){
    f.push_back((*i)->getDestSigId());
  }
}
