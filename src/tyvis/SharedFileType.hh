#ifndef SHARED_FILETYPE_HH
#define SHARED_FILETYPE_HH

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
#include "tyvis/VHDLKernel.hh"
#include <string>

using std::string;

extern const int maxExtraInFilesInProcedure;
extern const int maxExtraOutFilesInProcedure;

#define SHARED_FILE_HANDLE -2

class SharedFileState : public VHDLKernel_state {
public:
  SharedFileState()  {
    wait = NULL;
  }
  ~SharedFileState() {}

  class SignalBase* locateSig(int);
  void cleanState();
  unsigned int getStateSize();

protected:

private:
};

class SharedFileObject : public VHDLData, public VHDLKernel {
public:

  SharedFileObject(const char *const name, bool alias = false);
  SharedFileObject(const SharedFileObject &);
  SharedFileObject(const char*, const EnumerationLiteral);
  SharedFileObject(const Array&, const EnumerationLiteral);

  virtual ~SharedFileObject();

  virtual Type get_kind() const;

  ObjectBase* getObject() const { return NULL; }

  virtual ObjectBase::ObjectType getKind() const;

  virtual void print(ostream& = cout) const;
  
  RValue *clone() const;
  
  EnumerationLiteral savantendfile_boolean(int);
  
  int get_fileHandle() const;
  EnumerationLiteral& get_fileOpenKind();

  void set_fileOpenKind(const EnumerationLiteral &);

  RValue &operator=(const RValue &);
  SharedFileObject& operator=(const SharedFileObject &);

  bool operator==( const RValue & ) const;
  bool operator!=( const RValue & ) const;
  bool operator>( const RValue & ) const;
  bool operator>=( const RValue & ) const;
  bool operator<( const RValue & ) const;
  bool operator<=( const RValue & ) const;

  EnumerationLiteral openKind;

  State* allocateState();

  /** This should never be called but has to be here due to pure virtual
      inheritence. */
  void executeVHDL( VHDLKernel_state &currentState, 
		    Block &scalarSignalsUpdateArray,
		    Block &compositeResolvedSignalsUpdateArray );

  const char *getFileName() const { return fileName.c_str(); }

  void initialize();
  void executeProcess();

  void deallocateState( const State *toDealloc ){ delete toDealloc; }
  
protected:

  string fileName;

private:
};

#endif
