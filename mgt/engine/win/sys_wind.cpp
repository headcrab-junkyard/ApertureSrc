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

#include "quakedef.h"
#include "winquake.h"
#include <errno.h>
#include <sys\types.h>
#include <sys\timeb.h>

int Sys_FileOpenWrite(const char *path)
{
	FILE *f;
	int i;

	i = findhandle();

	f = fopen(path, "wb");
	if(!f)
		Sys_Error("Error opening %s: %s", path, strerror(errno));
	sys_handles[i] = f;

	return i;
}

/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_MakeCodeWriteable(unsigned long startaddr, unsigned long length)
{
}

void Sys_DebugLog(const char *file, const char *fmt, ...)
{
}

void Sys_Error(const char *error, ...)
{
	va_list argptr;
	char text[1024];

	va_start(argptr, error);
	vsprintf(text, error, argptr);
	va_end(argptr);

	//    MessageBox(nullptr, text, "Error", 0 /* MB_OK */ );
	printf("ERROR: %s\n", text);

	exit(1);
}

void Sys_Quit()
{
	exit(0);
}

double Sys_FloatTime()
{
	double t;
	struct _timeb tstruct;
	static int starttime;

	_ftime(&tstruct);

	if(!starttime)
		starttime = tstruct.time;
	t = (tstruct.time - starttime) + tstruct.millitm * 0.001;

	return t;
}

void Sys_HighFPPrecision()
{
}

void Sys_LowFPPrecision()
{
}

char *Sys_ConsoleInput()
{
	static char text[256];
	static int len;
	INPUT_RECORD recs[1024];
	int count;
	int i;
	int c;

	// read a line out
	while(_kbhit())
	{
		c = _getch();
		putch(c);
		if(c == '\r')
		{
			text[len] = 0;
			putch('\n');
			len = 0;
			return text;
		}
		if(c == 8)
		{
			putch(' ');
			putch(c);
			len--;
			text[len] = 0;
			continue;
		}
		text[len] = c;
		len++;
		text[len] = 0;
		if(len == sizeof(text))
			len = 0;
	}

	return nullptr;
}

/*
==================
main

==================
*/
char *newargv[256];

int main(int argc, char **argv)
{
	double time, oldtime;
	static char cwd[1024];

	parms.memsize = 16384 * 1024;

	_getcwd(cwd, sizeof(cwd));
	if(cwd[Q_strlen(cwd) - 1] == '\\')
		cwd[Q_strlen(cwd) - 1] = 0;
	parms.basedir = cwd;

	oldtime = Sys_FloatTime();

	/* main window message loop */
	while(1)
	{
	}

	/* return success of application */
	return TRUE;
}
