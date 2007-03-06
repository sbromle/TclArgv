AM_CFLAGS = -I$(top_builddir) -I$(top_srcdir)/src -I$(top_srcdir)/include \
$(GLIB_CFLAGS) \
-DDLSEARCHPATH=\"$(libdir):$(pkglibdir)\" -DPLUGINDIR=\"$(pkglibdir)\" \
-I/usr/include/tcl8.4


pkglib_LTLIBRARIES = mvthcore.la
pkglib_SCRIPTS = pkgIndex.tcl
lib_LTLIBRARIES = libTclParseArgv.la
libTclParseArgv_la_SOURCES = tclArgv.c tclArgv.h
libTclParseArgv_la_LDFLAGS = -export-dynamic @TCL_LIB_SPEC@

mvthcore_la_SOURCES = mvth_init.c \
mvthCore.c \
generic_contexts_types.h \
generic_contexts.h \
generic_contexts.c \
version.h \
version.c 

mvthcore_la_LDFLAGS= -module -export-dynamic @TCL_LIB_SPEC@  $(GLIB_LIBS)

include_HEADERS = \
generic_contexts_types.h \
generic_contexts.h \
tclArgv.h

EXTRA_DIST = ${pkglib_SCRIPTS}
