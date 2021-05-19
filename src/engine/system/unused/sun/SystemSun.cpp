/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2021 BlackPhrase
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
/// @brief SunOS-specific system code

#include "quakedef.h"

void CSystem::PlatformInit()
{
};

/*
================
Sys_MakeCodeWriteable
================
*/
void CSystem::MakeCodeWriteable(unsigned long startaddr, unsigned long length)
{
	int psize = getpagesize();

	unsigned long addr = (startaddr & ~(psize - 1)) - psize;

	//	fprintf(stderr, "writable code %lx(%lx)-%lx, length=%lx\n", startaddr,
	//			addr, startaddr+length, length);

	int r = mprotect((char *)addr, length + startaddr - addr + psize, 7);

	if(r < 0)
		Error("Protection change failed\n");
};

// NOTE: skipclient set to true will result in a legacy behavior (as Sys_Printf instead of Con_Printf)
void CSystem::Printf(bool skipclient, const char *fmt, ...)
{
	va_list argptr;
	
	// TODO: or windows dedicated server
	va_start(argptr, fmt);
	vprintf(fmt, argptr);
	va_end(argptr);
	
	// write it to the scrollable buffer on client-side
	if(!skipclient)
		if(gpEngineClient)
			gpEngineClient->ConPrint(fmt);

	// log all messages to file
	if(!skipclient)
		if(con_debuglog)
			Con_DebugLog("qconsole.log", "%s", msg);
};

void CSystem::Error(const char *error, ...)
{
	va_list argptr;

	printf("Sys_Error: ");
	va_start(argptr, error);
	vprintf(error, argptr);
	va_end(argptr);
	printf("\n");
	Host_Shutdown();
	exit(1);
};

void CSystem::Sleep(int anMilliSecs)
{
	// TODO: or dedicated
	
	// TODO
};