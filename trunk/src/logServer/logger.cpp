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
 
#include "logServer.h"

CLoggingServer logServer("../ftp_server.log");

void SignalHandler(int nSignal)
{
	switch (nSignal)
		{
			case SIGINT:
			case SIGTERM:
				printf("Received terminate signal, exiting ...\n");
				logServer.LogServerDestroy();
				break;
		}
}

int main()
{
	signal(SIGTERM, SignalHandler);
	signal(SIGINT, SignalHandler);
	logServer.LogServerInit();
	logServer.LogServerRun();
}
