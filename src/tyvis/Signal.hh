#ifndef SIGNAL_HH
#define SIGNAL_HH

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

class SourceBase;
class VHDLProcess;

class Signal : public SignalBase {
public:
  Signal( const TypeInfo & );

  Signal( const CompositeTypeInfo &,
	  const string &initName );

  Signal( const TypeInfo &,
	  const string &initName );

  Signal( const CompositeTypeInfo &,
	  const string &initName,
	  const RValue &initValue );

  Signal( const TypeInfo &,
	  const string &initName,
	  const RValue &initValue );

  Signal( const Signal & );

  void newSignal( int );

  ~Signal();

  bool isResolved() const;

  RValue &operator=(const RValue &);
  
  void updateDriver( const int, 
		     VHDLKernel *srcProcess,
		     const RValue *, 
		     const ArrayInfo *, 
		     const ArrayInfo * = NULL );

  void disconnectDriver(VHDLKernel *);

  RValue *readDriver(VHDLKernel *);
  void updateDrvVal( const RValue &,
		     const ArrayInfo* = NULL, 
		     const ArrayInfo* = NULL );

  RValue *readDrvVal();
  virtual void updateEffVal( const RValue &,
			     const ArrayInfo* = NULL,
			     const ArrayInfo* = NULL );

  RValue &readEffVal();

  void setSource(SourceBase *src);
  
  RValue *clone() const;

  Value resolve( VHDLKernel * );

  bool isCompositeResolvedSignal() const {
    return (getTypeInfo().isCompositeType() && isResolved() );
  }

  virtual SignalBase *findSigInBlock( int sigId, VHDLKernel *srcId );
  
  SourceBase *getSource() {
    return source;
  }

  SigType getType() const { return EXPLICIT; }

protected:
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  Signal();

  /** Number of sources will be fixed in initState() after dynamic
      elaboration.  Sources for this signal. if numSources > 1, busResFn
      must be valid */
  SourceBase* source;
  
  /** driving value of the signal. */
  RValue *drvVal;
};

#endif
