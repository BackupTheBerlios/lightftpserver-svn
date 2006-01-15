/**
Utils.cpp short description.

Utils.cpp long description.
*/
/***************************************************************************
 *   Copyright (C) 2005 by Cristi, Diana, Ion                              *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "utils.h"

#include <time.h>

void Log(char *message)
{
	syslog(LOG_FTP | LOG_INFO, message);
}

void LogSpecialChars(char *message)
{
	const int len = strlen(message);
	char buffer[(2 * (len + 1))];
	buffer[0] = '\0';
	int i, j = 0;
	for (i = 0; i < len; i++)
		{
			switch (message[i])
				{
					case '\\':
						{
							strcat(buffer, "\\\\");
							j += 2;
							break;
						}
					case '\n':
						{
							strcat(buffer, "\\n");
							j += 2;
							break;
						}
					case '\r':
						{
							strcat(buffer, "\\r");
							j += 2;
							break;
						}
					case '\t':
						{
							strcat(buffer, "\\t");
							j += 2;
							break;
						}
					default:
						{
							buffer[j++] = message[i];
						}
				}
		}
	buffer[j++] = '\0';
	Log(buffer);
}

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

char *StrTrim(char *szText, const char *szTrimChars)
{
        int i = strlen(szText) - 1;
        while ((i >= 0) && (strchr(szTrimChars, szText[i])))
                {
                        szText[i--] = '\0';
                }
        i = 0;
        while (((unsigned int )i < strlen(szText)) && (strchr(szTrimChars, szText[i])))
                {
                        i++;
                }
        if (i)
                {
                        int size = strlen(szText);
                        int j;
                        for (j = i; j <= size; j++) //shift the \0 as well
                                {
                                        szText[j - i] = szText[j];
                                }
//                      memmove(szText, szText + i, size - i + 1); //copy the string without the first i characters
                }
        return szText;
}

void ZeroMemory(void *data, size_t len)
{
	memset(data, 0, len);
}

sockaddr_in CreateSocketAddress(sa_family_t family, int port, char *host)
{
	sockaddr_in tmp = {0};
	tmp.sin_family = family;
	tmp.sin_port = htons(port);
	if ((in_addr_t) host != INADDR_ANY)
		{
			tmp.sin_addr.s_addr = inet_addr(host);
		}
		else{
			tmp.sin_addr.s_addr = INADDR_ANY;
		}
	return tmp;
}

void DebugMessage(const char *format, ...)
{
#ifdef SHOW_DEBUG_MESSAGES
	char str[4096];
	va_list	vararg;
	int tBytes;
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
	printf("%s", szDateTime);
	
	printf("%s", str);
#endif
}
