/***************************************************************************
 *   Copyright (C) 2005 by Cristi, Ion, Diana                              *
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


CClient::CClient()
{
	//TODO: do constructor
	connected = 0;
	nPathSize = 0;
	szCurrentPath = NULL;
}

CClient::CClient(int commandSocket)
{
	//TODO: do constructor
	connected = 0;
	nPathSize = 0;
	szCurrentPath = NULL;
	this->commandSocket = commandSocket;
}

CClient::~ CClient()
{
	//TODO: do destructor
	Disconnect();
	if (szCurrentPath)
		{
			free(szCurrentPath);
		}
	connected = 0;
}

int CClient::Connect(char *address, int port)
{
	//TODO: do connection
	connected = 1;
}

int CClient::Disconnect()
{
	//TODO: do disconnection
}

char *CClient::GetCurrentPath()
{
	return szCurrentPath;
}

void CClient::SetCurrentPath(char *newPath)
{
	int len = strlen(newPath) + 1;
	if (nPathSize < len)
		{
			szCurrentPath = (char *) realloc(szCurrentPath, len);
			nPathSize = len;
		}
	strcpy(szCurrentPath, newPath);
}
