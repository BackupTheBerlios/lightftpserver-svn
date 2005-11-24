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

#ifndef CHAT_SOCKET_H
#define CHAT_SOCKET_H

#include "commonheaders.h"

#define FALSE 0 
#define TRUE 1

class CSocket{
	protected:
		char bConnected;
//		char szHost[64];
//		int nPort;
		int nSyncronous;
		int nSocket;
		int nSocketError;
		
		
	public:
		CSocket(int domain = PF_INET, int type = SOCK_STREAM, int protocol = 0);
		~CSocket();
		
		int Bind(sa_family_t family, int port, char *host);
		CSocket *Accept();
		int Listen(int nQueueSize);
		int Connect(sa_family_t family, int port, char *host);
		int Receive(void *message, size_t length, int flags = 0);
		int Send(const void *message, size_t length, int flags = 0);
		int Disconnect(int how = SHUT_RDWR);
		int Close();
		
		char IsConnected();
		int GetLastError();
};

#endif
