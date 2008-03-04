AM_CFLAGS = -I$(top_builddir) -I$(top_srcdir)/src -I/usr/include/tcl8.4

if COND_TCLSTUBS
MYTCLLIBS = @TCL_STUB_LIB_SPEC@
else
MYTCLLIBS = @TCL_LIB_SPEC@
endif


pkglib_LTLIBRARIES = libTclParseArgv.la
libTclParseArgv_la_SOURCES = tclArgv.c tclArgv.h
libTclParseArgv_la_LDFLAGS = -export-dynamic $(MYTCLLIBS)

pkginclude_HEADERS = tclArgv.h
