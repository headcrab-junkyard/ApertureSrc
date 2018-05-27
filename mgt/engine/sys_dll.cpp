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
/// @brief

#include "quakedef.h"

#ifdef _WIN32
#include "winquake.h"

qboolean WinNT;

static double pfreq;
static double curtime = 0.0;
static double lastcurtime = 0.0;
static int lowshift;
#endif

void MaskExceptions();
void Sys_InitFloatTime();
void Sys_PushFPCW_SetHigh();
void Sys_PopFPCW();

void Sys_InitAuthentication(){
	// TODO
};

void Sys_ShutdownAuthentication(){
	// TODO
};

void Sys_InitLauncherInterface(){
	// TODO
};

void Sys_ShutdownLauncherInterface(){
	// TODO
};

void Sys_InitMemory(){
	// TODO
};

void Sys_ShutdownMemory(){
	// TODO
};

/*
================
Sys_InitFloatTime
================
*/
#ifdef _WIN32
void Sys_InitFloatTime()
{
	int j;

	Sys_FloatTime();

	j = COM_CheckParm("-starttime");

	if(j)
		curtime = (double)(Q_atof(com_argv[j + 1]));
	else
		curtime = 0.0;

	lastcurtime = curtime;
};
#endif // _WIN32

/*
================
Sys_Init
================
*/
void Sys_Init()
{
#ifdef _WIN32
	LARGE_INTEGER PerformanceFreq;
	unsigned int lowpart, highpart;
	OSVERSIONINFO vinfo;

	MaskExceptions();
	Sys_SetFPCW();

	if(!QueryPerformanceFrequency(&PerformanceFreq))
		Sys_Error("No hardware timer available");

	// get 32 out of the 64 time bits such that we have around
	// 1 microsecond resolution
	lowpart = (unsigned int)PerformanceFreq.LowPart;
	highpart = (unsigned int)PerformanceFreq.HighPart;
	lowshift = 0;

	while(highpart || (lowpart > 2000000.0))
	{
		lowshift++;
		lowpart >>= 1;
		lowpart |= (highpart & 1) << 31;
		highpart >>= 1;
	}

	pfreq = 1.0 / (double)lowpart;

	Sys_InitFloatTime();

	vinfo.dwOSVersionInfoSize = sizeof(vinfo);

	if(!GetVersionEx(&vinfo))
		Sys_Error("Couldn't get OS info");

	if((vinfo.dwMajorVersion < 4) ||
	   (vinfo.dwPlatformId == VER_PLATFORM_WIN32s))
	{
		Sys_Error("WinQuake requires at least Win95 or NT 4.0");
	}

	if(vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		WinNT = true;
	else
		WinNT = false;
#else // if linux
#if id386
	Sys_SetFPCW();
#endif
#endif
};

void Sys_Shutdown(){
	// TODO
};

void Sys_InitArgv(){
	// TODO
};

void Sys_ShutdownArgv(){
	// TODO
};

void Sys_Printf(const char *fmt, ...)
{
#ifdef _WIN32
	va_list argptr;
	char text[1024];
	DWORD dummy;

	if(isDedicated)
	{
		va_start(argptr, fmt);
		vsprintf(text, fmt, argptr);
		va_end(argptr);

		//WriteFile(houtput, text, strlen (text), &dummy, nullptr); // TODO: IDedicatedExports->Printf
	};
#else  // if linux
	va_list argptr;
	char text[1024];
	unsigned char *p;

	va_start(argptr, fmt);
	vsprintf(text, fmt, argptr);
	va_end(argptr);

	if(strlen(text) > sizeof(text))
		Sys_Error("memory overwrite in Sys_Printf");

	if(nostdout)
		return;

	for(p = (unsigned char *)text; *p; p++)
	{
		*p &= 0x7f;
		if((*p > 128 || *p < 32) && *p != 10 && *p != 13 && *p != 9)
			printf("[%02x]", *p);
		else
			putc(*p, stdout);
	};
#endif // _WIN32
};

/*
================
Sys_FloatTime
================
*/
double Sys_FloatTime()
{
#ifdef _WIN32
	static int sametimecount;
	static unsigned int oldtime;
	static int first = 1;
	LARGE_INTEGER PerformanceCount;
	unsigned int temp, t2;
	double time;

	Sys_PushFPCW_SetHigh();

	QueryPerformanceCounter(&PerformanceCount);

	temp = ((unsigned int)PerformanceCount.LowPart >> lowshift) |
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
			t2 = temp - oldtime;

			time = (double)t2 * pfreq;
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
	};

	Sys_PopFPCW();

	return curtime;
#else // if __linux__ or something else (that support POSIX API)
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
#endif
};