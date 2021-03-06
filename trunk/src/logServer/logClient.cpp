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


#include "logClient.h"



CLogClient::CLogClient(int nCurrentVerbosity)
{
	nServerPort = LOGSERVER_PORT;
	nVerbosity = nCurrentVerbosity;
}

CLogClient::~CLogClient()
{
}

int CLogClient::LogClientInit()
{
	DebugMessage("Creating logging client ...\n");
	nClientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (nClientSocket != -1)
		{
			DebugMessage("Connecting to localhost on port %d", nServerPort);
			sockaddr_in addr = CreateSocketAddress(AF_INET, nServerPort, "127.0.0.1");
			int res = connect(nClientSocket, (sockaddr *) &addr, sizeof(sockaddr_in));
			if (res)
				{
					DebugMessage("An error (%s) occured while trying to connect to logging server ...\n", strerror(errno));
					nClientSocket = 0;
				}
		}
		else{
					DebugMessage("An error (%s) occured while trying to create the logging client socket ...\n", strerror(errno));
					exit(255);
		}
}

int CLogClient::LogClientDestroy()
{
	DebugMessage("Closing logging client ...\n");
	if (nClientSocket)
		{
			int res = shutdown(nClientSocket, SHUT_RDWR);
			if (res)
				{
					DebugMessage("Error (%s) closing socket ...\n", strerror(errno));
				}
		}
}

void CLogClient::SetVerbosity(int nNewVerbosity)
{
	nVerbosity = nNewVerbosity;
}

int CLogClient::GetVerbosity()
{
	return nVerbosity;
}


int CLogClient::Log(int nNeededVerbosity, int params, char *format, ...)
{
	if (nNeededVerbosity > nVerbosity)
		{
			return -1;
		}
	DebugMessage("Sending log message ...\n");
	char str[4096];
	va_list	vararg;
	int tBytes;
	char szLogType[16] = {0};
	switch (params)
		{
			case LOG_EMERG: 
				strcpy(szLogType, STR_LOG_EMERG);
				break;
			case LOG_ALERT:
				strcpy(szLogType, STR_LOG_ALERT);
				break;
			case LOG_ERR:
				strcpy(szLogType, STR_LOG_ERR);
				break;
			case LOG_WARNING:
				strcpy(szLogType, STR_LOG_WARNING);
				break;
			case LOG_NOTICE:
				strcpy(szLogType, STR_LOG_NOTICE);
				break;
			case LOG_INFO:
				strcpy(szLogType, STR_LOG_INFO);
				break;
			case LOG_DEBUG:
				strcpy(szLogType, STR_LOG_DEBUG);
				break;
			default:
				strcpy(szLogType, STR_LOG_UNKNOWN);
		}
	va_start(vararg, format);

	tBytes = vsnprintf(str, sizeof(str), format, vararg);
	if (tBytes > 0)
		{
			str[tBytes] = 0;
		}
	va_end(vararg);
/*	snprintf(str, sizeof(str), "%s: %s", szLogType, str);
	if (str[strlen(str) - 1] != '\n')
		{
			strcat(str, "\n");
		}
	snprintf(str, sizeof(str), "(%d) %s", getpid(), str); */
	send(nClientSocket, str, strlen(str) + 1, 0);
	return 0;
}
