#include "ftp.h"
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef _FTP_HANDLERS_H
#define _FTP_HANDLERS_H

/* extern struct passwd ftp_user; */

typedef void (*ftphandler_t)(const char*, const char*, char*, unsigned int, bool*);
extern ftphandler_t ftphandlers[_FTPCMDS_END];

#endif
