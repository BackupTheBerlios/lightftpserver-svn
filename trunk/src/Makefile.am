bin_PROGRAMS = ftp_server
ftp_server_SOURCES = ftp_server.cpp utils.cpp client.cpp configUtils.cpp \
	logClient.cpp

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/logServer $(all_includes)

# the library search path.
ftp_server_LDFLAGS = $(all_libraries) 
noinst_HEADERS = commonheaders.h utils.h client.h configUtils.h logClient.h
SUBDIRS = logServer

