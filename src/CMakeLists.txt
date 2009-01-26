include (FindTCL)

include_directories(${TCL_INCLUDE_PATH})
include_directories(.)

########### next target ###############

SET (libTclParseArgv_SRCS
	tclArgv.c tclArgv.h
)

add_library(TclParseArgv ${libTclParseArgv_SRCS})
target_link_libraries(TclParseArgv ${TCL_LIBRARY})
set_target_properties (TclParseArgv PROPERTIES VERSION 1.0 SOVERSION 1 INSTALL_RPATH_USE_LINK_PATH on)

########### install files ###############

# Headers 
install(FILES tclArgv.h DESTINATION include/${PACKAGE})
install(TARGETS TclParseArgv DESTINATION lib/${PACKAGE})
