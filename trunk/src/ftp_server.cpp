/**
The main file.

Long description of the main file.
*/

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

/*
This is the main server file.
*/

//this file is in the include folder, not the src one so we need to include accordingly
#include "commonheaders.h"

#include "configUtils.h"
#include "client.h"
//#include "logClient.h"
#include <signal.h>
#include <syslog.h>
#include <time.h>

#include <sys/stat.h>

#define MAXFD 64
#define FTP_PORT 5001
#define MAX_CLIENTS 64

int MakeDaemon()
{
	int i;
  pid_t pid, sid;

	// Fork off the parent process
  pid = fork();
  if (pid <= 0) {
    syslog(LOG_FTP | LOG_ERR, "Error '%m' while trying to create the first child. (stage 1)");
    exit(200);
  }
  else if (pid >= 0)
    //parent - parent receives the pid of the child, not 0
    exit(EXIT_SUCCESS); //closing the parent

	//first child
	/* Change the file mode mask */
  umask(0);
  
  /* Create a new SID for the child process */
  sid = setsid(); //create a new process group for the child
  if (sid < 0)
  	{
  		syslog(LOG_FTP | LOG_ERR, "Error '%m' changing the process group of the first child");
  		exit(201);
  	}

  /* Change the current working directory */
  if (chdir("/") < 0)
  	{
  		syslog(LOG_FTP | LOG_ERR, "Error '%m' while trying to change the current path");
  		exit(202);
  	}
  	
  signal(SIGHUP, SIG_IGN); //ignore parent death signal and terminal hangup signal - do we need it ???

	/* Close out the standard file descriptors */
	close(STDIN_FILENO); //portability - we make sure these 3 are closed then we try to close other possibly open FDs
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
  for (i = 0; i < MAXFD; i++) //close all open handles
    close(i); 
	syslog(LOG_FTP | LOG_INFO, "Lightweight ftp server is now a daemon ...");
	return 0;
}

int Run()
{
	CSocket *clientSocket;
	CSocket serverSocket;
	CFTPClient *client;
	pid_t pid;
	
	if (serverSocket.Bind(AF_INET, FTP_PORT, "127.0.0.1") < 0)
		{
			syslog(LOG_FTP | LOG_ERR, "Error '%m' at bind()");
			printf("EACCES = %d EINVAL = %d\n", EACCES, EINVAL);
			exit(100);
		}
	if (serverSocket.Listen(MAX_CLIENTS) < 0)
		{
			syslog(LOG_FTP | LOG_ERR, "Error '%m' at listen()");
			exit(101);
		}
	syslog(LOG_FTP | LOG_INFO, "Lightweight ftp server started ... Waiting for connections ...");
	while (1) //main loop
		{
			clientSocket = serverSocket.Accept();
			if (clientSocket)
				{
					syslog(LOG_FTP | LOG_INFO, "A new client has connected to the server ...");
					pid = fork();
					if (pid < 0)
						{
							syslog(LOG_FTP | LOG_ERR, "Could not create a new process for the new client");
						}
						else{
							if (pid == 0)
								{//child
									syslog(LOG_FTP | LOG_INFO, "A new process has been created to serve the connected user ...");
									client = new CFTPClient(clientSocket);
									client->Run();
									delete client;
									syslog(LOG_FTP | LOG_INFO, "User has disconnected, closing clild process ...");
									exit(EXIT_SUCCESS);
								}
						}
				}
				else{
					syslog(LOG_FTP | LOG_ERR, "A NULL client ... What up wit dat ????");
				}
		}
}

int main()
{
	//MakeDaemon();
	syslog(LOG_FTP | LOG_INFO, "Preparing to start server ...");
	Run();
	//test
	
//        WriteSettingString("Config verbosity", "pipi uscat", "value for pipi uscat", "ftp.ini");
//        WriteSettingString("Fucking something", "verbosity", "18", "ftp.ini");
/*
				("Settings", "value", 00, caca, "ftp.ini");
				GetSettingInt("Settings", "hex", 0x11, caca, "ftp.ini");
				GetSettingInt("Settings", "octal", 0123, caca, "ftp.ini");
				GetSettingString("Fucking something", "caca", "<error at caca>", buffer, sizeof(buffer), "ftp.ini");
				GetSettingInt("Fucking something", "verbosity", 0, caca, "ftp.ini");
				GetSettingString("Config verbosity", "non existent", "<error at>", buffer, sizeof(buffer), "ftp.ini");
				GetSettingString("Pipi uscat", "anything", "<error at pipi uscat>", buffer, sizeof(buffer), "ftp.ini");
*/
  return 0;
}
