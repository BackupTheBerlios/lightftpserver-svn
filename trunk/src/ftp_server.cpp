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
void HandleChildTermination(int nSignal)
{
  int stat;
  wait(&stat);
  syslog(LOG_FTP | LOG_INFO, "slave exited with status: %d", stat);
}

int HookTerminationSignals()
{
  signal(SIGQUIT, HandleTerminationSignals);
  signal(SIGTERM, HandleTerminationSignals);
  signal(SIGINT, HandleTerminationSignals);
//  signal(SIGCHLD, HandleChildTermination); - popen nu mai functioneaza
}

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


/*TODO ioni - miscarea asta nu am inteles-o. Nu am facut CFTPClient ca sa fie totul centralizat ? 
	nu de aia am facut si clasa CSocket, sa fie mai usor de lucrat cu ele ?

int activeTCPLowLevel(u_int32_t address, u_int16_t port) {
  struct sockaddr_in server_addr;
  int asock;

  // fill the address structure
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = address;
  // create the socket
  if ( (asock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    syslog(LOG_FTP|LOG_ERR, "activeTCPLowLevel: socket: %m");
    exit(1);
  }
  // connect to the remote host
  if (connect(asock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)  {
    syslog(LOG_FTP|LOG_ERR, "activeTCPLowLevel: connect: %m");
    exit(1);
  }
  return asock;
}

int passiveTCPLowLevel(u_int32_t address, u_int16_t port, int queue_size) {
  struct sockaddr_in interface;
  int psock;

  // fill the address structure
  interface.sin_family = AF_INET;
  interface.sin_port = port;
  interface.sin_addr.s_addr = address;
  // create the socket
  if ( (psock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    syslog(LOG_FTP|LOG_ERR, "passiveTCPLowLevel: socket: %m");
    exit(1);
  }
  // bind socket to address
  if (bind(psock, (struct sockaddr*)&interface, sizeof(interface)) == -1) {
    syslog(LOG_FTP|LOG_ERR, "passiveTCPLowLevel: bind: %m");
    exit(1);
  }
  // listen on the given port
  if (listen(psock, queue_size) == -1) {
    syslog(LOG_FTP|LOG_ERR, "passiveTCPLowLevel: listen: %m");
    exit(1);
  }

  return psock;
}

// int activeTCP(char * host_pre, char * port_pre) {
//   struct hostent* host;
//   u_int16_t port;
  
//   // transf. dotted-quad/hostname to struct in_addr
//   host = gethostbyname(host_pre);
//   if (host == NULL) 
//     switch (h_errno) {
//     case HOST_NOT_FOUND: {
//       syslog(LOG_FTP|LOG_ERR,
// 	     "activeTCP: gethostbyname: the specified host is unknown");
//       exit(1);
//     }
//     case NO_ADDRESS: {
//       syslog(LOG_FTP|LOG_ERR,
// 	     "activeTCP: gethostbyname: the requested name is valid but it does not have an IP address");
//       exit(1);
//     }
//     case NO_RECOVERY: {
//       syslog(LOG_FTP|LOG_ERR,
// 	     "activeTCP: gethostbyname: a non-recoverable name server error occurred");
//       exit(1);
//     }
//     case TRY_AGAIN: {
//       syslog(LOG_FTP|LOG_ERR,
// 	     "activeTCP: gethostbyname: a temporary error occurred on an authoritative name server.  Try again later.");
//       exit(1);
//     }
//     default: {
//       syslog(LOG_FTP|LOG_ERR, "activeTCP: gethostbyname: %m");
//       exit(1);
//     }
//     }
  
//   // transf. port
//   char* end_pointer = NULL;
//   // try to convert
//   port = strtoumax(port_pre, &end_pointer, 10);

//   // TODO: test overflow/underflow
//   if (*end_pointer != 0) {
//     syslog(LOG_FTP|LOG_ERR, "activeTCP: invalid port specification");
//     exit(1);
//   }
//   return activeTCPLowLevel(((struct in_addr *)host->h_addr_list[0])->s_addr, port);
// }

void sendReply(int sock, char * reply) {
  if (write(sock, reply, strlen(reply)) == -1) {
    syslog(LOG_FTP | LOG_ERR, "sendReply: write: %m");
    exit(1);
  }
  if (write(sock, &cr, 1) == -1) {
    syslog(LOG_FTP | LOG_ERR, "sendReply: write: %m");
    exit(1);
  }
  if (write(sock, &lf, 1) == -1) {
    syslog(LOG_FTP | LOG_ERR, "sendReply: write: %m");
    exit(1);
  }
}

void ftpService(int sock) {
  char buf [BUF_SIZE];
  char reply [BUF_SIZE];
  bool terminate = false;
  int n, on, i;
  char *delimiters = " \n\r";
  char *temp, *cmd, *arg;

  // don't wait for system buffer to be full
  on = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_OOBINLINE, (char*)&on, sizeof(on)) == -1) {
    syslog(LOG_FTP | LOG_ERR, "ftpService: setsockopt: %m");
    exit(1);
  }

  // let the peer know that you are ready
  snprintf(reply, BUF_SIZE, FTP_R220, "LightFTPServer v0.1");
  sendReply(sock, reply);

  while (!terminate) {
    // read command
    memset(buf, 0, BUF_SIZE);
    // TODO: loop to read all data
    if ( (n = read(sock, buf, BUF_SIZE)) == -1) {
      syslog(LOG_FTP | LOG_ERR, "ftpService: read: %m");
      exit(1);
    }
    // TODO: check if command too large for buffer = not ending in CRLF

    // parse cmd and arguments
    temp = strdupa(buf);
    cmd = strtok(temp, delimiters);
    arg = strtok(NULL, delimiters);
    // should ignore blank commands or send a reply?
    if (cmd != NULL) {
      // look for handler
      for (i = 0; i < _FTPCMDS_END; i++)

{//	if (strcasecmp(cmd, ftpcmds[i]) == 0) {
//	  ftphandlers[i](cmd, arg, (char*)&reply, BUF_SIZE, &terminate);

// 	  // say good bye and terminate
// 	  if (i == FTP_CQUIT) {
// 	    terminate = true;
// 	    snprintf(reply, BUF_SIZE, FTP_R221);
// 	  }
// 	  else
// 	    // handle command
// 	    snprintf(reply, BUF_SIZE, FTP_R200, cmd);
	  break;
	}
      // unknown command
      if (i == _FTPCMDS_END)
	snprintf(reply, BUF_SIZE, FTP_R500, cmd);
      // send back reply
      sendReply(sock, reply);
    }
  }

  close(sock);
}
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
					delete serverSocket;
					
					CFTPClient *client = new CFTPClient(clientSocket);
					client->Run();
					delete client;
					//ftpService(csock);
					exit(0);
				} else {
					// parent
					syslog(LOG_FTP|LOG_INFO, "new connection");
					delete clientSocket;
				}
			}
  }
}
