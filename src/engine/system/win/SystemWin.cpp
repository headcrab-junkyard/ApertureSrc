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
/// @brief Windows-specific system code

#include "quakedef.h"
#include "winquake.h"
#include "SystemWin.hpp"

#define CONSOLE_ERROR_TIMEOUT 60.0 // # of seconds to wait on Sys_Error running dedicated before exiting

/*
================
Sys_PageIn
================
*/
void Sys_PageIn(void *ptr, int size)
{
	static volatile int sys_checksum{0};

	// touch all the memory to make sure it's there. The 16-page skip is to
	// keep Win 95 from thinking we're trying to page ourselves in (we are
	// doing that, of course, but there's no reason we shouldn't)
	auto x{reinterpret_cast<byte*>(ptr)};

	for(int n = 0; n < 4; ++n)
	{
		for(int m = 0; m < (size - 16 * 0x1000); m += 4)
		{
			sys_checksum += *(int *)&x[m];
			sys_checksum += *(int *)&x[m + 16 * 0x1000];
		};
	};
};

/*
================
Sys_InitFloatTime
================
*/
void CSystemWin::InitFloatTime()
{	
	int j = COM_CheckParm("-starttime");

	if(j)
		curtime = (double)(Q_atof(com_argv[j + 1]));
	else
		curtime = 0.0;

	lastcurtime = curtime;
};

/*
================
Sys_Init
================
*/
void CSystemWin::Init()
{
	LARGE_INTEGER PerformanceFreq;

	if(!QueryPerformanceFrequency(&PerformanceFreq))
		Error("No hardware timer available");

	// get 32 out of the 64 time bits such that we have around
	// 1 microsecond resolution
	uint lowpart = (uint)PerformanceFreq.LowPart;
	uint highpart = (uint)PerformanceFreq.HighPart;
	lowshift = 0;

	while(highpart || (lowpart > 2000000.0))
	{
		lowshift++;
		lowpart >>= 1;
		lowpart |= (highpart & 1) << 31;
		highpart >>= 1;
	};

	pfreq = 1.0 / (double)lowpart;
	
	GetFloatTime();
	
	InitFloatTime();

	OSVERSIONINFO vinfo;
	
	vinfo.dwOSVersionInfoSize = sizeof(vinfo);

	if(!GetVersionEx(&vinfo))
		Error("Couldn't get OS info");

	if((vinfo.dwMajorVersion < 4) || (vinfo.dwPlatformId == VER_PLATFORM_WIN32s))
		Error("WinQuake requires at least Win95 or NT 4.0");

	if(vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		WinNT = true;
	else
		WinNT = false;
};

/*
================
Sys_MakeCodeWriteable
================
*/
void CSystem::MakeCodeWriteable(unsigned long startaddr, unsigned long length)
{
#ifndef SWDS
	DWORD flOldProtect;

	if(!VirtualProtect((LPVOID)startaddr, length, PAGE_READWRITE, &flOldProtect))
		Error("Protection change failed\n");
#endif
};

void CSystemWin::Printf(const char *fmt, ...)
{
	va_list argptr;
	
	char text[1024]{};
	DWORD dummy;

	// echo to debugging console
	if(isDedicated)
	{
		va_start(argptr, fmt);
		vsprintf(text, fmt, argptr);
		va_end(argptr);
		
		printf("%s", text); // TODO

		//WriteFile(houtput, text, strlen (text), &dummy, nullptr); // TODO: IDedicatedExports->Printf
	};
	
	// Output to MSVC output window
	OutputDebugString(text);
};

void CSystemWin::Error(const char *text)
{
#ifdef SWDS
	//va_list argptr;
	//char text[1024];

	//va_start(argptr, error);
	//vsprintf(text, error, argptr);
	//va_end(argptr);

	//MessageBox(nullptr, text, "Error", 0 /* MB_OK */ );
	printf("ERROR: %s\n", text);

	exit(1);
#else
	//va_list argptr;
	//char text[1024];
	char text2[1024];
	const char *text3 = "Press Enter to exit\n";
	const char *text4 = "***********************************\n";
	const char *text5 = "\n";
	DWORD dummy;
	double starttime;
	static int in_sys_error0 = 0;
	static int in_sys_error1 = 0;
	static int in_sys_error3 = 0;

	if(!in_sys_error3)
	{
		in_sys_error3 = 1;
		//VID_ForceUnlockedAndReturnState(); // TODO
	};

	//va_start(argptr, error);
	//vsprintf(text, error, argptr);
	//va_end(argptr);

	if(isDedicated)
	{
		//va_start(argptr, error);
		//vsprintf(text, error, argptr);
		//va_end(argptr);

		sprintf(text2, "ERROR: %s\n", text);
		//WriteFile(houtput, text5, strlen(text5), &dummy, nullptr); // TODO
		//WriteFile(houtput, text4, strlen(text4), &dummy, nullptr); // TODO
		//WriteFile(houtput, text2, strlen(text2), &dummy, nullptr); // TODO
		//WriteFile(houtput, text3, strlen(text3), &dummy, nullptr); // TODO
		//WriteFile(houtput, text4, strlen(text4), &dummy, nullptr); // TODO

		starttime = GetFloatTime();
		//sc_return_on_enter = true; // so Enter will get us out of here // TODO

		//while(!Sys_ConsoleInput() && ((GetFloatTime() - starttime) < CONSOLE_ERROR_TIMEOUT)) // TODO
		{
		};
	}
	else
	{
		// switch to windowed so the message box is visible, unless we already
		// tried that and failed
		if(!in_sys_error0)
		{
			in_sys_error0 = 1;
			//VID_SetDefaultMode(); // TODO
			MessageBox(nullptr, text, "Engine Error", MB_OK | MB_SETFOREGROUND | MB_ICONSTOP);
		}
		else
			MessageBox(nullptr, text, "Double Engine Error", MB_OK | MB_SETFOREGROUND | MB_ICONSTOP);
	};

	if(!in_sys_error1)
	{
		in_sys_error1 = 1;
		//Host_Shutdown(); // TODO
	};

	exit(1);
#endif // SWDS
};

/*
================
Sys_FloatTime
================
*/
double CSystemWin::GetFloatTime()
{
#ifdef SWDS
	struct _timeb tstruct;
	static int starttime;

	_ftime(&tstruct);

	if(!starttime)
		starttime = tstruct.time;
	double t = (tstruct.time - starttime) + tstruct.millitm * 0.001;

	return t;
#else
	static int sametimecount;
	static unsigned int oldtime;
	static int first = 1;
	LARGE_INTEGER PerformanceCount;

	QueryPerformanceCounter(&PerformanceCount);

	uint temp = ((unsigned int)PerformanceCount.LowPart >> lowshift) |
	((unsigned int)PerformanceCount.HighPart << (32 - lowshift));

	if(first)
	{
		oldtime = temp;
		first = 0;
	}
	else
	{
		// check for turnover or backward time
		if((temp <= oldtime) && ((oldtime - temp) < 0x10000000))
			oldtime = temp; // so we can't get stuck
		else
		{
			uint t2 = temp - oldtime;

			double time = (double)t2 * pfreq;
			oldtime = temp;

			curtime += time;

			if(curtime == lastcurtime)
			{
				sametimecount++;

				if(sametimecount > 100000)
				{
					curtime += 1.0;
					sametimecount = 0;
				};
			}
			else
				sametimecount = 0;

			lastcurtime = curtime;
		};
	}

	return curtime;
#endif // SWDS
};

void CSystem::Sleep(int anMilliSecs)
{
	Sleep(anMilliSecs);
};