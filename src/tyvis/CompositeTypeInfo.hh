#ifndef COMPOSITE_TYPE_INFO_HH
#define COMPOSITE_TYPE_INFO_HH

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

#include "TypeInfo.hh"
class ScalarTypeInfo;
class SubElementFactory;
class RValue;

class CompositeTypeInfo : public TypeInfo {
public:
  virtual bool isConstrained() const;
  virtual bool isUnconstrained() const;

  virtual const ScalarTypeInfo *getRange(int dimension) const;

  virtual void constructFields( vector<RValue *> &elements,
				const SubElementFactory *subElementFactory,
				int numElements = -1 ) const = 0;

  virtual int storageIndex( int elementIndex ) const { return elementIndex; }

  virtual const TypeInfo &getElementTypeInfo( const RValue &index = defaultIndex() ) const = 0;
  virtual const ScalarTypeInfo &getIndexTypeInfo( const RValue &index = defaultIndex() ) const = 0;
};

#endif
