#include "ftp_handlers.h"

void handleUSER(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handlePASS(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleACCT(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleCWD(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleCDUP(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
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
void handleDELE(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleRMD(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handleMKD(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
void handlePWD(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}
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
void handleHELP(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R214, cmd);}
void handleNOOP(const char* cmd, const char* arg, char* reply, unsigned int reply_size, bool* terminate)
{snprintf(reply, reply_size, FTP_R202, cmd);}

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
