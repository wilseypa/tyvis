# Process this file with automake to produce Makefile.in

# Copyright (c) 2000 The University of Cincinnati.  
# All rights reserved.

# UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
# SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
# OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
# LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR
# DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

# By using or copying this Software, Licensee agrees to abide by the
# intellectual property laws, and all other applicable laws of the U.S.,
# and the terms of this license.

# You may modify, distribute, and use the software contained in this
# package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
# version 2, June 1991. A copy of this license agreement can be found in
# the file "LGPL", distributed with this archive.

# Authors: Adam Siefker asiefker@cliftonlabs.com

if CPPUNIT_TESTS
noinst_PROGRAMS = tyvisUnitTests
tyvisUnitTests_LDADD = $(top_builddir)/src/libVHDL.la
tyvisUnitTests_LDFLAGS = -static

AM_CPPFLAGS=-I$(top_srcdir)/src

#	ArrayTypeTest.cc \
#	ArrayTypeTest.hh \
#	EnumerationTypeTest.cc \
#	EnumerationTypeTest.hh \
#	FileOpenKindTest.cc \
#	FileOpenKindTest.hh \
#	FileOpenStatusTest.cc \
#	FileOpenStatusTest.hh \
#	SignalTest.cc \
#	SignalTest.hh \
#	SignalBaseTest.cc \
#	SignalBaseTest.hh
#	SharedFileEventTest.hh \
#	SharedFileEventTest.cc
#	SigEventTest.hh \
#	SigEventTest.cc 

tyvisUnitTests_SOURCES = \
	main.cc	\
	main.hh \
	AggregateTest.cc \
	AggregateTest.hh \
	ArrayLiteralTest.cc \
	ArrayLiteralTest.hh \
	ArrayTypeInfoTest.cc \
	ArrayTypeInfoTest.hh \
	BlockTest.cc \
	BlockTest.hh \
	CompositeLiteralTest.cc \
	CompositeLiteralTest.hh \
	EnumerationLiteralTest.cc \
	EnumerationLiteralTest.hh \
	EnumerationTypeInfoTest.cc \
	EnumerationTypeInfoTest.hh \
	IntegerTypeInfoTest.cc \
	ObjectBaseTest.cc \
	ObjectBaseTest.hh \
	PhysicalLiteralTest.cc \
	PhysicalLiteralTest.hh \
	PhysicalTypeInfoTest.cc \
	PhysicalTypeInfoTest.hh \
	RealTypeInfoTest.cc \
	RealTypeInfoTest.hh \
	RecordTest.cc \
	RecordTest.hh \
	RecordTypeInfoTest.cc \
	RecordTypeInfoTest.hh \
	SignalTest.cc \
	SignalTest.hh \
	SliceTest.cc \
	SliceTest.hh \
	StringLiteralTest.cc \
	StringLiteralTest.hh \
	UniversalIntegerTest.cc \
	UniversalIntegerTest.hh \
	UniversalRealTest.cc \
	UniversalRealTest.hh \
	VariableTest.hh \
	VariableTest.cc \
	VHDLDataTest.hh \
	VHDLDataTest.cc 
	VHDLEventTest.hh \
	VHDLEventTest.cc \
	WaitEventTest.hh \
	WaitEventTest.cc
endif
