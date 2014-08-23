cmake_minimum_required (VERSION 2.6)
project (ftpproxy C)

set (VERSION "2.1.0-beta5")

option (FTP_FILECOPY "Compile code for copies of transfered files." ON)
option (FACILITY_NAMES "Compile code for syslog facilities by name, requires compiler OS support." ON)

add_definitions (-DVERSION="${VERSION}")

if (FTP_FILECOPY)
	add_definitions (-DFTP_FILECOPY)
endif ()

if (FACILITY_NAMES)
	add_definitions (-DFACILITY_NAMES)
endif ()

add_subdirectory (doc)
add_subdirectory (src)
