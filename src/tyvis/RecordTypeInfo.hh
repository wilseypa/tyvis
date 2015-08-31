#ifndef RECORD_TYPE_INFO_HH
#define RECORD_TYPE_INFO_HH

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
#include "tyvis/CompositeTypeInfo.hh"
#include "tyvis/Types.hh"
#include "tyvis/ElementInfo.hh"

/** Info about Records. */
class RecordTypeInfo : public CompositeTypeInfo {
public:
  RecordTypeInfo();
  RecordTypeInfo( const vector<ElementInfo *> &initElements );
  RecordTypeInfo(const RecordTypeInfo&);
  ~RecordTypeInfo();

  TypeInfo *clone() const;
  bool operator==( const TypeInfo &that ) const;

  string getSubFieldName( const string &prefix, int d ) const;
  const ElementInfo &getFieldInfo( const int ) const;
  int getFieldCount() const;
  int length() const;

  void addField( const ElementInfo &toAdd );

  RecordTypeInfo &operator=( const RecordTypeInfo & );

  RValue *create( const SubElementFactory *parent ) const;

  bool isCompositeType() const { return true; }

  /**
     The vector passed in will be filled with Elements corresponding to
     this RecordTypeInfo.
  */
  void constructFields( vector<RValue *> &elements, 
			const SubElementFactory *subElementFactory,
			int numElements = -1 ) const;

  const TypeInfo &getElementTypeInfo( const RValue &index = UniversalInteger(1) ) const;
  const ScalarTypeInfo &getIndexTypeInfo( const RValue &index = UniversalInteger(1) ) const;

  bool isConstrained() const { return true; }
  bool isUnconstrained() const { return false; }
  virtual void debugPrintDetails( ostream &os, int indent ) const;

private:
  void cloneElementInfos( const vector<ElementInfo *> &those );
  vector<ElementInfo *> myElementInfos;
  /**
     Even though we're a RecordType, the methods used to traverse the type
     are the same for Records and arrays.  This type info is used to
     traverse this type info.
  */
  mutable const ScalarTypeInfo *indexTypeInfo;
};

#endif
