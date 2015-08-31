# Process this file with automake to produce Makefile.in

# Copyright (c) 2000-2007 The University of Cincinnati.  
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

AM_CPPFLAGS= @CPPFLAGS@ -DDEVELOPER_ASSERTIONS -I$(top_srcdir)/src
AM_CXXFLAGS= @CXXFLAGS@

SUBDIRS= src test

EXTRA_DIST= Makefile.am.common

doxygen:
	cd $(top_srcdir)/doc; doxygen doxygen.config

bin_PROGRAMS = tyvis

tyvis_SOURCES = \
	src/tyvis/tyvis.cc

tyvis_LDFLAGS = -static -export-dynamic

tyvis_LDADD = src/libVHDL.la

IEEE_DIR=@SCRAM_LIB_PATH@/ieee._savant_lib

IEEE_CPP_FILES = \
	$(IEEE_DIR)/ieee_std_logic_1164Pkg.hh \
	$(IEEE_DIR)/ieee_std_logic_1164Pkg.cc \
	$(IEEE_DIR)/ieee_std_logic_1164PkgBody.hh \
	$(IEEE_DIR)/ieee_std_logic_1164PkgBody.cc \
	$(IEEE_DIR)/ieee_numeric_bitPkg.cc \
	$(IEEE_DIR)/ieee_numeric_bitPkg.hh \
	$(IEEE_DIR)/ieee_numeric_stdPkg.cc \
	$(IEEE_DIR)/ieee_numeric_stdPkg.hh \
	$(IEEE_DIR)/ieee_vital_memoryPkg.hh \
	$(IEEE_DIR)/ieee_vital_primitivesPkg.hh \
	$(IEEE_DIR)/ieee_vital_timingPkg.hh \
	$(IEEE_DIR)/ieee_vital_memoryPkg.cc \
	$(IEEE_DIR)/ieee_vital_primitivesPkg.cc \
	$(IEEE_DIR)/ieee_vital_timingPkg.cc \
	$(IEEE_DIR)/ieee_std_logic_arithPkg.hh \
	$(IEEE_DIR)/ieee_std_logic_arithPkg.cc \
	$(IEEE_DIR)/ieee_std_logic_unsignedPkg.hh \
	$(IEEE_DIR)/ieee_std_logic_unsignedPkg.cc \
	$(IEEE_DIR)/ieee_std_logic_unsignedPkgBody.hh \
	$(IEEE_DIR)/ieee_std_logic_unsignedPkgBody.cc \
	$(IEEE_DIR)/ieee_std_logic_signedPkg.hh \
	$(IEEE_DIR)/ieee_std_logic_signedPkg.cc \
	$(IEEE_DIR)/ieee_std_logic_signedPkgBody.hh \
	$(IEEE_DIR)/ieee_std_logic_signedPkgBody.cc \
	$(IEEE_DIR)/ieee_std_logic_textioPkg.hh \
	$(IEEE_DIR)/ieee_std_logic_textioPkg.cc \
	$(IEEE_DIR)/ieee_std_logic_textioPkgBody.hh \
	$(IEEE_DIR)/ieee_std_logic_textioPkgBody.cc \
	$(IEEE_DIR)/ieee_attributesPkg.hh \
	$(IEEE_DIR)/ieee_attributesPkg.cc \
	$(IEEE_DIR)/ieee_attributesPkgBody.hh \
	$(IEEE_DIR)/ieee_attributesPkgBody.cc \
	$(IEEE_DIR)/ieee_std_logic_miscPkg.hh \
	$(IEEE_DIR)/ieee_std_logic_miscPkg.cc \
	$(IEEE_DIR)/ieee_std_logic_miscPkgBody.hh \
	$(IEEE_DIR)/ieee_std_logic_miscPkgBody.cc \
	$(IEEE_DIR)/ieee_math_complexPkg.hh \
	$(IEEE_DIR)/ieee_math_complexPkg.cc \
	$(IEEE_DIR)/ieee_math_complexPkgBody.hh \
	$(IEEE_DIR)/ieee_math_complexPkgBody.cc \
	$(IEEE_DIR)/ieee_math_realPkg.hh \
	$(IEEE_DIR)/ieee_math_realPkg.cc \
	$(IEEE_DIR)/ieee_math_realPkgBody.hh \
	$(IEEE_DIR)/ieee_math_realPkgBody.cc

IEEE_VHDL_FILES = \
        $(IEEE_DIR)/std_logic_1164.vhd \
        $(IEEE_DIR)/timing_p.vhd \
        $(IEEE_DIR)/timing_b.vhd \
        $(IEEE_DIR)/numeric_bit.vhd \
        $(IEEE_DIR)/numeric_std.vhd \
        $(IEEE_DIR)/prmtvs_p.vhd \
        $(IEEE_DIR)/prmtvs_b.vhd \
        $(IEEE_DIR)/memory_p.vhd \
        $(IEEE_DIR)/memory_b.vhd \
	$(IEEE_DIR)/std_logic_arith.vhd \
        $(IEEE_DIR)/std_logic_unsigned.vhd \
        $(IEEE_DIR)/std_logic_signed.vhd \
        $(IEEE_DIR)/std_logic_textio.vhd \
        $(IEEE_DIR)/attributes.vhd \
        $(IEEE_DIR)/std_logic_misc.vhd \
        $(IEEE_DIR)/math_complex.vhd \
        $(IEEE_DIR)/math_real.vhd

IEEE_LA_FILE = $(IEEE_DIR)/ieee._savant_lib.la

PROCESS_IEEE_LIBS=$(SCRAM) --design-library-name ieee --publish-cc

$(IEEE_CPP_FILES): $(IEEE_VHDL_FILES) $(libdir)/libtyvisplugin.la
	$(PROCESS_IEEE_LIBS) $(IEEE_VHDL_FILES)
	touch $(IEEE_CPP_FILES)

$(IEEE_LA_FILE): $(IEEE_CPP_FILES)
	$(MAKE) -C $(IEEE_DIR)

all-local: $(IEEE_LA_FILE)

