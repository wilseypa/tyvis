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

#include "CallStack.hh"
#include <stdarg.h>
#include "ObjectBase.hh"

void 
CallStack::cleanStack() {
  StackElement *elem;

  while(numElements > 0) {
    elem = pop();
    delete [] elem->args;
  }
}

bool
CallStack::empty() const {
  return numElements == 0;
}

void 
CallStack::push(StackElement *elem) {
  ASSERT(elem != NULL);
  if(numElements == 0) {
    stackTop = elem;
    stackBottom = elem;
    currentTop = elem;
    elem->next = NULL;
  } else {
    ASSERT(stackBottom != NULL);
    elem->next = stackTop;
    stackTop = elem;
  }
  numElements++;
}

void
CallStack::push(const int waitLabel, const int numArgs, ...) {
  RValue **args;
  va_list ap;
  int i;
  StackElement *newElem = new StackElement();

  va_start(ap, numArgs);
  if(numArgs > 0) {
    args = new RValue*[numArgs];
  } else {
    args = NULL;
  }

  for(i = 0; i < numArgs; i++) {
    args[i] = va_arg(ap, RValue*);
  }
  va_end(ap);
  
  newElem->args = args;
  newElem->waitLabel = waitLabel;
  newElem->numArgs = numArgs;
  push(newElem);
}

StackElement*
CallStack::pop() {
  ASSERT(numElements != 0);
  ASSERT(stackTop != NULL);
  StackElement *retval = stackTop;

  numElements--;
  stackTop = stackTop->next;
  return retval;
}

// Removes all the elements in the stack above and including the current
// pointer.
void
CallStack::popAboveCurrent() {
  ASSERT(numElements != 0);
  StackElement *elem;

  while(stackTop != currentTop) {
    elem = pop();
    delete elem->args;
    delete elem;
  }
  elem = pop();
  delete elem->args;
  delete elem;
}

StackElement*
CallStack::getTop() const {
  return stackTop;
}

StackElement*
CallStack::getCurrentTop() const {
  return currentTop;
}

void
CallStack::setCurrentToNext() {
  ASSERT(currentTop != NULL);
  currentTop = currentTop->next;
}

void
CallStack::setCurrentToTop() {
  currentTop = stackTop;
}

CallStack&
CallStack::operator = (const CallStack& stack) {
  StackElement *elem = stack.getTop();
  StackElement *newelem;
  while(elem != NULL) {
    newelem = new StackElement;
    newelem->numArgs = elem->numArgs;
    newelem->waitLabel = elem->waitLabel;
    newelem->args = new RValue*[elem->numArgs];
    for(int i = 0; i < elem->numArgs; i++) {
      newelem->args[i] = dynamic_cast<RValue *>(elem->args[i]->clone());
    }

    push(newelem);
    elem = elem->next;
  }
  return *this;
}
