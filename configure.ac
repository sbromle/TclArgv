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
SC_PATH_TCLCONFIG
SC_LOAD_TCLCONFIG

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
