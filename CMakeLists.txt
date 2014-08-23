cmake_minimum_required (VERSION 2.6)
project (ftpproxy C)

set (VERSION "2.1.0-beta5")

option (FTP_FILECOPY "Compile code for copies of transfered files." ON)
option (FACILITY_NAMES "Compile code for syslog facilities by name, requires compiler OS support." ON)

add_definitions (-DVERSION="${VERSION}")

if (FTP_FILECOPY)
	add_definitions (-DFTP_FILECOPY)
endif ()

include(CheckIncludeFile)
check_include_file("syslog.h" HAVE_SYSLOG_H)

if (FACILITY_NAMES AND HAVE_SYSLOG_H)
	add_definitions (-DFACILITY_NAMES)
endif ()

configure_file (
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories (
	${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory (doc)
add_subdirectory (src)
