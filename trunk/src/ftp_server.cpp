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

#include <time.h>

/**
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
