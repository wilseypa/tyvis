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

#include "tyvis/std_textioPkg.hh"
#include "tyvis/AccessObject.hh"
#include "tyvis/FileObject.hh"
#include <clutils/StringUtilities.h>

FileObject &
getstd_textio__inputObject(){
  static FileObject *std_textio_input = new FileObject(
    std_textio_textTypeInfo(),
    "stdin",
    (EnumerationLiteral(
      std_standard_file_open_kindTypeInfo(), 
      0
    ))
    
  )
  ;
  return *std_textio_input;
}

FileObject &
std_textio__outputObject(){
  static FileObject *std_textio_output = new FileObject(
    std_textio_textTypeInfo(),
    "stdout",
    (EnumerationLiteral(
      std_standard_file_open_kindTypeInfo(), 
      1
    ))
    
  )
  ;
  return *std_textio_output;
}

int
std_textio_textlinesavantreadline(VHDLProcess *processPtr,
                                  const RValue &f,
                                  LValue &l ){
  RValue &file = const_cast<RValue &>(f);
  dynamic_cast<FileObject &>(file).readline( l );
  dynamic_cast<AccessObject &>(l.readVal()).reset();
  return NORMAL_RETURN;
}

int
std_textio_linebit_vectorsidewidthsavantwrite( VHDLProcess *processPtr,
					       LValue &l,
					       const RValue &value,
					       const RValue &justified,
					       const RValue &field ){
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  
  return NORMAL_RETURN;
}

int
std_textio_textlinesavantwriteline(VHDLProcess *processPtr,
				   LValue &f,
				   LValue &l ){
  dynamic_cast<FileObject &>(f).writeline( l );
  l.updateVal(VHDL_STRING(""));
  return NORMAL_RETURN;
}

int
std_textio_linestringsidewidthsavantwrite( VHDLProcess *processPtr,
					   LValue &l,
					   const RValue &value,
					   const RValue &justified,
					   const RValue &field ){
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  return NORMAL_RETURN;
}

int
std_textio_linecharactersavantread(VHDLProcess *processPtr,
                                   LValue &l,
                                   LValue &value ){
  ASSERT(std_standard_characterTypeInfo().getImageMap() == value.getTypeInfo().getImageMap());
  AccessObject &lineObject = dynamic_cast<AccessObject &>(l.readVal());
  if( lineObject.getCurrentPos() == lineObject.length().getIntValue()){
    value.updateVal(EnumerationLiteral(std_standard_characterTypeInfo(),
                          UniversalInteger(13)));
  }
  else
  {
    string lineString = lineObject.toString();
    value.updateVal(EnumerationLiteral(std_standard_characterTypeInfo(), 
			    string("'") + lineString[0] + string("'")));
    if( lineObject.getCurrentPos() < lineObject.length().getIntValue()){
      lineObject.advance(1);
    }
  }
  return NORMAL_RETURN;
}

// TODO: Make this call lineintegerbooleansavantread
int
std_textio_lineintegersavantread(VHDLProcess *processPtr,
	                         LValue &l,
                                 LValue &value ){
  string lineString = l.readVal().toString();
  int intEnd;
  LONG longVal = stringToLong(lineString, intEnd);
  value.updateVal(UniversalInteger(longVal));
  return NORMAL_RETURN;
}

int
std_textio_lineintegerbooleansavantread(VHDLProcess *processPtr,
					LValue &l,
					LValue &value,
					LValue &good ){
  string lineString = l.readVal().toString();
  int intEnd;
  LONG longVal = stringToLong(lineString, intEnd);
  value.updateVal(UniversalInteger(longVal));
  // TODO: Need to determine if the read was successful.
  good.updateVal(EnumerationLiteral(std_standard_booleanTypeInfo(), "TRUE"));
  return NORMAL_RETURN;
}

			       
int
std_textio_linetimesavantread(VHDLProcess *processPtr,
	                      LValue &l,
			      LValue &value ){
  string lineString = l.readVal().toString();
  int unitStart = 0;
  LONG initMultiplier =  stringToLong( lineString, unitStart);
  string initUnit;
  for(unsigned int i = unitStart+1; i < lineString.length();i++){
    initUnit += lineString[i];
  }
  value.updateVal(PhysicalLiteral(std_standard_timeTypeInfo(),initMultiplier, initUnit));
  return NORMAL_RETURN;
}

int
std_textio_linerealsavantread( VHDLProcess *processPtr,
                               LValue &l,
                               LValue &value ){
  string lineString = l.readVal().toString();
  int realEnd;
  double realVal = stringToDouble(lineString, realEnd);
  value.updateVal(UniversalReal(realVal));
  return NORMAL_RETURN;
}

int
std_textio_linestringsavantread(VHDLProcess *processPtr,
                                LValue &l,
                                LValue &value ){
  string lineString = l.readVal().toString();
  value.updateVal(VHDL_STRING(lineString));
  return NORMAL_RETURN;
}

int
std_textio_linebitsavantread( VHDLProcess *processPtr,
                              LValue &l,
                              LValue &value ){
  AccessObject lineObject(dynamic_cast<AccessObject &>(l.readVal()));
  string lineString = l.readVal().toString();
  value.updateVal(EnumerationLiteral(std_standard_bitTypeInfo(), 
			  string("'") + lineString[lineObject.getCurrentPos()] +string("'")));
  dynamic_cast<AccessObject &>(l.readVal()).advance(1);
  return NORMAL_RETURN;
}

int
std_textio_linebit_vectorsavantread( VHDLProcess *processPtr,
                                     LValue &l,
                                     LValue &value ){
  string lineString = l.readVal().toString();
  string bvString;
  int basePos = dynamic_cast<AccessObject &>(l.readVal()).getCurrentPos();
  for (int i=0; i < value.length().getIntValue(); i++){
    bvString += lineString[i+basePos];
  }
  dynamic_cast<AccessObject &>(l.readVal()).advance(value.length().getIntValue());
  value.updateVal(StringLiteral(ArrayTypeInfo(std_standard_bitTypeInfo(),
				  std_standard_bit_vectorIndex0TypeInfo()), bvString));
  return NORMAL_RETURN;
}

int
std_textio_linebooleansavantread( VHDLProcess *processPtr,
                                  LValue &l,
                                  LValue &value ){
  string lineString = l.readVal().toString();
  int basePos = dynamic_cast<AccessObject &>(l.readVal()).getCurrentPos();
  string boolString;
  for (int i=0; i < string("TRUE").length(); i++){
    boolString += lineString[i+basePos];
  }
  if (boolString == "TRUE"){
    value.updateVal(EnumerationLiteral(std_standard_booleanTypeInfo(), "TRUE"));
    dynamic_cast<AccessObject &>(l.readVal()).advance(4);
  }
  else{
    boolString += lineString[(string("TRUE").length())+basePos];
    if( boolString == "FALSE"){
      value.updateVal(EnumerationLiteral(std_standard_booleanTypeInfo(), "FALSE")); 
      dynamic_cast<AccessObject &>(l.readVal()).advance(5);
    }
    else{ 
      cerr << "Error: std_textio_linebooleansavantread - BOOLEAN not in expected form" << endl;
      abort();
    }
  }
  return NORMAL_RETURN;  
}

int
std_textio_linebitsidewidthsavantwrite(VHDLProcess *processPtr,
                                       LValue &l,
                                       const RValue &value,
                                       const RValue &justified,
                                       const RValue &field ){
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  return NORMAL_RETURN;
}
	
int
std_textio_linetimesidewidthtimesavantwrite(VHDLProcess *processPtr,
                                            LValue &l,
	                                    const RValue &value,
					    const RValue &justified,
					    const RValue &field,
					    const RValue &unit ){
 
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  return NORMAL_RETURN; 
}

int
std_textio_linecharactersidewidthsavantwrite(VHDLProcess *processPtr,
                                           LValue &l,
                                           const RValue &value,
                                           const RValue &justified,
                                           const RValue &field ){
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  return NORMAL_RETURN;
}

int
std_textio_lineintegersidewidthsavantwrite(VHDLProcess *processPtr,
                                           LValue &l,
                                           const RValue &value,
                                           const RValue &justified,
                                           const RValue &field ){
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  return NORMAL_RETURN;
}

int
std_textio_linerealsidewidthnaturalsavantwrite(VHDLProcess *processPtr,
                                               LValue &l,
                                               const RValue &value,
			                       const RValue &justified,
                                               const RValue &field,
 	                                       const RValue &digits ){
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  return NORMAL_RETURN;
}

int
std_textio_linebooleansidewidthsavantwrite(VHDLProcess *processPtr,
                                           LValue &l,
                                           const RValue &value,
                                           const RValue &justified,
                                           const RValue &field ){
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  return NORMAL_RETURN;
}

int
std_textio_linestringbooleansavantread(VHDLProcess *processPtr,
                                       LValue &l,
                                       LValue &value,
                                       LValue &good ){
  const RValue &newValue = l.readVal().vhdlConcatenate( value );
  l.updateVal( newValue );
  good.updateVal(EnumerationLiteral(std_standard_booleanTypeInfo(), "TRUE"));
  return NORMAL_RETURN;
    
}


