dnl Process this file with autoconf to produce a configure script.
AC_INIT(tyvis, [2.0-pre031216])
AC_CONFIG_SRCDIR([src/tyvis/VHDLProcess.cc])
AM_INIT_AUTOMAKE([1.14 foreign subdir-objects -Wall])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PREREQ(2.57)
AC_CONFIG_MACRO_DIR([m4])
LT_INIT

AC_CONFIG_HEADERS( [src/tyvis/tyvis-config.h:src/tyvis/tyvis-config.h.in] )
AC_LANG([C++])
AC_SUBST(TYVIS_VERSION,[2.0-pre031216])

AC_ARG_ENABLE( [vhdl-ams], 
	AC_HELP_STRING([--enable-vhdl-ams], [Enable VHDL AMS support in tyvis build]),
	AC_DEFINE( VHDL_AMS, 1, [Did the user ask for VHDL AMS support?] )
)
AM_CONDITIONAL([VHDL_AMS], test "$enable_vhdl_ams" = yes)

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL
AC_CHECK_FUNCS([sqrt])
AC_CHECK_FUNCS([pow])

m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])

dnl Checks for libraries.
m4_include([m4/acinclude.m4])
CL_LIB_WARPED
CL_LIB_CLUTILS
CL_LIB_CPPUNIT
CL_INSTALL_AMS

AC_ARG_VAR([SCRAM], [The name of the scram executable to use during the build])
AC_CHECK_PROG( [SCRAM], [scram], [scram], [NOT_FOUND] )

AC_ARG_VAR([SCRAM_LIB_PATH], [Set based on $SCRAM --echo-library-directory call])

if test "x$SCRAM" = "xNOT_FOUND"; then
  AC_MSG_WARN([Did not find scram in path - cannot generate ieee libraries.
This could cause problems down the road.])
else
  SCRAM_LIB_PATH=`$SCRAM --echo-library-directory`
  AC_SUBST([SCRAM_LIB_PATH])
fi

dnl Checks for header files.
AC_CHECK_HEADERS(limits.h)

AM_CONDITIONAL([CPPUNIT_TESTS], test "x$cl_cv_lib_cppunit" != "xno" )

AC_OUTPUT( [Makefile test/Makefile src/Makefile src/plugin/Makefile] )

rm -rf ./config.cache
