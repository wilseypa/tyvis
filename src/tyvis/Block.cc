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

#include "Block.hh"
#include <iostream>
#include <algorithm>
using std::find;
using std::cout;
using std::endl;

Block::Block( const int initSize ){
  expandableArray.reserve( initSize );
}

void*
Block::getElement( const int index ){
  return expandableArray[index];
}

void
Block::removeElement(const int index)  {
  vector<void *>::iterator i = expandableArray.begin();
  i += index;
  expandableArray.erase( i );
}

void*
Block::operator [](const int index) {
  return getElement( index );
}

void 
Block::addElement(void* newElement) {
  if( newElement == 0 ){
    cout << "Adding NULL to block!" << endl;
  }
  void *found = find( newElement );
  if( found == 0 ){
    expandableArray.push_back( newElement );
  }
}

void *
Block::find( void* element ){
  void *retval = 0;
  vector<void *>::iterator found = std::find( expandableArray.begin(),
					      expandableArray.end(),
					      element );
  if( found != expandableArray.end() ){
    retval = *found;
  }
  return retval;
}

int
Block::getNumberOfElements() const {
  return expandableArray.size();
}

void
Block::reset() {
  expandableArray.clear();
}

Block&
Block::operator=(const Block& rhsBlock){
  expandableArray = rhsBlock.expandableArray;
  return *this;
}
