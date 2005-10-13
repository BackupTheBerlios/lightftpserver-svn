#ifndef FTP_UTILS_H
#define FTP_UTILS_H

#include "commonheaders.h"
/*
Initializes the log file.
*/
int LogInit();

/*
Closes the log file (if the file is not stdin, stdout or stderr)
*/
int LogDestroy();

/*
Writes a line to the log file.
Can have a variable number of parameters and behaves in the same manner as printf
(uses the same escape sequences - %s, %c, %d, ...)
nNeededVerbosity - is the needed verbosity level for the log event to show
format - the format of the event (can include %s, %c, %d, ...)
*/
int Log(int nNeededVerbosity, char *format, ...);

/*
Returns the local time as a string using the current locale
szTime - string where the time will be saved
size - maximum size of string
*/
char *GetLocalTimeAsString(char *szTime, size_t size);

/*
Returns the local date as a string using the current locale
szDate - string where the time will be saved
size - maximum size of string
*/
char *GetLocalDateAsString(char *szDate, size_t size);

/*
Returns the local time and date as a string using the current locale
szDateTime - string where the date and time will be saved
size - maximum size of the string
*/
char *GetLocalDateTimeAsString(char *szDateTime, size_t size);

#endif //ifdef
