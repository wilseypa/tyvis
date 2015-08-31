#ifndef SIGNALNETINFO_HH
#define SIGNALNETINFO_HH

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
#include "tyvis/IntegerTypeInfo.hh"
#include <fstream>
#include <vector>
using std::fstream;
using std::vector;
#include "tyvis/vhdl.hh"
#include "tyvis/FanOutList.hh"
#include "tyvis/ObjectBase.hh"
#include "tyvis/Block.hh"
#include "tyvis/AdditionalDrivers.hh"

class SourceBase;

/**
   This class represents a signal prior to elaboration.  SignalNetinfo
   instances get associated with the heirarchical classes that get
   published in "_elab" files.  During elaboration, these SignalNetinfo
   instantiations are then associated with processes that get registered in
   their fanout lists.  Ultimately this information gets copied into
   various signal instances spread through multiple processes where the
   signals have been instantiated.
*/

class SignalNetinfo : public ObjectBase {
public:
  SignalNetinfo( const TypeInfo &, const string &signalName );
  SignalNetinfo( const SignalNetinfo &, const IntegerTypeInfo & );
  SignalNetinfo( const ArrayTypeInfo &, TyvisVarArgs & );
  SignalNetinfo();
  virtual ~SignalNetinfo();
  
  /** global signal ID global allocator variable.  This variable is kept
      globally for all signals and is increased for each time a signal is
      created. */
  static int globalSignalId;

//   /** fanout list size.  This variable holds the current number of entries in
//       the fanout list (obj_ids). */
//   int getFanOut() const { return fanOut.size(); }
  const FanOutList &getFanOut() const { return fanOut; }

  /** Add fan out list from another SignalNetinfo object.  Adds the fan out
      list of the given SignalNetinfo object to the fan out list of the current
      object. */
  SignalNetinfo &addToFanOut( const RValue &sig );
  SignalNetinfo &addToFanOut( const RValue *ptr );
  SignalNetinfo &addToFanOut( const SignalNetinfo &sig );
  SignalNetinfo &addToFanOut( const SignalNetinfo *ptr );

  /** Add VHDLKernel object to the fan out list.  Adds the given VHDLKernel
      process object to the fan out list. */
  SignalNetinfo &addToFanOut( VHDLKernel * );
  SignalNetinfo &addChild( SignalNetinfo &sig, VHDLKernel *);
  const RValue &readVal() const;
  RValue &readVal();
  void updateVal( const RValue & );

  SignalNetinfo &operator=(const SignalNetinfo &s);
  RValue &operator=(const RValue &s);

  ObjectBase* clone() const;
  virtual void setResolutionFunctionId(int resolutionFnId);
  virtual void setTypeConversionFunctionId(int typeConversionFnId);


  void setDriverAddedFlag(const bool);
  void setDriverAddedFlag();
  bool getDriverAddedFlag() const;
  bool getSourceBaseDeleteFlag() const;
  void setSourceBaseDeleteFlag(const bool);

  void addAdditionalDriver( SignalBase *driver );
  void addAdditionalDrivers( const AdditionalDrivers &toAdd );

  void setSource( SourceBase *newSource ){
    source = newSource;
  }

  SourceBase *getSource() const {    
    return source;
  }

  const Block &getDownTypeConversionFnList() const {
    return downTypeConversionFnList;
  }

  void addDownTypeConversionFn( void *newFunction ){
    downTypeConversionFnList.addElement( newFunction );
  }

  bool isDriverAlreadySet() const {
    return driverAddedFlag;
  }

  const AdditionalDrivers &getAdditionalDrivers() const { return additionalDrivers; }

  int getId() const { return id; }
  void copyId( const SignalNetinfo &that );

  bool isResolvedSignal() const;

protected:
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  /** Add VHDLKernel object to the fan out list.  Adds the given VHDLKernel
      process object to the fan out list. */
  SignalNetinfo &addToFanOut( const string &processName, int destSigId );

  /** global signal ID.  A globally unique numeric identifier of a
      signal. */
  int id;

  bool driverAddedFlag;

  /** fanout list.  This contains the simulation object IDs that this
      signal gets transmitted to.  This data should never change during
      a simulation, so it is allocated only once, during initState, and
      deleted after the simulation is over.  The copy constructor and
      operator= simply copy the pointer to the new object. */
  FanOutList fanOut;

  SourceBase *source;
  Block      downTypeConversionFnList;
  bool sourceBaseDeleteFlag;
  AdditionalDrivers additionalDrivers;
};

#endif
