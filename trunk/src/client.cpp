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

#include "client.h"
#include "commonheaders.h"

#include "ftp_handlers.h"


#define BUF_SIZE 1024
#define TRANSLATE_UNKNOWN -1

CFTPClient::CFTPClient(CSocket *commandSocket)
{
	///\todo do constructor
	connected = 0;
	nPathSize = 0;
	loggedIn = 0;
	userEntered = 0;
	userName = NULL;
	szCurrentPath = NULL;
	this->commandSocket = commandSocket;
	dataSocket = NULL;
}

CFTPClient::~ CFTPClient()
{
	///\todo do destructor
	Disconnect();
	Clear();
}

void CFTPClient::Clear()
{
	connected = 0;
	if (szCurrentPath)
		{
			free(szCurrentPath);
		}
	if (commandSocket)
		{
			delete commandSocket;
		}
	if (dataSocket)
		{
			delete dataSocket;
		}
}

/**
This is the main ftp client function. Here we receive all the commands, send the requested data ...
*/
int CFTPClient::Run()
{
	commandSocket->Send("Welcome to Light ftp server ...\n");
	int done = 0;
	char buffer[BUF_SIZE];
	char caca[BUF_SIZE];
	int size;
	int index;
	while (!done)
		{
			syslog(LOG_FTP | LOG_INFO, "Waiting for message");
			size = WaitForMessage(buffer, BUF_SIZE);
			while ((buffer[size - 1] == '\n') || (buffer[size - 1] == '\r'))
				{
					buffer[size - 1 ] = '\0';
					size--;
				}
			sprintf(caca, "size = %d, buffer = %s", size, buffer);
			syslog(LOG_FTP | LOG_INFO, caca);
			if (size <= 0)
				{
					done = 1;
				}
				else{
					syslog(LOG_FTP | LOG_INFO, "Translating message into command");
					index = TranslateMessage(buffer);
					if (index >= 0)
						{
							sprintf(caca, "Handling the command %s (%d)", handlers[index].message, index);
						}
						else{
							sprintf(caca, "Unknown command %s", buffer);
						}
					syslog(LOG_FTP | LOG_INFO, caca);
					Handle(index, 0, buffer);
				}
		}
	commandSocket->Send("Thanks for using Light ftp server ...\n");
	Disconnect();
}

int CFTPClient::WaitForMessage(char *buffer, int size)
{
	return commandSocket->Receive(buffer, size);
}

int CFTPClient::TranslateMessage(char *buffer)
{
	int i;
	char message[12]; //should be big enough :)
	sscanf(buffer, "%[a-zA-Z0-9]", message);
	//TODO convert to lowercase
	for (i = 0; i < cHandlers; i++) //go through all the handlers and check for the given message
		{
			if (strcmp(handlers[i].message, message) == 0)
				{
					return i;
				}
		}
	return TRANSLATE_UNKNOWN;
}

int CFTPClient::Handle(int index, TParam1 param1, TParam2 param2)
{
	if (index != TRANSLATE_UNKNOWN)
		{
			syslog(LOG_FTP | LOG_INFO, "Calling the handler method");
			handlers[index].Handle(this, param1, param2);
		}
}

int CFTPClient::Disconnect(int part)
{
	///\todo do disconnection
	if (part & DISCONNECT_DATA)
		{
			if (dataSocket)
				{
					dataSocket->Disconnect();
				}
		}
	if (part & DISCONNECT_COMMAND)
		{
			if (commandSocket)
				{
					commandSocket->Disconnect();
				}
		}
}

char *CFTPClient::GetCurrentPath()
{
	return szCurrentPath;
}

void CFTPClient::SetCurrentPath(char *newPath)
{
	int len = strlen(newPath) + 1;
	if (nPathSize < len) //if we don't have enough allocated memory to store the new string
		{
			szCurrentPath = (char *) realloc(szCurrentPath, len); //then reallocate
			nPathSize = len;
		}
	strcpy(szCurrentPath, newPath); //copy the new string
}

void CFTPClient::SendReply(char *message)
{
	const int len = strlen(message);
	char x = message[len - 1];
	char y = message[len - 2];
	if ((x != '\n') || ((x != '\r')) && (y != '\n'))
		{
			char buffer[len + 2];
			strcpy(buffer, message);
			strcat(buffer, "\n");
			commandSocket->Send(buffer);
		}
		else{
			commandSocket->Send(message);
		}
}


/*************************HANDLER MEMBER FUNCTIONS****************************/

int CFTPClient::HandleUserCommand(TParam1 param1, TParam2 param2)
{
	//TODO mai trebuie verificat aici
	SendReply("you just used the USER command");
	userEntered = 1;
}

int CFTPClient::HandlePassCommand(TParam1 param1, TParam2 param2)
{
	//TODO trebuie facuta loginarea calumea
	SendReply("What ? you want a password too ?");
	if (userEntered)
		{
			SendReply("Congratulations, you're now logged in");
			loggedIn = 1;
			SetCurrentPath("~");
		}
		else{
			SendReply("You need to use USER first");
		}
}

int CFTPClient::HandleAcctCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleCwdCommand(TParam1 param1, TParam2 param2)
{
	if (!loggedIn)
		{
			SendReply("You need to be logged in first");
		}
		else{
			if (strlen(param2) > 4)
				{
					char *pos = param2 + 4;
					//TODO trim pos of spaces and newline
					SetCurrentPath(pos);
				}
			SendReply("path changed ok");
		}
}

int CFTPClient::HandleCdupCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleSmntCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleQuitCommand(TParam1 param1, TParam2 param2)
{
	SendReply("Leaving so soon ?");
	Disconnect();
}

int CFTPClient::HandleReinCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandlePortCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandlePasvCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleTypeCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleStruCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleModeCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleRetrCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleStorCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleStouCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleAppeCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleAlloCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleRestCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleRnfrCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleRntoCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleAborCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleDeleCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleRmdCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleMkdCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandlePwdCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, "Current working directory = %s", szCurrentPath);
	SendReply(buffer);
}

int CFTPClient::HandleListCommand(TParam1 param1, TParam2 param2)
{
	//TODO: use the data connection
	if (!loggedIn)
		{
			SendReply("You need to be logged in first");
		}
		else{
			char *buffer = (char *) malloc(1024 * 1024); //memory problems
			char tmp[BUF_SIZE];
			sprintf(buffer, "ls -al %s", szCurrentPath);
			FILE *fin = popen(buffer, "r");
			buffer[0] = '\0';
			fgets(tmp, sizeof(tmp), fin); //skip the total = ... line
			while (!feof(fin))
				{
					fgets(tmp, sizeof(tmp), fin);
					strcat(buffer, tmp);
				}
			SendReply(buffer);
			pclose(fin);
			free(buffer);
		}
}

int CFTPClient::HandleNlstCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleSiteCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleSystCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleStatCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleHelpCommand(TParam1 param1, TParam2 param2)
{
	int i;
	int j = 0;
	char buffer[4 * BUF_SIZE];
	strcpy(buffer, FTP_R214);
	strcat(buffer, " :");
	for (i = 0; i < cHandlers - 1; i++)
		{
			j++;
			if (j >= 5)
				{
					j = 0;
					strcat(buffer, "\n");
				}
			strcat(buffer, handlers[i].message);
			strcat(buffer, ", ");
		}
	strcat(buffer, handlers[cHandlers - 1].message);
	strcat(buffer, "\n");
	strcat(buffer, FTP_R214);
	strcat(buffer, " end of list");
	SendReply(buffer);
}

int CFTPClient::HandleNoopCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R200, "NOOP");
	SendReply(buffer);
}
