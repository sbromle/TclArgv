#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([Machine Vision Test Harness], [2.0.0], [Sam Bromley <sam@sbnumerics.ca>], [mvth])
AM_INIT_AUTOMAKE(mvth,[2.0.0])

AC_CONFIG_SUBDIRS(libltdl)

AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Build libtool ltdl as a convenience library. 
AC_LIBTOOL_DLOPEN
AC_LIBLTDL_CONVENIENCE
AC_PROG_LIBTOOL

# set versions of the built libraries
AC_SUBST(libmvthversion,["0:2:0"])
AC_SUBST(libmemutilsversion,["0:2:0"])

# Checks for libraries.

# Dynamic loading Library
AC_CHECK_LIB([dl], [dlopen],[],[echo dl library not found; exit 0;])

#glib library
AM_PATH_GLIB_2_0

# Math Library
AC_CHECK_LIB([m], [sqrt],[],[echo math library not found; exit 0;])

# tcl
SC_PATH_TCLCONFIG
SC_LOAD_TCLCONFIG

# GNU Libtool dynamic loading Library
AC_CHECK_LIB([ltdl], [lt_dlopen],[],[echo libtool dynamic library not found; exit 0;])

AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h limits.h netdb.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h sys/timeb.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRCOLL
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit ftime ftruncate gethostbyname getpagesize gettimeofday memmove memset mkdir munmap rmdir select socket sqrt strchr strdup strerror strrchr strtol])

AC_DEFINE(MAXCHAINS,20,[set to maximum number of command chains])
AC_DEFINE(MAX_IMAGE_SLOTS,10,[set to maximum number of image buffers])
AH_TEMPLATE(DEBUG,turn on extra debugging checks/output)

AC_CONFIG_FILES([Makefile
                 src/Makefile
								 src/include/Makefile
								 src/memutils/Makefile
                 src/servers/Makefile
                 src/MVTHCORE/Makefile])
AC_OUTPUT
