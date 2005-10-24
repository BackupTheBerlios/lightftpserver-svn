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
					printf("An error (%s) occured while trying to connect to logging server ...\n", strerror(errno));
					nClientSocket = 0;
				}
		}
		else{
					printf("An error (%s) occured while trying to create the logging client socket ...\n", strerror(errno));
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
					printf("Error (%s) closing socket ...\n", strerror(errno));
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


int CLogClient::Log(int nNeededVerbosity, char *format, ...)
{
	if (nNeededVerbosity > nVerbosity)
		{
			return -1;
		}
	DebugMessage("Sending log message ...\n");
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
	send(nClientSocket, szDateTime, strlen(szDateTime) + 1, 0);
	send(nClientSocket, str, strlen(str) + 1, 0);
//	fputs(szDateTime, LogFile);
//	fputs(str, LogFile);
	return 0;
}
