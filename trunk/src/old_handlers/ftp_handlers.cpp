#include "ftp_handlers.h"

//remove:
#include <syslog.h>

#define USERNAME_SIZE 128

struct spwd ftp_user;
char * username = NULL;

void handleUSER(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  if (arg == NULL)
      snprintf(reply, reply_size, FTP_R500, cmd);
  else {
    username = strdup(arg);
    snprintf(reply, reply_size, FTP_R331, arg);
  }
}
void handlePASS(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  struct spwd* tmps;
  struct passwd* tmpp;
  char salt[12];
//   uid_t euid;
//   gid_t egid;



  if (username != NULL) {
    // store the effective user id
//     euid = geteuid();
//     egid = getgid();

//     syslog(LOG_FTP|LOG_DEBUG, "uid:%d gid:%d", euid, egid);

    // try to become root
    if (seteuid(0) == -1)
      // won't be able to change euid, so fail
      snprintf(reply, reply_size, FTP_R530);
    else
      // try to read shadow and compare pass entry with encrypted argument
      if (((tmps = getspnam(username)) != NULL) &&
	  (strcmp(tmps->sp_pwdp, crypt(arg, strncpy(salt, tmps->sp_pwdp, 11))) == 0)) {
	
	tmpp = getpwnam(username);
	
	// try to change to the user's default group
	setegid(tmpp->pw_gid);
	seteuid(tmpp->pw_uid);
	snprintf(reply, reply_size, FTP_R230, arg);
      }
    // make sure entire logging process starts from the begining
    free(username);
    username = NULL;
  } else
    // PASS command before USER
    snprintf(reply, reply_size, FTP_R503);
}
void handleACCT(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleCWD(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  if (chdir(arg) == -1)
    switch (errno) {
    case ENOTDIR: {
      snprintf(reply, reply_size, FTP_R501, arg);
      break;
    }
    default:
      snprintf(reply, reply_size, FTP_R550);
    }
  else
    snprintf(reply, reply_size, FTP_R200, cmd);
}
void handleCDUP(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  handleCWD(cmd, "..", reply, reply_size, terminate);
}
void handleSMNT(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleQUIT(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{
  *terminate = true;
  snprintf(reply, reply_size, FTP_R221);
}
void handleREIN(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handlePORT(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handlePASV(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleTYPE(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleSTRU(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleMODE(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleRETR(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleSTOR(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleSTOU(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleAPPE(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleALLO(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleREST(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleRNFR(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleRNTO(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleABOR(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleDELE(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  if (unlink(arg) == -1)
    switch (errno) {
    case EBUSY: {
      snprintf(reply, reply_size, FTP_R450);
      break;
    }
    case ENAMETOOLONG: {
      snprintf(reply, reply_size, FTP_R501, arg);
      break;
    }
    default:
      snprintf(reply, reply_size, FTP_R550);
    }
  else
    snprintf(reply, reply_size, FTP_R250);
}
void handleRMD(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  if (rmdir(arg) == -1)
    switch (errno) {
    case ENAMETOOLONG:
    case ENOTDIR: 
    case EINVAL: {
      snprintf(reply, reply_size, FTP_R501, arg);
      break;
    }
    default:
      snprintf(reply, reply_size, FTP_R550);
    }
  else
    snprintf(reply, reply_size, FTP_R250);
}
void handleMKD(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  if (mkdir(arg, S_IRWXU) == -1)
    switch (errno) {
    case ENAMETOOLONG:
    case ENOTDIR: {
      snprintf(reply, reply_size, FTP_R501, arg);
      break;
    }
    default: snprintf(reply, reply_size, FTP_R550);
    } else
      snprintf(reply, reply_size, FTP_R257C, arg);
}
void handlePWD(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  snprintf(reply, reply_size, FTP_R257P, get_current_dir_name());
}
void handleLIST(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleNLST(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleSITE(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleSYST(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleSTAT(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleHELP(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  snprintf(reply, reply_size, FTP_R214, cmd);
}
void handleNOOP(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate) {
  snprintf(reply, reply_size, FTP_R200, cmd);
}

ftphandler_t ftphandlers[_FTPCMDS_END] = {
  &handleUSER,
  &handlePASS,
  &handleACCT,
  &handleCWD,
  &handleCDUP,
  &handleSMNT,
  &handleQUIT,
  &handleREIN,
  &handlePORT,
  &handlePASV,
  &handleTYPE,
  &handleSTRU,
  &handleMODE,
  &handleRETR,
  &handleSTOR,
  &handleSTOU,
  &handleAPPE,
  &handleALLO,
  &handleREST,
  &handleRNFR,
  &handleRNTO,
  &handleABOR,
  &handleDELE,
  &handleRMD,
  &handleMKD,
  &handlePWD,
  &handleLIST,
  &handleNLST,
  &handleSITE,
  &handleSYST,
  &handleSTAT,
  &handleHELP,
  &handleNOOP,
};
