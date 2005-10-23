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

#include <ctype.h> //for isalnum()

#include "configUtils.h"
#include "commonheaders.h"


/**
Gets the new valid line from the ini file, skipping comment lines

*/
int FindNextLine(FILE *iniFile, char *buffer, int size)
{
	int comment = 1;
	char *pos;
	while (comment)
		{
			pos = fgets(buffer, size, iniFile);
			if (pos)
				{
					StrTrim(buffer, WHITESPACE_CHARS);
				}
				else{
					buffer[0] = '\0';
					return 0;
				}
			comment = (int) strpbrk(buffer, COMMENT_CHARS);
		}
	return 1;
}

int IsSection(char *szBuffer)
{
	int i = strlen(szBuffer) - 1;
	if ((szBuffer[0] == SECTION_START_CHAR) && (szBuffer[i] == SECTION_END_CHAR))
		{
			return 1;
		}
	return 0;
}

int FindSection(FILE *iniFile, char *szSection)
{
	char buffer[INI_LINE_SIZE];
	char aux[INI_LINE_SIZE];
	sprintf(aux, "%c%s%c", SECTION_START_CHAR, szSection, SECTION_END_CHAR);
	int found = 0;
	while (!found)
		{
			if (FindNextLine(iniFile, buffer, sizeof(buffer)))
				{
					if (strcmp(buffer, aux) == 0)
						{
							found = 1;
						}
				}
				else{
					break;
				}
		}
	return found;
}

int ParseSetting(char *szSetting, char *szResult, int size)
{
	char *pos = strchr(szSetting, SETTING_ATTRIB_CHAR);
	if (pos) //valid line
		{
			strncpy(szResult, (pos + 1), size); //leave the ATRIB char out of the string
			StrTrim(szResult, WHITESPACE_CHARS);
		}
}

int FindSettingInSection(FILE *iniFile, char *szSettingName, char *szLine, int size)
{
	char buffer[INI_LINE_SIZE];
	int found = 0;
	char *pos;
	while (!found)
		{
			if (FindNextLine(iniFile, buffer, sizeof(buffer)))
				{
					if (IsSection(buffer)) //we reached another section so the value isn't here
						{
							strncpy(szLine, buffer, size);
							break;
						}
					pos = strstr(buffer, szSettingName);
					if ((pos == buffer) && (!isalnum((int) pos[strlen(szSettingName)]))) //setting name starts at the very beggining and it's a full word
						{
							strncpy(szLine, buffer, size);
							found = 1;
						}
				}
				else{
					break;
				}
		}
	return found;
}

FILE *LoadIniFile(char *szIniFile)
{
	fopen(szIniFile, "rt+");
}

int CloseIniFile(FILE *iniFile)
{
	fclose(iniFile);
}

int GetSettingString(char *szSection, char *szSettingName, char *szErrorString, char *szResult, int size, char *szIniFile)
{
	FILE *fin = LoadIniFile(szIniFile);
	char buffer[INI_LINE_SIZE];
	char value[INI_LINE_SIZE];
	char *result = NULL;
	int found = 1;
	if (FindSection(fin, szSection)) //section exists
		{
			if (FindSettingInSection(fin, szSettingName, buffer, sizeof(buffer)))
				{
					ParseSetting(buffer, value, sizeof(value));
					result = value;
				}
				else{ //setting doesn't exist
					result = szErrorString;
				}
		}
		else{ //section doesn't exist
			result = szErrorString;
		}
	if (result != NULL)
		{
			strncpy(szResult, result, size);
		}
		else{ //we can't return NULL :(
			szResult[0] = '\0';
			found = 0;
		}
	CloseIniFile(fin);
	return found;
}

int GetSettingInt(char *szSection, char *szSettingName, long nErrorValue, long &nResult, char *szIniFile)
{
	char buffer[INI_LINE_SIZE];
	if (GetSettingString(szSection, szSettingName, NULL, buffer, sizeof(buffer), szIniFile) == 1)
		{
			errno = 0;
			nResult = strtol(buffer, NULL, 0); //base depends on string
		}
		else{
			errno = 1;
		}
	if (errno != 0)
		{
			nResult = nErrorValue;
		}
	return 0;
}

int GetSettingBool(char *szSection, char *szSettingName, char bErrorValue, char &bResult, char *szIniFile)
{
	char buffer[INI_LINE_SIZE];
	if (GetSettingString(szSection, szSettingName, NULL, buffer, sizeof(buffer), szIniFile) == 1)
		{
			if ((strcmp(buffer, "yes") == 0) || (strcmp(buffer, "true") == 0) || (strcmp(buffer, "1") == 0) || (strcmp(buffer, "on") == 0))
				{
					bResult = 1;
				}
				else{
					bResult = 0;
				}
		}
		else{
			bResult = bErrorValue;
		}
	return 0;
}

int WriteSettingString(char *szSection, char *szSettingName, char *szValue, char *szIniFile)
{
	char buffer[INI_LINE_SIZE];
	FILE *fout = LoadIniFile(szIniFile);
	if (FindSection(fout, szSection))
		{
			if (FindSettingInSection(fout, szSettingName, buffer, sizeof(buffer)))
				{
					sprintf(buffer, "%s = %s", szSettingName, szValue);
					fputs(buffer, fout);
				}
				else{ //we're at the first line of the new section
					sprintf(buffer, "%s = %s", szSettingName, szValue);
					fputs(buffer, fout);
				}
		}
		else{
		}
	
	CloseIniFile(fout);
}



