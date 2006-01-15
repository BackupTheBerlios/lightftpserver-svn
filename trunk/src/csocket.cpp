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

#include "csocket.h"


CSocket::CSocket(int domain, int type, int protocol)
{
	bConnected = FALSE;
	nSocket = socket(domain, type, protocol);
	nSocketError = errno;
	ZeroMemory(&addrData, sizeof(addrData));
}


CSocket::~CSocket()
{
	if (IsConnected())
		{
			Disconnect();
		}
}

int CSocket::Disconnect(int how)
{
	int res = shutdown(nSocket, how);
	nSocketError = errno;
	bConnected = FALSE;
	return res;
}

int CSocket::Close()
{
	return close(nSocket);
}

char CSocket::IsConnected()
{
	return (bConnected);
}

int CSocket::Bind(sa_family_t family, int port, char *host)
{
	addrData = CreateSocketAddress(family, port, host);
	int res = bind(nSocket, (sockaddr *) &addrData, sizeof(addrData));
	nSocketError = errno;
	return res;
}

int CSocket::Listen(int nQueueSize)
{
	int res = listen(nSocket, nQueueSize);
	nSocketError = errno;
	return res;
}

CSocket *CSocket::Accept()
{
	int res;
	CSocket *tmp;
	sockaddr addr;
	socklen_t size = sizeof(sockaddr);
	
	res = accept(nSocket, &addr, &size);
	nSocketError = errno;
	if (res >= 0)
		{
			tmp = new CSocket();
			tmp->Close();
			tmp->nSocket = res;
			memmove(&(tmp->addrData), &addr, sizeof(addrData));
			bConnected = TRUE;
		}
		else{
			bConnected = FALSE;
			tmp = NULL;
		}
	return tmp;
}

int CSocket::Connect(sa_family_t family, int port, char *host)
{
	sockaddr_in tmp = CreateSocketAddress(family, port, host);
	int res = connect(nSocket, (sockaddr *) &tmp, sizeof(tmp));
	bConnected = (res == 0);
	nSocketError = errno;
	return res;
}

int CSocket::Send(const void *message, size_t length, int flags)
{
	int res = send(nSocket, message, length, flags);
	nSocketError = errno;
	return res;
}

int CSocket::Send(const char *message, int flags)
{
	Send(message, strlen(message) + 1, flags);
}

int CSocket::Receive(void *message, size_t length, int flags)
{
	int res = recv(nSocket, message, length, flags);
	nSocketError = errno;
	return res;
}

int CSocket::Blocking(int newState)
{
	int tmp = fcntl(nSocket, F_GETFL);
	tmp = (newState) ? tmp & (~O_NONBLOCK) : tmp | O_NONBLOCK; //need to recheck this
	int res = fcntl(nSocket, F_SETFL, tmp);
	nSocketError = errno;
	return res;
}

int CSocket::GetLastError()
{
	return nSocketError;
}

int CSocket::Port()
{
	return ntohs(addrData.sin_port);
}

char *CSocket::Host()
{
	return inet_ntoa(addrData.sin_addr);
}

int CSocket::Socket()
{
	return nSocket;
}
