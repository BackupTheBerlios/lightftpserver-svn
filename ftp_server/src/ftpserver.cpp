/*
This is the main server file.
*/

//this file is in the include folder, not the src one so we need to include accordingly
#include "../include/commonheaders.h"

#include <time.h>

/*
Current verbosity level ...
If a log entry's verbosity level is bigger or equal with this value the log
entry will be shown, otherwise it will not.
*/
int Verbosity = 5;

int main()
{
	LogInit();
	srand(time(NULL));
	int i = random() % 1000;
	printf("FTP Server started ...\nWaiting for clients to server ...\n");
	Log(0, "This is a log entry with a string (%s) and a number(%d)", "this is the string", i);
	LogDestroy();
	return 0;
}
