AM_CFLAGS = -I$(top_builddir) -I$(top_srcdir)/src -I/usr/include/tcl8.4


pkglib_LTLIBRARIES = libTclParseArgv.la
libTclParseArgv_la_SOURCES = tclArgv.c tclArgv.h
libTclParseArgv_la_LDFLAGS = -export-dynamic @TCL_LIB_SPEC@

pkginclude_HEADERS = tclArgv.h
