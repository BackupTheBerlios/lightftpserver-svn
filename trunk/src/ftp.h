#ifndef _FTP_H
#define _FTP_H

enum FTP_CMDS {
  FTP_CUSER = 0,
  FTP_CPASS,
  FTP_CACCT,
  FTP_CCWD,
  FTP_CCDUP,
  FTP_CSMNT,
  FTP_CQUIT,
  FTP_CREIN,
  FTP_CPORT,
  FTP_CPASV,
  FTP_CTYPE,
  FTP_CSTRU,
  FTP_CMODE,
  FTP_CRETR,
  FTP_CSTOR,
  FTP_CSTOU,
  FTP_CAPPE,
  FTP_CALLO,
  FTP_CREST,
  FTP_CRNFR,
  FTP_CRNTO,
  FTP_CABOR,
  FTP_CDELE,
  FTP_CRMD,
  FTP_CMKD,
  FTP_CPWD,
  FTP_CLIST,
  FTP_CNLST,
  FTP_CSITE,
  FTP_CSYST,
  FTP_CSTAT,
  FTP_CHELP,
  FTP_CNOOP,
  _FTPCMDS_END
};

//extern char* ftpcmds[];

// Used as format strings
// TODO: fill formatting information
//TODO: cum is acuma definite trebuie sa stiu codul erorii ca sa folosesc define-ul
//nu ar fi mai bine daca ar fi numele comenzii ? macar la alea care se poate
//sa fie de ex define-uri duble, unu cu codul (cum ii acuma) si unul cu comanda - la care se poate bineinteles (care au numai un cod de eroare)
#define FTP_R110 "110 Restart marker reply."
#define FTP_R120 "120 Service ready in nnn minutes."
#define FTP_R125 "125 Data connection already open; transfer starting."
#define FTP_R150 "150 File status okay; about to open data connection."
#define FTP_R200 "200 %s command okay."
#define FTP_R202 "202 %s command not implemented, superfluous at this site."
#define FTP_R211S "211-System status, or system help reply."
#define FTP_R211E "211 End of status or system help reply."
#define FTP_R212 "212 Directory status." 
#define FTP_R213 "213 File status."
#define FTP_R214S "214-Help:"
#define FTP_R214E "214 Help: end of list."

#define FTP_R215 "215 UNIX Type: L8"
#define FTP_R220 "220 %s Service ready for new user."
#define FTP_R221 "221 Service closing control connection. Good bye!"
#define FTP_R225 "225 Data connection open; no transfer in progress."
#define FTP_R226 "226 Closing data connection."
#define FTP_R227 "227 Entering Passive Mode (%s,%d,%d)."
#define FTP_R230 "230 User logged in, proceed." 
#define FTP_R250 "250 Requested file action okay, completed."
#define FTP_R257C "257 %s created."
#define FTP_R257P "257 %s is the current directory."
#define FTP_R331 "331 User name okay, need password."
#define FTP_R332 "332 Need account for login."
#define FTP_R350 "350 Requested file action pending further information."
#define FTP_R421 "421 Service not available, closing control connection."
#define FTP_R425 "425 Can't open data connection."
#define FTP_R426 "426 Connection closed; transfer aborted."
#define FTP_R450 "450 Requested file action not taken."
#define FTP_R451 "451 Requested action aborted: local error in processing."
#define FTP_R452 "452 Requested action not taken."
#define FTP_R500 "500 %s syntax error, command unrecognized."
#define FTP_R501 "501 Syntax error in parameters or arguments."
#define FTP_R502 "502 Command not implemented."
#define FTP_R503 "503 Bad sequence of commands."
#define FTP_R504 "504 Command not implemented for that parameter."
#define FTP_R530 "530 Not logged in."
#define FTP_R532 "532 Need account for storing files."
#define FTP_R550 "550 Requested action not taken."
#define FTP_R551 "551 Requested action aborted: page type unknown."
#define FTP_R552 "552 Requested file action aborted."
#define FTP_R553 "553 Requested action not taken."

#endif
