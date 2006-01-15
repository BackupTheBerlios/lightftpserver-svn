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
	dataConnActive = DATA_CONN_STOPPED;
	dataConnType = 0;
	dataFile = NULL;
	userName = "<not logged in>";
	nType = TYPE_ASCII;
	nStructure = STRUCT_FILE;
	nMode = MODE_STREAM;
	szCurrentPath = NULL;
	this->commandSocket = commandSocket;
	dataSocket = new CSocket();
	pasvDataSocket = NULL;
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
	if (pasvDataSocket)
		{
			delete pasvDataSocket;
		}
	if (dataFile)
		{
			fclose(dataFile); //what if it's a popen FD ?
		}
	if (userName)
		{
			free(userName);
		}
}

fd_set *GetDTPFDSet(int dataConnType, fd_set *fdsRead, fd_set *fdsWrite)
{
	switch (dataConnType & 0xFF)
		{
			case DATA_CONN_RECEIVE:
				{
					return fdsRead;
					break;
				}
			case DATA_CONN_SEND:
				{
					return fdsWrite;
					break;
				}
			default:
				{
					return NULL;
					break;
				}
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
	fd_set fdsRead;
	fd_set fdsWrite;
	fd_set *fdsDataConn = NULL; //points to either fdsRead or fdsWrite
	int n;
	int sock;
	int res;
	timeval timeout;
	timeout.tv_sec = 60;
	timeout.tv_usec = 0;
	while (!done)
		{
			syslog(LOG_FTP | LOG_INFO, "Waiting for message");
			FD_ZERO(&fdsRead);
			FD_ZERO(&fdsWrite);
			sock = commandSocket->Socket();
			n = sock;
			FD_SET(sock, &fdsRead);
			fdsDataConn = NULL;
			if (dataConnActive)
				{
					fdsDataConn = GetDTPFDSet(dataConnType, &fdsRead, &fdsWrite);
					sock = dataSocket->Socket();
					if (sock > n)
						{
							n = sock;
						}
					FD_SET(sock, fdsDataConn);
					if (pasvDataSocket)
						{
							sock = pasvDataSocket->Socket();
							if (sock > n)
								{
									n = sock;
								}
							FD_SET(sock, fdsDataConn);
						}
				}
			sprintf(caca, "fdsRead = %p, fdsWrite =%p, fdsDataConn = %p, dataConnType = %d, dataConnActive = %d", &fdsRead, &fdsWrite, fdsDataConn, dataConnType, dataConnActive);
			syslog(LOG_FTP | LOG_INFO, caca);
			res = select(n + 1, &fdsRead, &fdsWrite, NULL, &timeout);
			if (res < 0)
				{
					Log("Select failed, Shutting down connection !!!");
					done = 1;
				}
			if (FD_ISSET(commandSocket->Socket(), &fdsRead)) //data available on the PI
				{
					syslog(LOG_FTP | LOG_INFO, "Command socket active");
					size = WaitForMessage(buffer, BUF_SIZE);
					while ((buffer[size - 1] == '\n') || (buffer[size - 1] == '\r')) //remove all newline chars
						{
							buffer[size - 1] = '\0';
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
										sprintf(caca, FTP_R500, buffer);
									}
								syslog(LOG_FTP | LOG_INFO, caca);
								Handle(index, buffer, buffer);
						}
				}
			syslog(LOG_FTP | LOG_INFO, "before data connection check");
			if ((fdsDataConn) && (dataConnActive))
				{
					Log("Checking to see if the fd set for DTP contains any data");
					sprintf(caca, "pasvDataSocket = %p, dataSocket = %p, FD_ISSET(dataSocket) = %d, FD_ISSET(pasvDataSocket)", pasvDataSocket, dataSocket, FD_ISSET(dataSocket->Socket(), fdsDataConn), FD_ISSET(pasvDataSocket->Socket(), fdsDataConn));
					Log(caca);
					if ((FD_ISSET(dataSocket->Socket(), fdsDataConn)) || ((pasvDataSocket) && (FD_ISSET(pasvDataSocket->Socket(), fdsDataConn))))
						{
							syslog(LOG_FTP | LOG_INFO, "Data socket active");
							DoDTP();
						}
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
	sscanf(buffer, "%[a-zA-Z]", message);
	int len = strlen(message);
	for (i = 0; i < len; i++)
		{
			message[i] = tolower(message[i]);
		}
	for (i = 0; i < cHandlers; i++) //go through all the handlers and check for the given message
		{
			if (strcmp(handlers[i].message, message) == 0)
				{
					return i;
				}
		}
	return TRANSLATE_UNKNOWN;
}

int CFTPClient::Handle(int index, TParam1 buffer, TParam2 param2)
{
	if (index != TRANSLATE_UNKNOWN)
		{
			int cLen = strlen(handlers[index].message);
			char *param = NULL;
			if (strlen(buffer) > cLen)
				{
					param = buffer + cLen + 1;
				}
			syslog(LOG_FTP | LOG_INFO, "Calling the handler method");
			handlers[index].Handle(this, buffer, param);
		}
		else{
			char buffer[BUF_SIZE];
			sprintf(buffer, FTP_R500, param2);
			SendReply(buffer);
		}
}

int CFTPClient::DoDTP()
{
	char buffer[4 * BUF_SIZE];
	int size;
	static CSocket *sock = NULL; //i fucking hate this stuff
	sock = (dataConnType & DATA_CONN_FPASV) ? pasvDataSocket : dataSocket; //if type is pasv use the child socket
	switch (dataConnType & 0xFF) //only the send or receive flag
		{
			case DATA_CONN_RECEIVE:
				{
					Log("Inside DoDTP(), receiving ...");
					size = sizeof(buffer);
					int res = sock->Receive(buffer, size - 1);
					if (res > 0)
						{
							buffer[size - 1] = '\0'; //make sure it's null terminated
							fputs(buffer, dataFile);
						}
					if (size <= 0)
						{
							dataConnActive = DATA_CONN_STOPPED;
							dataConnType = 0;
							CloseDTPFileDescriptor();
							Disconnect(DISCONNECT_DATA);
							SendReply(FTP_R226);
						}
					break;
				}
			case DATA_CONN_SEND:
				{
					Log("Inside DoDTP(), sending ...");
					//sprintf(buffer, "dataFile = %p", dataFile);
					//Log(buffer);
					if (!feof(dataFile))
						{
							char *res = fgets(buffer, sizeof(buffer), dataFile);
							if (res)
								{
									sock->Send(buffer);
								}
						}
						else{
							dataConnActive = DATA_CONN_STOPPED;
							dataConnType = 0;
							Disconnect(DISCONNECT_DATA);
							CloseDTPFileDescriptor();
							SendReply(FTP_R226);
						}
					break;
				}
			default:
				{
					SendReply("Unknown data method");
					break;
				}
		}
}

int CFTPClient::CloseDTPFileDescriptor()
{
	if (dataFile)
		{
			switch (fileType)
				{
					case DATA_FILE_PIPE:
						{
							pclose(dataFile);
							break;
						}
					default:
						{
							fclose(dataFile);
							break;
						}
				}
		}
	dataFile = NULL;
}

int CFTPClient::Disconnect(int part)
{
	///\todo do disconnection
	if (part & DISCONNECT_DATA)
		{
			dataConnType = 0;
			if (dataSocket)
				{
					dataSocket->Disconnect();
				}
			if (pasvDataSocket)
				{
					pasvDataSocket->Disconnect();
					delete pasvDataSocket;
				}
			pasvDataSocket = NULL;
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
	if ((x != '\n') && ((x != '\r')) && (y != '\n'))
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

CSocket *CFTPClient::GetDataSocket()
{
	return dataSocket;
}

void CFTPClient::SetDataSocket(CSocket *newDataSocket)
{
	//TODO MEMORY PROBLEM, TOFIX
	Disconnect(DISCONNECT_DATA);
	delete dataSocket;
	dataSocket = newDataSocket;
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
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R202, param2);
	SendReply(buffer);
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
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R202, param2);
	SendReply(buffer);
}

int CFTPClient::HandleQuitCommand(TParam1 param1, TParam2 param2)
{
	SendReply(FTP_R221);
	Disconnect();
}

int CFTPClient::HandleReinCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R202, param2);
	SendReply(buffer);
}

int CFTPClient::HandlePortCommand(TParam1 param1, TParam2 param2)
{
	Disconnect(DISCONNECT_DATA);
	delete dataSocket;
	dataSocket = new CSocket();
	char h1[5], h2[5], h3[5], h4[5];
	char host[20]; //should be enough
	int p1, p2, port;
	char buffer[BUF_SIZE];
	if (param2)
		{
			sscanf(param2, "%[0-9],%[0-9],%[0-9],%[0-9],%d,%d", h1, h2, h3, h4, p1, p2);
			port = (p1 << 8) + p2;
			sprintf(host, "%s.%s.%s.%s", h1, h2, h3, h4);
			sprintf(buffer, "Connecting to host = %s, port = %d (param = %s; %s,%s,%s,%s,%d,%d", host, port, param2, h1, h2, h3, h4, p1, p2);
			Log(buffer);
			dataSocket->Connect(AF_INET, port, host);
			sprintf(buffer, FTP_R200, "PORT");
			SendReply(buffer);
		}
		else{
			SendReply(FTP_R501);
		}
	dataConnType = DATA_CONN_FPORT;
}

void *PasvAcceptWorkerThread(void *param)
{
	syslog(LOG_FTP | LOG_INFO, "inside worker thread ...");
	CFTPClient *client = (CFTPClient *) param;
	syslog(LOG_FTP | LOG_INFO, "inside worker thread before accept ...");
	CSocket *data = client->GetDataSocket()->Accept();
	syslog(LOG_FTP | LOG_INFO, "inside worker thread before set data socket...");
	client->SetDataSocket(data);
	syslog(LOG_FTP | LOG_INFO, "inside worker thread after set data socket...");
	return data;
}

int CFTPClient::HandlePasvCommand(TParam1 param1, TParam2 param2)
{
// 	TODO get our ip and also generate random port
	Disconnect(DISCONNECT_DATA);
	delete dataSocket; //WHY ??
	dataSocket = new CSocket(); //WHY ??? WTF
	int port; //dataSocket->Port();
	int res = 1;
	for (port = 12345; (res != 0); port++) //find an open port
		{
			res = dataSocket->Bind(AF_INET, port, "127.0.0.1");
			if (res != 0)
				{
					syslog(LOG_FTP | LOG_INFO, "Error at bind %m");
				}
		}
	port--;
	res = dataSocket->Listen(16);
	if (res < 0)
	{
		syslog(LOG_FTP | LOG_INFO, "Error at listen %m");
	}
	char buffer[BUF_SIZE];
	/*sprintf(buffer, "port = %d, dataSocket = %p (%m)", port, dataSocket);
	syslog(LOG_FTP | LOG_INFO, buffer);*/
	sprintf(buffer, FTP_R227, "127.0.0.1", (port >> 8), (port & 0xFF));
	SendReply(buffer);
	//dataConnActive = DATA_CONN_ACTIVE;
	dataConnType = DATA_CONN_FPASV;
	Log("Accepting pasv connection (please connect before continuing)  '%m' ...");
	while(!pasvDataSocket)
		{
			pasvDataSocket = dataSocket->Accept();
			/*char caca[200];
			sprintf(caca, "pasvDataSocket = %p, dataSocket = %p %s ('%m')", pasvDataSocket, dataSocket, strerror(dataSocket->GetLastError()));
			Log(caca);*/
		}
	/*Log("before sleep(10) '%m'");
	sleep(10);
	Log("After Accept()");
	SendReply("Sleepy time over");
	syslog(LOG_FTP | LOG_INFO, buffer);*/
	//PasvAcceptWorkerThread(this);
/*	pthread_t thread;
	res = pthread_create(&thread, NULL, PasvAcceptWorkerThread, (void *) this);
	sprintf(buffer, "thread = %d", thread);
	syslog(LOG_FTP | LOG_INFO, buffer);
	if (res != 0)
		{
			syslog(LOG_FTP | LOG_INFO, "Error at  pthread_create %m");
		}
		else{
			syslog(LOG_FTP | LOG_INFO, "Success:  pthread_create %m");
		}*/
}

int CFTPClient::HandleTypeCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R202, param2);
	SendReply(buffer);
}

int CFTPClient::HandleStruCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R202, param2);
	SendReply(buffer);
}

int CFTPClient::HandleModeCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R202, param2);
	SendReply(buffer);
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
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R202, param2);
	SendReply(buffer);
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
	if (!loggedIn)
		{
			SendReply(FTP_R530);
		}
		else{
			Disconnect(DISCONNECT_DATA);
			dataConnActive = DATA_CONN_STOPPED;
			dataConnType = 0;
			CloseDTPFileDescriptor();
			SendReply(FTP_R226);
		}
}

int CFTPClient::HandleDeleCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleRmdCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleMkdCommand(TParam1 param1, TParam2 param2)
{
  char buffer[BUF_SIZE];
  if (mkdir(param2, S_IRWXU) == -1)
    switch (errno) {
    case ENAMETOOLONG:
    case ENOTDIR: {
      sprintf(buffer, FTP_R501);
      break;
    }
    default: sprintf(buffer, FTP_R550);
    } else
      sprintf(buffer, FTP_R257C, param2);
  SendReply(buffer);
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
			SendReply(FTP_R530);
		}
		else{
			if (dataConnType && 0xF00) //are the flags set ? (data connection pending)
				{
					dataConnActive = DATA_CONN_ACTIVE;
					char buffer[BUF_SIZE];
					strcpy(buffer, "ls -la ");
					if (param2) // we have a param
						{
							strcat(buffer, param2);
						}
						else{
							strcat(buffer, GetCurrentPath());
						}
					FILE *fin = popen(buffer, "r");
					if (fin)
						{
							dataFile = fin;
							SendReply(FTP_R150);
							fileType = DATA_FILE_PIPE;
							fgets(buffer, sizeof(buffer), fin); //skip the "total ..." line
							dataConnType |= DATA_CONN_SEND;
						}
				}
				else{
					SendReply(FTP_R425);
				}
		}
}

int CFTPClient::HandleNlstCommand(TParam1 param1, TParam2 param2)
{
	if (!loggedIn)
		{
			SendReply(FTP_R530);
		}
		else{
			if (dataConnType && 0xF00) //are the flags set ? (data connection pending)
				{
					dataConnActive = DATA_CONN_ACTIVE;
					char buffer[BUF_SIZE];
					strcpy(buffer, "ls -1 ");
					if (param2) // we have a param
						{
							strcat(buffer, param2);
						}
						else{
							strcat(buffer, GetCurrentPath());
						}
					FILE *fin = popen(buffer, "r");
					if (fin)
						{
							dataFile = fin;
							SendReply(FTP_R150);
							fileType = DATA_FILE_PIPE;
							//fgets(buffer, sizeof(buffer), fin); //skip the "total ..." line
							dataConnType |= DATA_CONN_SEND;
						}
				}
				else{
					SendReply(FTP_R425);
				}
		}
}

int CFTPClient::HandleSiteCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R202, param2);
	SendReply(buffer);
}

int CFTPClient::HandleSystCommand(TParam1 param1, TParam2 param2)
{
}

int CFTPClient::HandleStatCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	SendReply(FTP_R211S);
	if (param2)
		{
			syslog(LOG_FTP | LOG_INFO, "received param for stat");
			strcpy(buffer, "ls -la ");
			strcat(buffer, param2);
			FILE *fin = popen(buffer, "r");
			while (!feof(fin))
				{
					fgets(buffer, sizeof(buffer), fin);
					SendReply(buffer);
				}
			pclose(fin);
		}
		else{
			//syslog(LOG_FTP | LOG_INFO, "no params for stat");
			strcpy(buffer, "Connected from: ");
			strcat(buffer, commandSocket->Host());
			strcat(buffer, "\nLogged in as: ");
			strcat(buffer, userName);
			strcat(buffer, "\nTYPE: ASCII, STRUcture: File, MODE: Stream");
			strcat(buffer, "\nTotal bytes transferred for this session: 000 (unknown)");
			if (dataConnActive)
				{
					strcat(buffer, "\nData connection active");
				}
				else{
					strcat(buffer, "\nNo data connection");
				}
			syslog(LOG_FTP | LOG_INFO, buffer);
			SendReply(buffer);
		}
	SendReply(FTP_R211E);
}

int CFTPClient::HandleHelpCommand(TParam1 param1, TParam2 param2)
{
	int i;
	int j = 0;
	char buffer[4 * BUF_SIZE];
	strcpy(buffer, FTP_R214S);
	strcat(buffer, " ");
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
	strcat(buffer, FTP_R214E);
	SendReply(buffer);
}

int CFTPClient::HandleNoopCommand(TParam1 param1, TParam2 param2)
{
	char buffer[BUF_SIZE];
	sprintf(buffer, FTP_R200, "NOOP");
	SendReply(buffer);
}
