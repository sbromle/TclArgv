AM_CFLAGS = -I$(top_builddir) -I$(top_srcdir)/src -I$(top_srcdir)/include \
$(GLIB_CFLAGS) \
-DDLSEARCHPATH=\"$(libdir):$(pkglibdir)\" -DPLUGINDIR=\"$(pkglibdir)\" \
-I/usr/include/tcl8.4

lib_LTLIBRARIES = libMemUtils.la

libMemUtils_la_LDFLAGS = -export-dynamic -version-info @libmemutilsversion@

libMemUtils_la_SOURCES = \
memUtils.c \
memUtils.h

include_HEADERS = \
memUtils.h 
