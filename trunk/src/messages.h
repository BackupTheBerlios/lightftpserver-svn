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
#ifndef CHAT_KNOWN_MESSAGES_H
#define CHAT_KNOWN_MESSAGES_H

#include "commonheaders.h"
#include "callback.h"

#define MESSAGE_SIZE 64

enum CommandCodes{
	CMD_HELP,
	CMD_LIST,
	CMD_STAT
};

struct TMessage{
	int code;
	char message[MESSAGE_SIZE];
	TMethodCallback callbackMethod;
	
	int operator()(TParam1 param1, TParam2 param2);
};

int InitMessage(TMessage &message, int code, char szMessage[MESSAGE_SIZE], TMethodCallback callback);
int InitializeMessages(TMessage *&messages, int &count, CFTPClient *clientInstance);


#endif
