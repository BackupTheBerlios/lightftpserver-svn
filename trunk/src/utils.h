/**
	Several utility functions.
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

#ifndef FTP_UTILS_H
#define FTP_UTILS_H

#include "commonheaders.h"

#define SHOW_DEBUG_MESSAGES


/**
Wrapper for syslog(LOG_FTP | LOG_INFO, message)

\param message The message to log.
*/

void Log(char *message); //wrapper for syslog(LOG_FTP | LOG_INFO, message) - easier to write
/**
Logs the string with escape characters visible.

Escape characters like '\n' become "\\n" so they in fact become visible. Knows about '\n', '\t', '\r'. '\\'.
\param message The message to log.
*/
void LogSpecialChars(char *message);

/**
Returns the local time as a string using the current locale.

\param szTime String where the time will be saved.
\param size Maximum size of string.
\return Returns the time as a string using the current locale.
\see GetLocalDateAsString, GetLocalDateTimeAsString
*/
char *GetLocalTimeAsString(char *szTime, size_t size);

/**
Returns the local date as a string using the current locale.

\param szDate String where the time will be saved
\param size Maximum size of string
\return Returns the date as a string using the current locale.
\see GetLocalTimeAsString, GetLocalDateTimeAsString
*/
char *GetLocalDateAsString(char *szDate, size_t size);

/**Returns the current date and time as a string
\param szDateTime Pointer to string that will hold the date and time.
\param size Maximum size of string.
\return Returns the date and time as a string.
\see GetLocalTimeAsString, GetLocalDateAsString
*/
char *GetLocalDateTimeAsString(char *szDateTime, size_t size);


/**
Removes trailing characters from a string.

Removes the characters found in szTrimChars that are either at the start or end of the string.
\param szText String to trim. Will be modified to reflect the changes.
\param szTrimChars Characters to trim.
\return Returns the modified string.
*/
char *StrTrim(char *szText, const char *szTrimChars);

/**
Fills a memory block with 0.

\param data Pointer to data to fill with 0's.
\param len Length of data.
\return Returns data filled with 0's.
*/
void ZeroMemory(void *data, size_t len);

/**
Creates a sockaddress from a set of parameters.

\param family Socket family.
\param port Socket port.
\param host Ip of host.
*/
sockaddr_in CreateSocketAddress(sa_family_t family, int port, char *host);

/**
Writes a debug message to screen.

Obsolete since the main process doesn't have any file descriptors open.
\param format The format of the message
*/
void DebugMessage(const char *format, ...);

#endif //ifdef
