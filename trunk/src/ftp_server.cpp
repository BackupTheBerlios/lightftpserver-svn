/**
   The main file.

   This is the main file for the server.
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
#include "ftp_handlers.h"
#include "client.h"
#include "ftp.h"

//#include "logClient.h"
#include <inttypes.h>
#include <netdb.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/stat.h>

#define MAX_CLIENTS 64
#define FTP_PORT 5001


char cr = 13, lf = 10;

/**
	Handles the signals received by the main program.
	
	This will handle the singnals received by the main program, including SIGQUIT, SIGINT and SIGTERM.
*/
void HandleTerminationSignals(int nSignal)
{
  switch (nSignal) //received singal
    {
    case SIGQUIT:
    case SIGINT:
    case SIGTERM:
      syslog(LOG_FTP | LOG_INFO, "Received termination signal, exiting ...");
      exit(EXIT_SUCCESS);
    }
}

/**
	This handles the signals received from child processes.
	
	This will handle signals received by the main program, notifying it of child processes status.
*/
void HandleChildTermination(int nSignal)
{
  int stat;
  wait(&stat);
  syslog(LOG_FTP | LOG_INFO, "slave exited with status: %d", stat);
}

/**
	This function hooks the necessary singnals.
*/
int HookTerminationSignals()
{
  signal(SIGQUIT, HandleTerminationSignals);
  signal(SIGTERM, HandleTerminationSignals);
  signal(SIGINT, HandleTerminationSignals);
//  signal(SIGCHLD, HandleChildTermination); - popen nu mai functioneaza
}

/**
	This function will make the main program a daemon.
	
	What this does is it closes all open file descriptors, creates a few child processes to change parents and changes the path to a safe location.
*/
void MakeDaemon()
{
  int i;
  pid_t pid;

  // Fork off the parent process
  pid = fork();
  if (pid < 0) {
    syslog(LOG_FTP | LOG_ERR, "Error '%m' while trying to create the first child. (stage 1)");
    exit(200);
  }
  else if (pid > 0)
    //parent - parent receives the pid of the child, not 0
    exit(EXIT_SUCCESS); //closing the parent
  //child

  umask(022);
  
  /* Create a new SID for the child process */
  if (setsid() == -1)
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

  // close all open handles
  for (i = getdtablesize() - 1; i >= 0; i--)
    close(i); 

  syslog(LOG_FTP | LOG_INFO, "Lightweight ftp server is now a daemon ...");
}


/**
	All the magic starts here.
*/
int main(int argc, char** argv)

{
  struct sockaddr_in server_address;
  //int ssock, csock;
  CSocket *clientSocket, *serverSocket;

  server_address.sin_port = htons(21);
  server_address.sin_addr.s_addr = INADDR_ANY;
  // Process Parameters
  bool daemon = false;
  int ch;
  static char optstring[] = "di:p:";
  opterr = 0; // prevent default error reporting
  while ( (ch = getopt(argc, argv, optstring)) != -1) {
    switch (ch) {
    case 'd': {
      // make me daemon
      daemon = true;
      break;
    }
    case 'i': {
      //  convert IP address from presentation (dotted-quad) to network representation
      if ( inet_pton(AF_INET, optarg, &server_address.sin_addr) == 0) {
	fprintf(stderr, "%s: %s: %s\n", argv[0], optarg, "invalid IP address format"); 
	exit(1);
      }
      break;
    }
    case 'p': {
      // convert port
      char* end_pointer = NULL;
      // try to convert
      server_address.sin_port = htons(strtoumax(optarg, &end_pointer, 10));

      // TODO: test overflow/underflow
      if (*end_pointer != 0) {
	fprintf(stderr, "%s: %s: %s\n", argv[0], optarg, "invalid port number");
	exit(1);
      }
      break;
    }
    default: {
      // report usage
      printf("Usage: %s [-d][-i <interface>][-p <port>]\n", argv[0]);
      exit(1);
    }
    }
  }
  
  if (daemon)
    MakeDaemon();

  syslog(LOG_FTP | LOG_INFO, "Hooking signals ...");
  HookTerminationSignals();

  syslog(LOG_FTP | LOG_INFO, "Preparing to start server ...");

	
	serverSocket = new CSocket();
	int res;
	res = serverSocket->Bind(AF_INET, FTP_PORT, INADDR_ANY);
	if (res < 0)
		{
			syslog(LOG_FTP | LOG_INFO, "Error at Bind() %m");
			exit(1);
		}
	res = serverSocket->Listen(MAX_CLIENTS);
	if (res < 0)
		{
			syslog(LOG_FTP | LOG_INFO, "Error at Listen() %m");
			exit(2);
		}
	
	/*TODO ioni - de ce nu folosim CSocket ?
  ssock = passiveTCPLowLevel(server_address.sin_addr.s_addr,
			     server_address.sin_port,
			     MAX_CLIENTS);
	*/

  syslog(LOG_FTP | LOG_INFO, "Lightweight ftp server started ... Waiting for connections ...");

  while (1) {
    int pid;
    // show no interest in the address of the remote host
    //csock = accept(ssock, NULL, NULL);
    clientSocket = serverSocket->Accept();
    if (clientSocket)
    	{
				pid = fork();
				if (pid == -1) {
					// error
					syslog(LOG_FTP|LOG_ERR, "main: fork: %m");
					exit(1);
				} else if (pid == 0) {
					//child
					//close(ssock);
				  serverSocket->Close();
					CFTPClient *client = new CFTPClient(clientSocket);
					client->Run();
 					delete client;
					//ftpService(csock);
					exit(0);
				} else {
					// parent
					syslog(LOG_FTP|LOG_INFO, "new connection");
// 					delete clientSocket;
					clientSocket->Close();
				}
			}
  }
}
