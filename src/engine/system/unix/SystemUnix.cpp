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
/// @brief Unix-specific system code

#include "quakedef.h"
#include "SystemUnix.hpp"

int nostdout{0};

/*
================
Sys_Init
================
*/
void CSystemUnix::Init()
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

void CSystemUnix::Printf(const char *fmt, ...)
{
	va_list argptr;
	
	char text[1024]{};

	va_start(argptr, fmt);
	vsprintf(text, fmt, argptr);
	va_end(argptr);

	if(Q_strlen(text) > sizeof(text))
		Sys_Error("memory overwrite in Sys_Printf");
	
	if(nostdout)
		return;

	//fprintf(stderr, "%s", text);
	
	for(auto p = reinterpret_cast<byte *>(text); *p; p++)
	{
		*p &= 0x7f;
		if((*p > 128 || *p < 32) && *p != 10 && *p != 13 && *p != 9)
			printf("[%02x]", *p);
		else
			putc(*p, stdout);
	};
};

void CSystemUnix::Error(const char *error, ...)
{
	va_list argptr;
	char string[1024];

	// change stdin to non blocking
	fcntl(0, F_SETFL, fcntl(0, F_GETFL, 0) & ~FNDELAY);

	va_start(argptr, error);
	vsprintf(string, error, argptr);
	va_end(argptr);
	fprintf(stderr, "Error: %s\n", string);

	Host_Shutdown();
	exit(1);
};

/*
================
Sys_FloatTime
================
*/
double CSystemUnix::GetFloatTime()
{
	struct timeval tp;
	struct timezone tzp;
	static int secbase;

	gettimeofday(&tp, &tzp);

	if(!secbase)
	{
		secbase = tp.tv_sec;
		return tp.tv_usec / 1000000.0;
	};

	return (tp.tv_sec - secbase) + tp.tv_usec / 1000000.0;
};

void CSystem::Sleep(int anMilliSecs)
{
	usleep(anMilliSecs * 1000);
};