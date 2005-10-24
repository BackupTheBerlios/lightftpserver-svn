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

#include "logServer.h"
//#include <pthread.h>

//int loggingServerSocket = 0;
//pid_t loggingServerPID = 0;

CLoggingServer::CLoggingServer(char *szLogFileName)
{
	nServerSocket = 0;
//	nServerPID = 0;
	nServerPort = LOGSERVER_PORT;
	nMaxClients = LOGSERVER_CLIENTS;
	int size = strlen(szLogFileName) + 1;
	szFileName = (char *) malloc(size);
	strncpy(szFileName, szLogFileName, size);
//	LogServerInit();
}

CLoggingServer::~CLoggingServer()
{
	free(szFileName);
//	LogServerDestroy();
}

int CLoggingServer::WriteLogMessage(char *szMessage, int size)
{
///\todo fix the multi-process issue(s) 
	DebugMessage("(%d) Inside WriteLogMessage() ...\n", getpid());
	FILE *fin = fopen("ftp_server.log", "at"); //multitasking problem ??
	int printed = 0;
	int tmp;
	while (printed < size)
		{
			tmp = fprintf(fin, "%s", &szMessage[printed]);
			printed += tmp + 1;
		}
	
	fclose(fin);
}

int CLoggingServer::LogServerRun()
{
	int res;
	sockaddr_in client = {0};
	int clientSocket;
	int size;
	DebugMessage("(%d) In LogServerRun() ...\n", getpid());
	while (1)
		{
			DebugMessage("(%d) Waiting for connections ...\n", getpid());
			size = sizeof(client);
			clientSocket = accept(nServerSocket, (sockaddr *) &client, (socklen_t *) &size);
			DebugMessage("(%d) Client connected ...\n", getpid());
			if (clientSocket != -1)
				{
					DebugMessage("(%d) Creating a new process to handle the messages ...\n", getpid());
					if (fork() == 0)
						{
							DebugMessage("(%d) Child process groupID = %d\tParent process ID = %d\tParent process groupID = %d\n", getpid(), getpgid(getpid()), getppid(), getpgid(getppid()));
							DebugMessage("(%d) Created a new process ...\n", getpid());
							WaitForMessages(clientSocket);
						}
				}
				else{
					DebugMessage("(%d) Something went wrong here with the accept() method ...\n", getpid());
				}
		}
}

int CLoggingServer::WaitForMessages(int nClientSocket)
//void *WaitForMessages(void *data)
{
//	int nClientSocket = *(int *) data;
	char buffer[LOGSERVER_LINE_SIZE];
	int size = 1;
	DebugMessage("(%d) In WaitForMessages() ...\n", getpid());
	while (size)
		{
			DebugMessage("(%d) Waiting for message ...\n", getpid());
			size = recv(nClientSocket, buffer, sizeof(buffer), 0);
			DebugMessage("(%d) Message received (%d bytes long) ...\n", getpid(), size);
			if (size)
				{
					buffer[size] = '\0';
					DebugMessage("(%d) Writing log message ...\n", getpid());
					WriteLogMessage(buffer, size);
				}
		}
	DebugMessage("(%d) Connection closed, closing the listening process as well ...\n", getpid());
	exit(0);
}

int CLoggingServer::StartLogServer()
{
	pid_t pid;
	int res;
	DebugMessage("(%d) Starting log server ...\n", getpid());
	setpgrp(); //set the process group of the child to the child process ID (to differenciate between the logging server and the ftp server)
	nServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (nServerSocket != -1)
		{
			sockaddr_in addr = CreateSocketAddress(AF_INET, nServerPort, "127.0.0.1");
			res = bind(nServerSocket, (sockaddr *) &addr, sizeof(sockaddr));
			if (res)
				{
					printf("An error (%s) occured while trying to bind the socket ...\n", strerror(errno));
					exit(254);
				}
			res = listen(nServerSocket, nMaxClients);
			if (res)
				{
					printf("An error (%s) occured while trying to listen for connections ...\n", strerror(errno));
					exit (253);
				}
		}
		else{
			printf("An error (%s) occured while trying to create the logging server socket ...\n", strerror(errno));
			exit(255);
		}
}

int CLoggingServer::StopLogServer()
{
//	DebugMessage("(%d) Closing log server (%d)...\n", getpid(), nServerPID);
//	DebugMessage("Current process group = %d\tChild process group = %d\n", getpgid(getpid()), getpgid(nServerPID));
	if (nServerSocket)
		{
			shutdown(nServerSocket, SHUT_RDWR);
		}
	int res = kill(-getpid(), SIGQUIT); //close the logging server in the child process
	if (res)
		{
			DebugMessage("An error (%s) occured while trying to kill process group ...\n", strerror(errno));
		}
}

int CLoggingServer::LogServerInit()
{
	
	char tmp[256];
	char buffer[LOGSERVER_LINE_SIZE];
	StartLogServer();
	GetLocalDateTimeAsString(tmp, sizeof(tmp));
	sprintf(buffer, "\n\n========================| Logging started: %s |========================\n", tmp);
	WriteLogMessage(buffer, strlen(buffer));
	return 0;
}

int CLoggingServer::LogServerDestroy()
{
	char tmp[256];
	char buffer[LOGSERVER_LINE_SIZE];
	GetLocalDateTimeAsString(tmp, sizeof(tmp));
	sprintf(buffer, "========================| Logging ended:  %s |=========================\n", tmp);
	WriteLogMessage(buffer, strlen(buffer));
	StopLogServer();
	return 0;
}

