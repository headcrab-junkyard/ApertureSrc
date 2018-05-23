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
#include "Engine.hpp"

// TODO: temp
#define PAUSE_SLEEP		50				// sleep time on pause or minimization
#define NOT_FOCUS_SLEEP	20				// sleep time when not focus

//static CEngine gEngine; // g_Engine
//IEngine *gpEngine = &gEngine; // eng

// TODO
#ifdef _WIN32
qboolean ActiveApp, Minimized;
#endif

EXPOSE_SINGLE_INTERFACE(CEngine, IEngine, MGT_ENGINE_INTERFACE_VERSION);

CEngine::CEngine() = default;
CEngine::~CEngine() = default;

//bool CEngine::Init(bool dedicated, const char *basedir, const char *cmdline)
bool CEngine::Init(const SInitData &apInitParams)
{
	//static quakeparms_t parms; // TODO: static?
	
	//memset(&parms, 0, sizeof(parms));
	
	//mParms. =;
	
	//parms.memsize = 8*1024*1024;
	//parms.membase = malloc (parms.memsize);
	//parms.basedir = ".";
	
	//COM_InitArgv (argc, argv); // TODO: parms.argc, parms.argv
	
	//parms.argc = com_argc;
	//parms.argv = com_argv;
	
	//isDedicated = (COM_CheckParm ("-dedicated") != 0);
	
	//Sys_InitGame(char *, char *, void *, int); // TODO
	
	//printf("Host_Init\n");
	Host_Init(&mParms);
	return true;
};

void CEngine::Shutdown()
{
	Host_Shutdown();
};

// TODO
#ifdef _WIN32
void SleepUntilInput(int time)
{
	//MsgWaitForMultipleObjects(1, &tevent, FALSE, time, QS_ALLINPUT); // TODO
};
#endif

bool /*void*/ CEngine::Frame() // TODO
{
	static double time, oldtime, newtime;
	
	if (isDedicated)
	{
		newtime = Sys_FloatTime ();
		time = newtime - oldtime;

		while (time < sys_ticrate.value )
		{
			Sys_Sleep();
			newtime = Sys_FloatTime ();
			time = newtime - oldtime;
		}
	}
	else
	{
#ifdef _WIN32
	// yield the CPU for a little while when paused, minimized, or not the focus
		//if ((cl.paused && (!ActiveApp && !DDActive)) || Minimized || block_drawing) // TODO
		{
			SleepUntilInput (PAUSE_SLEEP);
			//scr_skipupdate = 1;		// no point in bothering to draw // TODO
		}
		//else if (!ActiveApp && !DDActive) // TODO
			SleepUntilInput (NOT_FOCUS_SLEEP);
#endif // _WIN32

		newtime = Sys_FloatTime ();
		time = newtime - oldtime;
	};

	Host_Frame (time);
	oldtime = newtime;
	
	return true;
};