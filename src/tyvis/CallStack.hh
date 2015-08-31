#ifndef PROCESSSTACK_HH
#define PROCESSSTACK_HH

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
#include <stdlib.h>

class RValue;

struct StackElement {
  int waitLabel;
  RValue **args;
  StackElement *next;
  int numArgs;

  StackElement(): waitLabel(-1), args(NULL), next(NULL), numArgs(0) {};
};

class CallStack {
public:
  CallStack() {
    stackTop = NULL;
    stackBottom = NULL;
    numElements = 0;
  };
  virtual ~CallStack() {
    cleanStack();
  }

  bool empty() const;
  void push(StackElement *);
  void push(const int waitLabel, const int numArgs, ...);
  StackElement* pop();
  void popAboveCurrent();
  StackElement* getTop() const;
  StackElement* getCurrentTop() const;
  void setCurrentToNext();
  void setCurrentToTop();

  CallStack& operator = (const CallStack&);

  int stackSize() const { 
    return numElements;
  }

private:
  void cleanStack();

  StackElement *stackTop;
  StackElement *stackBottom;

  // This is used to peek into the stack without actually pushing or
  // popping any item from it.
  StackElement *currentTop;
  int numElements;
};

#endif
