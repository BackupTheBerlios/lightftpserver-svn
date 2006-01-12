/* #define FTP_CUSER */
/* #define FTP_CPASS */
/* #define FTP_CACCT */
/* #define FTP_CCWD */
/* #define FTP_CCDUP */
/* #define FTP_CSMNT */
/* #define FTP_CQUIT */
/* #define FTP_CREIN */
/* #define FTP_CPORT */
/* #define FTP_CPASV */
/* #define FTP_CTYPE */
/* #define FTP_CSTRU */
/* #define FTP_CMODE */
/* #define FTP_CRETR */
/* #define FTP_CSTOR */
/* #define FTP_CSTOU */
/* #define FTP_CAPPE */
/* #define FTP_CALLO */
/* #define FTP_CREST */
/* #define FTP_CRNFR */
/* #define FTP_CRNTO */
/* #define FTP_CABOR */
/* #define FTP_CDELE */
/* #define FTP_CRMD */
/* #define FTP_CMKD */
/* #define FTP_CPWD */
/* #define FTP_CLIST */
/* #define FTP_CNLST */
/* #define FTP_CSITE */
/* #define FTP_CSYST */
/* #define FTP_CSTAT */
/* #define FTP_CHELP */
/* #define FTP_CNOOP */

// Used as format strings
// TODO: fill formatting information
#define FTP_R110 "Restart marker reply."
#define FTP_R120 "Service ready in nnn minutes."
#define FTP_R125 "Data connection already open; transfer starting."
#define FTP_R150 "File status okay; about to open data connection."
#define FTP_R200 "Command okay."
#define FTP_R202 "Command not implemented, superfluous at this site."
#define FTP_R211 "System status, or system help reply."
#define FTP_R212 "Directory status." 
#define FTP_R213 "File status."
#define FTP_R214 "Help message." 
#define FTP_R215 "NAME system type."
#define FTP_R220 "%s Service ready for new user."
#define FTP_R221 "Service closing control connection."
#define FTP_R225 "Data connection open; no transfer in progress."
#define FTP_R226 "Closing data connection."
#define FTP_R227 "Entering Passive Mode (h1,h2,h3,h4,p1,p2)."
#define FTP_R230 "User logged in, proceed." 
#define FTP_R250 "Requested file action okay, completed."
#define FTP_R257 "PATHNAME created."
#define FTP_R331 "User name okay, need password."
#define FTP_R332 "Need account for login."
#define FTP_R350 "Requested file action pending further information."
#define FTP_R421 "Service not available, closing control connection."
#define FTP_R425 "Can't open data connection."
#define FTP_R426 "Connection closed; transfer aborted."
#define FTP_R450 "Requested file action not taken."
#define FTP_R451 "Requested action aborted: local error in processing."
#define FTP_R452 "Requested action not taken."
#define FTP_R500 "Syntax error, command unrecognized."
#define FTP_R501 "Syntax error in parameters or arguments."
#define FTP_R502 "Command not implemented."
#define FTP_R503 "Bad sequence of commands."
#define FTP_R504 "Command not implemented for that parameter."
#define FTP_R530 "Not logged in."
#define FTP_R532 "Need account for storing files."
#define FTP_R550 "Requested action not taken."
#define FTP_R551 "Requested action aborted: page type unknown."
#define FTP_R552 "Requested file action aborted."
#define FTP_R553 "Requested action not taken."
