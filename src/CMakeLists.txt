set (ftpproxy_SOURCES
	config.c
	daemon.c
	filecopy.c
	ftp.c
	ip-lib.c
	lib.c
	main.c
	procinfo.c)

set (ftpproxy_HEADERS
	ftp.h
	ip-lib.h
	lib.h
	procinfo.h)

add_executable (ftp.proxy
	${ftpproxy_SOURCES}
	${ftpproxy_HEADERS})

install (TARGETS ftp.proxy
	RUNTIME DESTINATION bin)