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
#ifndef CHAT_FTPCLIENT_CALLBACKS_H
#define CHAT_FTPCLIENT_CALLBACKS_H

#include "commonheaders.h"
#include "client.h"

typedef int (CFTPClient::*ClassCallback)(TParam1, TParam2);

struct TMethodCallback{
	CFTPClient *instance;
	ClassCallback callback;
	
	int operator()(TParam1 param1, TParam2 param2);
};

//binds a class method an in instance of that class
TMethodCallback Binder(ClassCallback callback, CFTPClient *instance);

#endif

