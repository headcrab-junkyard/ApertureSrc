/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#ifdef NeXT
#include <libc.h>
#endif

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include <fcntl.h>
#include "quakedef.h"

#include "engine/IConsole.hpp"

bool con_debuglog{false};

void Con_Debug_f()
{
	// TODO
};

/*
================
Con_Init
================
*/
void Con_Init()
{
	con_debuglog = COM_CheckParm("-condebug");

	//
	// register our commands
	//
	Cmd_AddCommand("condebug", Con_Debug_f);
	
	if(gpEngineClient)
		gpEngineClient->ConInit();
	
	Con_Printf("Console initialized.\n");
};

/*
================
Con_DebugLog
================
*/
void Con_DebugLog(const char *file, const char *fmt, ...)
{
	va_list argptr;
	static char data[1024]{};

	va_start(argptr, fmt);
	vsprintf(data, fmt, argptr);
	va_end(argptr);
	int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	write(fd, data, strlen(data));
	close(fd);
};

/*
================
Con_Printf

Handles cursor positioning, line wrapping, etc
================
*/
#define MAXPRINTMSG 4096
// FIXME: make a buffer size safe vsprintf?
void Con_Printf(const char *fmt, ...)
{
	va_list argptr;
	char msg[MAXPRINTMSG]{};

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	// also echo to debugging console
	Sys_Printf("%s", msg);

	// log all messages to file
	if(con_debuglog)
		Con_DebugLog(va("%s/qconsole.log", com_gamedir), "%s", msg);

	// write it to the scrollable buffer on client-side
	if(gpEngineClient)
		gpEngineClient->ConPrint(msg);
};

/*
================
Con_DPrintf

A Con_Printf that only shows up if the "developer" cvar is set
================
*/
void Con_DPrintf(const char *fmt, ...)
{
	va_list argptr;
	char msg[MAXPRINTMSG]{};

	// don't confuse non-developers with techie stuff...
	if(!developer.value)
		return;

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	Con_Printf("%s", msg);
};

/*
==================
Con_SafePrintf

Okay to call even when the screen can't be updated
==================
*/
void Con_SafePrintf(const char *fmt, ...)
{
	va_list argptr;
	char msg[1024]{};

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);

	bool temp = scr_disabled_for_loading;
	scr_disabled_for_loading = true;
	Con_Printf("%s", msg);
	scr_disabled_for_loading = temp;
};

class CConsole final : public IConsole
{
public:
	CConsole();
	~CConsole();
	
	void Printf(const char *msg, ...) override;
	void DevPrintf(const char *msg, ...) override;
};

EXPOSE_SINGLE_INTERFACE(CConsole, IConsole, MGT_CONSOLE_INTERFACE_VERSION);

CConsole::CConsole() = default;
CConsole::~CConsole() = default;

void CConsole::Printf(const char *fmt, ...)
{
	va_list argptr;
	char msg[MAXPRINTMSG]{};

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);
	
	Con_Printf(msg);
};

void CConsole::DevPrintf(const char *fmt, ...)
{
	va_list argptr;
	char msg[MAXPRINTMSG]{};

	va_start(argptr, fmt);
	vsprintf(msg, fmt, argptr);
	va_end(argptr);
	
	Con_DPrintf(msg);
};