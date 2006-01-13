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


#define DISCONNECT_DATA          1
#define DISCONNECT_COMMAND       2
#define DISCONNECT_DATA_COMMAND  DISCONNECT_DATA | DISCONNECT_COMMAND

typedef long long TParam1;
typedef long TParam2;

//needs work
class CFTPClient{
	protected:
		char *szCurrentPath;
		int nMode;
		int nType;
		int nStructure;
		int nPathSize;
		int connected;
		CSocket *commandSocket;
		CSocket *dataSocket;
		
		int WaitForMessage(char *buffer, int size);
		int TranslateMessage(char *buffer);
		
		int Handle(int index, TParam1 param1, TParam2 param2);
		
	public:
		CFTPClient(CSocket *commandSocket);
		~CFTPClient();
		void Clear();
		int Run();
		//int Connect(char *address, int port);
		int Disconnect(int part = DISCONNECT_DATA_COMMAND);
		
		char *GetCurrentPath();
		void SetCurrentPath(char *newPath);
		
		void SendReply(char *message);
		
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
