#include "ftp.h"
#include <stdio.h>

#ifndef _FTP_HANDLERS_H
#define _FTP_HANDLERS_H

#include "client.h"
#include "ftp.h"
#include "callback.h"


#define MESSAGE_SIZE 64

struct CommandHandler{
	int code;
	char message[MESSAGE_SIZE];
	ClassCallback callback;
	
	int Handle(CFTPClient *instance, TParam1 param1, TParam2 param2);
};

/*
typedef void (*ftphandler_t)(const char*, const char*, char*, unsigned int, bool*);
extern ftphandler_t ftphandlers[_FTPCMDS_END];
*/

extern CommandHandler handlers[];
extern int cHandlers; //number of handlers we have

#endif
