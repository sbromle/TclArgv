include (FindTclStub)
option (USE_TCL_STUBS "Enable use of TCL stubs library")
option (USE_CPP_ARGS "Enable use of Tcl_GetArgsFromObjs variadic template function. Requires C++11")

include_directories(${TCL_INCLUDE_PATH})
include_directories(.)

########### next target ###############

SET (libTclParseArgv_SRCS
	tclArgv.c tclArgv.h
)

if (USE_CPP_ARGS)
	SET (libTclParseArgv_SRCS
		${libTclParseArgv_SRCS}
		tclCppArgs.cpp
	)
endif (USE_CPP_ARGS)

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
if (USE_CPP_ARGS)
	SET (CMAKE_CXX_FLAGS
		${CMAKE_CXX_FLAGS}
		-std=gnu++11
	)
endif(USE_CPP_ARGS)
set_target_properties (TclParseArgv PROPERTIES VERSION 1.0 SOVERSION 1 INSTALL_RPATH_USE_LINK_PATH on INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

########### install files ###############

# Headers 
install(FILES tclArgv.h DESTINATION include/${PACKAGE} COMPONENT headers)
if (USE_CPP_ARGS)
	install(FILES tclCppArgs.h DESTINATION include/${PACKAGE} COMPONENT headers)
endif (USE_CPP_ARGS)
install(TARGETS TclParseArgv DESTINATION lib COMPONENT libraries)
