#ifndef OBJECT_FACTORY_HH
#define OBJECT_FACTORY_HH

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
#include <string>
using std::string;

class TypeInfo;
class ObjectBase;
class RValue;
class Signal;
class SignalNetinfo;
class Variable;

class SubElementFactory {
public:
  enum ObjectType { VARIABLE, SIGNAL, NETINFO };

  SubElementFactory( const TypeInfo &initParentTypeInfo,
		     ObjectBase *initParent,
		     const string &initParentName,
		     ObjectType initObjectType );

  SubElementFactory( const SubElementFactory &that );

  ~SubElementFactory();

  RValue *operator()( const int subField ) const;

  const TypeInfo &getParentTypeInfo() const;
  const string &getParentName() const;

  SubElementFactory *clone() const;

private:
  Variable *createVariable( const int subField ) const;
  Signal *createSignal( const int subField ) const;
  SignalNetinfo *createNetInfo( const int subField ) const;

  const TypeInfo &myParentTypeInfo;
  ObjectBase *myParent;
  const string &myParentName;
  const ObjectType myObjectType;
};

#endif
