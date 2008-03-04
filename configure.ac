#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([Tcl Argv Handling], [2.0.0], [Sam Bromley <sam@sambromley.com>], [tclargv])
AM_INIT_AUTOMAKE(tclargv,[2.0.0])

AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Build libtool ltdl as a convenience library. 
AC_PROG_LIBTOOL

# set versions of the built libraries
AC_SUBST(libtclargvversion,["0:2:0"])

# Checks for libraries.

# tcl
# Use the TEA (Tcl Extention Architecture) macros (in tcl.m4).
TEA_INIT([3.6])
# Load the tclConfig.sh file
# adds the --with-tcl configure option.
TEA_PATH_TCLCONFIG
TEA_LOAD_TCLCONFIG
# Define path to Tcl/Tk headers
# adds the --with-tclincludes configure option.
# and defines TCL_INCLUDES
TEA_PUBLIC_TCL_HEADERS
# If Tcl was compiles with threads, the following line enables threads here.
TEA_ENABLE_THREADS
# We should be using the Tcl stubs libraries.
AC_ARG_ENABLE([tclstubs],
 AS_HELP_STRING([--enable-tclstubs],[use Tcl stubs interface (default is YES)]),
 [case "${enableval}" in
   yes) ac_cv_enable_tclstubs=$enableval;;
	 no)  ac_cv_enable_tclstubs=$enableval;;
	 *) AC_MSG_ERROR([bad value ${enableval} for --enable-tclstubs]);;
	esac],[ac_cv_enable_tclstubs=yes])
AC_CACHE_CHECK([whether to use Tcl stubs],
               [ac_cv_enable_tclstubs],
							 [ac_cv_enable_tclstubs=yes])
AM_CONDITIONAL([COND_TCLSTUBS],[test "x$ac_cv_enable_tclstubs" = "xyes"])
if test "x${ac_cv_enable_tclstubs}" = "xyes"; then
	AC_DEFINE(USE_TCL_STUBS,1,[Use Tcl stubs])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_MODE_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_STRCOLL
AC_FUNC_STRTOD
AC_CHECK_FUNCS([strchr strdup strerror strrchr strtol])

AH_TEMPLATE(DEBUG,turn on extra debugging checks/output)

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
