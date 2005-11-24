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
#include "logClient.h"
#include <signal.h>
#include <syslog.h>
#include <time.h>

#include <sys/stat.h>

#define MAXFD 64

/**
Current verbosity level ...

If a log entry's verbosity level is bigger or equal with this value the log
entry will be shown, otherwise it will not.
*/
int Verbosity = 5;

int main()
{
  int rcode, i, listenfd, connfd, len;
  struct sockaddr_in addr;
  
  //daemonization >:|
  rcode = spoon();
  if (rcode == -1) {
    syslog(LOG_FTP|LOG_ERR, "daemonization stage 1 %m");
    exit(1);
  }
  else if (rcode == 0)
    //parent
    exit(0);

  //child 1
  setsid();
  signal(SIGHUP, SIG_IGN);

  if (rcode == -1) {
    syslog(LOG_FTP|LOG_ERR, "daemonization stage 2 %m");
    exit(1);
  }
  else if (rcode == 0)
    //parent
    exit(0);

  //child 2
  chdir("/");
  umask(0);

  for (i = 0; i < MAXFD; i++)
    close(i);

  //daemon
  if ( (listenfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    syslog(LOG_FTP|LOG_ERR, "socket: %m");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(21);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
 
  if ( (rcode = bind(listenfd, (struct sockaddr *)&addr, sizeof(addr))) == -1) {
    syslog(LOG_FTP|LOG_ERR, "bind: %m");
    exit(1);
  }

  if ( (rcode = listen(listenfd, 5)) == -1) {
    syslog(LOG_FTP|LOG_ERR, "listen: %m");
    exit(1);
  }

  syslog(LOG_FTP|LOG_INFO, "FTP Server started. Waiting for connections...");
  while (1) {
    if ( (connfd = accept(listenfd,(struct sockaddr *) &addr, (socklen_t *)&len)) == -1) {
      syslog(LOG_FTP|LOG_ERR, "accept: %m");
      exit(1);
    }
    
    rcode = fork();
    if (rcode == -1) {
      syslog(LOG_FTP|LOG_ERR, "child: %m");
      exit(1);
    } else if (rcode != 0) {
      //child
      close(listenfd);
      //start the actual server :D
    }
    //parent
    close(connfd);
  }

				CLogClient log(5);
        srand(time(NULL));
        i = random() % 1000;
//         printf("FTP Server started ...\nWaiting for clients to server ...\n");
//        Log(0, "This is a log entry with a string (%s) and a number(%d)", "this is the string", i);
        char buffer[INI_LINE_SIZE];
        long caca;
//        WriteSettingString("Config verbosity", "pipi uscat", "value for pipi uscat", "ftp.ini");
//        WriteSettingString("Fucking something", "verbosity", "18", "ftp.ini");
/*
				GetSettingInt("Settings", "value", 00, caca, "ftp.ini");
				GetSettingInt("Settings", "hex", 0x11, caca, "ftp.ini");
				GetSettingInt("Settings", "octal", 0123, caca, "ftp.ini");
        GetSettingString("Fucking something", "caca", "<error at caca>", buffer, sizeof(buffer), "ftp.ini");
      	GetSettingInt("Fucking something", "verbosity", 0, caca, "ftp.ini");
        GetSettingString("Config verbosity", "non existent", "<error at>", buffer, sizeof(buffer), "ftp.ini");
        GetSettingString("Pipi uscat", "anything", "<error at pipi uscat>", buffer, sizeof(buffer), "ftp.ini");
*/
        return 0;
}
