/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2019, (2020?)-2021 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"
#include "CmdArgs.hpp"

static const char *cmd_null_string{""};

CCmdArgs::CCmdArgs(const char *asCmdLine)
{
	TokenizeString(asCmdLine);
};

//CCmdArgs::~CCmdArgs() = default;

/*
============
Cmd_Argc
============
*/
int CCmdArgs::GetCount() const
{
	return cmd_argc;
};

/*
============
Cmd_Argv
============
*/
const char *CCmdArgs::GetByIndex(int arg) const
{
	if((uint)arg >= cmd_argc)
		return cmd_null_string;
	return cmd_argv[arg];
};

/*
const char *CCmdArgs::GetByName(const char *asArg) const
{
	auto nArg{CheckParm(asArg)};
	return nArg ? GetByIndex(nArg) : "";
};
*/

/*
================
Cmd_CheckParm

Returns the position (1 to argc-1) in the command's argument list
where the given parameter apears, or 0 if not present
================
*/
int CCmdArgs::CheckParm(const char *parm) const
{
	if(!parm)
	{
		//mpSystem->Error("Cmd_CheckParm: NULL");
		return 0;
	};

	for(int i = 1; i < GetCount(); ++i)
		if(!Q_strcasecmp(parm, GetByIndex(i)))
			return i;

	return 0;
};

/*
============
Cmd_Args
============
*/
const char *CCmdArgs::ToString() const
{
	return cmd_args;
};

/*
============
Cmd_TokenizeString

Parses the given string into command line tokens.
============
*/
void CCmdArgs::TokenizeString(const char *text)
{
	// clear the args from the last string
	for(int i = 0; i < cmd_argc; ++i)
		Z_Free(cmd_argv[i]);

	cmd_argc = 0;
	cmd_args = nullptr;

	while(1)
	{
		// skip whitespace up to a /n
		while(*text && *text <= ' ' && *text != '\n')
			text++;

		if(*text == '\n')
		{
			// a newline seperates commands in the buffer
			text++;
			break;
		};

		if(!*text)
			return;

		if(cmd_argc == 1)
			cmd_args = text;

		text = COM_Parse(text);
		if(!text)
			return;

		if(cmd_argc < MAX_ARGS)
		{
			cmd_argv[cmd_argc] = (char *)Z_Malloc(Q_strlen(com_token) + 1);
			Q_strcpy(cmd_argv[cmd_argc], com_token);
			cmd_argc++;
		};
	};
};