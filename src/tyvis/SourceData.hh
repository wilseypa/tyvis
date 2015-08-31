#ifndef SOURCEDATA_HH
#define SOURCEDATA_HH

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
#include "tyvis/SourceBase.hh"
#include "tyvis/ObjectBase.hh"

class VHDLKernel;

class SourceData : public SourceBase {
public:
  SourceData();

  SourceData(VHDLKernel *sourceProcessPtr);
  
  ~SourceData();

  void disconnect() { connected = false; }

  VHDLKernel *getSourceId() const;
  void setSourceId(VHDLKernel *);

  SignalBase *getData();
  void setData( SignalBase * );

  int addChild(ObjectBase*, VHDLKernel *);
  int addChild(SourceBase *);

  void updateData( const RValue & );

  bool _is_child_present(VHDLKernel *);
  bool _is_driver_connected() { return connected; }
  bool isAnonymousDriver();
  
  SourceData *getDriver( VHDLKernel * );
  SourceData *getDriver( int processId );
  SourceData *getDriveratIndex(int);

  Value resolve( VHDLKernel *, SigType type = EXPLICIT );

  void print(ostream&) const;

  /**
     The following are pure virtual in the parent, but SourceData does not
     really implement them.  If they are called, a runtime error will result.
  */
  //@{
  void addDriversToRootDriverTable( SourceBase * );
  void addToRootDriverTable( SourceData * );
  //@}
  
private:
  VHDLKernel *sourceProcess;
  SignalBase *sourceObject;
  bool       connected;  
  bool ambiguousSource;
  vector<VHDLKernel *> *sourceProcessVec;
  vector<SignalBase *> *sourceObjectVec;
};
  
#endif
