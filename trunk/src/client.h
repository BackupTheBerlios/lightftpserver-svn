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

/**
First parameter type for handlers.
*/
typedef char * TParam1; //the raw sent message

/**
Second parameter type for handlers.
*/
typedef char * TParam2; //the params of the command

/**
This class handles all the commands for a given client.

It holds all the relevant data for a client, the status of the connection and in fact, anything to do with the client.
*/
class CFTPClient{
	protected:
		///Users name.
		char *userName; //user's name
		///Transfer mode.
		int nMode; //MODE
		///Transfer data type.
		int nType; //TYPE
		///Transfer structure type.
		int nStructure; //STRUcture type
		///Are we connected or not ...
		int connected; //are we connected - i think it's not used currently
		///DATA_CONN_ACTIVE if data connection is active.
		int dataConnActive; //if DTP is active (DATA_CONN_STOPPED || DATA_CONN_ACTIVE)
		///Type of data connection (Receive or send and passive or active).
		int dataConnType; //(DATA_CONN_RECEIVE || DATA_CONN_SEND ) | (DATA_CONN_FPASV || DATA_CONN_FPORT)
		///File descriptor for open file (used for sending and receiving by DTP).
		FILE *dataFile; //open file descriptor - the handlers should open this FD, the DTP connection will close it
		///Type of file open (file or pipe).
		int fileType; //DATA_FILE_FILE | DATA_FILE_PIPE
/* 		int userEntered; // should not be needed anymore userName instead */
		///True if user is correctly logged in.
		int loggedIn; //if user an pass ok
		///The command socket. All commands and replies are sent through this socket.
		CSocket *commandSocket; //PI
		///The data socket. Used in active mode and also in passive mode as a listening server.
		CSocket *dataSocket; //DTP
		///Data client socket for passive connections.
		CSocket *pasvDataSocket; //DTP for pasv connections - the child socket
		
		///Waits for the client to issue a command and returns it.
		int WaitForMessage(char *buffer, int size);
		///Translates a string into a command or returns TRANSLATE_UNKNOWN if command is unknown.
		int TranslateMessage(char *buffer);
		
		/**
		Handles a given command.
		
		\param index Code of command to handle.
		\param buffer First parameter of the handler function. It represents the whole command received by the server.
		\param param2 Second parameter of the handler function. It represents the parameter(s) given (NULL if no parameters were given).
		*/
		int Handle(int index, TParam1 buffer, TParam2 param2);
		///Does the necesary data transfer. Sends a message at a time.
		int DoDTP();
		///Closes the open file descriptor of the data connection. Called when a transfer is done or aborted.
		int CloseDTPFileDescriptor();
		
	public:
		///Constructor for the CFTPClient class. Takes a command socket (PI) to listen on for commands.
		CFTPClient(CSocket *commandSocket);
		///Destructor.
		~CFTPClient();
		///Frees all data of the class.
		void Clear();
		///Main handling loop. Waits for a command from the client, translates it and then processes it. If there is data that should be send via the data connection it does that as well.
		int Run();
		//int Connect(char *address, int port);
		///Disconnects one or both of the sockets. DISCONNECT_DATA will disconnect both data sockets and DISCONNECT_COMMAND will disconnect the command socket (flags can be combined).
		int Disconnect(int part = DISCONNECT_DATA_COMMAND);
		
		///Returns the current path of the client.
		char *GetCurrentPath();
		//void SetCurrentPath(char *newPath);
		
		///Returns the data socket.
		CSocket *GetDataSocket();
		///Changes the data socket to the given value.
		void SetDataSocket(CSocket *newDataSocket);
		
		///Sends a reply through the command socket. Message should be formatted according to the RFC standard.
		void SendReply(char *message);
		///In ascii mode we should send both a CR and a LF and this ensures just that. No such correction is applied in image mode.
		char *CorrectRepresentation(char *message);
		
//	private:
		///Handles a USER command.
		int HandleUserCommand(TParam1 param1, TParam2 param2);
		///Handles a PASS command.
		int HandlePassCommand(TParam1 param1, TParam2 param2);
		///Handles a ACCT command.
		int HandleAcctCommand(TParam1 param1, TParam2 param2);
		///Handles a CWD command.
		int HandleCwdCommand(TParam1 param1, TParam2 param2);
		///Handles a CDUP command.
		int HandleCdupCommand(TParam1 param1, TParam2 param2);
		///Handles a SMNT command.
		int HandleSmntCommand(TParam1 param1, TParam2 param2);
		///Handles a QUIT command.
		int HandleQuitCommand(TParam1 param1, TParam2 param2);
		///Handles a REIN command.
		int HandleReinCommand(TParam1 param1, TParam2 param2);
		///Handles a PORT command.
		int HandlePortCommand(TParam1 param1, TParam2 param2);
		///Handles a PASV command.
		int HandlePasvCommand(TParam1 param1, TParam2 param2);
		///Handles a TYPE command.
		int HandleTypeCommand(TParam1 param1, TParam2 param2);
		///Handles a STRU command.
		int HandleStruCommand(TParam1 param1, TParam2 param2);
		///Handles a MODE command.
		int HandleModeCommand(TParam1 param1, TParam2 param2);
		///Handles a RETR command.
		int HandleRetrCommand(TParam1 param1, TParam2 param2);
		///Handles a STOR command.
		int HandleStorCommand(TParam1 param1, TParam2 param2); 
		///Handles a STOU command.
		int HandleStouCommand(TParam1 param1, TParam2 param2); 
		///Handles a APPE command.
		int HandleAppeCommand(TParam1 param1, TParam2 param2);
		///Handles a ALLO command.
		int HandleAlloCommand(TParam1 param1, TParam2 param2);
		///Handles a REST command.
		int HandleRestCommand(TParam1 param1, TParam2 param2);
		///Handles a RNFR command.
		int HandleRnfrCommand(TParam1 param1, TParam2 param2);
		///Handles a RNTO command.
		int HandleRntoCommand(TParam1 param1, TParam2 param2);
		///Handles a ABOR command.
		int HandleAborCommand(TParam1 param1, TParam2 param2);
		///Handles a DELE command.
		int HandleDeleCommand(TParam1 param1, TParam2 param2);
		///Handles a RMD command.
		int HandleRmdCommand(TParam1 param1, TParam2 param2);
		///Handles a MKD command.
		int HandleMkdCommand(TParam1 param1, TParam2 param2);
		///Handles a PWD command.
		int HandlePwdCommand(TParam1 param1, TParam2 param2);
		///Handles a LIST command.
		int HandleListCommand(TParam1 param1, TParam2 param2);
		///Handles a NLST command.
		int HandleNlstCommand(TParam1 param1, TParam2 param2);
		///Handles a SITE command.
		int HandleSiteCommand(TParam1 param1, TParam2 param2);
		///Handles a SYST command.
		int HandleSystCommand(TParam1 param1, TParam2 param2);
		///Handles a STAT command.
		int HandleStatCommand(TParam1 param1, TParam2 param2);
		///Handles a HELP command.
		int HandleHelpCommand(TParam1 param1, TParam2 param2);
		///Handles a NOOP command.
		int HandleNoopCommand(TParam1 param1, TParam2 param2);
};


#endif
