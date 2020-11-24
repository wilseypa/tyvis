dnl Check for libclutils
dnl
dnl Usage: CL_LIB_CLUTILS
dnl
AC_DEFUN([CL_LIB_CLUTILS],
[
AC_CACHE_CHECK([for location of libclutils], [cl_cv_lib_clutils],
  [AC_ARG_WITH(clutils,
    AC_HELP_STRING([--with-clutils],
      [location of libclutils (default is /usr)]),
      [
        case "${withval}" in
          yes)  cl_cv_lib_clutils=/usr ;;
          no)   cl_cv_lib_clutils=no ;;
          *)    cl_cv_lib_clutils=$withval ;;
        esac
      ],
      [cl_cv_lib_clutils=/usr]
  )]
)
if test $cl_cv_lib_clutils = no ; then
AC_MSG_ERROR(The clutils package is required.)
fi
LDFLAGS="$LDFLAGS -L$cl_cv_lib_clutils/lib"
CPPFLAGS="$CPPFLAGS -I$cl_cv_lib_clutils/include/clutils-0.0"
AC_CHECK_HEADER( [Debug.h], ,
  AC_MSG_ERROR( [Couldn't find clutils-0.0/Debug.h which is required and 
    part of the clutils distribution.  You probably need to install it
    and/or specify the location of it with the --with-clutils option.] ) )
AC_SEARCH_LIBS(dlopen, [dl])
AC_CHECK_LIB([clutils], [fake], ,
  AC_MSG_ERROR([Could not find a workable libclutils.so or libclutils.a.
    You probably need to install clutils and/or specify the location of
    it with the --with-clutils option.]), [-lltdl] )
]) dnl end CL_LIB_CLUTILS


dnl Check for libwarped
dnl
dnl Usage: CL_LIB_WARPED
dnl
AC_DEFUN([CL_LIB_WARPED],
[
AC_CACHE_CHECK([for location of libwarped], [cl_cv_lib_warped],
  [AC_ARG_WITH(warped,
    AC_HELP_STRING([--with-warped],
      [location of libwarped (default is /usr)]),
      [
        case "${withval}" in
          yes)  cl_cv_lib_warped=/usr ;;
          no)   cl_cv_lib_warped=no ;;
          *)    cl_cv_lib_warped=$withval ;;
        esac
      ],
      [cl_cv_lib_warped=/usr]
  )]
)
if test $cl_cv_lib_warped = no ; then
AC_MSG_ERROR(The warped package is required.)
fi

if test $cl_cv_lib_warped != /usr ; then
LDFLAGS="$LDFLAGS -L$cl_cv_lib_warped/lib"
CPPFLAGS="$CPPFLAGS -I$cl_cv_lib_warped/include"
fi
AC_CHECK_HEADER(LogicalProcess.hpp, ,
  AC_MSG_ERROR(Couldn't find warped/LogicalProcess.hpp which is required and 
    part of the warped2 distribution.  You probably need to install it
    and/or specify the location of it with the --with-warped option.))
AC_CHECK_LIB([warped], [warped_is_present], ,
  AC_MSG_ERROR(Could not find a workable libwarped.so or libwarped.a.
    You probably need to install warped2 and/or specify the location of
    it with the --with-warped option.), -lltdl )
]) dnl end CL_LIB_WARPED

Check for cppunit

Usage: CL_LIB_CPPUNIT

AC_DEFUN([CL_LIB_CPPUNIT],
[
AC_CACHE_CHECK([for location of libcppunit], [cl_cv_lib_cppunit],
  [AC_ARG_WITH(cppunit,
    AC_HELP_STRING([--with-cppunit],
      [location of libcppunit (default is /usr)]),
      [
        case "${withval}" in
          yes)  cl_cv_lib_cppunit=/usr ;;
          no)   cl_cv_lib_cppunit=no ;;
          *)    cl_cv_lib_cppunit=$withval ;;
        esac
      ],
      [cl_cv_lib_cppunit=/usr]
  )]
)
if test $cl_cv_lib_cppunit = no ; then
#   Disable the build of the unit tests
  AC_MSG_NOTICE([cpp_unit_disabled via --with-cppunit])
  AC_SUBST(CPP_UNIT_TEST_DIR, "")
else
dnl We might want to add addition checks for CPPUNIT headers later
AC_CHECK_HEADER(cppunit/Test.h, ,
  AC_MSG_WARN(Couldn't find cppunit/TestCase.h which is optional but
    required to run the unit tests in the 'test' directory. This file is
    part of the cppunit distribution.  You probably need to install it
    and/or specify the location of it with the --with-cppunit option.))

dnl Check for the CPPUNIT library
AC_CHECK_LIB(cppunit, main, 
  [AC_SUBST(CPP_UNIT_TEST_DIR,[test]) \
        LIBS="$LIBS -lcppunit"], 
  [AC_SUBST(CPP_UNIT_TEST_DIR, "") \
  AC_MSG_WARN(Couldn't find cppunit library which is optional but
    required to run the unit tests in the 'test' directory. This file is
    part of the cppunit distribution.  You probably need to install it
    and/or specify the location of it with the --with-cppunit option.)
    cl_cv_lib_cppunit=no])

if test $cl_cv_lib_cppunit != /usr ; then
  if test $cl_cv_lib_cppunit != no ; then
  LDFLAGS="$LDFLAGS -L$cl_cv_lib_cppunit/lib"
  CPPFLAGS="$CPPFLAGS -I$cl_cv_lib_cppunit/include"
  fi
fi

fi

]) dnl end CL_LIB_WARPED

dnl Install VHDLAMS
dnl Usage: CL_INSTALL_AMS
dnl
AC_DEFUN([CL_INSTALL_AMS],
[
if test "$enable_vhdl_ams" = yes ; then
AC_CACHE_CHECK([for location of ADOLC and SPARSE], [cl_cv_lib_ams],
  [AC_ARG_WITH(ams,
    AC_HELP_STRING([--with-ams],
      [for VHDL_AMS installation(points to directory which contains SPARSE and ADOLC packages)]),
      [
        case "${withval}" in
          yes)  cl_cv_lib_ams=/usr ;;
          no)   cl_cv_lib_ams=no ;;
          *)    cl_cv_lib_ams=$withval ;;
        esac
      ],
      [cl_cv_lib_ams=/usr]
  )]
)

if test $cl_cv_lib_ams = no ; then
   Disable the build of the ams
  AC_MSG_WARN("vhdl_ams_install_disabled via --with-ams")
  AC_SUBST(AMS_KERNEL_SRC_DIR, "")
  return;
fi
if test $cl_cv_lib_ams != /usr ; then
LDFLAGS="$LDFLAGS -L$cl_cv_lib_ams/ADOLC18/SRC"
CPPFLAGS="$CPPFLAGS -I$cl_cv_lib_ams/ADOLC18/SRC"
fi

AC_CHECK_LIB(ad,gradient,
  [AC_SUBST(AMS_KERNEL_SRC_DIR, [src]) \
        LIBS="$LIBS -lad"],
  [AC_SUBST(AMS_KERNEL_SRC_DIR, "") \
  AC_MSG_WARN(Couldn't find ADOLC package-required for VHDL-AMS installation.)
               ])

dnl We might want to add addition checks for ADOLC headers later
AC_CHECK_HEADER(adouble.h, ,
  AC_MSG_WARN(Couldn't find ADOLC18/SRC/adouble.h which is required for
    installing tyvis distribution with VHDL-AMS. You probably need to
    install it and/or specify the location of it with the --with-ams option.))

AC_CHECK_HEADER(adutils.h, ,
  AC_MSG_WARN(Couldn't find ADOLC18/SRC/adutils.h which is required for
    installing tyvis distribution with VHDL-AMS. You probably need to
    install it and/or specify the location of it with the --with-ams option.))
if test $cl_cv_lib_ams != /usr ; then
LDFLAGS="$LDFLAGS -L$cl_cv_lib_ams/sparse"
CPPFLAGS="$CPPFLAGS -I$cl_cv_lib_ams/sparse"
fi

dnl Not defining AMS_KERNEL_SRC_DIR for true case below as it may be false for ADOLC
AC_CHECK_LIB(sparse,spCreate,
        LIBS="$LIBS -lsparse",
  [AC_SUBST(AMS_KERNEL_SRC_DIR, "") \
  AC_MSG_WARN(Couldn't find 'sparse' package - required for VHDL-AMS installation)
               ])

dnl We might want to add addition checks for 'sparse' headers later

AC_CHECK_HEADER(spmatrix.h, ,
  AC_MSG_WARN(Couldn't find sparse/spmatrix.h which is required for
    installing tyvis distribution with VHDL-AMS. You probably need to
    install it and/or specify the location of it with the --with-ams option.))

AC_CHECK_HEADER(spconfig.h, ,
  AC_MSG_WARN(Couldn't find sparse/spconfig.h which is required for
    installing tyvis distribution with VHDL-AMS. You probably need to
    install it and/or specify the location of it with the --with-ams option.))
else
  AC_MSG_NOTICE([Skipping VHDL-AMS header and library tests, VHDL-AMS not enabled])
fi
])

