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

template <class Type>
inline
ImplicitSignal<Type>::ImplicitSignal() :
  SignalBase( IMPLICIT, true, drvVal.getUniversalKind() ),
  timeval( VHDLVTime::getVHDLVTimeZero() ){
  source = NULL;
}

template <class Type>
inline
ImplicitSignal<Type>::ImplicitSignal(const int i)
  : SignalBase( IMPLICIT, true, drvVal.getUniversalKind() ),
    timeval( VHDLVTime::getVHDLVTimeZero() ){
  source = NULL;
  drvVal = Type(i);
  effVal = Type(i);
}

template <class Type>
inline
ImplicitSignal<Type>::ImplicitSignal(const VHDLData& val):
  SignalBase( IMPLICIT,
	      true,
	      val.getUniversalKind() ),
  timeval( VHDLVTime::getVHDLVTimeZero() ){
  source = NULL;
  this->operator=(val);
}


template <class Type>
inline
ImplicitSignal<Type>::ImplicitSignal(const char* newname) : SignalBase(newname), 
  timeval( VHDLVTime::getVHDLVTimeZero() ) {
  sensitive = true;
  numAttributes = 0;
  attributeList = NULL;
  source = NULL;
}


template <class Type>
inline void
ImplicitSignal<Type>::updateDriver( const int, 
				    int,
				    const VHDLData*,
				    const ArrayInfo*,
				    const ArrayInfo*) {
  // An implicit signal will not have any driver.  So there is no need to
  // update it.
}


template <class Type>
inline VHDLType*
ImplicitSignal<Type>::readDriver(VHDLKernel *) {
  // An implicit signal does not have any driver.
  return NULL;
}


template <class Type>
inline void
ImplicitSignal<Type>::updateDrvVal(const VHDLData* data, const ArrayInfo*,
				       const ArrayInfo*) {
  drvVal = *(Type*)data;
}


template <class Type>
inline VHDLData*
ImplicitSignal<Type>::readDrvVal() {
  return &drvVal;
}


template <class Type>
inline void
ImplicitSignal<Type>::updateEffVal( const RValue *newVal,
				    const ArrayInfo*,
				    const ArrayInfo*) {
  effVal = *newVal;
}

template <class Type>
inline void
ImplicitSignal<Type>::updateEffVal( const RValue &newVal, const ArrayInfo*,
				       const ArrayInfo*) {
  effVal = newVal;
}

template <class Type>
inline RValue *
ImplicitSignal<Type>::readEffVal() {
  return &effVal;
}


template <class Type>
inline VHDLVTime&
ImplicitSignal<Type>::readTimeVal() {
  return timeval;
}


template <class Type>
inline VHDLVTime&
ImplicitSignal<Type>::updateTimeVal(const VHDLVTime& v) {
  timeval = v;
  return timeval;
}

template <class Type>
inline ObjectBase&
ImplicitSignal<Type>:: operator=(const ObjectBase& s) {
  // ImplicitSignal<Type> sig = (ImplicitSignal<Type>&) s;

  SignalBase::operator=((const SignalBase &) s);
  
  timeval = ((ImplicitSignal<Type> &) s).timeval;
  source  = ((ImplicitSignal<Type> &) s).source;
  drvVal  = ((ImplicitSignal<Type> &) s).drvVal;
  effVal  = ((ImplicitSignal<Type> &) s).effVal;
  
  // Yes, these make multiple pointers to the same memory.  Since the
  // contents of this memory never change, it is allocated once at the
  // beginning of the simulation, and deleted at the end.  SO, everyone
  // looks at the same memory, and it never needs to be
  // newd/deleted/copied.
  //fanDest = sig.fanDest;
  //attributeList = sig.attributeList;
  return *this;
}

template <class Type>
inline ObjectBase&
ImplicitSignal<Type>:: operator=(const VHDLData& s) {
  //This function is used only during initialization
  // source = s;
  drvVal = s;
  effVal = s;
  return *this;
}

// if you can figure out how to make operator<< link for this, do it!
template <class Type>
inline void 
ImplicitSignal<Type>::print(ostream& os) const {
  register int i;

  os << "ImplicitSignal \"" << getName() << "\" (ID " << getSigId() << ") of type " 
     << getType() << ":" << endl;
  os << " driving value: " << drvVal << endl << " effective value: " 
     << effVal << endl;
  os << endl;
  os << "   current timeval: " << timeval << endl;

  if( getFanOut() > 0 ){
    os << "    fanouts: ";
    for (i = 0; i < getFanOut(); i++) {
      os << getFanDest()[i] << " ";
    }
    os << endl;
  }
  if( getNumAttributes() > 0) {
    os << "    Attributes: ";
    for (i = 0; i < getNumAttributes(); i++) {
      os << const_cast<ImplicitSignal<Type> *>(this)->getAttributeList()[i] << " ";
    }
    os << endl;
  }
}
