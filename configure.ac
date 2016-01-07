dnl Process this file with autoconf to produce a configure script.
AC_INIT(tyvis, [2.0-pre031216])
AC_CONFIG_SRCDIR([src/tyvis/VHDLProcess.cc])
AM_INIT_AUTOMAKE([1.14 foreign subdir-objects])
AC_PREREQ(2.57)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_CONFIG_HEADER( src/tyvis/tyvis-config.h )

AC_SUBST(TYVIS_VERSION,[2.0-pre031216])

AC_ARG_ENABLE( [vhdl-ams], 
	AC_HELP_STRING([--enable-vhdl-ams], [Enable VHDL AMS support in tyvis build]),
	AC_DEFINE( VHDL_AMS, 1, [Did the user ask for VHDL AMS support?] )
)
AM_CONDITIONAL([VHDL_AMS], test "$enable_vhdl_ams" = yes)

dnl Checks for programs.
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

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

dnl Checks for libraries.
CL_LIB_CLUTILS
CL_LIB_WARPED
CL_INSTALL_AMS
CL_LIB_CPPUNIT
AM_CONDITIONAL([CPPUNIT_TESTS], test "x$cl_cv_lib_cppunit" != "xno" )

AC_SUBST(CPPFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST(LIBS)
AC_SUBST(CXX)

CXXFLAGS="$CXXFLAGS -std=c++11 -Wall"

AC_OUTPUT( [Makefile test/Makefile src/Makefile src/plugin/Makefile] )

rm -rf ./config.cache
