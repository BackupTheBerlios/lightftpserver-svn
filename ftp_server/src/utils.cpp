#include "../include/utils.h"

#include <time.h>

/*
The file where all the log output is written.
*/
FILE *LogFile;

struct tm *GetCurrentLocalTime()
{
	time_t timp;
	time(&timp);
	return localtime(&timp);
}

char *GetLocalTimeAsString(char *szTime, size_t size)
{
	tm *local = GetCurrentLocalTime();
	strftime(szTime, size, "%X", local);
	return szTime;
}

char *GetLocalDateAsString(char *szDate, size_t size)
{
	tm *local = GetCurrentLocalTime();
	strftime(szDate, size, "%x", local);
	return szDate;
}

char *GetLocalDateTimeAsString(char *szDateTime, size_t size)
{
	tm *local = GetCurrentLocalTime();
	strftime(szDateTime, size, "%d %B %Y, %H:%M:%S", local);
	return szDateTime;
}


int LogInit()
{
	LogFile = fopen("ftp_server.log", "at");
	char tmp[256];
	GetLocalDateTimeAsString(tmp, sizeof(tmp));
	fprintf(LogFile, "\n\n========================| Logging started: %s |========================\n", tmp);
	return 0;
}

int LogDestroy()
{
	if ((LogFile != NULL) && (LogFile != stdin) && (LogFile!= stdout) && (LogFile != stderr))
	  {
			char tmp[256];
			GetLocalDateTimeAsString(tmp, sizeof(tmp));
			fprintf(LogFile, "========================| Logging ended:  %s |=========================\n", tmp);
			fclose(LogFile);
		}
	return 0;
}

int Log(int nNeededVerbosity, char *format, ...)
{
	if (nNeededVerbosity > Verbosity)
		{
			return -1;
		}
	char str[4096];
	va_list	vararg;
	int tBytes;
	//FILE *fout = fopen(LogFile, "at");
	va_start(vararg, format);

	tBytes = vsnprintf(str, sizeof(str), format, vararg);
	if (tBytes > 0)
		{
			str[tBytes] = 0;
		}
	va_end(vararg);
	if (str[strlen(str) - 1] != '\n')
		{
			strcat(str, "\n");
		}
	char szDateTime[512];
	char szTime[256];
	GetLocalDateAsString(szDateTime, sizeof(szDateTime));
	strcat(szDateTime, " at ");
	GetLocalTimeAsString(szTime, sizeof(szTime));
	strcat(szDateTime, szTime);
	strcat(szDateTime, " : ");
	fputs(szDateTime, LogFile);
	fputs(str, LogFile);
	return 0;
}
