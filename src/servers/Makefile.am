AM_LDFLAGS = -L$(top_builddir)/src/MVTHCORE @INCLTDL@ $(GLIB_LIBS)

AM_CFLAGS = -I$(top_builddir) -I$(top_srcdir)/src -I$(top_srcdir)/src/MVTHCORE \
-I$(top_srcdir)/include -DPLUGINDIR=\"$(pkglibdir)\"  $(GLIB_CFLAGS)

bin_PROGRAMS = test_unix_server 
bin_SCRIPTS = mvth_main_loop.tcl docmd.tcl help_cmd.tcl ls_cmd.tcl set_cmd.tcl

EXTRA_DIST = ${bin_SCRIPTS}

test_unix_server_SOURCES = test_unix_server.c
