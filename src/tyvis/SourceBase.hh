#ifndef SOURCEBASE_HH
#define SOURCEBASE_HH

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
#include <iostream>
#include "tyvis/ObjectBase.hh"

class SourceData;
class VHDLKernel;
class SignalNetinfo;

using std::cerr;

/** SourceBase forms the base class for the nodes of the source tree.  The
    nodes may be one of SourceData or SourceInfo.  This class privides
    virtual functions for these two derived classes. */

class SourceBase {
public:
  SourceBase();
  virtual ~SourceBase() = 0;

  /** Methods to act on parentVHDLKernel of this node. */
  void setParent(SourceBase*);
  SourceBase *getParent() const { return parent; };

  virtual SourceBase& operator[](const int);
  
  /**
     Methods for manipulating data.
  */
  //@{
  virtual SignalBase *getData() = 0;
  virtual void setData( SignalBase * ) = 0;
  virtual void updateData( const RValue & ) = 0;
  //@}

  /**
     Find the driver for this process.
  */
  virtual SourceData *getDriver( VHDLKernel * ) { return NULL; };

  /** While getDriver(id) returns the driver with VHDLKernel* == sourceProcess,
      getDriveratIndex(id) returns the driver at childTable[id] */
  virtual SourceData* getDriveratIndex(int)  { return NULL; };

  virtual VHDLKernel* getSourceId() const { return DEFAULT_ID; };
  virtual void setSourceId(VHDLKernel *) { abort(); };

  virtual int addChild(SourceBase *) { return 0; };
  virtual int addChild(ObjectBase *, VHDLKernel *) { return 0; };
  virtual int getNumChildren() const { return 0; };

  /** Returns the number of connected drivers at the time applying the
      resolution function. */
  virtual int getNumActiveChildren() const { return 0; }

  virtual bool _is_child_present(VHDLKernel *) { return false; }

  /** This functions returns if the driver is active in the current simulation
      cycle */
  virtual bool _is_driver_connected() { return true; }

  /** This function is needed to handle default initial values for signals
      in the system. This function is overloaded in SourceData to do the right
      job. This virtual function is here just to be handy. */
  virtual bool isAnonymousDriver() { return false; }
  
  /** Methods to deal with up and down type conversion functions and
      resolution functions. */
  virtual void setUpTypeConversionFnId(TypeConversionFnId_t) { return; };
  virtual void setDownTypeConversionFnId(TypeConversionFnId_t) { return; };
  virtual void setResolutionFnId(ResolutionFnId_t) { return; };
  virtual TypeConversionFnId_t getUpTypeConversionFnId(void) { return DEFAULT_TCF_ID; }
  virtual TypeConversionFnId_t getDownTypeConversionFnId(void) { return DEFAULT_TCF_ID; }
  virtual ResolutionFnId_t getResolutionFnId(void) { return DEFAULT_RF_ID; };

  virtual Value resolve( VHDLKernel *, SigType typ = EXPLICIT );

  virtual void addDriversToRootDriverTable( SourceBase * ) = 0;
  virtual void addToRootDriverTable( SourceData * ) = 0;

  /** Table size is equivalent to one since this is a SourceData object
      and it has one driver.  This will be used to get the driver using
      getDriver(n) which will return the driver in this node. */
  virtual int getTableSize(void) const { return 1; };

  virtual void print(ostream&) const { return; };

  static VHDLKernel           *ANONYMOUS_PROCESS_ID;
  static VHDLKernel           *DEFAULT_ID;
  static TypeConversionFnId_t DEFAULT_TCF_ID;
  static ResolutionFnId_t     DEFAULT_RF_ID;
  
//   struct CompareString {
//     size_t operator()(const char *s1, const char *s2) const {
//       return (strcmp(s1, s2) == 0);
//     }
//   };

protected:
  /** A pointer to the parent of this node.  Required for type
      conversion and resolution functions. */
  SourceBase * parent;
};
#endif
