set (PACKAGE tclargv)
project (${PACKAGE})
cmake_minimum_required (VERSION 1.0)

option (BUILD_SHARED_LIBS "Build Shared Libraries." ON)

# Set up some variables that we can use in our code.
set (TCLARGV_VERSION 1.0)
set (TCLARGV_MAJOR_VERSION 1)
set (TCLARGV_MINOR_VERSION 0)
set (TCLARGV_PATCH_LEVEL 0)
set (TCLSCRIPTDIR ${CMAKE_INSTALL_PREFIX}/lib/${PACKAGE})
set (VERSION ${TCLARGV_VERSION})
add_definitions (-DHAVE_CONFIG_H)
include_directories (${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Fix for static library linking to shared library on 64 bit systems:
# with -fPIC
IF(UNIX AND NOT WIN32)
  FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
  IF(CMAKE_UNAME)
    EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
    SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL
			"processor type (i386 and x86_64)")
		 IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
		   ADD_DEFINITIONS(-fPIC)
		 ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
  ENDIF(CMAKE_UNAME)
ENDIF(UNIX AND NOT WIN32)

add_subdirectory(src)
