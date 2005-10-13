#ifndef FTP_COMMON_HEADERS_H
#define FTP_COMMON_HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef WIN32 //only when building with DEV-CPP
#include <conio.h>
#endif

#include "utils.h"

/*
Current verbosity level ... if a log entry has a biger or equal 
verbosity level to this one it will be printed
*/
extern int Verbosity;

/*
File where all the log output will be shown
*/
extern FILE *LogFile;

#endif
