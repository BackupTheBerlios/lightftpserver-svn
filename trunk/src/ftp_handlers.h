#include "ftp.h"
#include <stdio.h>

#ifndef _FTP_HANDLERS_H
#define _FTP_HANDLERS_H

typedef void (*ftphandler_t)(const char*, const char*, char*, unsigned int, bool*);
extern ftphandler_t ftphandlers[_FTPCMDS_END];

#endif
