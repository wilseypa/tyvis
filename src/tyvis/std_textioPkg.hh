#ifndef std_textioPkg_hh
#define std_textioPkg_hh

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
#include "tyvis/IntegerTypeInfo.hh"
#include "tyvis/AccessTypeInfo.hh"
#include "tyvis/FileTypeInfo.hh"
#include "tyvis/EnumerationTypeInfo.hh"
#include "tyvis/VHDLProcess.hh"
#include "tyvis/STDTypes.hh"
#include "tyvis/FileObject.hh"
#include "tyvis/std_textioPkg.hh"

// This package defines the runtime functions for the functions in the VHDL
// textio package. 

inline const AccessTypeInfo &
std_textio_lineTypeInfo(){
  static AccessTypeInfo _ti(std_standard_stringTypeInfo());
  return _ti;
}

inline const FileTypeInfo &
std_textio_textTypeInfo(){
  static FileTypeInfo _ti(std_standard_stringTypeInfo());
  return _ti;
}

inline static vector<string> &
std_textio_sideLiterals(){
  static vector<string> _literals;
  if( _literals.size() == 0 ){
    _literals.push_back("right");
    _literals.push_back("left");
    
  }
  return _literals;
}
inline const EnumerationTypeInfo &
std_textio_sideTypeInfo(){
  static EnumerationTypeInfo _ti(std_textio_sideLiterals());
  return _ti;
}

inline const IntegerTypeInfo &
std_textio_widthTypeInfo(){
  static IntegerTypeInfo _ti(0,
  ArrayInfo::to, 
  2147483647);
  return _ti;
};

FileObject &
std_textio__inputObject();

FileObject &
std_textio__outputObject();

int
std_textio_textlinesavantreadline(VHDLProcess *processPtr,
				  const RValue &f,
				  LValue &l );

int
std_textio_linebitbooleansavantread( VHDLProcess *processPtr,
				     LValue &l,
				     LValue &value,
				     LValue &good );
int
std_textio_linebitsavantread( VHDLProcess *processPtr,
			      LValue &l,
			      LValue &value );
int
std_textio_linebit_vectorbooleansavantread(VHDLProcess *processPtr,
					   LValue &l,
					   LValue &value,
					   LValue &good );

int
std_textio_linebit_vectorsavantread( VHDLProcess *processPtr,
				    LValue &l,
				    LValue &value );
int
std_textio_linebooleanbooleansavantread( VHDLProcess *processPtr,
					 LValue &l,
					 LValue &value,
					 LValue &good );
int
std_textio_linebooleansavantread( VHDLProcess *processPtr,
				  LValue &l,
				  LValue &value );
int
std_textio_linecharacterbooleansavantread( VHDLProcess *processPtr,
					   LValue &l,
					   LValue &value,
					   LValue &good);
int
std_textio_linecharactersavantread(VHDLProcess *processPtr,
				   LValue &l,
				   LValue &value );
int
std_textio_lineintegerbooleansavantread(VHDLProcess *processPtr,
					LValue &l,
					LValue &value,
					LValue &good );
int
std_textio_lineintegersavantread(VHDLProcess *processPtr,
				 LValue &l,
				 LValue &value );
int
std_textio_linerealbooleansavantread(VHDLProcess *processPtr,
				     LValue &l,
				     LValue &value,
				     LValue &good );
int
std_textio_linerealsavantread( VHDLProcess *processPtr,
			       LValue &l,
			       LValue &value );
int
std_textio_linestringbooleansavantread(VHDLProcess *processPtr,
				       LValue &l,
				       LValue &value,
				       LValue &good );
int
std_textio_linestringsavantread(VHDLProcess *processPtr,
				LValue &l,
				LValue &value );
int
std_textio_linetimebooleansavantread(VHDLProcess *processPtr,
				     LValue &l,
				     LValue &value,
				     LValue &good );
int
std_textio_linetimesavantread(VHDLProcess *processPtr,
			      LValue &l,
			      LValue &value );

int
std_textio_textlinesavantwriteline(VHDLProcess *processPtr,
				   LValue &f,
				   LValue &l );

int
std_textio_linebitsidewidthsavantwrite(VHDLProcess *processPtr,
				       LValue &l,
				       const RValue &value,
				       const RValue &justified,
				       const RValue &field);

int
std_textio_linebit_vectorsidewidthsavantwrite(VHDLProcess *processPtr,
					      LValue &l,
					      const RValue &value,
					      const RValue &justified,
					      const RValue &field );

int
std_textio_linebooleansidewidthsavantwrite(VHDLProcess *processPtr,
					   LValue &l,
					   const RValue &value,
					   const RValue &justified,
					   const RValue &field );

int
std_textio_linecharactersidewidthsavantwrite(VHDLProcess *processPtr,
					     LValue &l,
					     const RValue &value,
					     const RValue &justified,
					     const RValue &field );

int
std_textio_lineintegersidewidthsavantwrite(VHDLProcess *processPtr,
					   LValue &l,
					   const RValue &value,
					   const RValue &justified,
					   const RValue &field );

int
std_textio_linerealsidewidthnaturalsavantwrite(VHDLProcess *processPtr,
					       LValue &l,
					       const RValue &value,
					       const RValue &justified,
					       const RValue &field,
					       const RValue &digits );

int
std_textio_linestringsidewidthsavantwrite( VHDLProcess *processPtr,
					   LValue &l,
					   const RValue &value,
					   const RValue &justified,
					   const RValue &field );

int
std_textio_linetimesidewidthtimesavantwrite(VHDLProcess *processPtr,
					    LValue &l,
					    const RValue &value,
					    const RValue &justified,
					    const RValue &field,
					    const RValue &unit );

#endif
