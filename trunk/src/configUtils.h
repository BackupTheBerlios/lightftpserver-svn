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

#define SECTION_START_CHAR '['
#define SECTION_END_CHAR ']'

#define COMMENT_CHARS "#;"
#define WHITESPACE_CHARS "\n \t"

#define SETTING_ATTRIB_CHAR '='

#define INI_LINE_SIZE 1024

//int GetSettingSection(char *szSection, char *szResult, char *szIniFile);
int GetSettingString(char *szSection, char *szSettingName, char *szErrorString, char *szResult, int size, char *szIniFile);
int GetSettingInt(char *szSection, char *szSettingName, long nErrorValue, long &nResult, char *szIniFile);
//int GetSettingLong(char *szSection, char *szSettingName, long nErrorValue, long &nResult, char *szIniFile);
int GetSettingBool(char *szSection, char *szSettingName, char bErrorValue, char &bResult, char *szIniFile);

/**
\todo a lot of work here :(
*/
int WriteSettingString(char *szSection, char *szSettingName, char *szValue, char *szIniFile);

/**
\todo a lot of work here also :(
*/
int WriteSettingInt(char *szSection, char *szSettingName, int nValue, char *szIniFile);
//int WriteSettingLong(char *szSection, char *szSettingName, long nValue, char *szIniFile);

#endif
