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

#include "messages.h"

int TMessage::operator()(TParam1 param1, TParam2 param2)
{
	callbackMethod(param1, param2);
}


int InitMessage(TMessage &message, int code, char szMessage[MESSAGE_SIZE], TMethodCallback callback)
{
	message.code = code;
	strncpy(message.message, szMessage, MESSAGE_SIZE);
	message.callbackMethod = callback;
	return 0;
}

int InitializeMessages(TMessage *&messages, int &count, CFTPClient *clientInstance)
{
	int size = 3; //we have to know how many messages we know about
	messages = (TMessage *) malloc(size * sizeof(TMessage)); //allocate enough space for them
	//and then fill them one by one :(
	InitMessage(messages[0], CMD_HELP, "help", Binder(&CFTPClient::HandleHelpCommand, clientInstance));
	InitMessage(messages[1], CMD_LIST, "list", Binder(&CFTPClient::HandleListCommand, clientInstance));
	InitMessage(messages[2], CMD_STAT, "stat", Binder(&CFTPClient::HandleStatCommand, clientInstance));
}

