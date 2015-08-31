#ifndef BLOCK_HH
#define BLOCK_HH

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
#include <vector>
using std::vector;

/** This indexes the elements from zero. Be careful while indexing
    it. Start from zero.*/
class Block {
public:
  Block(){expandableArray.clear();}
  Block( const int initSize );
  ~Block(){}

  void *getElement(const int index);
  void *operator[](const int index);
  void addElement(void *newElement);
  void removeElement(const int index);
  int getNumberOfElements() const;
  void reset();
  Block &operator=(const Block &);
  
private:
  /** The array pointer pointing to Valid Location */
  vector<void *> expandableArray;

  void *find(void *);
};

#endif


