include (FindTclStub)
option (USE_TCL_STUBS "Enable use of TCL stubs library")

include_directories(${TCL_INCLUDE_PATH})
include_directories(.)

########### next target ###############

SET (libTclParseArgv_SRCS
	tclArgv.c tclArgv.h
	${LIBTCLPARSEARGV_BUILDINFO}
)

add_library(TclParseArgv ${libTclParseArgv_SRCS})
if (USE_TCL_STUBS)
	if (DEFINED TCL_STUB_LIBRARY)
		set (TCLLIB ${TCL_STUB_LIBRARY})
	else (DEFINED TCL_STUB_LIBRARY)
		message(FATAL_ERROR "TCL_STUB_LIBRARY not defined.")
	endif (DEFINED TCL_STUB_LIBRARY)
else (USE_TCL_STUBS)
	set (TCLLIB ${TCL_LIBRARY})
endif (USE_TCL_STUBS)
target_link_libraries(TclParseArgv ${TCLLIB})
set_target_properties (TclParseArgv PROPERTIES VERSION 1.0 SOVERSION 1 INSTALL_RPATH_USE_LINK_PATH on)

########### install files ###############

# Headers 
install(FILES tclArgv.h DESTINATION include/${PACKAGE} COMPONENT headers)
install(TARGETS TclParseArgv DESTINATION lib/${PACKAGE} COMPONENT libraries)
