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

#define DISCONNECT_DATA          1
#define DISCONNECT_COMMAND       2
#define DISCONNECT_DATA_COMMAND  DISCONNECT_DATA | DISCONNECT_COMMAND

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
		
		int WaitForMessage(char *buffer, int &size);
		int TranslateMessage(char *buffer);
		
	public:
		CFTPClient(CSocket *commandSocket);
		~CFTPClient();
		void Clear();
		int Run();
		//int Connect(char *address, int port);
		int Disconnect(int part = DISCONNECT_DATA_COMMAND);
		
		char *GetCurrentPath();
		void SetCurrentPath(char *newPath);
};


#endif
