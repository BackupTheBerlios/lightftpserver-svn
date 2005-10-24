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

#ifndef FTP_LOGGING_CLIENT_H
#define FTP_LOGGING_CLIENT_H

#include "commonheaders.h"
#include "logServer.h"

class CLogClient{
	protected:
		int nServerPort;
		int nVerbosity;
		int nClientSocket;
		
	public:
		CLogClient(int nCurrentVerbosity);
		~CLogClient();
		int LogClientInit();
		int LogClientDestroy();
		int Log(int nNeededVerbosity, char *format, ...);
		
		void SetVerbosity(int nNewVerbosity);
		int GetVerbosity();
		
};

/**
Writes a line to the log file.


Can have a variable number of parameters and behaves in the same manner as printf
(uses the same escape sequences - %s, %c, %d, ...)
\param nNeededVerbosity The needed verbosity level for the log event to show. If  \a nNeededVerbosity is smaller than \a Verbosity then the event won't appear in the log.
\param format The format of the event (can include %s, %c, %d, ...)
\see LogServerInit, LogServerDestroy Verbosity
*/
int Log(int nNeededVerbosity, char *format, ...);


#endif
