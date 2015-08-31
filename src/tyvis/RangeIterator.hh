#ifndef RANGE_ITERATOR_HH
#define RANGE_ITERATOR_HH

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

class ScalarTypeInfo;
class ArrayInfo;

class RangeIterator {
public:
  RangeIterator( const ScalarTypeInfo *initRange );
  ~RangeIterator();

  /** Is there a next value?  */
  bool hasNext() const;
  /** Get the current value and increment. */
  int next();
  /** Get the current value; don't increment. */
  int peek();
  /** Set the current value */
  void set (int);

  /**
     Returns the length() of the range.  If the range is 1 to 3, then would
     return 3.  If the range was 5 downto 2, would return 4.
  */
  int length() const;
  RangeIterator( const RangeIterator &that );

  /**
     Reset this iterator back to 'left.
  */
  void reset();
  void reset( const ScalarTypeInfo & );
  void reset( const ArrayInfo & );
  
private:
  /**
     Used by the class to set the _hasNext member.
  */
  void setHasNext();

  ScalarTypeInfo *myRange;
  int pos;
  bool _hasNext;
};
#endif
