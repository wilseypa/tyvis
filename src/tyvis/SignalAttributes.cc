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

#include "VHDLProcess.hh"
#include "SignalBase.hh"
#include "Variable.hh"
#include "UniversalInteger.hh"
#include "UniversalReal.hh"

// This file contains the methods in VHDLProcess class that relate to signal
// attributes.  This file has been created to increase the readability of
// both VHDLProcess and SignalAttributes.

// Attribute implementation: The attribute is called with a VHDLType and
// corresponding parameter as arguments.  This is the function defined below.
// This function finds the "kind" of the type using the "get_kind()" member
// function and correspondingly calls another function.  This function
// extracts the signal information from the type and calls yet another
// function which computes the attribute info.
//     This elaborate mechanism is to make the attribute evaluation
// transparent to scalar types, array types and record types.

// Note: The method "get_kind()" MUST NOT be overloaded by a derived type.
// This is to ensure that the type is recognized correctly.

UniversalInteger
VHDLProcess::locateQuietAttribute(const VHDLType *sig, const VHDLVTime time) {
  UniversalInteger retval(0);
  
  ASSERT(sig != NULL);
  ASSERT(time >= VHDLVTime::getVHDLVTimeZero());

  switch(sig->get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    retval = locateQuietAttribute((const ScalarType*)sig, time);
    break;
  case ARRAY_TYPE:
    retval = locateQuietAttribute(((const Array *)sig), time);
    break;
  case RECORD_TYPE:
    retval = locateQuietAttribute((const Record*)sig, time);
    break;
  default:
    cerr << "Unknown Type!! Asking for Quiet Attribute, Sssh, Keep QUIET ;-)"
	 << endl;
    break;
  }
  return retval;
}

UniversalInteger
VHDLProcess::locateQuietAttribute(const ScalarType *sig, const VHDLVTime time) {
  ASSERT(sig != NULL);
  ASSERT(time >= VHDLVTime::getVHDLVTimeZero());

  return locateQuietAttribute((const SignalBase*)sig->getObject(), time);
}			     

UniversalInteger
VHDLProcess::locateQuietAttribute(const Array *sig, const VHDLVTime time) {
  UniversalInteger retval(1), tempVal(1);
  int noOfElements = sig->length(0);

  retval = locateQuietAttribute(&(sig->getElement(0)), time);
  for(int i = 1; (i < noOfElements); i++) {
    tempVal = locateQuietAttribute(&(sig->getElement(i)), time);
    retval  = retval.vhdlAnd(tempVal);
  }
  
  return retval;
}

UniversalInteger
VHDLProcess::locateQuietAttribute( const Record* sig, const VHDLVTime time ){
  int noOfElements = sig->get_number_of_fields();
  UniversalInteger retval(1), tempVal(1);

  for (int i=1; i<= noOfElements; i++) {
    tempVal = locateQuietAttribute((const VHDLType*)&(sig->get_field(i)),time);
    retval = retval.vhdlAnd(tempVal);
  }
  return retval;
}

UniversalInteger
VHDLProcess::locateQuietAttribute( const SignalBase *sig,
				   const VHDLVTime time ){
  vector<Attribute *> attribute;
  Attribute *retval = NULL;
  PhysicalLiteral lastActiveTime;
  int i;

  ASSERT(sig != NULL);

  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for (i = 0; i < sig->getNumAttributes(); i++ ){
    if( attribute[i]->attrib == LAST_ACTIVE ){
      retval = attribute[i];
    }
  }
  ASSERT((retval != NULL && retval->attrib == LAST_ACTIVE));
  lastActiveTime = ((ScalarType *)retval->value)->getObject()->readVal();
 
  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for (i = 0; i <sig->getNumAttributes(); i++) {
    if(attribute[i]->attrib == QUIET) 
      retval = attribute[i];
  }
  ASSERT((retval != NULL && retval->attrib == QUIET));

  if(time == VHDLVTime::getVHDLVTimeZero()) {
    if( lastActiveTime < PhysicalLiteral(getTimeNow().getMajor()) ){
      updateAttribute(retval, SAVANT_BOOLEAN_TRUE);
    } 
    else {
      updateAttribute(retval, SAVANT_BOOLEAN_FALSE);
    }
  } 
  else {
    if( lastActiveTime < PhysicalLiteral(getTimeNow().getMajor() - time.getMajor()) ){
      updateAttribute(retval, SAVANT_BOOLEAN_TRUE);
    } else {
      updateAttribute(retval, SAVANT_BOOLEAN_FALSE);
    }
  }
    
  return UniversalInteger(retval->value->readVal());
}

UniversalInteger
VHDLProcess::locateStableAttribute(const VHDLType *sig, const VHDLVTime time) {
  UniversalInteger retval;

  ASSERT(sig != NULL);
  ASSERT(time >= VHDLVTime::getVHDLVTimeZero());
  switch(sig->get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    retval = locateStableAttribute((const ScalarType*)sig, time);
    break;
  case ARRAY_TYPE:
    retval = locateStableAttribute((const Array *) sig, time);
    break;
  case RECORD_TYPE:
    retval = locateStableAttribute((const Record*)sig, time);
    break;

  default:
    cerr << "Unknown Type!! Asking for Stable Attribute, Sssh, Keep QUIET ;-)"
	 << endl;
    break;
  }
  return retval;
}

UniversalInteger
VHDLProcess::locateStableAttribute(const ScalarType *sig, const VHDLVTime time) {
  UniversalInteger retval(1);
  ASSERT(sig != NULL);
  ASSERT(time >= VHDLVTime::getVHDLVTimeZero());

  retval = locateStableAttribute((const SignalBase*)sig->getObject(), time);

  return retval;
}			     

UniversalInteger
VHDLProcess::locateStableAttribute(const Array *sig, const VHDLVTime time) {
  UniversalInteger retval(1), tempVal(1);
  int noOfElements = sig->length(0);

  retval = locateStableAttribute(&(sig->getElement(0)), time);
  for(int i = 0; (i < noOfElements); i++) {
    tempVal = locateStableAttribute(&(sig->getElement(i)), time);
    retval = retval.vhdlAnd(tempVal);
  }
  
  return retval;
}

UniversalInteger
VHDLProcess::locateStableAttribute(const Record* sig, const VHDLVTime time) {
  UniversalInteger retval(1), tempVal(1);
  int noOfElements = sig->get_number_of_fields();
  for(int i=1; i<= noOfElements; i++) {
    tempVal = locateStableAttribute((const VHDLType*)&(sig->get_field(i)),time);
    retval = retval.vhdlAnd(tempVal);
  }
  return retval;
}

UniversalInteger
VHDLProcess::locateStableAttribute( const SignalBase *sig, 
				    const VHDLVTime time ){
  vector<Attribute *> attribute;
  Attribute *retval = 0;
  PhysicalLiteral lastEventTime;
  int i;

  ASSERT(sig != NULL);

  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for (i = 0; i <sig->getNumAttributes(); i++) {
    if(attribute[i]->attrib == LAST_EVENT){
      retval = attribute[i];
    }
  }

  ASSERT((retval != NULL && retval->attrib == LAST_EVENT));
  lastEventTime = ((ScalarType *)retval->value)->getObject()->readVal();

  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for (i = 0; i <sig->getNumAttributes(); i++) {
    if(attribute[i]->attrib == STABLE) 
      retval = attribute[i];
  }

  ASSERT((retval != NULL && retval->attrib == STABLE));
  PhysicalLiteral lvt(getTimeNow().getMajor());
  PhysicalLiteral timeExpr(time.getMajor());
  if (time == VHDLVTime::getVHDLVTimeZero()) {
    if( lastEventTime < lvt ){
      updateAttribute(retval, SAVANT_BOOLEAN_TRUE);
    } else {
      updateAttribute(retval, SAVANT_BOOLEAN_FALSE);
    }
  } 
  else {
    if( lastEventTime <= lvt.vhdlMinus(timeExpr) ){
      updateAttribute(retval, SAVANT_BOOLEAN_TRUE);
    } 
    else {
      updateAttribute(retval, SAVANT_BOOLEAN_FALSE);
    }
  }

  return UniversalInteger(retval->value->readVal());
}

UniversalInteger
VHDLProcess::locateEventAttribute(const VHDLType *sig) {
  UniversalInteger retval;

  ASSERT(sig != NULL);

  switch(sig->get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    retval = locateEventAttribute((const ScalarType *)sig);
    break;
  case ARRAY_TYPE:
    retval = 
      locateEventAttribute((const Array *) sig);
    break;
  case RECORD_TYPE:
    retval = locateEventAttribute((const Record*)sig);
    break;
  default:
    cerr << "Unknown Type!! Asking for Event Attribute" << endl;
    break;
  }
  return retval;
}

UniversalInteger
VHDLProcess::locateEventAttribute(const ScalarType *sig) {
  UniversalInteger retval;

  ASSERT(sig != NULL);

  retval = locateEventAttribute((SignalBase*)sig->getObject());

  return retval;
}			     

UniversalInteger
VHDLProcess::locateEventAttribute(const Array *sig) {
  UniversalInteger retval(0), tempVal(0);
  int noOfElements = sig->length(0);

  for(int i = 0; (i < noOfElements); i++) {
    tempVal = locateEventAttribute(&(sig->getElement(i)));
    retval = retval.vhdlOr(tempVal);
  }
  
  return retval;
}

UniversalInteger
VHDLProcess::locateEventAttribute(const Record* sig) {
  UniversalInteger retval(0), tempVal(0);
  int noOfElements = sig->get_number_of_fields();
  if(noOfElements >= 1) {
    retval = locateEventAttribute((const VHDLType*)&(sig->get_field(1)));
  }

  for(int i=2; i< noOfElements; i++) {
    tempVal = locateEventAttribute((const VHDLType*)&(sig->get_field(i)));
    retval = retval.vhdlOr(tempVal);
  }
  return retval;
}

UniversalInteger
VHDLProcess::locateEventAttribute(SignalBase *sig) {
  VHDLVTime lastEventTime;

  lastEventTime = sig->getAllAttributes().last_event;
  
  // taking the default stuff
  if ( lastEventTime == getTimeNow() ){ 
    sig->getAllAttributes().event = true;
    return SAVANT_BOOLEAN_TRUE;
  } 
  else  {
    sig->getAllAttributes().event = false;
    return SAVANT_BOOLEAN_FALSE;
  }    
}

UniversalInteger *
VHDLProcess::locateTransactionAttribute(const VHDLType *sig) {
  UniversalInteger *retval = 0;
  ASSERT(sig != NULL);
  switch(sig->get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    retval = locateTransactionAttribute((const ScalarType*)sig);
    break;
  case ARRAY_TYPE:
    retval = locateTransactionAttribute((const Array *) sig);
    break;
  case RECORD_TYPE:
    retval = locateTransactionAttribute((const Record*)sig);
    break;
  default:
    cerr << "Unknown Type!! Asking for Transaction Attribute !"
	 << endl;
    break;
  }
  return retval;
}

UniversalInteger *
VHDLProcess::locateTransactionAttribute(const ScalarType *sig) {
  ASSERT(sig != NULL);
  return locateTransactionAttribute((const SignalBase*)sig->getObject());
}			     

UniversalInteger *
VHDLProcess::locateTransactionAttribute(const Array *sig) {
  UniversalInteger *retval = 0;
  UniversalInteger *tempVal = 0;
  int noOfElements = sig->length(0);

  for(int i = 0; (i < noOfElements); i++) {
    tempVal = locateTransactionAttribute(&(sig->getElement(i)));
    *retval = retval->vhdlAnd(*tempVal);
    delete tempVal;
  }
  
  return retval;
}

UniversalInteger *
VHDLProcess::locateTransactionAttribute(const Record* sig) {
  UniversalInteger *retval = 0;
  UniversalInteger *tempVal = 0;
  int noOfElements = sig->get_number_of_fields();

  for(int i=1; i<= noOfElements; i++) {
    tempVal = locateTransactionAttribute((const VHDLType*)&(sig->get_field(i)));
    *retval = retval->vhdlAnd(*tempVal);
    delete tempVal;
  }
  return retval;
}

UniversalInteger *
VHDLProcess::locateTransactionAttribute(const SignalBase *sig) {
  vector<Attribute *> attribute;
  Attribute *retval = NULL;
  
  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for (int i = 0; i <sig->getNumAttributes(); i++) {
    if(attribute[i]->attrib == TRANSACTION){
      retval = attribute[i];
    }
  }
  ASSERT((retval != NULL && retval->attrib == TRANSACTION));
  return new UniversalInteger(retval->value->readVal());
}  

// ***************** Active Attributes **********************************
 
UniversalInteger
VHDLProcess::locateActiveAttribute(const VHDLType *sig) {
  UniversalInteger retval;

  ASSERT(sig != NULL);

  switch(sig->get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    retval = locateActiveAttribute((const ScalarType*)sig);
    break;
  case ARRAY_TYPE:
    retval = locateActiveAttribute((const Array *) sig);
    break;
  case RECORD_TYPE:
    retval = locateActiveAttribute((const Record*)sig);
    break;
  default:
    cerr << "Unknown Type!! Asking for Active Attribute !"
	 << endl;
    break;
  }
  return retval;
}


UniversalInteger
VHDLProcess::locateActiveAttribute(const ScalarType *sig) {
  UniversalInteger retval;

  ASSERT(sig != NULL);

  retval = locateActiveAttribute((const SignalBase*)sig->getObject());

  return retval;
}			     

UniversalInteger
VHDLProcess::locateActiveAttribute(const Array *sig) {
  UniversalInteger retval, tempVal;
  int noOfElements = sig->length(0);
  
  for(int i = 0; (i < noOfElements); i++) {
    tempVal = locateActiveAttribute(&(sig->getElement(i)));
    retval = retval.vhdlOr( tempVal );
  }
  
  return retval;
}


UniversalInteger
VHDLProcess::locateActiveAttribute(const Record* sig) {
  UniversalInteger retval, tempVal;
  int noOfElements = sig->get_number_of_fields();

  for(int i=1; i<= noOfElements; i++) {
    tempVal = locateActiveAttribute((const VHDLType*)&(sig->get_field(i)));
    retval = retval.vhdlOr( tempVal );
  }
  return retval;
}

UniversalInteger
VHDLProcess::locateActiveAttribute(const SignalBase *sig) {
  vector<Attribute *> attribute;
  Attribute *retval = NULL;
  PhysicalLiteral lastActiveTime;
  int i;

  ASSERT(sig != NULL);

  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for(i = 0; i <sig->getNumAttributes(); i++) {
    if(attribute[i]->attrib == LAST_ACTIVE){
      retval = attribute[i];
    }
  }
  ASSERT((retval != NULL && retval->attrib == LAST_ACTIVE));
  lastActiveTime = (PhysicalLiteral &) ((PhysicalLiteral *)retval->value)->getObject()->readVal();
 
  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for(i = 0; i <sig->getNumAttributes(); i++) {
    if(attribute[i]->attrib == ACTIVE) 
      retval = attribute[i];
  }
  ASSERT((retval != NULL && retval->attrib == ACTIVE));

  if ( lastActiveTime == PhysicalLiteral(getTimeNow().getMajor() )){
    updateAttribute(retval, SAVANT_BOOLEAN_TRUE);
    return UniversalInteger(retval->value->readVal());
  } 
  else {
    updateAttribute(retval, SAVANT_BOOLEAN_FALSE);
    return UniversalInteger(retval->value->readVal());
  }
}

// ***************** last_event Attributes **********************************
 
const PhysicalLiteral
VHDLProcess::locateLastEventAttribute(const VHDLType *sig) {
  PhysicalLiteral retval(ObjectBase::VARIABLE, 
		      "",
		      PHYSICAL_MAX(),
		      std_standard_timeTypeInfo()() );

  ASSERT(sig != NULL);

  switch(sig->get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    retval = locateLastEventAttribute((const ScalarType*)sig);
    break;
  case ARRAY_TYPE:
    retval = locateLastEventAttribute((const Array *) sig);
    break;
  case RECORD_TYPE:
    retval = locateLastEventAttribute((const Record*)sig);
    break;
  default:
    cerr << "Unknown Type!! Asking for last_event Attribute !"
	 << endl;
    break;
  }
  return retval;
}


const PhysicalLiteral
VHDLProcess::locateLastEventAttribute(const ScalarType *sig) {
  ASSERT(sig != NULL);

  return locateLastEventAttribute((const SignalBase*)sig->getObject());
}			     

const PhysicalLiteral
VHDLProcess::locateLastEventAttribute(const Array *sig) {
  PhysicalLiteral retval(ObjectBase::VARIABLE,
		      "_lastEvent",
		      PHYSICAL_MAX(),
		      std_standard_timeTypeInfo()() );
  PhysicalLiteral tempVal(ObjectBase::VARIABLE,
		       "",
		       PHYSICAL_MAX(),
		       std_standard_timeTypeInfo()() );

  int noOfElements = sig->length(0);
  for(int i = 0; (i < noOfElements); i++) {
    tempVal = locateLastEventAttribute(&(sig->getElement(i)));
    if( tempVal <= retval ){
      retval = tempVal;
    }
  }
  
  return retval;
}


const PhysicalLiteral
VHDLProcess::locateLastEventAttribute(const Record* sig) {
  PhysicalLiteral retval(ObjectBase::VARIABLE,
		      "_lastEvent",
		      PHYSICAL_MAX(),
		      std_standard_timeTypeInfo()() );
  PhysicalLiteral tempVal(ObjectBase::VARIABLE,
		       "",
		       PHYSICAL_MAX(),
		       std_standard_timeTypeInfo()() );

  int noOfElements = sig->get_number_of_fields();

  for(int i=1; i<= noOfElements; i++) {
    tempVal = locateLastEventAttribute((const VHDLType*)&(sig->get_field(i)));
    if( tempVal < retval ){
      retval = tempVal;
    }
  }
  return retval;
}

const PhysicalLiteral
VHDLProcess::locateLastEventAttribute(const SignalBase *sig) {
  ASSERT(sig != NULL);
  
  VHDLVTime lastEventTime = const_cast<SignalBase *>(sig)->getAllAttributes().last_event;
  lastEventTime = getTimeNow().getMajor() - lastEventTime.getMajor();

  return PhysicalLiteral(ObjectBase::VARIABLE, "_lastEvent", lastEventTime, std_standard_timeTypeInfo()() );
}

// ***************** last_active Attributes **********************************
 
PhysicalLiteral
VHDLProcess::locateLastActiveAttribute(const VHDLType *sig) {
  PhysicalLiteral retval(ObjectBase::VARIABLE, 
		      "_lastActive",
		      PHYSICAL_MAX(), 
		      std_standard_timeTypeInfo()() );

  ASSERT(sig != NULL);

  switch(sig->get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    retval = locateLastActiveAttribute((const ScalarType*)sig);
    break;
  case ARRAY_TYPE:
    retval = locateLastActiveAttribute((const Array *) sig);
    break;
  case RECORD_TYPE:
    retval = locateLastActiveAttribute((const Record*)sig);
    break;
  default:
    cerr << "Unknown Type!! Asking for last_active Attribute !"
	 << endl;
    break;
  }
  return retval;
}

PhysicalLiteral
VHDLProcess::locateLastActiveAttribute(const ScalarType *sig) {
  PhysicalLiteral retval(ObjectBase::VARIABLE,
		      "_lastActive",
		      PHYSICAL_MAX(),
		      std_standard_timeTypeInfo()() );

  ASSERT(sig != NULL);

  retval = locateLastActiveAttribute((const SignalBase*)sig->getObject());

  return retval;
}			     

PhysicalLiteral
VHDLProcess::locateLastActiveAttribute(const Array *sig) {
  PhysicalLiteral retval(ObjectBase::VARIABLE,
		      "_lastActive",
		      PHYSICAL_MAX(),
		      std_standard_timeTypeInfo()() );
  PhysicalLiteral tempVal(ObjectBase::VARIABLE, 
		       "",
		       PHYSICAL_MAX(),
		       std_standard_timeTypeInfo()() );

  int noOfElements = sig->length(0);
  for(int i = 0; (i < noOfElements); i++) {
    tempVal = locateLastActiveAttribute((const VHDLType*)&(sig->getElement(i)));
    if( tempVal < retval ){
      retval = tempVal;
    }
  }
  
  return retval;
}


PhysicalLiteral
VHDLProcess::locateLastActiveAttribute(const Record* sig) {
  PhysicalLiteral retval( ObjectBase::VARIABLE, "_lastActive",
		       PHYSICAL_MAX(),
		       std_standard_timeTypeInfo()() );
  PhysicalLiteral tempVal( ObjectBase::VARIABLE, "",
			PHYSICAL_MAX(),
			std_standard_timeTypeInfo()() );

  int noOfElements = sig->get_number_of_fields();
  for(int i=0; i< noOfElements; i++) {
    tempVal = locateLastActiveAttribute((const VHDLType*)&(sig->get_field(i)));
    if( tempVal < retval ){
      retval = tempVal;
    }
  }
  return retval;
}

PhysicalLiteral
VHDLProcess::locateLastActiveAttribute(const SignalBase *sig) {
  vector<Attribute *> attribute;
  Attribute *attribval = NULL;
  PhysicalLiteral lastActiveTime;

  ASSERT(sig != NULL);

  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for (int i = 0; i < sig->getNumAttributes(); i++) {
    if(attribute[i]->attrib == LAST_ACTIVE){
      attribval = attribute[i];
      break;
    }
  }

  ASSERT(attribval != NULL && attribval->attrib == LAST_ACTIVE);

  lastActiveTime = attribval->value->getInt64Value();
  lastActiveTime = getTimeNow().getMajor() - lastActiveTime;
  return PhysicalLiteral(ObjectBase::VARIABLE, "_lastActive", lastActiveTime, std_standard_timeTypeInfo()() );
}

// ***************** last_value Attributes **********************************
 
VHDLType *
VHDLProcess::locateLastValueAttribute(const VHDLType *sig) {
  VHDLType *retval = 0;

  ASSERT(sig != NULL);

  switch(sig->get_kind()) {
  case INTEGER_TYPE:
  case REAL_TYPE:
  case ENUMERATION_TYPE:
  case PHYSICAL_TYPE:
    retval = locateLastValueAttribute((const ScalarType*)sig);
    break;
  case ARRAY_TYPE:
    retval = locateLastValueAttribute((const Array*) sig);
    break;
  case RECORD_TYPE:
    retval = locateLastValueAttribute((const Record*)sig);
    break;
  default:
    cerr << "Unknown Type!! Asking for last_value Attribute !"
	 << endl;
    break;
  }
  return retval;
}

VHDLType *
VHDLProcess::locateLastValueAttribute(const ScalarType *sig) {
  VHDLType *retval;
  static ScalarType* ptr = NULL;
  ObjectBase* objectPtr  = NULL;
  ASSERT(sig != NULL);

  retval = locateLastValueAttribute((const SignalBase*)sig->getObject());
  if(retval == NULL) {
    switch(((SignalBase*)sig->getObject())->getAllAttributes().sigtype) {
    case VHDLData::UNIVERSAL_BOOLEAN:
    case VHDLData::UNIVERSAL_INTEGER:
      objectPtr = new Variable<UniversalInteger>( UniversalInteger(((SignalBase*)sig->getObject())->getAllAttributes().last_value.inttype), "" );
      break;
    case VHDLData::UNIVERSAL_REAL:
      objectPtr = new Variable<UniversalReal>(UniversalReal(((SignalBase*)sig->getObject())->getAllAttributes().last_value.floattype), "" );
      break;
    case VHDLData::UNIVERSAL_LONG_LONG_INTEGER:
      objectPtr = new Variable<PhysicalLiteral>(PhysicalLiteral(((SignalBase*)sig->getObject())->getAllAttributes().last_value.physicaltype), "");
      break;
    default:
      objectPtr = NULL;
      break;
    }
    if(ptr != NULL) {
      delete ptr;
    }
    ptr = new UniversalInteger(ObjectBase::VARIABLE, "", sig->getTypeInfo() );
    ptr->setObject( objectPtr );
    retval = ptr;
  }
  return retval;
}			     

VHDLType *
VHDLProcess::locateLastValueAttribute(const Array* /* sig */) {
  VHDLType *retval = NULL ;

  cerr << "VHDLProcess<State>::locateLastValueAttribute() : This will not work" 
       << endl; 
  abort();

  return retval;
}


VHDLType *
VHDLProcess::locateLastValueAttribute(const Record* /* sig */) {
  VHDLType *retval = NULL;

  cerr << "VHDLProcess<State>::locateLastValueAttribute():This will not work" 
       << endl; 
  abort();
  return retval;
}

// Caution: Does not return a new'ed pointer.  Retruns the same poointer
// as in the attribute.

VHDLType*
VHDLProcess::locateLastValueAttribute(const SignalBase *sig) {
  vector<Attribute *> attribute;
  Attribute *attribval = NULL;

  ASSERT(sig != NULL);

  attribute = const_cast<SignalBase *>(sig)->getAttributeList();
  for (int i = 0; i < sig->getNumAttributes(); i++) {
    if(attribute[i]->attrib == LAST_VALUE){
      attribval = attribute[i];
    }
  }

  //  ASSERT(attribval != NULL && attribval->attrib == LAST_VALUE);
  if(attribval != NULL && attribval->attrib == LAST_VALUE) {
    return attribval->value;
  }
  else {
    return NULL;
  }
}
