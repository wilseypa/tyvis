#ifndef _SOURCE_TYPE_CONVERT_HH_
#define _SOURCE_TYPE_CONVERT_HH_

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

class TypeConvert;
class ObjectBase;
class VHDLProcess;

/** This class is like SourceData but the difference is that it comes into
    picture only if there is a TypeConversion. It has a pointer to the driver
    and the TypeConvert Node which does the actual TypeConversion. */

class SourceTypeConvert : public SourceBase {
public:
  SourceTypeConvert();
  ~SourceTypeConvert();

  /** When a resolve is called on this node. We in turn call a resolve on
      the TypeConvert node which does the TypeConversion. TypeConvert does
      the type conversion and puts the resultant value in the driver
      pointer to by data.  Once the typeconversion is over, the data
      contains the typeconverted value. */
  Value resolve( VHDLKernel *, SigType type = EXPLICIT );

  void setData( SignalBase * );

  void setTypeConvert(TypeConvert *);

  /**
     These methods are pure virtual in SourceBase so we are overriding
     them.  However, calling them will generate a runtime error.
  */
  //@{
  SignalBase *getData();
  void updateData( const RValue & );
  void addDriversToRootDriverTable( SourceBase * );
  void addToRootDriverTable( SourceData * );
  //@}
    

private:
  SignalBase *source;
  TypeConvert *conversionNode;
};

#endif

