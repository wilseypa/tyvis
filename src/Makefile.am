# Process this file with automake to produce Makefile.in

# Copyright (c) 2000-2003 The University of Cincinnati.  
# All rights reserved.

# UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
# SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
# OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
# LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR DISTRIBUTING
# THIS SOFTWARE OR ITS DERIVATIVES.

# By using or copying this Software, Licensee agrees to abide by the
# intellectual property laws, and all other applicable laws of the U.S.,
# and the terms of this license.

# You may modify, distribute, and use the software contained in this
# package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
# version 2, June 1991. A copy of this license agreement can be found in
# the file "LGPL", distributed with this archive.

# Authors: Adam Siefker 
#          Dale Martin  dmartin@cliftonlabs.com
#	   Vinod Chamarty chamarv@ececs.uc.edu

SUBDIRS=plugin

if VHDL_AMS
LIBAMS = libAMS.la
AMS_EXTRA_DIST = \
	tyvis/AMSProcess.hh \
	tyvis/AMSProcess.cc \
	tyvis/Component.hh \
	tyvis/Component.cc \
	tyvis/FreeEquation.hh \
	tyvis/FreeEquation.cc
	tyvis/AMSProcess.hh \
	tyvis/AMSProcess.cc \
	tyvis/Component.hh \
	tyvis/Component.cc \
	tyvis/FreeEquation.hh

AMS_H_FILES = \
	tyvis/vhdl_ams.hh \
	tyvis/AMSApplication.hh \
	tyvis/_savant_entity_ams_elab.hh \
	tyvis/AMSType.hh \
	tyvis/Quantity.hh \
	tyvis/Terminal.hh \
	tyvis/AMSKernel_state.hh \
	tyvis/AMSKernel.hh \
	tyvis/Node.hh \
	tyvis/NodeList.hh \
	tyvis/QuantityNode.hh \
	tyvis/ContributionNode.hh \
	tyvis/Tolerance.hh \
	tyvis/Constants.hh \
	tyvis/BranchEquation.hh \
	tyvis/AMSProcess.hh \
	tyvis/Component.hh \
	tyvis/FreeEquation.hh \
	tyvis/DifferentialEquation.hh
libAMS_la_LDFLAGS =
libAMS_la_LIBADD = -lwarped -lVHDL

libAMS_la_SOURCES = \
	tyvis/AMSApplication.cc \
	tyvis/_savant_entity_ams_elab.cc \
	tyvis/AMSType.cc \
	tyvis/Quantity.cc \
	tyvis/Terminal.cc \
	tyvis/Component.cc \
	tyvis/AMSKernel_state.cc \
	tyvis/AMSKernel.cc \
	tyvis/AMSProcess.cc \
	tyvis/NodeList.cc \
	tyvis/QuantityNode.cc \
	tyvis/ContributionNode.cc \
	tyvis/Tolerance.cc \
	tyvis/BranchEquation.cc \
	tyvis/FreeEquation.cc \
	tyvis/DifferentialEquation.cc
endif

libVHDL_la_LIBADD = -lwarped
lib_LTLIBRARIES = libVHDL.la $(LIBAMS)

#files with template classes upset make when listed in sources.
#They are needed for make dist so put them in as extras
EXTRA_DIST= \
	tyvis/tyvis-config.h.in \
	$(AMS_EXTRA_DIST) \
	tyvis/ImplicitSignal.hh \
	tyvis/ImplicitSignal.cc \
	tyvis/Signal.hh \
	tyvis/Signal.cc \
	tyvis/Variable.hh \
	tyvis/Variable.cc 

libVHDL_la_SOURCES = \
	tyvis/_savant_entity_elab.cc \
	tyvis/std_standardPkg.cc \
	tyvis/std_textioPkg.cc \
	tyvis/vhdl.cc \
	tyvis/AdditionalDrivers.cc \
	tyvis/AdditionalDrivers.hh \
	tyvis/AccessTypeInfo.cc \
	tyvis/AccessObject.cc \
	tyvis/Aggregate.cc \
	tyvis/Alias.cc \
	tyvis/ArrayInfo.cc \
	tyvis/ArrayTypeInfo.cc \
	tyvis/Block.cc \
	tyvis/CallStack.cc \
	tyvis/CompositeLiteral.cc \
	tyvis/CompositeTypeInfo.cc \
	tyvis/EnumerationTypeInfo.cc \
	tyvis/EventSigs.cc \
	tyvis/FileTypeInfo.cc \
	tyvis/GlobalControl.cc \
	tyvis/Hierarchy.cc \
	tyvis/DefaultAttributes.cc \
	tyvis/Element.cc \
	tyvis/ElementInfo.cc \
	tyvis/EnumerationLiteral.cc \
	tyvis/FanDest.cc \
	tyvis/FanOutList.cc \
	tyvis/FileObject.cc \
	tyvis/ForeignProcess.cc \
	tyvis/ForeignProcessFactory.cc \
	tyvis/ForeignArchitecture.cc \
	tyvis/IntegerLiteral.cc \
	tyvis/IntegerTypeInfo.cc \
	tyvis/ObjectBase.cc \
	tyvis/PhysicalLiteral.cc \
	tyvis/PhysicalTypeInfo.cc \
	tyvis/PortMap.cc \
	tyvis/PortAssociation.cc \
	tyvis/RangeIterator.cc \
	tyvis/RealTypeInfo.cc \
	tyvis/Record.cc \
	tyvis/RecordTypeInfo.cc \
	tyvis/RValue.cc \
	tyvis/Slice.cc \
	tyvis/Slice.hh \
	tyvis/SignalBase.cc \
	tyvis/Signal.cc \
	tyvis/StringLiteral.cc \
	tyvis/StringLiteral.hh \
	tyvis/UnitDeclaration.cc \
	tyvis/UniversalInteger.cc \
	tyvis/UniversalReal.cc \
	tyvis/Value.cc \
	tyvis/Variable.cc \
	tyvis/ScalarTypeInfo.cc \
	tyvis/SignalNetinfo.cc \
	tyvis/SourceBase.cc \
	tyvis/SourceData.cc \
	tyvis/SourceTypeConvert.cc \
	tyvis/SourceInfo.cc \
	tyvis/SubElementFactory.cc \
	tyvis/Transaction.cc \
	tyvis/TransactionList.cc \
	tyvis/Types.cc \
	tyvis/TypeConvert.cc \
	tyvis/TypeInfo.cc \
	tyvis/TyvisPluginLoader.cc \
	tyvis/TyvisVarArgs.cc \
	tyvis/VCD.cc \
	tyvis/VHDLApplication.cc \
	tyvis/VHDLData.cc \
	tyvis/VHDLKernel.cc \
	tyvis/VHDLKernel_state.cc \
	tyvis/VHDLProcess.cc \
	tyvis/VHDLVTime.cc \
	tyvis/Wait.cc

#	tyvis/Record.hh


library_includedir=$(includedir)/tyvis
library_include_HEADERS=  \
	$(AMS_H_FILES) \
	tyvis/_savant_entity_elab.hh \
	tyvis/vhdl.hh \
	tyvis/std_standardPkg.hh \
	tyvis/std_textioPkg.hh \
	tyvis/AccessTypeInfo.hh \
	tyvis/AccessObject.hh \
	tyvis/AdditionalDrivers.hh \
	tyvis/Aggregate.hh \
	tyvis/Alias.hh \
	tyvis/ArrayInfo.hh \
	tyvis/ArrayTypeInfo.hh \
	tyvis/Attribute.hh \
	tyvis/Block.hh \
	tyvis/CallStack.hh \
	tyvis/CompositeLiteral.hh \
	tyvis/CompositeTypeInfo.hh \
	tyvis/DefaultAttributes.hh \
	tyvis/Element.hh \
	tyvis/ElementInfo.hh \
	tyvis/ElementAssociation.hh \
	tyvis/EnumerationLiteral.hh \
	tyvis/EnumerationTypeInfo.hh \
	tyvis/EventSigs.hh \
	tyvis/FanDest.hh \
	tyvis/FanOutList.hh \
	tyvis/FileObject.hh \
	tyvis/FileTypeInfo.hh \
	tyvis/ForeignArchitecture.hh \
	tyvis/ForeignProcess.hh \
	tyvis/ForeignProcessFactory.hh \
	tyvis/GlobalControl.hh \
	tyvis/Hierarchy.hh \
	tyvis/ImplicitSignal.hh \
	tyvis/IntegerLiteral.hh \
	tyvis/IntegerTypeInfo.hh \
	tyvis/KernelTypes.hh \
	tyvis/LValue.hh \
	tyvis/ObjectBase.hh \
	tyvis/PhysicalLiteral.hh \
	tyvis/PhysicalTypeInfo.hh \
	tyvis/PortAssociation.hh \
	tyvis/PortMap.hh \
	tyvis/RangeIterator.hh \
	tyvis/RealTypeInfo.hh \
	tyvis/Record.hh \
	tyvis/RecordTypeInfo.hh \
	tyvis/RValue.hh \
	tyvis/STDTypes.hh \
	tyvis/ScalarTypeInfo.hh \
	tyvis/Signal.hh \
	tyvis/SignalBase.hh \
	tyvis/SignalNetinfo.hh \
	tyvis/Slice.hh \
	tyvis/SourceBase.hh \
	tyvis/SourceData.hh \
	tyvis/SourceInfo.hh \
	tyvis/SourceTypeConvert.hh \
	tyvis/StringLiteral.hh \
	tyvis/Transaction.hh \
	tyvis/TransactionList.hh \
	tyvis/TypeConvert.hh \
	tyvis/TypeInfo.hh \
	tyvis/TypeKind.hh \
	tyvis/Types.hh \
	tyvis/TyvisPluginLoader.hh \
	tyvis/TyvisVarArgs.hh \
	tyvis/UniversalInteger.hh \
	tyvis/UniversalReal.hh \
	tyvis/UnitDeclaration.hh \
	tyvis/Value.hh \
	tyvis/VCD.hh \
	tyvis/VHDLApplication.hh \
	tyvis/VHDLData.hh \
	tyvis/VHDLEvent.hh \
	tyvis/VHDLKernel.hh \
	tyvis/VHDLKernel_state.hh \
	tyvis/VHDLProcess.hh \
	tyvis/VHDLVTime.hh \
	tyvis/Variable.hh \
	tyvis/Wait.hh \
	tyvis/WaitEvent.hh \
        tyvis/tyvis-config.h


