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

#ifndef FTP_LOGGER_H
#define FTP_LOGGER_H

#include "../commonheaders.h"
#include "../utils.h"

#define LOGSERVER_PORT 54345
#define LOGSERVER_CLIENTS 64

#define LOGSERVER_LINE_SIZE 4096

class CLoggingServer{
	protected:
		int nServerSocket;
//		int nServerPID;
		char *szFileName;
		
		int nServerPort;
		int nMaxClients;
		
		int WaitForMessages(int nClientSocket);
		int WriteLogMessage(char *message, int size);
		
		int StartLogServer();
		int StopLogServer();
	public:
		/**
		\todo write doc
		*/
		CLoggingServer(char *szLogFileName);
		
		/**
		\todo write doc
		*/
		~CLoggingServer();
		
		/**
		This function initialized the logging server.
		
		\sa LogServerDestroy, Log
		*/
		int LogServerInit();
		
		/**
		Closes the logging server
		\sa LogServerInit, Log
		*/
		int LogServerDestroy();
		
		int LogServerRun();
};
//extern int loggingSeverSocket;
//extern int loggingServerPID;

#endif
