#ifndef IMPSIGNAL_HH
#define IMPSIGNAL_HH

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
#include "tyvis/SignalBase.hh"
#include "tyvis/UniversalInteger.hh"

class SourceBase;

template <class Type> 
class ImplicitSignal : public SignalBase {
public:
  ImplicitSignal(void);
  ImplicitSignal(const VHDLData&);
  ImplicitSignal(const char*);
  ImplicitSignal(const int);

  void print(ostream&) const;

  virtual ObjectType getKind() const {return IMPLICIT_SIGNAL;};

  void updateDriver( const int, 
		     int processId,
		     const VHDLData *, 
		     const ArrayInfo * = NULL,
		     const ArrayInfo * = NULL );
  
  VHDLType* readDriver(VHDLKernel *);
  void updateDrvVal(const VHDLData*, const ArrayInfo* = NULL, 
		    const ArrayInfo* = NULL);
  VHDLData* readDrvVal();
  void updateEffVal(const RValue *, const ArrayInfo* = NULL,
		    const ArrayInfo * = NULL);
  void updateEffVal(const RValue &, const ArrayInfo* = NULL,
		    const ArrayInfo* = NULL);
  RValue* readEffVal();
  VHDLVTime& readTimeVal();
  VHDLVTime& updateTimeVal(const VHDLVTime& v = ZERO);

  ObjectBase& operator=(const ObjectBase&);
  ObjectBase& operator=(const VHDLData&);
  const RValue &readVal() const {
    return effVal;
  }
  void updateVal(const RValue &val ) {
    updateEffVal( val );
  }

  /**  time value for Stable, Quiet, Delayed */
  VHDLVTime timeval;       

  /** Source for this signal */
  SourceBase *source;

  /** driving value of the signal */
  Type drvVal;
  
  /** effective value of the signal */
  Type effVal;

  UniversalInteger (*guardExpr)(void);

  ObjectBase* clone() const {
    ImplicitSignal<Type> *retval = new ImplicitSignal<Type>;
    *retval = *this;
    return retval;
  }

  void setSource(SourceBase *src) {
    source = src;
  }
};

#include "tyvis/ImplicitSignal.cc"
#endif





