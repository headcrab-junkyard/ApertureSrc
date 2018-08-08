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
/// @brief Win32 system interface code

#include <errno.h>
#include "quakedef.h"
#include "winquake.h"
#include "resource.h"
#include "conproc.h"

#ifdef SWDS
#include <sys\types.h>
#include <sys\timeb.h>
#endif

#define MINIMUM_WIN_MEMORY 0x0880000
#define MAXIMUM_WIN_MEMORY 0x1000000

#define CONSOLE_ERROR_TIMEOUT 60.0 // # of seconds to wait on Sys_Error running dedicated before exiting

int starttime;

static char *tracking_tag{"Clams & Mooses"}; // TODO: unused?

// TODO: unused by dedicated
static HANDLE tevent;
static HANDLE hFile;
static HANDLE heventParent;
static HANDLE heventChild;
//

int Sys_FileOpenWrite(const char *path)
{
	FILE *f;
	int i;
	int t;

	t = VID_ForceUnlockedAndReturnState(); // TODO: non-dedicated win only

	i = findhandle();

	f = fopen(path, "wb");
	if(!f)
		Sys_Error("Error opening %s: %s", path, strerror(errno));
	sys_handles[i] = f;

	VID_ForceLockState(t); // TODO: non-dedicated win only

	return i;
}

/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_MakeCodeWriteable(unsigned long startaddr, unsigned long length)
{
#if defined(_WIN32) && defined(SWDS)
	// Nothing
#endif
}

void Sys_DebugLog(const char *file, const char *fmt, ...)
{
#if defined(_WIN32) && defined(SWDS)
	// Nothing
#endif
}

void Sys_Error(const char *error, ...)
{
#if defined(_WIN32) && defined(SWDS)
	va_list argptr;
	char text[1024];

	va_start(argptr, error);
	vsprintf(text, error, argptr);
	va_end(argptr);

	//    MessageBox(nullptr, text, "Error", 0 /* MB_OK */ );
	printf("ERROR: %s\n", text);

	exit(1);
#endif
}

void Sys_Quit()
{
#if defined(_WIN32) && defined(SWDS)
	exit(0);
#endif
}

double Sys_FloatTime()
{
#if defined(_WIN32) && defined(SWDS)
	double t;
	struct _timeb tstruct;
	static int starttime;

	_ftime(&tstruct);

	if(!starttime)
		starttime = tstruct.time;
	t = (tstruct.time - starttime) + tstruct.millitm * 0.001;

	return t;
#endif
}

void Sys_HighFPPrecision()
{
#if defined(_WIN32) && defined(SWDS)
	// Nothing
#endif
}

void Sys_LowFPPrecision()
{
#if defined(_WIN32) && defined(SWDS)
	// Nothing
#endif
}

char *Sys_ConsoleInput()
{
#if defined(_WIN32) && defined(SWDS)
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
#endif
}

/*
==============================================================================

 WINDOWS CRAP

==============================================================================
*/

/*
==================
main

==================
*/
#ifdef SWDS
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
#endif

/*
==================
WinMain
==================
*/

/*
char		*argv[MAX_NUM_ARGVS];
static char	*empty_string = "";
HWND		hwnd_dialog;


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG				msg;
	
	MEMORYSTATUS	lpBuffer;
	static	char	cwd[1024];
	int				t;
	RECT			rect;

    

	

	lpBuffer.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus (&lpBuffer);

	if (!GetCurrentDirectory (sizeof(cwd), cwd))
		Sys_Error ("Couldn't determine current directory");

	if (cwd[Q_strlen(cwd)-1] == '/')
		cwd[Q_strlen(cwd)-1] = 0;

	parms.basedir = cwd;
	parms.cachedir = nullptr;

	parms.argc = 1;
	argv[0] = empty_string;

	while (*lpCmdLine && (parms.argc < MAX_NUM_ARGVS))
	{
		while (*lpCmdLine && ((*lpCmdLine <= 32) || (*lpCmdLine > 126)))
			lpCmdLine++;

		if (*lpCmdLine)
		{
			argv[parms.argc] = lpCmdLine;
			parms.argc++;

			while (*lpCmdLine && ((*lpCmdLine > 32) && (*lpCmdLine <= 126)))
				lpCmdLine++;

			if (*lpCmdLine)
			{
				*lpCmdLine = 0;
				lpCmdLine++;
			}
			
		}
	}

	parms.argv = argv;

	

	if (!isDedicated)
	{
		hwnd_dialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, nullptr);

		if (hwnd_dialog)
		{
			if (GetWindowRect (hwnd_dialog, &rect))
			{
				if (rect.left > (rect.top * 2))
				{
					SetWindowPos (hwnd_dialog, 0,
						(rect.left / 2) - ((rect.right - rect.left) / 2),
						rect.top, 0, 0,
						SWP_NOZORDER | SWP_NOSIZE);
				}
			}

			ShowWindow (hwnd_dialog, SW_SHOWDEFAULT);
			UpdateWindow (hwnd_dialog);
			SetForegroundWindow (hwnd_dialog);
		}
	}

// take the greater of all the available memory or half the total memory,
// but at least 8 Mb and no more than 16 Mb, unless they explicitly
// request otherwise
	parms.memsize = lpBuffer.dwAvailPhys;

	if (parms.memsize < MINIMUM_WIN_MEMORY)
		parms.memsize = MINIMUM_WIN_MEMORY;

	if (parms.memsize < (lpBuffer.dwTotalPhys >> 1))
		parms.memsize = lpBuffer.dwTotalPhys >> 1;

	if (parms.memsize > MAXIMUM_WIN_MEMORY)
		parms.memsize = MAXIMUM_WIN_MEMORY;

	if (COM_CheckParm ("-heapsize"))
	{
		t = COM_CheckParm("-heapsize") + 1;

		if (t < com_argc)
			parms.memsize = Q_atoi (com_argv[t]) * 1024;
	}

	parms.membase = malloc (parms.memsize);

	if (!parms.membase)
		Sys_Error ("Not enough memory free; check disk space\n");

	Sys_PageIn (parms.membase, parms.memsize);

	tevent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	if (!tevent)
		Sys_Error ("Couldn't create event");

	Sys_Init ();

// because sound is off until we become active
	S_BlockSound ();

	Sys_Printf ("Host_Init\n");
	Host_Init (&parms);

	oldtime = Sys_FloatTime ();

    // main window message loop
	while (1) // TODO: true
	{
		Host_Frame(0.1);
	}
}
*/