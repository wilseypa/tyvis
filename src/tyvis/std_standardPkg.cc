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

#include "std_standardPkg.hh"
#include "EnumerationTypeInfo.hh"
#include "PhysicalLiteral.hh"
#include "VHDLProcess.hh"
#include "VHDLVTime.hh"
#include <vector>
#include <string>

using std::vector;
using std::string;

vector<string> &std_standard_bitLiterals(){
  static vector<string> _literals;
  if( _literals.empty() ){
    _literals.push_back("'0'");
    _literals.push_back( "'1'" );
  }
  return _literals;
}

const EnumerationTypeInfo &std_standard_bitTypeInfo(){
  static EnumerationTypeInfo _std_standard_bitTypeInfo( std_standard_bitLiterals() );
  return _std_standard_bitTypeInfo;
}

static const char *std_standard_characterTypeInfo_imageMap[] =
{"nul","soh","stx", "etx", "eot", "enq", "ack", "bel", "bs", "ht", "lf",
 "vt", "ff", "cr", "so", "si", "dle", "dc1", "dc2", "dc3", "dc4", "nak",
 "syn", "etb", "can", "em", "sub", "esc", "fsp", "gsp", "rsp", "usp",
 "' '", "'!'", "'\"'", "'#'", "'$'", "'%'", "'&'", "'''", "'('", "')'", "'*'",
 "'+'", "','", "'-'", "'.'", "'/'", "'0'", "'1'", "'2'", "'3'", "'4'", "'5'",
 "'6'", "'7'", "'8'", "'9'", "':'", "';'", "'<'", "'='", "'>'", "'?'", "'@'",
 "'A'", "'B'", "'C'", "'D'", "'E'", "'F'", "'G'", "'H'", "'I'", "'J'", "'K'",
 "'L'", "'M'", "'N'", "'O'", "'P'", "'Q'", "'R'", "'S'", "'T'", "'U'", "'V'",
 "'W'", "'X'", "'Y'", "'Z'", "'['", "'\'", "']'", "'^'", "'_'", "'`'", "'a'",
 "'b'", "'c'", "'d'", "'e'", "'f'", "'g'", "'h'", "'i'", "'j'", "'k'", "'l'",
 "'m'", "'n'", "'o'", "'p'", "'q'", "'r'", "'s'", "'t'", "'u'", "'v'", "'w'",
 "'x'", "'y'", "'z'", "'{'", "'|'", "'}'", "'~'", "del", "c128", "c129",
 "c130", "c131", "c132", "c133", "c134", "c135", "c136", "c137", "c138",
 "c139", "c140", "c141", "c142", "c143", "c144", "c145", "c146", "c147",
 "c148", "c149", "c150", "c151", "c152", "c153", "c154", "c155", "c156",
 "c157", "c158", "c159", "' '", "'¡'", "'¢'", "'£'", "'¤'",
 "'¥'", "'¦'", "'§'", "'¨'", "'©'", "'ª'", "'«'",
 "'¬'", "'­'", "'®'", "'¯'", "'°'", "'±'", "'²'",
 "'³'", "'´'", "'µ'", "'¶'", "'·'", "'¸'", "'¹'",
 "'º'", "'»'", "'¼'", "'½'", "'¾'", "'¿'", "'À'",
 "'Á'", "'Â'", "'Ã'", "'Ä'", "'Å'", "'Æ'", "'Ç'",
 "'È'", "'É'", "'Ê'", "'Ë'", "'Ì'", "'Í'", "'Î'",
 "'Ï'", "'Ð'", "'Ñ'", "'Ò'", "'Ó'", "'Ô'", "'Õ'",
 "'Ö'", "'×'", "'Ø'", "'Ù'", "'Ú'", "'Û'", "'Ü'",
 "'Ý'", "'Þ'", "'ß'", "'à'", "'á'", "'â'", "'ã'",
 "'ä'", "'å'", "'æ'", "'ç'", "'è'", "'é'", "'ê'",
 "'ë'", "'ì'", "'í'", "'î'", "'ï'", "'ð'", "'ñ'",
 "'ò'", "'ó'", "'ô'", "'õ'", "'ö'", "'÷'", "'ø'",
 "'ù'", "'ú'", "'û'", "'ü'", "'ý'", "'þ'", "'ÿ'", 
 0 };

vector<string> &std_standard_characterLiterals(){
  static vector<string> _literals =
    TypeInfo::createImageMap( std_standard_characterTypeInfo_imageMap );
  return _literals;
}

const EnumerationTypeInfo &
std_standard_characterTypeInfo(){
  static EnumerationTypeInfo _std_standard_characterTypeInfo( std_standard_characterLiterals() );
  return _std_standard_characterTypeInfo;
}

vector<string> &std_standard_booleanLiterals(){
  static vector<string> _literals;
  if( _literals.empty() ){
    _literals.push_back("FALSE");
    _literals.push_back("TRUE");
  }
  return _literals;
}
const EnumerationTypeInfo &
std_standard_booleanTypeInfo(){
  static EnumerationTypeInfo _std_standard_booleanTypeInfo( std_standard_booleanLiterals() );
  return _std_standard_booleanTypeInfo;
}

vector<string> &
std_standard_severity_levelLiterals(){
  static vector<string> _literals;
  if( _literals.empty() ){
    _literals.push_back("NOTE");
    _literals.push_back( "WARNING" );
    _literals.push_back( "ERROR" );
    _literals.push_back( "FAILURE" );
  }
  return _literals;
}

const EnumerationTypeInfo &
std_standard_severity_levelTypeInfo(){
  static EnumerationTypeInfo _std_standard_severity_levelTypeInfo( std_standard_severity_levelLiterals() );
  return _std_standard_severity_levelTypeInfo;
}

vector<string> &std_standard_file_open_kindLiterals(){
  static vector<string> _literals;
  if( _literals.empty() ){
    _literals.push_back("READ_MODE");
    _literals.push_back( "WRITE_MODE" );
    _literals.push_back( "APPEND_MODE" );
  }
  return _literals;
}

const EnumerationTypeInfo
&std_standard_file_open_kindTypeInfo(){
  static EnumerationTypeInfo _std_standard_file_open_kindTypeInfo(std_standard_file_open_kindLiterals());
  return _std_standard_file_open_kindTypeInfo;
}

vector<string> &std_standard_file_open_statusLiterals(){
  static vector<string> _literals;
  if( _literals.empty() ){
    _literals.push_back("OPEN_OK");
    _literals.push_back("STATUS_ERROR");
    _literals.push_back("NAME_ERROR");
    _literals.push_back("MODE_ERROR");
  }
  return _literals;
}

const EnumerationTypeInfo &
std_standard_file_open_statusTypeInfo(){
  static EnumerationTypeInfo _std_standard_file_open_statusTypeInfo(std_standard_file_open_statusLiterals());
  return _std_standard_file_open_statusTypeInfo;
}

const IntegerTypeInfo &
std_standard_integerTypeInfo(){
  static IntegerTypeInfo _std_standard_integerTypeInfo( IntegerTypeInfo::getMin(),
							ArrayInfo::to,
							IntegerTypeInfo::getMax()
							);
  return _std_standard_integerTypeInfo;
}

const IntegerTypeInfo &
std_standard_naturalTypeInfo(){
  static IntegerTypeInfo _std_standard_naturalTypeInfo(0, ArrayInfo::to, IntegerTypeInfo::getMax());
  return _std_standard_naturalTypeInfo;
}

const IntegerTypeInfo &
std_standard_positiveTypeInfo(){
  static IntegerTypeInfo _std_standard_positiveTypeInfo(1, ArrayInfo::to, IntegerTypeInfo::getMax());
  return _std_standard_positiveTypeInfo;
}

const IntegerTypeInfo &
std_standard_widthTypeInfo(){
  static IntegerTypeInfo _std_standard_widthTypeInfo(0, ArrayInfo::to, IntegerTypeInfo::getMax());
  return _std_standard_widthTypeInfo;
}

const RealTypeInfo &std_standard_realTypeInfo(){
 static RealTypeInfo _std_standard_realTypeInfo(-1.0E38, ArrayInfo::to, 1.0E38);
 return _std_standard_realTypeInfo;
}

const vector<UnitDeclaration> &
std_standard_timeUnits(){
  static vector<UnitDeclaration> units;
  if( units.empty() ){
    units.push_back( UnitDeclaration("fs", PhysicalLiteral( 1, "fs" ) ) );
    units.push_back( UnitDeclaration("ps", PhysicalLiteral( 1000, "fs" ) ) );
    units.push_back( UnitDeclaration("ns", PhysicalLiteral( 1000, "ps" ) ) );
    units.push_back( UnitDeclaration("us", PhysicalLiteral( 1000, "ns" ) ) );
    units.push_back( UnitDeclaration("ms", PhysicalLiteral( 1000, "us" ) ) );
    units.push_back( UnitDeclaration("sec", PhysicalLiteral( 1000, "ms" ) ) );
    units.push_back( UnitDeclaration("min", PhysicalLiteral( 60, "sec" ) ) );
    units.push_back( UnitDeclaration("hr", PhysicalLiteral( 60, "min" ) ) );
  }
  return units;
}

const PhysicalTypeInfo &
std_standard_timeTypeInfo(){
  static PhysicalTypeInfo std_standard_timeTypeInfo;
  if( std_standard_timeTypeInfo.getPrimaryUnit() == "" ){
    std_standard_timeTypeInfo.addUnits( std_standard_timeUnits() );
  }
  return std_standard_timeTypeInfo;
}

const PhysicalTypeInfo &
std_standard_delay_lengthTypeInfo(){
  return std_standard_timeTypeInfo();
}

const IntegerTypeInfo &
std_standard_stringIndex0TypeInfo(){
  return std_standard_positiveTypeInfo();
}

const ArrayTypeInfo &
std_standard_stringTypeInfo(){
  static ArrayTypeInfo _std_standard_stringTypeInfo( std_standard_characterTypeInfo(),
						     std_standard_positiveTypeInfo() );
  return _std_standard_stringTypeInfo;
}

const IntegerTypeInfo &
std_standard_bit_vectorIndex0TypeInfo(){
  return std_standard_naturalTypeInfo();
}

const ArrayTypeInfo &
std_standard_bit_vectorTypeInfo(){
  static ArrayTypeInfo _std_standard_bit_vectorTypeInfo( std_standard_bitTypeInfo(),
							 std_standard_naturalTypeInfo() );
  return _std_standard_bit_vectorTypeInfo;
}

Value
std_standard_now_std_standard_delay_length(VHDLProcess *processPtr ){
  return PhysicalLiteral( std_standard_delay_lengthTypeInfo(),
			  processPtr->getTimeNow().getMajor() );
}
