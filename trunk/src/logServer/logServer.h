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

#include "commonheaders.h"
#include "utils.h"

#define LOGSERVER_PORT 54345
#define LOGSERVER_CLIENTS 64

#define LOGSERVER_LINE_SIZE 4096

#define LOG_EMERG     1
#define LOG_ALERT     2
#define LOG_ERR       4
#define LOG_WARNING   8
#define LOG_NOTICE   16
#define LOG_INFO     32
#define LOG_DEBUG    64

#define LOG_FTP     256

#define STR_LOG_EMERG    "!EMERGENCY!"
#define STR_LOG_ALERT    "!ALERT    !"
#define STR_LOG_ERR      "!ERROR    !"
#define STR_LOG_WARNING  "!WARNING  !"
#define STR_LOG_NOTICE   "|NOTICE   |"
#define STR_LOG_INFO     "|INFO     |"
#define STR_LOG_DEBUG    "|DEBUG    |"
#define STR_LOG_UNKNOWN  "[Unknown  ]"


class CLogServer{
	protected:
		int nServerSocket;
//		int nServerPID;
		char *szFileName;
		
		int nServerPort;
		int nMaxClients;
		
		int WaitForMessages(int nClientSocket);
		int WriteLogMessage(char *message, int size, char bWriteHeader);
		
		int StartLogServer();
		int StopLogServer();
	public:
		/**
		\todo write doc
		*/
		CLogServer(char *szLogFileName);
		
		/**
		\todo write doc
		*/
		~CLogServer();
		
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
