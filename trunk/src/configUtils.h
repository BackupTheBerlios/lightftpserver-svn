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

#ifndef CHAT_CONFIGURATION_UTILS_H
#define CHAT_CONFIGURATION_UTILS_H


int GetSettingString(char *szSection, char *szErrorString, char *szResult, int size, char *szIniFile);
int GetSettingInt(char *szSection, int nErrorValue, int &nResult, char *szIniFile);
int GetSettingLong(char *szSection, long nErrorValue, long &nResult, char *szIniFile);
int GetSettingBool(char *szSection, char bErrorValue, char &bResult, char *szIniFile);

int WriteSettingString(char *szSection, char *szValue, char *szIniFile);
int WriteSettingInt(char *szSection, int nValue, char *szIniFile);
int WriteSettingLong(char *szSection, long nValue, char *szIniFile);

#endif