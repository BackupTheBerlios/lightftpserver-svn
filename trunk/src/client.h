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

#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "commonheaders.h"
#include <syslog.h>
#include <sys/select.h> //new posix standard


#define DISCONNECT_DATA          1
#define DISCONNECT_COMMAND       2
#define DISCONNECT_DATA_COMMAND  DISCONNECT_DATA | DISCONNECT_COMMAND

#define TYPE_ASCII			1
#define TYPE_EBCDIC			2
#define TYPE_IMAGE			3
#define TYPE_LOCAL			4

#define TYPEF_NONPRINT	0x00100
#define TYPEF_TELNET		0x00200
#define TYPEF_ASA				0x00400

#define STRUCT_FILE			1
#define STRUCT_RECORD		2
#define STRUCT_PAGE			3

#define MODE_STREAM			1
#define MODE_BLOCK			2
#define MODE_COMPRESSED	3


#define DATA_CONN_STOPPED 0x00000
#define DATA_CONN_ACTIVE	0x00001

#define DATA_CONN_RECEIVE 0x00001
#define DATA_CONN_SEND		0x00002

#define DATA_CONN_FPASV		0x00100
#define DATA_CONN_FPORT		0x00200

#define DATA_FILE_PIPE		0x00001
#define DATA_FILE_FILE		0x00002

typedef char * TParam1; //the raw sent message
typedef char * TParam2; //the params of the command

//needs work
class CFTPClient{
	protected:
		char *szCurrentPath;
		char *userName; //user's name
		int nMode; //MODE
		int nType; //TYPE
		int nStructure; //STRUcture type
		int nPathSize;
		int connected; //are we connected - i think it's not used currently
		int dataConnActive; //if DTP is active (DATA_CONN_STOPPED || DATA_CONN_ACTIVE)
		int dataConnType; //(DATA_CONN_RECEIVE || DATA_CONN_SEND ) | (DATA_CONN_FPASV || DATA_CONN_FPORT)
		FILE *dataFile; //open file descriptor - the handlers should open this FD, the DTP connection will close it
		int fileType; //DATA_FILE_FILE | DATA_FILE_PIPE
/* 		int userEntered; // should not be needed anymore userName instead */
		int loggedIn; //if user an pass ok
		CSocket *commandSocket; //PI
		CSocket *dataSocket; //DTP
		CSocket *pasvDataSocket; //DTP for pasv connections - the child socket
		
		int WaitForMessage(char *buffer, int size);
		int TranslateMessage(char *buffer);
		
		int Handle(int index, TParam1 buffer, TParam2 param2);
		int DoDTP();
		int CloseDTPFileDescriptor();
		
	public:
		CFTPClient(CSocket *commandSocket);
		~CFTPClient();
		void Clear();
		int Run();
		//int Connect(char *address, int port);
		int Disconnect(int part = DISCONNECT_DATA_COMMAND);
		
		char *GetCurrentPath();
		void SetCurrentPath(char *newPath);
		
		CSocket *GetDataSocket();
		void SetDataSocket(CSocket *newDataSocket);
		
		void SendReply(char *message);
		char *CorrectRepresentation(char *message);
		
//	private:
		int HandleUserCommand(TParam1 param1, TParam2 param2);
		int HandlePassCommand(TParam1 param1, TParam2 param2);
		int HandleAcctCommand(TParam1 param1, TParam2 param2);
		int HandleCwdCommand(TParam1 param1, TParam2 param2);
		int HandleCdupCommand(TParam1 param1, TParam2 param2);
		int HandleSmntCommand(TParam1 param1, TParam2 param2);
		int HandleQuitCommand(TParam1 param1, TParam2 param2);
		int HandleReinCommand(TParam1 param1, TParam2 param2);
		int HandlePortCommand(TParam1 param1, TParam2 param2);
		int HandlePasvCommand(TParam1 param1, TParam2 param2);
		int HandleTypeCommand(TParam1 param1, TParam2 param2);
		int HandleStruCommand(TParam1 param1, TParam2 param2);
		int HandleModeCommand(TParam1 param1, TParam2 param2);
		int HandleRetrCommand(TParam1 param1, TParam2 param2);
		int HandleStorCommand(TParam1 param1, TParam2 param2);
		int HandleStouCommand(TParam1 param1, TParam2 param2);
		int HandleAppeCommand(TParam1 param1, TParam2 param2);
		int HandleAlloCommand(TParam1 param1, TParam2 param2);
		int HandleRestCommand(TParam1 param1, TParam2 param2);
		int HandleRnfrCommand(TParam1 param1, TParam2 param2);
		int HandleRntoCommand(TParam1 param1, TParam2 param2);
		int HandleAborCommand(TParam1 param1, TParam2 param2);
		int HandleDeleCommand(TParam1 param1, TParam2 param2);
		int HandleRmdCommand(TParam1 param1, TParam2 param2);
		int HandleMkdCommand(TParam1 param1, TParam2 param2);
		int HandlePwdCommand(TParam1 param1, TParam2 param2);
		int HandleListCommand(TParam1 param1, TParam2 param2);
		int HandleNlstCommand(TParam1 param1, TParam2 param2);
		int HandleSiteCommand(TParam1 param1, TParam2 param2);
		int HandleSystCommand(TParam1 param1, TParam2 param2);
		int HandleStatCommand(TParam1 param1, TParam2 param2);
		int HandleHelpCommand(TParam1 param1, TParam2 param2);
		int HandleNoopCommand(TParam1 param1, TParam2 param2);
};


#endif
