#ifndef STD_STANDARDPKG_HH
#define STD_STANDARDPKG_HH

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
#include "tyvis/ArrayTypeInfo.hh"
#include "tyvis/EnumerationTypeInfo.hh"
#include "tyvis/IntegerTypeInfo.hh"
#include "tyvis/RealTypeInfo.hh"
#include "tyvis/PhysicalTypeInfo.hh"
#include "tyvis/PhysicalLiteral.hh"

vector<string> &std_standard_bitLiterals();
const EnumerationTypeInfo &std_standard_bitTypeInfo();
vector<string> &std_standard_characterLiterals();
const EnumerationTypeInfo &std_standard_characterTypeInfo();
vector<string> &std_standard_booleanLiterals();
const EnumerationTypeInfo &std_standard_booleanTypeInfo();
vector<string> &std_standard_severity_levelLiterals();
const EnumerationTypeInfo &std_standard_severity_levelTypeInfo();
vector<string> &std_standard_file_open_kindLiterals();
const EnumerationTypeInfo &std_standard_file_open_kindTypeInfo();
vector<string> &std_standard_file_open_statusLiterals();
const EnumerationTypeInfo &std_standard_file_open_statusTypeInfo();

const IntegerTypeInfo &std_standard_integerTypeInfo();
const IntegerTypeInfo &std_standard_naturalTypeInfo();
const IntegerTypeInfo &std_standard_positiveTypeInfo();
const IntegerTypeInfo &std_standard_widthTypeInfo();

const RealTypeInfo &std_standard_realTypeInfo();

const vector<UnitDeclaration> &std_standard_timeUnits();
const PhysicalTypeInfo &std_standard_timeTypeInfo();
const PhysicalTypeInfo &std_standard_delay_lengthTypeInfo();

const IntegerTypeInfo &std_standard_stringIndex0TypeInfo();
const ArrayTypeInfo &std_standard_bit_vectorTypeInfo();
const IntegerTypeInfo &std_standard_bit_vectorIndex0TypeInfo();
const ArrayTypeInfo &std_standard_stringTypeInfo();

#define SAVANT_BOOLEAN_TRUE  EnumerationLiteral::getEnumerationTrue()
#define SAVANT_BOOLEAN_FALSE EnumerationLiteral::getEnumerationFalse()
#define SAVANT_READ_MODE     EnumerationLiteral::getEnumerationReadMode()
#define SAVANT_WRITE_MODE    EnumerationLiteral::getEnumerationWriteMode()
#define SAVANT_ZERO_TIME     PhysicalTypeInfo::getZeroTime()
#define SAVANT_INFINITY_TIME PhysicalTypeInfo::getZeroTime()
#define OPEN_OK              EnumerationLiteral( std_standard_file_open_statusTypeInfo(), 0 )
#define STATUS_ERROR         EnumerationLiteral( std_standard_file_open_statusTypeInfo(), 1 )
#define NAME_ERROR           EnumerationLiteral( std_standard_file_open_statusTypeInfo(), 2 )
#define MODE_ERROR           EnumerationLiteral( std_standard_file_open_statusTypeInfo(), 3 )


Value std_standard_now_std_standard_delay_length(VHDLProcess *processPtr );

#ifndef savantnow
#define savantnow std_standard_now_std_standard_delay_length(processPtr)
#endif

#endif

